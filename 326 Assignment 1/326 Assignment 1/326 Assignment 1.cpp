// 326 Assignment 1.cpp : This file contains the 'main' function. Program execution begins and ends here.

#include <iostream>
#include <time.h>
#include "LinkedList.h"
#include "ProcessControlBlock.h"

using namespace std;

/*
=============================
=== Function Declarations ===
=============================
*/
void PrintMenu();
int randNum(int limit);

/* 
=======================
=== Structs/Classes === 
=======================
*/
// MBT
struct MemoryBlockTable
{
    ~MemoryBlockTable()
    {
    }

    int blocks; // number of free blocks in MBT
    bool stateOfMem[512]; // to show which blocks are occupied
};

/*
============================
=== VariableDeclarations ===
============================
*/
int choice;
int newId;
int memSize;
int counter;
int newLine;
int memLeft;
int userID;
int rqSize;
string exitChoice;
Node* temp;


/*
=====================
=== MAIN FUNCTION ===
=====================
*/
int main()
{
    // creating a memory block table (MBT)
    MemoryBlockTable MBT;
    // setting up the initial 512 blocks
    MBT.blocks = 512;
    for (int i = 0; i < 512; i++)
    {
        MBT.stateOfMem[i] = false;
    }

    // allocating 32 blocks for OS
    MBT.blocks -= 32;
    for (int i = 0; i < 32; i++) 
    {
        MBT.stateOfMem[i] = true;
    }
    cout << "Memory Block Table created.\n";

    // creating empty ready queue (RQ)
    LinkedList RQ;
    cout << "Ready Queue created.\n";

    do 
    {
        // Prints the menu and asks user for their choice:
        PrintMenu();
        cout << "Please enter your choice (1-4): ";
        cin >> choice;

        switch(choice) 
        {
        case 1: // === Initiate a process ===
            cout << "\n*Initializing a process*\n";
            memSize = randNum(94) + 26; // making a random memory size from 25 to 120 blocks

            // checking if there is enough space in the MBT
            if (memSize > MBT.blocks) 
            {
                cerr << "There are not enough space to intitiate another process.\n";
                cerr << "MBT memory: " << MBT.blocks << "\nProgram size: " << memSize << "\n";
                break;
            }

            // creating random processer id (PID) from 100 - 999
            // and checks if there is an existing pid and recreates a new one
            do 
            {
                newId = randNum(899) + 100;
            } while (RQ.find(newId));

            // creating new page table with size of memSize
            int* pt;
            pt = new int[memSize];
            counter = 0;
            memLeft = memSize;
            // allocating space for new program
            for (int i = 0; i < 512; i++)
            {
                // finding free block in MBT memory
                if (!MBT.stateOfMem[i])
                {
                    memLeft--; // subtracts the remaining memory to be allocated
                    MBT.blocks--; // subtracting block from amount
                    MBT.stateOfMem[i] = true; // setting block to occupied
                    *(pt + counter) = i; // placing the block number into the page table
                    counter++; // moving to next array space in pt
                }

                if (memLeft == 0)
                    break;
            }
            processControlBlock* pcb;
            pcb = new processControlBlock(newId, memSize, pt);
            RQ.addNode(pcb);
            break;
        case 2: // === Print system state ===
            cout << "\n*Printing system state*\n";
            cout << "Available left: " << MBT.blocks << "\n";
            
            // printing MBT state of memory array
            counter = 0;
            newLine = 0;
            for (int i = 0; i < 512; i++)
            {
                // prints the array in groups of 10
                if (counter == 10)
                {
                    cout << " <-" << i << " ";
                    counter = 0;
                    newLine++;
                }

                // makes a new line every 5 groups
                if (newLine == 5)
                {
                    cout << "\n";
                    newLine = 0;
                }

                if (MBT.stateOfMem[i])
                    cout << "|";
                else
                    cout << "O";
                counter++;
                if (i == 511)
                    cout << " <-" << i + 1 << "\n";
            }

            // outputing the ready queue contents
            RQ.displayAll();

            break;
        case 3: // === Terminate process with a specific PID ===
            RQ.displayPID();
            cout << "\nPID of process to be terminated: ";
            cin >> userID;

            if (RQ.find(userID))
            {
                Node *pNode = RQ.deleteNode(userID);
                MBT.blocks += pNode->content->size; // adds the amount of space back to the total amount of free space in MBT
                
                // reopening the array space
                int* pPT = pNode->content->pageTable;
                for (int i = 0; i < pNode->content->size; i++)
                {
                    MBT.stateOfMem[pPT[i]] = false;
                }
                delete pNode->content->pageTable; //deleting page table
                delete pNode->content; // deleting pcb
                delete pNode; // deleting node
            }
            else
            {
                cout << "PID not found.\nReturning to menu.\n";
            }
            break;
        case 4: // === Exit ===
            RQ.displayPID();
            cout << "You are about to exit, continue? (Y/N): ";
            cin >> exitChoice;
            // asks the user if they want to continue to exit
            if (exitChoice == "Y" || exitChoice == "y")
            {
                rqSize = RQ.size();
                for (int i = rqSize; i > 0; i--)
                {
                    temp = RQ.deleteNode(RQ.getHeadId());
                    delete temp->content->pageTable; //deleting page table
                    delete temp->content; // deleting pcb
                    delete temp; // deleting node
                }
            }
            else if (exitChoice == "N" || exitChoice == "n")
            {
                choice = 0;
            }
            break;

        }
    } while (choice != 4);
}

/*
=================
=== Functions ===
=================
*/
// prints the menu
void PrintMenu() 
{
    cout << "\n***MENU***\n1 - Initiate a response\n2 - Print system state\n3 - Terminate process with a specific PID\n4 - Exit\n";
}

// generates a random number from 0 - limit
int randNum(int limit) 
{
    srand(time(NULL));
    int i = rand() % limit + 1;
    return i;
}