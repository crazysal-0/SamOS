#include "print.h"

const static size_t NUMBER_OF_COLUMNS = 80;
const static size_t NUMBER_OF_ROWS = 25;

struct Char {
    uint8_t character;
    uint8_t color;
};

struct Char* buffer = (struct Char*) 0xb8000;
size_t column = 0;
size_t row = 0;
uint8_t color = PRINT_COLOR_WHITE | PRINT_COLOR_BLACK << 4;

void clear_row(size_t row) {
    struct Char empty = (struct Char) {
        character: ' ',
        color: color,
    };

    for (size_t column = 0; column < NUMBER_OF_COLUMNS; column++) {
        buffer[column + NUMBER_OF_COLUMNS * row] = empty;
    }
}

void print_clear() {
    for (size_t i = 0; i < NUMBER_OF_ROWS; i++) {
        clear_row(i);
    }
}

void print_newline() {
    column = 0;

    if (row < NUMBER_OF_ROWS - 1) {
        row++;
        return;
    }

    for (size_t row = 1; row < NUMBER_OF_ROWS; row++) {
        for (size_t column = 0; column < NUMBER_OF_COLUMNS; column++) {
            struct Char character = buffer[column + NUMBER_OF_COLUMNS * row];
            buffer[column + NUMBER_OF_COLUMNS * (row - 1)] = character;
        }
    }

    clear_row(NUMBER_OF_COLUMNS - 1);
}

void print_char(char character) {
    if (character == '\n') {
        print_newline();
        return;
    }

    if (column > NUMBER_OF_COLUMNS) {
        print_newline();
    }

    buffer[column + NUMBER_OF_COLUMNS * row] = (struct Char) {
        character: (uint8_t) character,
        color: color,
    };

    column++;
}

void print_string(char* str) {
    for (size_t i = 0; 1; i++) {
        char character = (uint8_t) str[i];

        if (character == '\0') {
            return;
        }

        print_char(character);
    }
}

void print_set_color(uint8_t foreground, uint8_t background) {
    color = foreground + (background << 4);
}