#include "global.h"
link_num *head;

int main()
{
    //initialize the important parameters
    CW_0 = 4;
    CW_max = 1024;
    SIFS_duration = 0.5;
    DIFS_duration = 2;

    //t_a is traffice at slot A and t_b is traffic at slot B
    int Arate_a[3] = {50, 100, 200, 300}; //Arrical rate
    int Arate_c[3] = {50, 100, 200, 300};

    slot_time = 0.00002; //this is in us for testing, to translate to slots

    //For Station A
    int n_a_slots = create_traffic(Arate_a[0], slot_time);
    int t_a[n_a_slots]; //array of slots generated at A
    store_slots(t_a);   ///store the heap values in stack of t_a

    //For station B
    int n_c_slots = create_traffic(Arate_c[0], slot_time);
    int t_c[n_c_slots];
    store_slots(t_c);

    print_slots(t_a, n_a_slots); //print t_a
    print_slots(t_c, n_c_slots); //print t_a

    CSMA_CA_hid_comm(t_a,t_c,n_a_slots,n_c_slots);

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
    float x, x_prev;
    int i = 0;
    int time_limit = 10; //time limit set to 10 sec
    int max_time = time_limit / slot_time;
    x_prev = 0;
    srand((long)time(NULL));
    head = NULL;
    link_num *t_num, *t_next;
    t_next = NULL;
    for (; x_prev < max_time; i++)
    {
        x = random_0_1();
        x = -(logf(1 - x) / (Arate * slot_time));
        x = roundf(x);
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

float random_0_1()
{
    return rand() / (float)RAND_MAX;
}