#include "hid_comm.h"
//intialize the status of the channel where status 1&2 are for station 1 and 3&4 are stateion 2
// 1 ->>node a , 2 for node c , 3 for node b and node d ..indicating there station if active or not
static int status[4][500000];
//Amount of time to differ the communication for the other stations in the domain...1 is for channel and 2 for ch 2
static int NAV[2][500000];
// This indicates if the slot is in Diffs mode for either of te channel..so that they dont transmit...1 is for channel and 2 for ch 2
static int updt_slot[2][500000];
//1 & 2 indicites if the frame is successfully transmitted or not...1 is for channel and 2 for ch 2
static int frm_lst[2][500000];

int CSMA_con_comm(int *frm_slt1, int *frm_slt2, int a, int c)
{
    sim_slot = 500000;
    //This indicate if the channel is busy or not..
    int *channel_s = malloc(sim_slot * sizeof(int));
    //slots numbers at which the frames are recived for station 1 and 2..defined clearly yet
    int *frm_rcv_slot1 = malloc(1000 * sizeof(int));
    int *frm_rcv_slot2 = malloc(1000 * sizeof(int));
    int *frm_rcv_slot21 = malloc(1000 * sizeof(int));

    //iniitialize all the arrays to zero indiciating nothing is active yet..
    initialize_array((int *)status, 4, sim_slot);
    initialize_array((int *)NAV, 2, sim_slot);
    initialize_single_array(channel_s, sim_slot);
    initialize_array((int *)updt_slot, 2, sim_slot);
    initialize_array((int *)frm_lst, 2, sim_slot);

    //to indiicate the total time duratiuon for which the channels are active
    OccupyChannel1 = 0;
    OccupyChannel2 = 0;

    //back off for channel 1 and 2 signal
    int bck_of[2] = {0, 0};
    //Number of collision happened at each station
    int numberOfCollisions[2] = {0, 0};
    //flag indicates that its reacdy to send the data on either of the frames when both 1 than collision
    int flag[2] = {0, 0};
    //This is for doubling back off if the collision occurs
    int prev_back_off[2] = {0, 0};

    //when either of station is transmitting
    int netf = 0;

    //used by receiver to send the acknowledgement..
    int sendAck=0;
    //node incrementer for the frames received stack
    int frm_rcv_num1 = 0;
    int frm_rcv_num2 = 0;

    //becomes 1 when no ack is received for the receiver
    int noAck = 0;
    int frm_rcv_num21 = 0;

    //frames that want to send the data on the respective channel 1 & 2
    int s1, s2;
    //carry the simulation here
    for (int i = 0; i < sim_slot; i++)
    {
        //check if the slot is available to send the data for either of the frame
        if (channel_s[i] == 0)
        {
            for (int s = 0; s < a; s++)
            {
                //check the node can send the frame w.r.t to its slots no wuth global time , and see if that frame is already succcessfully transmitted
                //or not
                if (frm_slt1[s] != 0 && frm_lst[0][s] == 0 && i >= frm_slt1[s])
                {
                    status[0][i] = 1;
                    s1 = s;
                    break;
                }
            }
            for (int s = 0; s < c; s++)
            {
                //check the node can send the frame w.r.t to its slots no wuth global time , and see if that frame is already succcessfully transmitted
                //or not
                if (frm_slt2[s] != 0 && frm_lst[1][s] == 0 && i >= frm_slt2[s])
                {
                    status[1][i] = 1;
                    s2 = s;
                    break;
                }
            }
        }
        //len stands for the station number
        for (int len = 0; len < 2; len++)
        {
            int i_prev, i_next;
            //variables so that we dont decrement when the slot is zero
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
            //checks if the channel is available to send the data and its not in the DIFS slot...and the back of is zerp
            if (status[len][i] == 1 && channel_s[i] == 0 && updt_slot[len][i_prev] == 0 && updt_slot[len][i] == 0 && bck_of[len] == 0)
            {
                //update the slot so that its in DIFS time period
                occupy_slot_2dem((int *)updt_slot, len, i_next, i + difs_slot, sim_slot, 1);
                //update the status of the node A or B whichever so that its in diffs position
                occupy_slot_2dem((int *)status, len, i_next, i + difs_slot, sim_slot, 1);
            }
            //if the send just finished DIFFS slot than strat with back off
            else if (status[len][i] == 1 && channel_s[i] == 0 && updt_slot[len][i_prev] == 1 && updt_slot[len][i] == 0 && bck_of[len] == 0)
            {
                //This code is to choose the back off for Node-1, given that it has not undergone any recent collision (doubleCW(1) == 0).
                if (len == 0)
                {
                    //generate random back off
                    nwcw1 = pow(2, numberOfCollisions[len]);
                    if (numberOfCollisions[len] > 10)
                        cw1 = 1024;
                    else
                        cw1 = rand() % (nwcw1 * cw0);
                    //keep the status of the respective node 1 for the duration of the back off time
                    occupy_slot_2dem((int *)status, len, i_next, i + cw1, sim_slot, 1);
                    //and put the back off inidicator to 1 for that node
                    bck_of[len] = 1;
                }
                else if (len == 1) // The following lines of code are for Node-2, given that it has not undergone any recent collision (doubleCW(1) == 0).
                {
                    //generate random back off
                    nwcw2 = pow(2, numberOfCollisions[len]);
                    if (numberOfCollisions[len] > 10)
                        cw2 = 1024;
                    else
                        cw2 = rand() % (nwcw2 * cw0);
                    //keep the status of the respective node 1 for the duration of the back off time
                    occupy_slot_2dem((int *)status, len, i_next, i + cw2, sim_slot, 1);
                    //and put the back off inidicator to 1 for that node
                    bck_of[len] = 1;
                }
            }
            // channel is free and the node is in back off period and no one is transmitting indicatecd by flag
            else if (status[len][i] == 1 && channel_s[i] == 0 && bck_of[len] == 1 && flag[len] == 0)
            {
                if (len == 0)
                {
                    //decraese the back off
                    if (cw1 > 0)
                    {
                        // Reduce the back off until it becomes 0. cw1 is the back off for Node - 1
                        cw1 = cw1 - 1;
                        //proceed the time slot by 1
                        status[len][i_next] = 1;
                    }
                    //if the back off has become zero than we should start transmitting
                    if (cw1 == 0)
                    {
                        flag[len] = 1;
                        bck_of[len] = 0;
                        //indiates that its was in back of in previous frame for the next calculatioins
                        prev_back_off[len] = 1;
                        //make the status of that node to indicate its transmitting
                        occupy_slot_2dem((int *)status, len, i_next, i + frame_slot, sim_slot, 1);
                        //check if either of the transmiiting nde is sending.if zero than no one is trasmtting
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
                        occupy_slot_2dem((int *)status, len, i_next, i + frame_slot, sim_slot, 1);
                        netf = flag[0] || flag[1];
                    }
                }
                //indicates nsome omne is ready to send the data
                if (netf == 1)
                {
                    //node 1 is sending
                    if (flag[0] == 1 && flag[1] == 0)//status to 1 till the acknowledghement is received
                        occupy_slot_2dem((int *)status, 0, i, i + frame_slot + sifs_slot + ack_slot + 1, sim_slot, 1);
                    else if (flag[0] == 0 && flag[1] == 1)//node 2 is sendiong
                        occupy_slot_2dem((int *)status, 1, i, i + frame_slot + sifs_slot + ack_slot + 1, sim_slot, 1); //both are sending and there uis collision
                    else if (flag[0] == 1 && flag[1] == 1)
                    {
                        occupy_slot_2dem((int *)status, 0, i, i + frame_slot + sifs_slot + ack_slot + 1, sim_slot, 1);
                        occupy_slot_2dem((int *)status, 1, i, i + frame_slot + sifs_slot + ack_slot + 1, sim_slot, 1);
                        // It is certain to undergo the collision.Hence, in the next cycle the collision window must be doubled.
                        cnt = cnt + 1;
                        noAck = 1;
                        // Under the condition of collision no Acknowledgments must be sent from the receiver to the sender.
                    }
                }
            }
            //indicate that back of is just over and that the frame has taken over to transmit
            else if (status[len][i] == 1 && bck_of[len] == 0 && prev_back_off[len] == 1 && flag[len] == 1)
            {
                  
                //status occupied the channel til;l the ack is received no other work can be done by the trasmittion nodes
                occupy_slot_1dem(channel_s, i, i + frame_slot + sifs_slot + ack_slot, sim_slot, 1);
                if (len == 0)   //inidcate for how much duration the of the channnel the channel is taken or occupied 
                    OccupyChannel1 = OccupyChannel1 + frame_slot + sifs_slot + ack_slot;
                else
                    OccupyChannel2 = OccupyChannel2 + frame_slot + sifs_slot + ack_slot;
                //set prev back of to zerp
                prev_back_off[len] = 0;
                //this is to indicate total length of transmission 
                //decrement for every clock iternation so that no data is trasnmitted
                NAV[len][i] = frame_slot + sifs_slot + ack_slot; 
                NAV[len][i_next] = NAV[len][i] - 1;        
                //status of node that its occupied till the frameslot is finisged duration    
                occupy_slot_2dem((int *)status, len, i_next, i + frame_slot, sim_slot, 1);

                //colliusion has occured
                if (flag[1] == 1 && flag[0] == 1)
                {
                    noAck = 1; //no acknowledgement will be sent
                    numberOfCollisions[0] = numberOfCollisions[0] + 1;
                    numberOfCollisions[1] = numberOfCollisions[1] + 1;
                }
                flag[len] = 0;
            }
            //The node wants to data to trasmit the channel is busy and the back of len is set..
            else if (status[len][i] == 1 && channel_s[i] == 1 && bck_of[len] == 1)
            {
            
                //do nothning sit idle and incremnt the status to check again
                if (len == 0)
                {
                    status[len][i_next] = 1;
                    //if channle is becoming free in next cycle than unset the sending flag
                    if (channel_s[i + 1] == 0)
                        netf = 0;
                }
                else if (len == 1)
                {
                    status[len][i_next] = 1;
                    if (channel_s[i + 1] == 0)
                        netf = 0;
                }
            }
            //the channel is busy and the transmission is still in the trasnmission slots
            else if (channel_s[i] == 1 && (NAV[len][i] > sifs_slot + ack_slot))
            {
              
                NAV[len][i_next] = NAV[len][i] - 1;           // decrease tehe NAVC l;emgth
                if (NAV[len][i_next] == sifs_slot + ack_slot) // if transmission is juxt finishing
                    if (status[0][i] == 1 && len == 0)      //inidcate the station is in charge
                    {
                        occupy_slot_2dem((int *)status, 2, i, i + sifs_slot + ack_slot, sim_slot, 1); //time to send acknowledgement now from the node, go to receiver
                        status[0][i_next] = 0;                                                        //node is done with transmission and shouldnt be transmission state
                    }
                    else if (status[1][i] == 1 && len == 1)
                    {
                        occupy_slot_2dem((int *)status, 3, i, i + sifs_slot + ack_slot, sim_slot, 1); 
                        status[1][i_next] = 0;                                                     
                    }
            }
            else if (channel_s[i] == 1 && (status[2][i] == 1 || status[3][i] == 1) && NAV[len][i] > ack_slot)   //either of the receiver are transmitting and in siffs slot
                NAV[len][i_next] = NAV[len][i] - 1; // redcue for the siffs slot
            else if (channel_s[i] == 1 && (status[2][i] == 1 || status[3][i] == 1) && NAV[len][i] == ack_slot)  //finsihged with sifs
            {
                if (noAck == 1) //no ack...means collision
                {
                    NAV[len][i_next] = NAV[len][i] - 2; //reduce the NAV..so that wait period wuill get over without transmission of ack frame
                    noAck = 0;                          
                }   
                else
                {
                    sendAck = 1;                        // send ack so that data is successfully transmitted
                    NAV[len][i_next] = NAV[len][i] - 1;
                }
            }
            else if (channel_s[i] == 1 && (status[2][i] == 1 || status[3][i] == 1) && NAV[len][i] == 1) //finsiged with acknowledgement duration
            {
                if (status[2][i] == 1)  //receiver is still in transmission with ack
                {
                    if (sendAck == 1)   //data successfully send
                    {
                       // printf("%i, ", i);
                        frm_rcv_slot1[frm_rcv_num1++] = i;
                        frm_lst[0][s1] = 1;     //indicate that the frame was  successfully transmitetd
                        sendAck = 0;    
                        netf = 0;
                        flag[0] = 0;
                        occupy_slot_2dem((int *)status, 0, i, sim_slot, sim_slot, 0);
                    }
                }
                else if (status[3][i] == 1)
                {
                    if (sendAck == 1)
                    {
                        frm_rcv_slot2[frm_rcv_num2++] = i;
                        frm_rcv_slot21[frm_rcv_num21++] = s2;
                        frm_lst[1][s2] = 1;
                        sendAck = 0;
                        netf = 0;
                        flag[1] = 0;
                        occupy_slot_2dem((int *)status, 1, i, sim_slot, sim_slot, 0);
                    }
                }
                NAV[len][i] = NAV[len][i] - 1; // Reduce the NAV by 1.
            }
        }
    }
    //  numberOfCollisions;
    numberOfSuccess1 = sum_array_elements((int *)frm_lst, 0, sim_slot);
    numberOfSuccess2 = sum_array_elements((int *)frm_lst, 1, sim_slot);
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
    free(channel_s);
    free(frm_rcv_slot1);
    free(frm_rcv_slot2);
    free(frm_rcv_slot21);

    return;
}
