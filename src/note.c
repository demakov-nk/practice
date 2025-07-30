#include "note.h"

#include "header.h"
#include "funcs.h"

#include <ncurses.h>

void write_note(const Note* noteptr, size_t count, FILE* file)
{
    if (!noteptr || !file) return;

    Header header = get_header(file);

    fseek(file, (long) (sizeof(Header) + header.volume*sizeof(Note)), SEEK_SET);
    
    fwrite(noteptr, sizeof(Note), count, file);

    header.transaction++;
    header.volume += count;
    update_checksum(&header, noteptr);

    set_header(&header, file);
}

void print_note(WINDOW* win, const Note* note)
{
    if (!win || !note) return;

    wprintw(win, "Surname: %s\nName: %s\nInsurance number: %d\nDate of birth: %d.%d.%d\n",
            note->surname,
            note->name,
            note->insurance,
            note->birthday[0], note->birthday[1], note->birthday[2]);
    wrefresh(win);
}

