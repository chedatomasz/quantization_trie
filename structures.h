//
// Created by chedat on 15.03.19.
//

#ifndef IPP_QUANT_STRUCTURES_H
#define IPP_QUANT_STRUCTURES_H

#include <stdint.h>
#include <stdbool.h>

#define CHILDREN 4
struct _History {
    int *states;
    uint64_t length;
};

struct _Energy {
    uint64_t value;
};

struct _State {
    struct _EnergyState *energystate;
    struct _State *next[CHILDREN];
};

struct _EnergyState {
    bool is_dummy;
    uint64_t value;
    struct _EnergyState *real;
    int members;
};


typedef struct _History History;

typedef struct _Energy Energy;

typedef struct _EnergyState EnergyState;

typedef struct _State State;

//Global variable locating the root of trie structure.
extern State *state_root;

extern State *newState();

extern EnergyState *newEnergy();

extern State *goToState(History h);

//Removes dummy energy structs at given state.
extern void collapseDummyEnergies(State *state);

//Adds a path described in History h to the trie starting at state_root.
extern void trieCreateHistory(History h);

//Removes all paths with given prefix from the trie starting at state_root.
extern void trieRemoveHistory(History h);

//Recursively removes nodes starting at the argument.
// Public for use by cleanUpDataStructures - see engine.c
extern void recursiveCleanup(State *node);

//Checks if trie starting at state_root contains given path.
extern bool trieValid(History h);

//Sets energy for given node at given value.
extern bool trieSetEnergy(History h, Energy e);

//Gets energy for given node.
extern uint64_t trieGetEnergy(History h);

//Links given histories into one energy equivalence class.
extern bool trieEqualStates(History h1, History h2);

#endif //IPP_QUANT_STRUCTURES_H
