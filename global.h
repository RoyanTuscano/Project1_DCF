#ifndef _GLOBAL_H_
#define _GLOBAL_H_
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>


int create_traffic(int Arate, float slot_time);
float random_0_1();
void store_slots(int *t);
void print_slots(int *t ,int n);

typedef struct linked_numbers{
int num;
struct linked_numbers *next;
}link_num;

#endif