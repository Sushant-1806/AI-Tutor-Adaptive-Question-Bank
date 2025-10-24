#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "datastructures.h"

// --- Linked List Implementation ---

// Function to create a new question node
QNode* create_node(Question q) {
    QNode* newNode = (QNode*)malloc(sizeof(QNode));
    if (!newNode) {
        printf("FATAL ERROR: Malloc failed in create_node\n");
        exit(1);
    }
    newNode->data = q;
    newNode->next = NULL;
    return newNode;
}

// Function to load all questions from file into a linked list
QNode* load_questions() {
    FILE* fp = fopen("questions.txt", "r");
    if (fp == NULL) {
        printf("Error: Could not open questions.txt. Make sure it's in the same folder.\n");
        return NULL;
    }

    QNode* head = NULL;
    QNode* tail = NULL;
    char line[512];

    while (fgets(line, sizeof(line), fp)) {
        Question q;
        char* token;

        // Use strtok to split the line by the '|' character
        token = strtok(line, "|");
        if (token) strcpy(q.topic, token);

        token = strtok(NULL, "|");
        if (token) strcpy(q.question_text, token);

        token = strtok(NULL, "|");
        if (token) strcpy(q.options[0], token); // Option A

        token = strtok(NULL, "|");
        if (token) strcpy(q.options[1], token); // Option B

        token = strtok(NULL, "|");
        if (token) strcpy(q.options[2], token); // Option C

        token = strtok(NULL, "|");
        if (token) strcpy(q.options[3], token); // Option D

        token = strtok(NULL, "|\n"); // Read until | or newline
        if (token) q.correct_answer = token[0];

        // Create a new node and add it to the end of the list
        QNode* newNode = create_node(q);
        if (head == NULL) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }
    fclose(fp);
    printf("Successfully loaded questions from file.\n");
    return head;
}


// --- HEAP IMPLEMENTATION ---
// We'll store the heap as an array of TopicTracker
TopicTracker heap[MAX_TOPICS];
int heap_size = 0;

// Helper to swap two items
void swap(TopicTracker* a, TopicTracker* b) {
    TopicTracker temp = *a;
    *a = *b;
    *b = temp;
}

// Function to "bubble up" a new item to its correct spot
void heapify_up(int index) {
    if (index == 0) return;
    int parent = (index - 1) / 2;
    // If child is bigger than parent, swap
    if (heap[index].mistake_count > heap[parent].mistake_count) {
        swap(&heap[index], &heap[parent]);
        heapify_up(parent); // Recurse
    }
}

// Function to insert into the heap
void heap_insert(TopicTracker item) {
    if (heap_size < MAX_TOPICS) {
        heap[heap_size] = item;
        heapify_up(heap_size);
        heap_size++;
    }
}

// Function to "bubble down" an item to its correct spot
void heapify_down(int index) {
    int left = 2 * index + 1;
    int right = 2 * index + 2;
    int largest = index;

    if (left < heap_size && heap[left].mistake_count > heap[largest].mistake_count)
        largest = left;
    if (right < heap_size && heap[right].mistake_count > heap[largest].mistake_count)
        largest = right;

    if (largest != index) {
        swap(&heap[index], &heap[largest]);
        heapify_down(largest);
    }
}

// Function to extract the MAX item (the root)
TopicTracker heap_extract_max() {
    if (heap_size <= 0) {
        TopicTracker empty = {"ERROR", 0};
        return empty;
    }
    TopicTracker max_item = heap[0];
    heap[0] = heap[heap_size - 1]; // Move last item to root
    heap_size--;
    heapify_down(0); // Fix the heap
    return max_item;
}