#ifndef PINLIST_H
#define PINLIST_H

namespace pin
{
    // abort interrupt pins
    const int reset{28};
    const int abort{29};

    // safety enable pins
    const int isoSafe{24};
    const int fuelVentSafe{25};
    const int loxVentSafe{26};
};




#endif