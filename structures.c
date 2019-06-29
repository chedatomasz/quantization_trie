#include "structures.h"
#include "safe_alloc.h"

State *state_root;

//Cleans up leftover EnergyState structs that belonged to a deleted history node.
static void energyCleanup(EnergyState *node) {
    if (node) {
        node->members--;
        if (node->members == 0) {
            if (node->is_dummy) {
                energyCleanup(node->real);
            }
            free(node);
        }
    }
}

//Creates and returns an empty history node.
State *newState() {

    State *result = safe_malloc(sizeof(State));
    for (int i = 0; i < CHILDREN; i++)
        result->next[i] = NULL;
    result->energystate = NULL;
    return result;
}

//Creates and returns an empty energy node.
EnergyState *newEnergy() {
    EnergyState *result = safe_malloc(sizeof(EnergyState));
    result->is_dummy = false;
    result->real = NULL;
    result->members = 1;
    return result;
}

//Returns a node reached by given path from state_root. Cleans up the history struct.
State *goToState(History h) {
    State *current = state_root;
    for (uint64_t i = 0; i < h.length; i++) {
        if (current->next[h.states[i]]) {
            current = current->next[h.states[i]];
        }
        else {
            free(h.states);
            return NULL;
        }
    }
    free(h.states);
    return current;
}

//Removes dummy energy structs at given state.
void collapseDummyEnergies(State *state) {
    while (state->energystate->is_dummy) {
        state->energystate->members--;
        EnergyState *forward = state->energystate->real;

        if (state->energystate->members == 0) {
            state->energystate->real->members--;
            free(state->energystate);
        }
        state->energystate = forward;
        state->energystate->members++;
    }
}

//Adds a path described in History h to the trie starting at state_root.
void trieCreateHistory(History h) {
    State *current = state_root;
    for (uint64_t i = 0; i < h.length; i++) {
        if (current->next[h.states[i]]) {
            current = current->next[h.states[i]];
        }
        else {
            current->next[h.states[i]] = newState();
            current = current->next[h.states[i]];
        }
    }
}

//Removes all paths with given prefix from the trie starting at state_root.
void trieRemoveHistory(History h) {
    State *current = state_root;
    State *previous = state_root;
    for (uint64_t i = 0; i < h.length; i++) {
        if (current->next[h.states[i]]) {
            previous = current;
            current = current->next[h.states[i]];
        }
        else {
            return;
        }
    }
    previous->next[h.states[h.length - 1]] = NULL;
    recursiveCleanup(current);
}

//Recursively removes State nodes starting at the argument.
void recursiveCleanup(State *node) {
    for (int i = 0; i < CHILDREN; i++) {
        if (node->next[i]) {
            recursiveCleanup(node->next[i]);
        }
    }
    energyCleanup(node->energystate);
    free(node);
}

//Checks if trie starting at state_root contains given path.
bool trieValid(History h) {
    State *current = state_root;
    for (uint64_t i = 0; i < h.length; i++) {
        if (current->next[h.states[i]]) {
            current = current->next[h.states[i]];
        }
        else {
            return false;
        }
    }
    return true;
}

//Sets energy for given node at given value.
bool trieSetEnergy(History h, Energy e) {
    State *current = goToState(h);
    if (current == NULL) {
        return false;
    }
    if (current->energystate) {
        collapseDummyEnergies(current);
    }
    else {
        current->energystate = newEnergy();
    }
    current->energystate->value = e.value;
    return true;
}

//Gets energy for given node.
uint64_t trieGetEnergy(History h) {
    State *current = goToState(h);
    if (current == NULL) {
        return 0;
    }
    if (current->energystate) {
        collapseDummyEnergies(current);
    }
    else {
        return 0;
    }
    return current->energystate->value;
}

//Links given histories into one energy equivalence class.
bool trieEqualStates(History h1, History h2) {
    State *first = goToState(h1);
    State *second = goToState(h2);
    //If one of the states isn't valid:
    if (!first || !second) {
        return false;
    }
    //If states are equal:
    if (first == second) {
        return true;
    }
    //Collapse dummy energies first to avoid loops
    if (first->energystate)
        collapseDummyEnergies(first);
    if (second->energystate)
        collapseDummyEnergies(second);
    //If states are already linked or both don't have energies
    if (first->energystate == second->energystate) {
        if (!first->energystate) {
            //Both no energy
            return false;
        }
        //Already linked
        return true;
    }
    //Both energies set
    if (first->energystate && second->energystate) {
        //Carryover computes the carryover from the least significant bits, which get lost in the following bitshift
        uint64_t carryover = (uint64_t) ((first->energystate->value & 1) &&
                                         (second->energystate->value & 1));
        uint64_t first_shifted = first->energystate->value >> 1;
        uint64_t second_shifted = second->energystate->value >> 1;
        first->energystate->value = carryover + first_shifted + second_shifted;
        second->energystate->is_dummy = true;
        second->energystate->real = first->energystate;
        first->energystate->members++;
        return true;
    }
    //One energy set
    if (first->energystate) {
        second->energystate = first->energystate;
        second->energystate->members++;
        return true;
    }
    if (second->energystate) {
        first->energystate = second->energystate;
        first->energystate->members++;
        return true;
    }
    return false;
}