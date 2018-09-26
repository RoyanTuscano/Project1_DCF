/**************code for Hidden communication******************/

int CSMA_CA_hid_comm(int *t_a, int *t_c, int n_a, int n_c)
{
    int t_global = 1; //global clock for concurrent network
    int aFrame = 1;
    int cFrame = 1;
    int no_of_collision = 0;
    for (; t_global < 10000; t_global++)
    {
        //check if any frame is transmitting
        if (t_a[aFrame] == t_global && t_c[cFrame] == t_global)  //this means there is collison
        {
        }
        else if (t_a[aFrame] <= t_global)                       //this means t_a[aFrame] is due to be sent
        {
        }
        else if (t_c[cFrame] <= t_global)                       //this means t_c[cFrame] is due to be sent
        {
        }
        else
        {
        }
    }
}