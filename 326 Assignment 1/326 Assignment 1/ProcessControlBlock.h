#pragma once

#include <iostream>
#include "LinkedList.h"

using namespace std;

// PCB
struct processControlBlock
{
    processControlBlock(int id, int size, int* pageTable)
    {
        this->ID = id;
        this->size = size;
        this->pageTable = pageTable;
    }

    ~processControlBlock()
    {
        cout << this->ID << " deconstructed.\n";
    }

    int ID;
    int size;
    int* pageTable;
};