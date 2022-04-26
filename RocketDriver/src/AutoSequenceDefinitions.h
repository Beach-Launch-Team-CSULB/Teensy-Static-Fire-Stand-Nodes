#ifndef AUTOSEQUENCEDEFINITIONS_H
#define AUTOSEQUENCEDEFINITIONS_H

#include "AutoSequenceClass.h"
#include <array>

// Define the number of autosequences here
#define NUM_AUTOSEQUENCES 2


AutoSequence IgnitionAutoSequence{-15000000, 2}; // X seconds in micros to set the countdownStart
AutoSequence PasafireIgnitionAutoSequence{-15000000, 15}; // X seconds in micros to set the countdownStart

std::array<AutoSequence*, NUM_AUTOSEQUENCES> autoSequenceArray{&IgnitionAutoSequence, &PasafireIgnitionAutoSequence};


#endif