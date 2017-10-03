//
// Created by Yujian Zhang on 10/3/17.
//

#ifndef COEN383_P2_OUTPUT_H
#define COEN383_P2_OUTPUT_H

#include <stdio.h>

typedef struct {
    int process_id[];
    int size;
} history_t;

/**
 * print out the history into the console
 *
 * @param history pointer to the history structure
 */
void print_history(history_t *history);




#endif //COEN383_P2_OUTPUT_H
