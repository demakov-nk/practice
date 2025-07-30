#include "../src/funcs.h"
#include "../src/note.h"
#include "../src/notearr.h"
#include "../src/header.h"

int main()
{
    // initscr();
    // cbreak();
    // noecho();
    // keypad(stdscr, TRUE);

    // /* Узнать номер клавиши */
    // while (true)
    // {
        // int c = getch();
        // printw("%d\n", c);
        // refresh();
    // }
    // getch();

    FILE* file = fopen("../build/database.dema", "rb+");
    // Header header = get_header(file);
    fseek(file, (long)sizeof(Header), SEEK_SET);

    Note note;
    fread(&note, sizeof(Note), 1, file);

    printf("`%s`\n", note.surname);
    printf("`%s`\n", note.name);
    printf("`%d`\n", note.insurance);
    printf("`%d.%d.%d`\n", note.birthday[0], note.birthday[1], note.birthday[2]);

    fclose(file);
    
    // refresh();
    // getch();
    
    // endwin();
    return 0;
}
