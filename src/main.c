
#include "funcs.h"
#include "note.h"
#include "notearr.h"
#include "header.h"

int main()
{
    initscr();
    start_color();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    init_pair(1, COLOR_WHITE, COLOR_RED);

    WINDOW* main_win = newwin(40, 40, 4, 4);
    keypad(main_win, TRUE);

    init_interf(&start_interf, START_INTERF_CODE, main_win);
    init_interf(&main_interf, MAIN_INTERF_CODE, main_win);
    init_interf(&leave_interf, LEAVE_INTERF_CODE, main_win);
    init_interf(&input_date_interf, INPUT_DATE_INTERF_CODE, main_win);
    init_interf(&input_note_interf, INPUT_NOTE_INTERF_CODE, main_win);

    wprintw(main_win, "Welcome to DataBaseManager.\n\n");
    wrefresh(main_win);
    
    /* Пытаемся открыть файл с базой. Если его не существует, создаем новый. */
    FILE* file = fopen(FILENAME, "rb+");
    Header header = get_header(file); // если файл пуст, header останется пустым
    if (!file)
    {
        file = fopen(FILENAME, "wb+");
        header.signature = (int)('d' + 'e' + 'm' + 'a');

        werase(main_win);
        int n_notes;
        int action = form_process(main_win, &start_interf, START_INTERF_CODE, &n_notes);

        switch (action)
        {
            case PROCESS_DONE:
            {
                bool quit_to_main_menu = false;
                int i;
                for (i = 0; i < n_notes && !quit_to_main_menu; i++)
                {
                    Note new_note;
                    int action2 = form_process(main_win, &input_note_interf, INPUT_NOTE_INTERF_CODE, &new_note);
                    
                    switch (action2)
                    {
                        case PROCESS_DONE:
                        {
                            write_note(&new_note, 1, file);
                            print_note(main_win, &new_note);
                            wprintw(main_win, "\n%d note is added successfully.\nPress any key to continue... ", i+1);
                            wrefresh(main_win);
                            wgetch(main_win);
                        } break;
                        case INVALID_ARG:
                        {
                            wprintw(main_win, "The process been started with errors, please try again.\nPress any key to go back... ");
                            wrefresh(main_win);
                            wgetch(main_win);
                        } break;
                        default: quit_to_main_menu = true; break;
                    }
                }

                werase(main_win);
                wprintw(main_win, "%d notes were added successfully.\nPress any key to join main menu... ", i==n_notes ? i : i-1);
                wrefresh(main_win);
                wgetch(main_win);
            } break;
            case INVALID_ARG:
            {
                wprintw(main_win, "The process been started with errors. Reboot the program, please\nPress any key to quit... ");
                wrefresh(main_win);
                wgetch(main_win);
            } break;
        }
    }

    ITEM* main_choice;
    bool leave = false;
    do
    {
        werase(main_win);
        wprintw(main_win, "Welcome to DataBaseManager.\n\n");
        post_menu(main_interf.menu);
        wrefresh(main_win);
        
        main_choice = menu_process(main_win, main_interf.menu);

        /* ADD NEW NOTE */
        if (main_choice == main_interf.items[0])
        {            
            werase(main_win);
            
            Note new_note;
            int action = form_process(main_win, &input_note_interf, INPUT_NOTE_INTERF_CODE, &new_note);

            switch (action)
            {
                case PROCESS_DONE:
                {
                    write_note(&new_note, 1, file);
                    print_note(main_win, &new_note);
                    wprintw(main_win, "\nNew note is added successfully.\nPress any key to continue... ");
                    wrefresh(main_win);
                    wgetch(main_win);
                } break;
                case INVALID_ARG:
                {
                    wprintw(main_win, "The process been started with errors, please try again.\nPress any key to go back... ");
                    wrefresh(main_win);
                    wgetch(main_win);
                } break;
                default: break;
            }
        }
        /* SEARCH BY BIRTHDAY */
        else if (main_choice == main_interf.items[1])
        {            
            werase(main_win);
            
            int input_date[3] = {0, 0, 0};
            int action = form_process(main_win, &input_date_interf, INPUT_DATE_INTERF_CODE, input_date);

            switch (action)
            {
                case PROCESS_DONE:
                {
                    NoteArr note_arr = search(input_date, file);
                    werase(main_win);
                    if (note_arr.size == 0)
                        wprintw(main_win, "There are no people with such birthday.\nPress any key to continue... ");
                    else {
                        print_notearr(main_win, &note_arr);
                        wprintw(main_win, "\n%d people were found.\nPress any key to go back... ", note_arr.size);
                    }
                    wrefresh(main_win);
                    wgetch(main_win);
                } break;
                case INVALID_ARG:
                {
                    wprintw(main_win, "The process been started with errors, please try again.\nPress any key to continue... ");
                    wrefresh(main_win);
                    wgetch(main_win);
                } break;
                default: break;
            }
        }
        /* LEAVE */
        else if (main_choice == main_interf.items[2])
        {
            werase(main_win);
            wprintw(main_win, "Are you sure you want to leave?\n\n");
            post_menu(leave_interf.menu);
            wrefresh(main_win);
    
            ITEM* leave_choice = menu_process(main_win, leave_interf.menu);
            if (leave_choice == leave_interf.items[1])
            {
                leave = true;
            }
        }
    } while (!leave);

    destroy_all(main_win, file);
    
    return 0;
}
