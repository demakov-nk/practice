#pragma once

#include "note.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct Header
{
    int signature;
    int transaction;
    int volume;
    int checksum;
} Header;

Header get_header(const FILE* file);
void set_header(const Header* header, FILE* file);
void update_checksum(Header* header, const Note* new_note);
