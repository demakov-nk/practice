#pragma once

#include "note.h"

typedef struct NoteArr
{
    Note* buffer;
    int size;
    int capacity;
} NoteArr;

NoteArr init_note_arr(int capacity);
void clear_note_arr(NoteArr* arr);
void push_note_back(NoteArr* arr, Note new_note);

void print_notearr(WINDOW* win, const NoteArr* notearr);

