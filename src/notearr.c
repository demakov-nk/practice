#include "notearr.h"

#include <stdlib.h>

NoteArr init_note_arr(int capacity)
{
    NoteArr arr = {NULL, 0, 0};
    if (capacity <= 0) return arr;

    arr.buffer = (Note*) calloc(capacity, sizeof(Note));
    arr.capacity = capacity;

    return arr;
}

void delete_note_arr(NoteArr* arr)
{
    if (!arr) return;
    free(arr->buffer);
    arr->size = arr->capacity = 0;
}

void push_note_back(NoteArr* arr, Note new_note)
{
    if (!arr) return;

    if (arr->size == arr->capacity)
    {
        Note* new_buffer = realloc(arr->buffer, arr->capacity + 5);
        if (!new_buffer) return;
        arr->buffer = new_buffer;
        arr->capacity += 5;
    }

    arr->buffer[arr->size++] = new_note;
}

void print_notearr(WINDOW* win, const NoteArr* notearr)
{
    if (!win || !notearr) return;

    for (int i = 0; i < notearr->size; i++)
    {
        wprintw(win, "\nNote %d.\n", i+1);
        print_note(win, &notearr->buffer[i]);
    }
}

