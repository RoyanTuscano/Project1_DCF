#ifndef _GLOBAL_H_
#define _GLOBAL_H_
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>


typedef struct linked_numbers{
int num;
struct linked_numbers *next;
}link_num;

int create_traffic(int Arate, float slot_time);
float random_0_1();
float slot_time;
float SIFS_duration;
float DIFS_duration;
int CW_0;
int CW_max;



int CSMA_CA_hid_comm(int *t_a, int *t_c, int n_a, int n_c);
void store_slots(int *t);
void print_slots(int *t ,int n);



#endif