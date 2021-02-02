/*
 * buffer.h
 *
 *  Created on: 30-Jan-2021
 *      Author: YURI
 */

#ifndef BUFFER_H
#define BUFFER_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "r_cg_macrodriver.h"

typedef struct cbuff_{
    uint8_t * buff;
    int start;
    int end;
    int size;
    int count;
} cbuff_t;

cbuff_t * cbuff_new(int size);
void cbuff_add(cbuff_t * cb, int elem);
int cbuff_remove(cbuff_t * cb);
void cbuff_print(cbuff_t * cb);
void cbuff_delete(cbuff_t * cb);

int cbuff_data_count(cbuff_t * cb);
void cbuff_reset(cbuff_t * cb);


#endif
