#ifndef STATELIST_H
#define STATELIST_H

// This header defines all the tracked states the system an be in, using an enum class for protection

enum class State
{
    debug,      //this is a debug state
    // These are the God States, they can be reached from any position
    abort,      // lox vent open, fuel vent open,   lox iso open,   fuel iso open,   main lox closed, main fuel closed, iso enable on,  fuel vent enable on,  lox vent enable off, MV enable off
    test,       // only this state allows individual actuation
    passive,    // lox vent open, fuel vent closed, lox iso closed, fuel iso closed, main lox closed, main fuel closed, iso enable off, fuel vent enable off, lox vent enable off, MV enable off 
    vent,
    
    // These states can only be accessed in sequence, from passive
    loxLoad,    // lox vent open, fuel vent closed, lox iso closed, fuel iso closed, main lox closed, main fuel closed, iso enable off, fuel vent enable off, lox vent enable on, MV enable off
    pressArm,
    pressurize,
    fireArmed,
    fire,

};


#endif