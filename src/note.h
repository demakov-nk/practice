#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <form.h>

#define SURNAME_LEN 20
#define NAME_LEN 20
#define INSURANCE_LEN 8
#define DAY_LEN 2
#define MONTH_LEN 2
#define YEAR_LEN 4

#define FIELD_START_POS 18

typedef struct Note
{
    char surname[SURNAME_LEN + 1];
    char name[NAME_LEN + 1];
    int insurance;
    int birthday[3];
} Note;

void write_note(const Note* noteptr, size_t count, FILE* file);

void print_note(WINDOW* win, const Note* note);
