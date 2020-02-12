#pragma once

#include <iostream>
#include "ProcessControlBlock.h"
using namespace std;


// Nodes for Ready Queue
struct Node
{
    Node* next;
    processControlBlock* content;
};
// Linked List used for Ready Queue
class LinkedList
{
private:
    Node* head, * tail;
    int length;
public:
    LinkedList()
    {
        head = NULL;
        tail = NULL;
        length = 0;
    }

    // adds a node to the linked list
    void addNode(processControlBlock* pcb)
    {
        this->length++;

        Node* node = new Node();
        node->content = pcb;

        if (this->head == NULL)
        {
            head = node;
            tail = node;
        }
        else
        {
            tail->next = node;
            tail = node;
        }
    }

    // returns the size of the linked list
    int size()
    {
        return this->length;
    }

    // returns true if an ID is found in the linked list and false if not
    bool find(int id)
    {
        Node* tmp = new Node;
        tmp = this->head;
        while (tmp)
        {
            if (tmp->content->ID == id)
            {
                return true;
            }
            tmp = tmp->next;
        }
        return false;
    }

    // deletes the node from the linked list using the PID
    Node* deleteNode(int id)
    {
        Node* p;
        Node* cur = this->head;
        Node* prev = nullptr;

        while (cur)
        {
            if (cur->content->ID == id)
            {
                if (cur == head)
                {
                    head = cur->next;
                }
                else
                {
                    prev->next = cur->next;

                }
                this->length--;
                p = cur;
                return p;
            }
            prev = cur;
            cur = cur->next;
        }
    }

    // prints the contents of the linked list
    void displayAll()
    {
        Node* tmp = head;
        while (tmp)
        {
            cout << "ID: " << tmp->content->ID << "\nSize: " << tmp->content->size << "\nBlocks Taken:\n";
            // displaying the pagetable contents
            for (int i = 0; i < tmp->content->size; i++)
            {
                int* temp = tmp->content->pageTable;

                cout << i+1 << ")" << *(temp + i) + 1 << "\t";
            }

            tmp = tmp->next;
            cout << "\n\n";
        }
    }

    void displayPID()
    {
        if (head == NULL)
        {
            cout << "There are no processes running.\n";
            return;
        }
        Node* tmp = head;
        int counter = 0;
        cout << "\nProcess IDs that are still running: \n";
        while (tmp)
        {
            counter++;
            cout << counter << ") " << tmp->content->ID << "\n";
            tmp = tmp->next;
        }
    }

    int getHeadId()
    {
        return head->content->ID;
    }
};