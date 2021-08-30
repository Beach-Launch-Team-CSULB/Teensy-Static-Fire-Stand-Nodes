#ifndef STATELIST_H
#define STATELIST_H

// This header defines all the tracked states the system an be in, using an enum class for protection

enum class State
{
    debug,      //this is a debug state

    passive,    // lox vent open,   fuel vent closed, lox iso closed, fuel iso closed, main lox closed, main fuel closed, iso enable off, fuel vent enable off, lox vent enable off, MV enable off 
    test,       // only this state allows individual actuation
    loxLoad,    // lox vent open,   fuel vent closed, lox iso closed, fuel iso closed, main lox closed, main fuel closed, iso enable off, fuel vent enable off, lox vent enable on,  MV enable off
    pressArm,   // lox vent closed, fuel vent closed, lox iso closed, fuel iso closed, main lox closed, main fuel closed, iso enable on,  fuel vent enable off, lox vent enable on,  MV enable off
    pressurize, // lox vent closed, fuel vent closed, lox iso open,   fuel iso open,   main lox closed, main fuel closed, iso enable on,  fuel vent enable off, lox vent enable on,  MV enable off
    abort,      // lox vent open,   fuel vent open,   lox iso open,   fuel iso open,   main lox closed, main fuel closed, iso enable on,  fuel vent enable on,  lox vent enable off, MV enable off
    fireArmed,  // lox vent closed, fuel vent closed, lox iso open,   fuel iso open,   main lox closed, main fuel closed, iso enable on,  fuel vent enable off, lox vent enable on,  MV enable on
    fire,       // lox vent closed, fuel vent closed, lox iso open,   fuel iso open,   main lox open,   main fuel open,   iso enable on,  fuel vent enable off, lox vent enable on,  MV enable on
    vent,       // lox vent open,   fuel vent open,   lox iso closed, fuel iso closed, main lox closed, main fuel closed, iso enable off, fuel vent enable on,  lox vent enable on,  MV enable off
    
    
    
    

};


#endif