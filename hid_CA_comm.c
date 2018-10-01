#include "global.h"

static int status[4][500000];
static int NAV[2][500000];
static int updt_slot[4][500000];
static int frm_lst[4][500000];
static int channel_s[2][500000];
int CSMA_hid_CA_comm(int *frm_slt1, int *frm_slt2, int a, int c)
{
    sim_slot = 500000;
    // int *channel_s = malloc(sim_slot * sizeof(int));
    int *frm_rcv_slot1 = malloc(1000 * sizeof(int));
    int *frm_rcv_slot2 = malloc(1000 * sizeof(int));
    int *frm_rcv_slot21 = malloc(1000 * sizeof(int));

    initialize_array((int *)status, 4, sim_slot);
    initialize_array((int *)NAV, 2, sim_slot);
    initialize_array((int *)channel_s, 2, sim_slot);
    // initialize_single_array(channel_s, sim_slot);
    initialize_array((int *)updt_slot, 4, sim_slot);
    OccupyChannel1 = 0;
    OccupyChannel2 = 0;

    int bck_of[2] = {0, 0};
    int numberOfCollisions[2] = {0, 0};
    int flag[2] = {0, 0};
    int prev_back_off[2] = {0, 0};
    int netf = 0;

    int sendCts;
    int noCts[2] = {0, 0};
    int frm_rcv_num1 = 0;
    int frm_rcv_num2 = 0;

    int frm_rcv_num21 = 0;
    initialize_array((int *)frm_lst, 4, sim_slot);

    int s1, s2;

    for (int i = 0; i < sim_slot; i++)
    {
        if (channel_s[0][i] == 0) //channel 1 free
        {
            for (int s = 0; s < a; s++)
            {
                if (frm_slt1[s] != 0 && frm_lst[0][s] == 0 && i >= frm_slt1[s])
                {
                    status[0][i] = 1;
                    s1 = s;
                    break;
                }
            }
        }
        if (channel_s[1][i] == 0) //channel 1 free
        {
            for (int s = 0; s < c; s++)
            {
                if (frm_slt2[s] != 0 && frm_lst[2][s] == 0 && i >= frm_slt2[s])
                {
                    status[1][i] = 1;
                    s2 = s;
                    break;
                }
            }
        }
        for (int len = 0; len < 2; len++)
        {
            int i_prev, i_next;
            if (i != 0)
            {
                i_prev = i - 1;
                i_next = i + 1;
            }
            else
            {
                i_prev = 0;
                i_next = i + 1;
            }
            if (status[len][i] == 1 && channel_s[len][i] == 0 && updt_slot[len][i_prev] == 0 && updt_slot[len][i] == 0 && bck_of[len] == 0)
            {

                occupy_slot_2dem((int *)updt_slot, len, i_next, i + difs_slot, sim_slot, 1);
                occupy_slot_2dem((int *)status, len, i_next, i + difs_slot, sim_slot, 1);
            }
            else if (status[len][i] == 1 && channel_s[len][i] == 0 && updt_slot[len][i_prev] == 1 && updt_slot[len][i] == 0 && bck_of[len] == 0)
            {
                if (len == 0)
                {
                    nwcw1 = pow(2, numberOfCollisions[len]);
                    if (numberOfCollisions[len] > 10)
                        cw1 = 1024;
                    else
                        cw1 = rand() % (nwcw1 * cw0);
                    occupy_slot_2dem((int *)status, len, i_next, i + cw1, sim_slot, 1);
                    bck_of[len] = 1;
                }
                else if (len == 1)
                {
                    nwcw2 = pow(2, numberOfCollisions[len]);
                    if (numberOfCollisions[len] > 10)
                        cw2 = 1024;
                    else
                        cw2 = rand() % (nwcw2 * cw0);
                    occupy_slot_2dem((int *)status, len, i_next, i + cw2, sim_slot, 1);
                    bck_of[len] = 1;
                }
            }
            else if (status[len][i] == 1 && channel_s[len][i] == 0 && bck_of[len] == 1 && flag[len] == 0)
            {
                if (len == 0)
                {
                    if (cw1 > 0)
                    {
                        cw1 = cw1 - 1;
                        status[len][i_next] = 1;
                    }
                    if (cw1 == 0)
                    {
                        flag[len] = 1;
                        bck_of[len] = 0;
                        prev_back_off[len] = 1;
                        occupy_slot_2dem((int *)status, len, i_next, i + rts_slot, sim_slot, 1);
                        netf = flag[0] || flag[1];
                    }
                }
                else if (len == 1)
                {
                    if (cw2 > 0)
                    {
                        cw2 = cw2 - 1;
                        status[len][i_next] = 1;
                    }
                    if (cw2 == 0)
                    {
                        flag[len] = 1;
                        bck_of[len] = 0;
                        prev_back_off[len] = 1;
                        occupy_slot_2dem((int *)status, len, i_next, i + rts_slot, sim_slot, 1);
                        netf = flag[0] || flag[1];
                    }
                }
                if (netf == 1)
                {
                    //  Only 1 is sending
                    if (flag[0] == 1)
                        occupy_slot_2dem((int *)status, 0, i, i + rts_slot + sifs_slot + cts_slot + 1, sim_slot, 1);
                    else if (flag[1] == 1)
                        occupy_slot_2dem((int *)status, 1, i, i + rts_slot + sifs_slot + cts_slot + 1, sim_slot, 1);
                }
            }
            else if (status[len][i] == 1 && bck_of[len] == 0 && prev_back_off[len] == 1 && flag[len] == 1)
            {
                occupy_slot_2dem((int *)channel_s, len, i, i + rts_slot + sifs_slot + cts_slot, sim_slot, 1);
                prev_back_off[len] = 0;

                NAV[len][i] = rts_slot + sifs_slot + cts_slot; // Define the length of the transmission.
                NAV[len][i_next] = NAV[len][i] - 1;            // Reduce the NAV by 1 in each iteration once the transmission has started.
                occupy_slot_2dem((int *)status, len, i_next, i + rts_slot, sim_slot, 1);

                flag[len] = 0;
            }
            else if (channel_s[len][i] == 1 && (NAV[len][i] > sifs_slot + cts_slot))
            {
                NAV[len][i_next] = NAV[len][i] - 1;           // Reduce NAV by 1
                if (NAV[len][i_next] == sifs_slot + cts_slot) // After completion of transmission of the data frame
                    if (status[0][i] == 1 && len == 0)
                    {
                        occupy_slot_2dem((int *)status, 2, i, i + sifs_slot + cts_slot, sim_slot, 1); // Node-3 (Node B, the receiver) has to send the ACK
                    }
                    else if (status[1][i] == 1 && len == 1)
                    {
                        occupy_slot_2dem((int *)status, 3, i, i + sifs_slot + cts_slot, sim_slot, 1); // Node-3 (Node B, the receiver) has to send the ACK
                    }
            }
            else if (channel_s[len][i] == 1 && (status[2][i] == 1 || status[3][i] == 1) && NAV[len][i] > cts_slot)
                NAV[len][i_next] = NAV[len][i] - 1; //Reduce NAV by 1 until the completion of the SIFS time.
            else if (channel_s[len][i] == 1 && (status[2][i] == 1 || status[3][i] == 1) && NAV[len][i] == cts_slot)
            {
                if (noCts[len] == 1) // If no ACK, do nothing.
                {
                    NAV[len][i_next] = NAV[len][i] - 2; // Reduce NAV by 1.
                    noCts[len] = 0;
                }
                else
                {
                    sendCts = 1;                        // Set sendCts flag as 1.
                    NAV[len][i_next] = NAV[len][i] - 1; // Reduce NAV by 1.
                }
            }
            else if (channel_s[len][i] == 1 && (status[2][i] == 1 || status[3][i] == 1) && NAV[len][i] == 1)
            {
                if (status[2][i] == 1)
                {
                    if (sendCts == 1)
                    {
                        // printf("%i, ", i);
                        //  frm_rcv_slot1[frm_rcv_num1++] = i;
                        frm_lst[0][s1] = 1;
                        sendCts = 0;
                        netf = 0;
                        flag[0] = 0;
                        occupy_slot_2dem((int *)status, 0, i, sim_slot, sim_slot, 0);
                        i = i + sifs_slot + frame_slot + sifs_slot + ack_slot;
                    }
                }
                else if (status[3][i] == 1)
                {
                    if (sendCts == 1)
                    {
                        frm_rcv_slot2[frm_rcv_num2++] = i;
                        frm_rcv_slot21[frm_rcv_num21++] = s2;
                        frm_lst[2][s2] = 1;
                        sendCts = 0;
                        netf = 0;
                        flag[1] = 0;
                        occupy_slot_2dem((int *)status, 1, i, sim_slot, sim_slot, 0);
                        i = i + sifs_slot + frame_slot + sifs_slot + ack_slot;
                    }
                }
                NAV[len][i] = NAV[len][i] - 1; // Reduce the NAV by 1.
            }

            if (status[3][i] == 0 && status[4][i] == 0)
            {
                if (channel_s[0][i] == 1 && channel_s[1][i] == 1)
                {
                    if (noCts[0] == 0 && noCts[1] == 0)
                    {
                        noCts[0] = 1;                                      // Raise the flag such that it should not send any ACK
                        noCts[1] = 1;                                      // Raise the flag such that it should not send any ACK
                        numberOfCollisions[0] = numberOfCollisions[0] + 1; // Increment the number of collisions by 1
                        numberOfCollisions[1] = numberOfCollisions[1] + 1; // Increment the number of collisions by 1
                    }
                }
            }
            else if (status[3][i] == 1 && status[4][i] == 0)
            {
                if (channel_s[0][i] == 1 && channel_s[1][i] == 1)
                {
                    if (sendCts == 1)
                        if (noCts[1] == 0)
                        {                                                      // Raise the flag such that it should not send any ACK
                            noCts[1] = 1;                                      // Raise the flag such that it should not send any ACK
                            numberOfCollisions[1] = numberOfCollisions[1] + 1; // Increment the number of collisions by 1
                        }
                }
            }
            else if (status[3][i] == 0 && status[4][i] == 1)
            {
                if (channel_s[0][i] == 1 && channel_s[1][i] == 1)
                {
                    if (sendCts == 1)
                        if (noCts[0] == 0)
                        {                                                      // Raise the flag such that it should not send any ACK
                            noCts[0] = 1;                                      // Raise the flag such that it should not send any ACK
                            numberOfCollisions[1] = numberOfCollisions[1] + 1; // Increment the number of collisions by 1
                        }
                }
            }
        }
    }
    //  numberOfCollisions;
    numberOfSuccess1 = sum_array_elements((int *)frm_lst, 0, sim_slot);
    numberOfSuccess2 = sum_array_elements((int *)frm_lst, 2, sim_slot);
    printf("numberOfSuccess1 %i\n", numberOfSuccess1);
    printf("numberOfSuccess2 %i\n", numberOfSuccess2);
    printf("numberOfCollisions1 %i\n", numberOfCollisions[0]);
    printf("numberOfCollisions2 %i\n", numberOfCollisions[1]);
    printf("OccupyChannel1 %f\n", OccupyChannel1);
    printf("OccupyChannel2 %f\n", OccupyChannel2);
    OccupyChannel1 = numberOfSuccess1 * 103;
    OccupyChannel2 = numberOfSuccess2 * 103;
    Throughput1 = ((numberOfSuccess1)*1500 * 8 / 10);
    Throughput2 = ((numberOfSuccess2)*1500 * 8 / 10);
    printf("Throughput1 %f\n", Throughput1);
    printf("Throughput2 %f\n", Throughput2);
    FI = OccupyChannel1 / OccupyChannel2;
    printf("fairness Index %f ", FI);
    // free(channel_s);
    free(frm_rcv_slot1);
    free(frm_rcv_slot2);
    free(frm_rcv_slot21);
    free(frm_slt1);
    free(frm_slt2);

    return;
}
