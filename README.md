# AI Tutor — C Data Structures Quiz App

A terminal-based quiz application written in C that tests your knowledge of core CS topics and generates a personalized study schedule based on your mistakes. Built around a **linked list** (question bank) and a **max-heap** (mistake tracker).

---

## Features

- **Multiple choice quiz** — loads questions dynamically from a text file
- **Mistake tracking** — logs which topics you got wrong and how many times
- **Personalized study schedule** — uses a max-heap to surface your weakest topics first
- **Practice question review** — shows all relevant questions for each weak topic after the test

---

## Project Structure

```
.
├── main.c              # Menu, test engine, schedule generator
├── datastructures.c    # Linked list + max-heap implementations
├── datastructures.h    # Structs, constants, and function prototypes
├── questions.txt       # Question bank (pipe-delimited)
└── README.md
```

---

## How to Compile & Run

### Prerequisites
- GCC (or any C99-compatible compiler)

### Compile
```bash
gcc main.c datastructures.c -o ai_tutor
```

### Run
```bash
./ai_tutor
```

> Make sure `questions.txt` is in the **same directory** as the executable.

---

## Question Bank Format

Questions are stored in `questions.txt`, one per line, using `|` as a delimiter:

```
Topic|Question Text|Option A|Option B|Option C|Option D|Correct Answer
```

**Example:**
```
Pointers|What does & do?|Get value|Get address|Dereference|Declare|B
```

- The correct answer must be a single character: `A`, `B`, `C`, or `D`
- To add more questions, simply append new lines in the same format

---

## Data Structures Used

### Linked List (`QNode`)
- Stores all questions loaded from `questions.txt`
- Dynamically allocated; traversed sequentially during the test
- Freed on exit via `free_question_list()`

### Max-Heap (`TopicTracker[]`)
- Fixed-size array of up to `MAX_TOPICS` (10) entries
- Each entry tracks a topic name and its mistake count
- After a test, mistakes are inserted into the heap so the topic with the most errors is always at the root
- Extracted in order to produce a ranked study schedule

---

## Usage Walkthrough

```
============================
    AI Tutor Main Menu
============================
1. Take Full Test
2. Generate My Study Schedule
3. Exit
```

1. **Take Full Test** — answer all loaded questions; mistakes are logged by topic
2. **Generate My Study Schedule** — displays topics ranked by mistake count, with practice questions for each
3. **Exit** — frees all allocated memory and quits

---

## Limitations & Potential Improvements

| Area | Current Behavior | Possible Improvement |
|---|---|---|
| Topic cap | Max 10 topics (`MAX_TOPICS`) | Dynamic heap allocation |
| Question order | Always sequential | Fisher-Yates shuffle |
| Persistence | Mistakes reset each run | Save/load results to file |
| Answer feedback | Shows correct letter only | Show correct option text |
| Input validation | Basic `scanf` | Robust line-based input |
