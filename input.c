#include <stdio.h>
#include <ctype.h>
#include <stdint.h>
#include "input.h"
#include "safe_alloc.h"

// All instructions start with a block of capital letters.
// The longest viable block is "DECLARE" - 7 letters long.
// Any line beginning with a longer block may be discarded upfront.
#define LONGEST_CORRECT_INSTRUCTION 7

//Read the stdin until \n or EOF.
static void readUntilLineEnd() {
    int input;
    do {
        input = getc(stdin);
    } while (input != (int) '\n' && input != EOF);
}

//View the next character without removing it from stdin.
static int getNextCharacter() {
    int first_character = getc(stdin);
    if (first_character != EOF) {
        ungetc(first_character, stdin);
    }
    return first_character;
}

//Match the input int array to pattern char array.
static bool
matchIntString(int length, const int input[], const char pattern[]) {
    for (int i = 0; i < length; i++) {
        if ((int) pattern[i] != input[i])
            return false;
    }
    return true;
}

//Recognize the instruction from a block of ints representing capital letters.
static int recognizeInstruction(int length, int input[]) {
    if (length == 5 && matchIntString(5, input, "EQUAL"))
        return EQUAL;
    if (length == 5 && matchIntString(5, input, "VALID"))
        return VALID;
    if (length == 6 && matchIntString(6, input, "REMOVE"))
        return REMOVE;
    if (length == 6 && matchIntString(6, input, "ENERGY"))
        return ENERGY_UNDEFINED;
    if (length == 7 && matchIntString(7, input, "DECLARE"))
        return DECLARE;
    return NOTB;
}

//Read a block of digits from stdin, return it by setting variables at given pointers.
static void readDigitBlock(int **arg, uint64_t *arg_length) {
    *arg_length = 0;
    uint64_t allocated_size = *arg_length;
    *arg = safe_malloc(sizeof(int) * (*arg_length + 8));
    while (isdigit((*arg)[*arg_length] = getc(stdin))) {
        (*arg_length)++;
        if (*arg_length > allocated_size) {
            *arg = safe_realloc(*arg, sizeof(int) * ((*arg_length * 2) + 8));
            allocated_size = *arg_length * 2;
        }
    }
    if ((*arg)[*arg_length] != EOF) {
        ungetc((*arg)[*arg_length], stdin);
    }
}
//Checks if another line is available at stdin. Assumes that last character was \n
bool nextLineAvailable() {
    return getNextCharacter() != EOF;
}

//Parses the next line of standard input and returns
// a pointer to Instructions - see instruction_interface.h.
Instruction *getNextInstruction() {
    Instruction *answer = safe_malloc(sizeof(Instruction));
    answer->arg1 = NULL;
    answer->arg2 = NULL;
    answer->validity = INCORRECT;

    // Filtering out lines that are empty or start with #
    if (getNextCharacter() == (int) '\n' || getNextCharacter() == (int) '#') {
        answer->validity = EMPTY;
        readUntilLineEnd();
        return answer;
    }

    // Recognizing the initial capital letters block
    int command[LONGEST_CORRECT_INSTRUCTION];
    int characters_read = 0;
    while (characters_read < LONGEST_CORRECT_INSTRUCTION &&
           isupper(getNextCharacter())) {
        command[characters_read] = getc(stdin);
        characters_read++;
    }
    answer->signal = recognizeInstruction(characters_read, command);
    if (answer->signal == NOTB) {
        answer->validity = INCORRECT;
        readUntilLineEnd();
        return answer;
    }

    // Checking for space after the initial capital letters block
    if (getNextCharacter() != ' ') {
        answer->validity = INCORRECT;
        readUntilLineEnd();
        return answer;
    }
    else {
        getc(stdin);
    }

    // Reading the first block of history
    readDigitBlock(&answer->arg1, &answer->arg1_length);
    if (answer->arg1_length == 0) {
        answer->validity = INCORRECT;
        readUntilLineEnd();
        return answer;
    }

    // Checking for end of command
    if (getNextCharacter() == (int) '\n') {
        getc(stdin);
        if (answer->signal == ENERGY_UNDEFINED) {
            answer->signal = ENERGY1;
        }
        if (answer->signal == EQUAL) {
            answer->validity = INCORRECT;
            // We don't read to line end - \n is already removed
            return answer;
        }
        answer->validity = CORRECT;
        return answer;
    }

    // Checking for 2-argument commands
    if ((answer->signal == ENERGY_UNDEFINED || answer->signal == EQUAL) &&
        getNextCharacter() == (int) ' ') {
        getc(stdin);
        if (answer->signal == ENERGY_UNDEFINED) {
            answer->signal = ENERGY2;
        }
        readDigitBlock(&answer->arg2, &answer->arg2_length);
        if (answer->arg2_length == 0 || getNextCharacter() != (int) '\n') {
            answer->validity = INCORRECT;
            readUntilLineEnd();
            return answer;
        }
        getc(stdin);
        answer->validity = CORRECT;
        return answer;
    }

    answer->validity = INCORRECT;
    readUntilLineEnd();
    return answer;

}