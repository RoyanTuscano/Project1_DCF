#include "global.h"
link_num *head;

int main()
{
    //initialize the important parameters
    trans_rate = 6000000;  //Transmission rate of 6[Mbps]
    max_back_off = 1024;   // Maximum back off as per the project description = 1024 [slots]
    frame_size = 1500 * 8; // Frame size once the frame transmission begins = 1500 * 8 [bits]
    ack_size = 30 * 8;     // Acknowledgement size = 30 * 8 [bits]
    rts_size = 30 * 8;     // Request to Send frame size = 30 * 8 [bits]
    cts_size = 30 * 8;     // Clear to send frame size = 30 * 8 [bits]
    cnt = 0;
    sim_time = 10; // Simulation time of 10 [seconds]
    OccupyChannel1 = 0;
    OccupyChannel2 = 0;
    // Time required to transmit the SIFS frame. [seconds per sifs].
    sifs_time = 0.000010;
    // Time required to transmit the DIFS frame. [seconds per diffs].
    difs_time = 0.000040;
    slot_time = 0.00002; //this is in us for testing, to translate to slots

    sim_slot = ceil(sim_time / slot_time);
    frame_slot = ceil(frame_size / (trans_rate * slot_time)); // number of slots per frame [slots per frame(packet)]
    ack_slot = ceil(ack_size / (trans_rate * slot_time));     // number of slots for ACK [slots per frame(packet)]
    rts_slot = ceil(rts_size / (trans_rate * slot_time));     // number of slots for RTS [slots per frame(packet)]
    cts_slot = ceil(cts_size / (trans_rate * slot_time));     // number of slots for CTS [slots per frame(packet)]
    sifs_slot = ceil(sifs_time / slot_time);                  // number of slots for SIFS [slots per frame(packet)]
    difs_slot = ceil(difs_time / slot_time);                  // number of slots for SIFS [slots per frame(packet)]
    cw0 = 4;                                                  // Initial back off for the sender nodes [slots]
    nwcw1 = cw0;                                              // New Back off in case of collision
    nwcw2 = cw0;                                              // New Back off in case of collision
    cw1 = 0;                                                  // Back off for node1. Initialized to the initial back off
    cw2 = 0;                                                  // Back off for node2. Initialized to the initial back off
    numberOfSuccess1 = 0;
    numberOfSuccess2 = 0;

    //t_a is traffice at slot A and t_b is traffic at slot B
    int Arate_a[4] = {50, 100, 200, 300}; //Arrical rate
    int Arate_c[4] = {50, 100, 200, 300};

    //For Station A
    int n_a_slots = create_traffic(Arate_a[0], slot_time);
    int *t_a = malloc(--n_a_slots * sizeof(int)); //array of slots generated at A
    store_slots(t_a);                             ///store the heap values in stack of t_a

    //For station B
    int n_c_slots = create_traffic(Arate_c[0], slot_time);
    int *t_c = malloc(--n_c_slots * sizeof(int));
    store_slots(t_c);

    //  print_slots(t_a, n_a_slots); //print t_a
    //  print_slots(t_c, n_c_slots); //print t_a

    CSMA_con_comm(t_a, t_c, n_a_slots, n_c_slots);
    CSMA_hid_comm(t_a, t_c, n_a_slots, n_c_slots);
    CSMA_con_CA_comm(t_a, t_c, n_a_slots, n_c_slots);
    CSMA_hid_CA_comm(t_a, t_c, n_a_slots, n_c_slots);

    return 0;
}

void store_slots(int *t)
{
    link_num *tmp;
    int i = 0;
    while (head != NULL)
    {
        tmp = head;
        t[i++] = tmp->num;
        head = head->next;
        free(tmp); //free the heap
    }
}

int create_traffic(int Arate, float slot_time)
{
    double x, x_prev;
    int i = 0;
    int time_limit = 10; //time limit set to 10 sec
    int max_time = (int)time_limit / slot_time;
    x_prev = 0;
    //srand(time(0));
    head = NULL;
    link_num *t_num, *t_next;
    t_next = NULL;
    printf("\n");
    for (; x_prev < max_time; i++)
    {
        x = random_0_1();
        //   printf("%f,",x);
        x = -(logf(1 - x) / ((double)Arate * (double)slot_time));
        x = ceil(x);
        t_num = malloc(sizeof(link_num));
        if (i == 0)
            head = t_num;
        else
            t_next->next = t_num;

        x_prev = x + x_prev;

        t_num->num = (int)x_prev;
        t_num->next = NULL;
        t_next = t_num;
    }
    return i;
}

void print_slots(int *t, int n)
{
    printf("\n");
    for (int i = 0; i < n; i++)
        printf("%d, ", t[i]);
}

double random_0_1()
{
    return rand() / (double)RAND_MAX;
}
void initialize_array(int *a, int x, int y)
{
    for (int i = 0; i < x; i++)
    {
        for (int j = 0; j < y; j++)
            *((a + i * y) + j) = 0;
    }
}
void initialize_single_array(int *a, int x)
{
    for (int i = 0; i < x; i++)
    {
        a[i] = 0;
    }
}
void copy_slot_array(int *dest, int dest_row, int col, int *src, int cnt)
{
    for (int i = 0; i < cnt; i++)
    {
        *((dest + dest_row * col) + i) = src[i];
        //dest[dest_row][i] = src[i];
    }
}
void occupy_slot_2dem(int *dest, int dest_row, int s_col, int f_col, int cnt, int value)
{
    for (int i = s_col; i < cnt && i <= f_col; i++)
    {
        *((dest + dest_row * cnt) + i) = value;
        //dest[dest_row][i] = value;
    }
}
void occupy_slot_1dem(int *dest, int s_col, int f_col, int cnt, int value)
{
    for (int i = s_col; i < cnt && i <= f_col; i++)
    {
        dest[i] = value;
    }
}
int sum_array_elements(int *dest, int row, int col)
{
    int sum = 0;
    for (int i = 0; i < col; i++)
    {
        if (*((dest + row * col) + i) == 1)
            sum++;
    }

    return sum;
}
