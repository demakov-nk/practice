#pragma once

#include "notearr.h"

#include <stdbool.h>
#include <form.h>
#include <menu.h>

#define FILENAME "database.dema"

typedef union Interface
{
    struct {
        int n_fields;
        FIELD** fields;
        FORM* form;
    };

    struct {
        int n_choices;
        const char** choices;
        ITEM** items;
        MENU* menu;  
    };
} Interface;

enum {
    START_INTERF_CODE,
    MAIN_INTERF_CODE,
    LEAVE_INTERF_CODE,
    INPUT_DATE_INTERF_CODE,
    INPUT_NOTE_INTERF_CODE
};

enum {
    PROCESS_DONE,
    PROCESS_ABORTED,
    INVALID_ARG
};

extern Interface start_interf;
extern Interface main_interf;
extern Interface leave_interf;
extern Interface input_date_interf;
extern Interface input_note_interf;

void init_interf(Interface* interf, int interf_code, WINDOW* window);
void init_input_fields(Interface* interf, int interf_code);
void destroy_all(WINDOW* main_win, FILE* db_file);

ITEM* menu_process(WINDOW* win, MENU* menu);
int form_process(WINDOW* win, Interface* interf, int interf_code, void* dest);

bool read_input(Interface* interf, int interf_code, void* dest);

int copy_str(char* dest, const char* src);
void capitalize(char* str);
bool is_leap_year(int y);
bool check_date(int date[3]);

bool same_date(int date1[3], int date2[3]);

NoteArr search(int date[3], const FILE* file);
