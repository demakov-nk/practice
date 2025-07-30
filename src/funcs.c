#include "funcs.h"

#include "header.h"
#include "note.h"

#include <ncurses.h>

Interface start_interf = {
    .n_fields = 1,
    .fields = NULL,
    .form = NULL
};
Interface main_interf = {
    .n_choices = 3,
    .choices = NULL,
    .items = NULL,
    .menu = NULL
};
Interface leave_interf = {
    .n_choices = 2,
    .choices = NULL,
    .items = NULL,
    .menu = NULL
};
Interface input_date_interf = {
    .n_fields = 3,
    .fields = NULL,
    .form = NULL
};
Interface input_note_interf = {
    .n_fields = 6,
    .fields = NULL,
    .form = NULL
};

void init_interf(Interface* interf, int interf_code, WINDOW* window)
{
    static const char* all_choices[2][3] = {
        {
            "Add new note",
            "Search by birthday",
            "Leave"
        },
        {
            "Stay", 
            "Leave"
        }
    };

    switch (interf_code)
    {
        case START_INTERF_CODE:
        {
            interf->n_fields = 1;
            interf->fields = (FIELD**) calloc(interf->n_fields + 1, sizeof(FIELD*));
            init_input_fields(interf, interf_code);
            interf->form = new_form(interf->fields);

            set_form_win(interf->form, window);
            set_form_sub(interf->form, window);
        } break;
        
        case LEAVE_INTERF_CODE: interf->n_choices = 2;
        case MAIN_INTERF_CODE:
        {
            interf->choices = (const char**) calloc(interf->n_choices, sizeof(char*));
            interf->items = (ITEM**) calloc(interf->n_choices + 1, sizeof(ITEM*));
            for (int i = 0; i <= interf->n_choices; i++)
            {
               interf->choices[i] = all_choices[interf_code][i];
               interf->items[i] = new_item(all_choices[interf_code - 1][i], "");
            }
            interf->items[interf->n_choices] = (ITEM*) NULL;
            interf->menu = new_menu(interf->items);
        
            set_menu_win(interf->menu, window);
            set_menu_sub(interf->menu, derwin(window, 3, 20, 3, 0));
            set_menu_mark(interf->menu, ">");
        } break;

        case INPUT_DATE_INTERF_CODE: 
        case INPUT_NOTE_INTERF_CODE:
        {
            interf->fields = (FIELD**) calloc(interf->n_fields + 1, sizeof(FIELD*));
            init_input_fields(interf, interf_code);
            interf->form = new_form(interf->fields);

            set_form_win(interf->form, window);
            set_form_sub(interf->form, window);
        } break;
    }
}

void init_input_fields(Interface* interf, int interf_code)
{
    if (!interf) return;

    switch (interf_code)
    {
        case START_INTERF_CODE:
        {
            interf->fields[0] = new_field(1, 5, 2, 24, 0, 0);
            interf->fields[1] = NULL;

            set_field_type(interf->fields[0], TYPE_INTEGER, 5, 5, 5);

            return;
        } break;
        
        case INPUT_DATE_INTERF_CODE:
        {
            interf->fields[0] = new_field(1, DAY_LEN, 2, 0, 0, 0);
            interf->fields[1] = new_field(1, MONTH_LEN, 2, DAY_LEN+1, 0, 0);
            interf->fields[2] = new_field(1, YEAR_LEN, 2, DAY_LEN+1+MONTH_LEN+1, 0, 0);
            interf->fields[3] = NULL;
        
            set_field_type(interf->fields[0], TYPE_INTEGER, DAY_LEN, DAY_LEN, DAY_LEN);
            set_field_type(interf->fields[1], TYPE_INTEGER, MONTH_LEN, MONTH_LEN, MONTH_LEN);
            set_field_type(interf->fields[2], TYPE_INTEGER, YEAR_LEN, YEAR_LEN, YEAR_LEN);
        } break;

        case INPUT_NOTE_INTERF_CODE:
        {
            interf->fields[0] = new_field(1, SURNAME_LEN, 0, FIELD_START_POS, 0, 0);
            interf->fields[1] = new_field(1, NAME_LEN, 2, FIELD_START_POS, 0, 0);
            interf->fields[2] = new_field(1, INSURANCE_LEN, 4, FIELD_START_POS, 0, 0);
            interf->fields[3] = new_field(1, DAY_LEN, 6, FIELD_START_POS, 0, 0);
            interf->fields[4] = new_field(1, MONTH_LEN, 6, FIELD_START_POS+DAY_LEN+1, 0, 0);
            interf->fields[5] = new_field(1, YEAR_LEN, 6, FIELD_START_POS+DAY_LEN+1+MONTH_LEN+1, 0, 0);
            interf->fields[6] = NULL;
        
            set_field_type(interf->fields[0], TYPE_ALPHA);
            set_field_type(interf->fields[1], TYPE_ALPHA);
            set_field_type(interf->fields[2], TYPE_INTEGER, INSURANCE_LEN, INSURANCE_LEN, INSURANCE_LEN);
            set_field_type(interf->fields[3], TYPE_INTEGER, DAY_LEN, DAY_LEN, DAY_LEN);
            set_field_type(interf->fields[4], TYPE_INTEGER, MONTH_LEN, MONTH_LEN, MONTH_LEN);
            set_field_type(interf->fields[5], TYPE_INTEGER, YEAR_LEN, YEAR_LEN, YEAR_LEN);
        } break;

        default: return;
    }

    for (int i = 0; i < interf->n_fields; i++)
        set_field_back(interf->fields[i], A_UNDERLINE);
}

void destroy_all(WINDOW* main_win, FILE* db_file)
{   
    /* Очистка всех меню */
    for (int i = 0; i < 3; i++)
    {
        free_item(main_interf.items[i]);
        if (i < 2) free_item(leave_interf.items[i]);   
    }
    free_menu(main_interf.menu);
    free_menu(leave_interf.menu);

    /* Очистка всех форм */
    int n_fields_arr[] = {start_interf.n_fields, input_date_interf.n_fields, input_note_interf.n_fields};
    free_form(start_interf.form);
    free_form(input_date_interf.form);
    free_form(input_note_interf.form);
    for (int i = 0; i < n_fields_arr[0] || i < n_fields_arr[1] || i < n_fields_arr[2]; i++)
    {
        if (i < n_fields_arr[0]) free_field(start_interf.fields[i]);
        if (i < n_fields_arr[1]) free_field(input_date_interf.fields[i]);
        if (i < n_fields_arr[2]) free_field(input_note_interf.fields[i]);
    }

    /* Удаление окна */
    if (main_win) delwin(main_win);

    /* Зарытие файла */
    fclose(db_file);

    endwin();
}

ITEM* menu_process(WINDOW* win, MENU* menu)
{
    if (!win || !menu) return NULL;

    post_menu(menu);
    wrefresh(win);

    ITEM* choice = NULL;
    int action;
    do
    {
        action = wgetch(win);
        switch (action)
        {   
            case KEY_DOWN:
            case KEY_RIGHT:
            case 9: 
            case 106: menu_driver(menu, REQ_NEXT_ITEM); break;

            case KEY_UP:
            case KEY_LEFT:
            case 353: 
            case 107: menu_driver(menu, REQ_PREV_ITEM); break;
            
            case KEY_ENTER:
            case 10: choice = current_item(menu); break;
        }
    } while (!choice);

    unpost_menu(menu);
    wrefresh(win);

    return choice;
}

int form_process(WINDOW* win, Interface* interf, int interf_code, void* dest)
{
    if (!win || !interf || !dest) return INVALID_ARG;

    post_form(interf->form);

    /* Подписи */
    switch (interf_code)
    {
        case START_INTERF_CODE:
        {
            mvwprintw(win, 2, 0, "Enter number of notes: ");
            mvwprintw(win, 4, 0, "Press `F5` to continue");
            mvwprintw(win, 5, 0, "Press `F6` to quit to main menu");
            wmove(win, 2, 24);
        } break;
        case INPUT_DATE_INTERF_CODE:
        {
            mvwprintw(win, 0, 0, "Please enter date of birth:\n");
            mvwprintw(win, 4, 0, "Press `F5` to search");
            mvwprintw(win, 5, 0, "Press `F6` to quit to main menu");
            wmove(win, 2, 0);
        } break;
        case INPUT_NOTE_INTERF_CODE:
        {
            mvwprintw(win, 0, 0, "Surname: ");
            mvwprintw(win, 2, 0, "Name: ");
            mvwprintw(win, 4, 0, "Insurance number: ");
            mvwprintw(win, 6, 0, "Date of birth: ");
            mvwprintw(win, 8, 6, "Press `F5` to save and finish");
            mvwprintw(win, 9, 5, "Press `F6` to quit to main menu");
            wmove(win, 0, FIELD_START_POS);
        } break;
    }    
    wrefresh(win);

    /* Обработка клавиш */
    int result = PROCESS_DONE;
    int action;
    bool correct_input = false;
    do
    {
        action = wgetch(win);
        switch(action)
        {
            case 9: // tab
            case 10: // enter
            case KEY_ENTER: form_driver(interf->form, REQ_VALIDATION);
            case KEY_DOWN: form_driver(interf->form, REQ_NEXT_FIELD);
                           form_driver(interf->form, REQ_END_LINE);
                           break;
            case 353: form_driver(interf->form, REQ_VALIDATION); // shift+tab
            case KEY_UP: form_driver(interf->form, REQ_PREV_FIELD);
                         form_driver(interf->form, REQ_END_LINE);
                         break;
            case KEY_RIGHT: form_driver(interf->form, REQ_NEXT_CHAR); break;
            case KEY_LEFT: form_driver(interf->form, REQ_PREV_CHAR); break;

            case KEY_DC: form_driver(interf->form, REQ_DEL_CHAR); break;
            case KEY_BACKSPACE: form_driver(interf->form, REQ_DEL_PREV); break;

            // NumPad
            case 585: form_driver(interf->form, 48); break; // 0
            case 351: form_driver(interf->form, 49); break; // 1
            case 577: form_driver(interf->form, 50); break; // 2
            case 352: form_driver(interf->form, 51); break; // 3
            case 575: form_driver(interf->form, 52); break; // 4
            case 350: form_driver(interf->form, 53); break; // 5
            case 576: form_driver(interf->form, 54); break; // 6
            case 348: form_driver(interf->form, 55); break; // 7
            case 574: form_driver(interf->form, 56); break; // 8
            case 349: form_driver(interf->form, 57); break; // 9

            case KEY_F(5): correct_input = read_input(interf, interf_code, dest); break;
            case KEY_F(6): result = PROCESS_ABORTED; correct_input = true; break;
            
            default: form_driver(interf->form, action); break;
        }
        wrefresh(win);
    } while (!correct_input);

    unpost_form(interf->form);
    for (int i = 0; i < interf->n_fields; i++)
    {
        set_field_buffer(interf->fields[i], 0, "");
        set_field_back(interf->fields[i], A_UNDERLINE | A_NORMAL);
    }
    set_current_field(interf->form, interf->fields[0]);

    return result;
}

bool read_input(Interface* interf, int interf_code, void* dest)
{
    if (!interf || !dest) return false;

    bool result = true;

    switch (interf_code)
    {
        case START_INTERF_CODE:
        {
            int* dest_n_notes = (int*) dest;

            int n_notes = atoi(field_buffer(interf->fields[0], 0));
            if (n_notes <= 0) return false;

            *dest_n_notes = n_notes;
        } break;
        
        case INPUT_DATE_INTERF_CODE:
        {
            int* dest_date = (int*) dest;

            int birthday[3];
            for (int i = 0; i < 3; i++)
                birthday[i] = atoi(field_buffer(interf->fields[i], 0));

            if (check_date(birthday) == false) {
                for (int i = 0; i < 3; i++)
                    set_field_back(interf->fields[i], A_UNDERLINE | COLOR_PAIR(1));
                result &= false;
            } else {
                for (int i = 0; i < 3; i++)
                    set_field_back(interf->fields[i], A_UNDERLINE | A_NORMAL);
                result &= true;
            }

            if (!result) return result;

            for (int i = 0; i < 3; i++)
                dest_date[i] = birthday[i];
        } break;
        
        case INPUT_NOTE_INTERF_CODE:
        {
            Note* dest_note = (Note*) dest;
        
            int insurance = atoi(field_buffer(interf->fields[2], 0));
            int birthday[3];
            for (int i = 0; i < 3; i++)
               birthday[i] = atoi(field_buffer(interf->fields[3+i], 0));
        
            /* SURNAME */
            if (field_buffer(interf->fields[0], 0)[0] == ' ') {
                set_field_back(interf->fields[0], A_UNDERLINE | COLOR_PAIR(1));
                result &= false;
            }
            else {
                set_field_back(interf->fields[0], A_UNDERLINE | A_NORMAL);
                result &= true;
            }
            /* NAME */
            if (field_buffer(interf->fields[1], 0)[0] == ' ') {
                set_field_back(interf->fields[1], A_UNDERLINE | COLOR_PAIR(1));
                result &= false;
            }
            else {
                set_field_back(interf->fields[1], A_UNDERLINE | A_NORMAL);
                result &= true;
            }
            /* INSURANCE NUMBER */
            if (insurance < 10000000) {
                set_field_back(interf->fields[2], A_UNDERLINE | COLOR_PAIR(1));
                result &= false;
            }
            else {
                set_field_back(interf->fields[2], A_UNDERLINE | A_NORMAL);
                result &= true;
            }
            /* BIRTHDAY */
            if (check_date(birthday) == false) {
                for (int i = 3; i <= 5; i++) {
                    set_field_back(interf->fields[i], A_UNDERLINE | COLOR_PAIR(1));
                }
                result &= false;
            } else {
                for (int i = 3; i <= 5; i++) {
                    set_field_back(interf->fields[i], A_UNDERLINE | A_NORMAL);
                }
                result &= true;
            }
        
            if (!result) return result;
            
            copy_str(dest_note->surname, field_buffer(interf->fields[0], 0));
            copy_str(dest_note->name, field_buffer(interf->fields[1], 0));
            dest_note->insurance = insurance;
            for (int i = 0; i < 3; i++)
                dest_note->birthday[i] = birthday[i];

            capitalize(dest_note->surname);
            capitalize(dest_note->name);
        } break;

        default: result = false; break;
    }

    return result;
}


int copy_str(char* dest, const char* src)
{
    if (!dest || !src) return 1;

    int i;
    for (i = 0; src[i] != '\0'; i++)
        dest[i] = src[i];

    dest[i] = '\0';

    return 0;
}

void capitalize(char* str)
{
    /* Если в строке первая буква строчная, то она превращается в заглавную */

    if (!str || *str < 'a' || *str > 'z') return;

    *str -= 32;
}

bool is_leap_year(int y)
{
    return (y % 4 == 0) && (y % 100 != 0) || (y % 400 == 0);
}

bool check_date(int date[3])
{
    int d = date[0], m = date[1], y = date[2];

    if (d<1 || d>31 || m<1 || m>12 || y<1 || y>2025) return false;

    if ((m % 2) == (m > 7)) {
        if (m == 2) {
            if ((is_leap_year(y) && (d<=29)) || (!is_leap_year(y) && (d<=28)))
                return true;
        } else {
            if (d <= 30) return true;
        }
    } else {
        return true;
    }

    return false;
}


bool same_date(int date1[3], int date2[3])
{
    return (date1[0] == date2[0]) && (date1[1] == date2[1]) && (date1[2] == date2[2]);
}


NoteArr search(int date[3], const FILE* file)
{
    if (!file) return init_note_arr(0);
    
    Header header = get_header(file);
    header.transaction++;

    FILE* fileptr = (FILE*)file;
    fseek(fileptr, (long)sizeof(Header), SEEK_SET);
    
    NoteArr arr = init_note_arr(10);
    for (int i = 0; i < header.volume; i++)
    {
        static Note buffer_note;
        fread(&buffer_note, sizeof(Note), 1, fileptr);
        if (same_date(date, buffer_note.birthday))
            push_note_back(&arr, buffer_note);
    }

    set_header(&header, fileptr);

    return arr;
}
