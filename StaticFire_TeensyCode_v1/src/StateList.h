#ifndef STATELIST_H
#define STATELIST_H

// This header defines all the tracked states the system an be in, using an enum class for protection

enum class State
{
    // These are the God States, they can be reached from any position
    abort,
    setup,      // fuel iso closed, lox iso closed, lox vent closed, fuel vent closed, lox main closed, fuel main closed
    vent,       // fuel iso closed, lox iso closed, lox vent open,   fuel vent open,   lox main closed, fuel main closed
    test,
    passive,    // fuel iso closed, lox iso closed, lox vent open,   fuel vent open,   lox main closed, fuel main closed
    offNominal, // off nominal is for when individual valves are actuated out of sequence

    // These states can only be accessed in sequence, from passive
    loxLoad,
    pressArm,
    pressurize,
    fireArmed,
    fire,

};


#endif