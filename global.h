#ifndef _GLOBAL_H_
#define _GLOBAL_H_
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

typedef struct linked_numbers
{
    int num;
    struct linked_numbers *next;
} link_num;

int create_traffic(int Arate, float slot_time);

double random_0_1();

int trans_rate;

float sifs_time, difs_time, sim_time, slot_time;

int max_back_off;
int frame_size;
int ack_size;
int rts_size;
int cts_size;
int cnt;
int sim_slot;
int frame_slot;
int ack_slot;
int rts_slot;
int cts_slot;
int sifs_slot;
int difs_slot;

int cw0;
int nwcw1, nwcw2;
int cw1, cw2;

int numberOfSuccess1;
int numberOfSuccess2;


float  OccupyChannel1, OccupyChannel2;

float FI;

float Throughput1, Throughput2;

int CSMA_con_comm(int *t_a, int *t_c, int n_a, int n_c);
int CSMA_hid_comm(int *t_a, int *t_c, int n_a, int n_c);
int CSMA_con_CA_comm(int *frm_slt1, int *frm_slt2, int a, int c);
int CSMA_hid_CA_comm(int *t_a, int *t_c, int n_a, int n_c);


void store_slots(int *t);
void print_slots(int *t, int n);
void initialize_array(int *a, int x, int y);
void copy_slot_array(int *dest, int dest_row, int col, int *src, int cnt);
void initialize_single_array(int *a, int x);

void occupy_slot_2dem(int *dest, int dest_row, int s_col, int f_col, int cnt, int value);
void occupy_slot_1dem(int *dest, int s_col, int f_col, int cnt, int value);
int sum_array_elements(int *dest, int row, int col);

#endif