#include "global.h"
link_num *head;

int main()
{
    int Arate_1 = 50;
    float slot_time = 0.00002;                      //this is in us for testing, to translate to slots
    int n_a_slots=create_traffic(Arate_1, slot_time);
    int t_a[n_a_slots]; //array of slots generated at A
    store_slots(t_a);   ///store the heap values in stack of t_a
    print_slots(t_a,n_a_slots);   //print t_a
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
        free(tmp);  //free the heap
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

void print_slots(int *t ,int n)
{
    for(int i=0;i<n; i++)
    printf("%d, ",t[i]);
}

float random_0_1()
{
    return rand() / (float)RAND_MAX;
}