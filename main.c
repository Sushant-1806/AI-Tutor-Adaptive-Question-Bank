#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "datastructures.h" // Include your custom header!

// --- Global Variables ---
// This array will hold our mistake counts for the *last test*
TopicTracker mistake_log[MAX_TOPICS];
int num_topics_tracked = 0;

// Global pointer to the start of our question list
QNode* question_list_head = NULL;

// --- Function Declarations ---
void run_test();
void generate_schedule();
void free_question_list();
int find_topic_in_log(char* topic);
void print_topic_questions(char* topic); // <-- ADD THIS LINE

// --- Main Function ---
int main() {
    // Load questions once at the start
    question_list_head = load_questions();
    
    if (question_list_head == NULL) {
        printf("Failed to load questions. Exiting.\n");
        return 1;
    }

    int choice = 0;
    while (choice != 3) {
        printf("\n============================\n");
        printf("    AI Tutor Main Menu\n");
        printf("============================\n");
        printf("1. Take Full Test\n");
        printf("2. Generate My Study Schedule\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        
        // Safe way to get integer input
        if (scanf("%d", &choice) != 1) {
            // Clear the invalid input
            while (getchar() != '\n'); 
            choice = 0; // Reset choice
        }

        switch (choice) {
            case 1:
                run_test();
                break;
            case 2:
                generate_schedule();
                break;
            case 3:
                printf("Goodbye! Freeing memory...\n");
                break;
            default:
                printf("Invalid choice. Please enter 1, 2, or 3.\n");
        }
    }
    
    free_question_list(); 
    return 0;
}

// --- Test Engine ---

// Helper function to find a topic in our log
int find_topic_in_log(char* topic) {
    for (int i = 0; i < num_topics_tracked; i++) {
        if (strcmp(mistake_log[i].topic, topic) == 0) {
            return i; // Found it!
        }
    }
    return -1; // Not found
}

// Main function to run the test
void run_test() {
    if (question_list_head == NULL) {
        printf("Test cannot start. No questions loaded.\n");
        return;
    }
    
    // Reset the mistake log for this new test
    num_topics_tracked = 0;
    for(int i = 0; i < MAX_TOPICS; i++) {
        mistake_log[i].mistake_count = 0;
        strcpy(mistake_log[i].topic, "");
    }
    
    QNode* current = question_list_head;
    int question_num = 1;
    char user_answer;
    int correct_count = 0;

    while (current != NULL) {
        Question q = current->data;

        printf("\n-------------------------\n");
        printf("Q%d (%s): %s\n", question_num, q.topic, q.question_text);
        printf("  A) %s\n", q.options[0]);
        printf("  B) %s\n", q.options[1]);
        printf("  C) %s\n", q.options[2]);
        printf("  D) %s\n", q.options[3]);
        printf("Your answer (A, B, C, or D): ");
        
        scanf(" %c", &user_answer);       // Read the user's answer
        user_answer = toupper(user_answer); // Convert to uppercase

        // Clear input buffer
        while (getchar() != '\n');

        if (user_answer == q.correct_answer) {
            printf("Correct!\n");
            correct_count++;
        } else {
            printf("Incorrect. The correct answer was %c.\n", q.correct_answer);
            
            // LOG THE MISTAKE
            int topic_index = find_topic_in_log(q.topic);
            
            if (topic_index == -1) { 
                // First mistake for this topic
                if(num_topics_tracked < MAX_TOPICS) {
                    strcpy(mistake_log[num_topics_tracked].topic, q.topic);
                    mistake_log[num_topics_tracked].mistake_count = 1;
                    num_topics_tracked++;
                }
            } else {
                // Topic already in our log, just increment
                mistake_log[topic_index].mistake_count++;
            }
        }
        
        current = current->next;
        question_num++;
    }
    printf("\n--- Test Complete! ---\n");
    printf("You scored %d / %d\n", correct_count, question_num - 1);
    printf("Select 'Generate My Study Schedule' to see your weak areas.\n");
}

// --- Schedule Generator ---

// in main.c

// --- Schedule Generator ---

void generate_schedule() {
    printf("\n--- Your Personalized Study Schedule ---\n");
    
    if (num_topics_tracked == 0) {
        printf("You haven't taken a test or made no mistakes! Great job!\n");
        return;
    }

    // 1. Insert all tracked mistakes into the Max-Heap
    heap_size = 0; // Clear the heap (global in datastructures.c)
    for (int i = 0; i < num_topics_tracked; i++) {
        heap_insert(mistake_log[i]);
    }

    if (heap_size == 0) {
        printf("You made no mistakes! No schedule needed.\n");
        return;
    }
    
    // 2. Extract from the heap until it's empty
    int rank = 1;
    printf("Based on your last test, focus on these topics first:\n");
    while (heap_size > 0) {
        TopicTracker weakest_topic = heap_extract_max();
        
        // This is the original line
        printf("\n%d. STUDY: %s (You made %d mistake(s))\n", 
               rank, 
               weakest_topic.topic, 
               weakest_topic.mistake_count);
        
        // --- ADD THIS LINE ---
        // This calls your new function to print matching questions
        print_topic_questions(weakest_topic.topic);
        // --- END OF NEW LINE ---
        
        rank++;
    }
}
// in main.c

// ... (This is after your generate_schedule() function) ...

/**
 * @brief Prints all questions from the question bank that match a given topic.
 * * @param topic The name of the topic (e.g., "Pointers") to search for.
 */
void print_topic_questions(char* topic) {
    printf("  Here are some practice questions for '%s':\n", topic);
    
    QNode* current = question_list_head; // Start at the beginning of your main list
    int q_count = 1;
    int found_count = 0;

    while (current != NULL) {
        // Check if the topic of the current question matches
        if (strcmp(current->data.topic, topic) == 0) {
            Question q = current->data;
            printf("\n    %d. %s\n", q_count, q.question_text);
            printf("       A) %s\n", q.options[0]);
            printf("       B) %s\n", q.options[1]);
            printf("       C) %s\n", q.options[2]);
            printf("       D) %s\n", q.options[3]);
            printf("       (Scroll up for answer... or check the key)\n"); 
            // We hide the answer to make it a real quiz!
            // To show it, uncomment the line below:
            // printf("       (Correct Answer: %c)\n", q.correct_answer);
            
            q_count++;
            found_count++;
        }
        current = current->next; // Move to the next question in the list
    }

    if (found_count == 0) {
        printf("    (No practice questions found in the bank for this topic.)\n");
    }
}

// --- Memory Management ---

void free_question_list() {
    QNode* current = question_list_head;
    QNode* next_node = NULL;
    while (current != NULL) {
        next_node = current->next;
        free(current);
        current = next_node;
    }
    question_list_head = NULL;
}