#include "header.h"

#include <ncurses.h>
#include <zlib.h>

Header get_header(const FILE* file)
{
    /* Считывает заголовок файла */

    Header header = {0, 0, 0, 0};

    if (!file) return header;

    FILE* fileptr = (FILE*)file;
    fseek(fileptr, 0, SEEK_SET);

    fread(&header.signature, sizeof(int), 1, fileptr);
    fread(&header.transaction, sizeof(int), 1, fileptr);
    fread(&header.volume, sizeof(int), 1, fileptr);
    fread(&header.checksum, sizeof(int), 1, fileptr);

    return header;
}

void set_header(const Header* header, FILE* file)
{
    if (!header || !file) return;

    long current_pos = ftell(file);
    fseek(file, 0, SEEK_SET);

    fwrite(header, sizeof(Header), 1, file);

    fseek(file, current_pos, SEEK_SET);
}

void update_checksum(Header* header, const Note* new_note)
{
    if (!header || !new_note) return;

    uLong crc = header->checksum;
    const Bytef* surname = (const Bytef*)(&(new_note->surname[0]));
    const Bytef* name = (const Bytef*)(&(new_note->name[0]));
    const Bytef* insurance = (const Bytef*)(&(new_note->insurance));
    const Bytef* birthday = (const Bytef*)(&(new_note->birthday[0]));
    
    crc = crc32(crc, surname, sizeof(new_note->surname) / sizeof(char));
    crc = crc32(crc, name, sizeof(new_note->name) / sizeof(char));
    crc = crc32(crc, insurance, sizeof(int));
    crc = crc32(crc, birthday, sizeof(new_note->birthday) / sizeof(int));
    
    header->checksum = (int)crc;
}
