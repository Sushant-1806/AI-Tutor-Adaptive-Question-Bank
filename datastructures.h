#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

// --- Question Bank (Linked List) ---

// A struct to hold one question's data
typedef struct {
    char topic[50];
    char question_text[256];
    char options[4][100]; // Array of 4 strings for options
    char correct_answer;    // 'A', 'B', 'C', or 'D'
} Question;

// A node in your linked list
typedef struct QNode {
    Question data;        // The data for this node
    struct QNode* next; // Pointer to the next node
} QNode;

// --- Mistake Tracking (for Heap) ---
#define MAX_TOPICS 10

// A struct to count mistakes for one topic
typedef struct {
    char topic[50];
    int mistake_count;
} TopicTracker;

// --- FUNCTION PROTOTYPES ---

// From datastructures.c
QNode* load_questions();
void heap_insert(TopicTracker item);
TopicTracker heap_extract_max();
extern int heap_size; // Lets main.c know about this global variable

// From main.c (so datastructures.c could use them if needed, though not in this design)
// (None needed for this simple project)

#endif