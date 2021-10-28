// in c++ you have to make this generic struct type first before you put actual data in it
// they can have default values
struct LoadcellCoefs
{
    double a0{};
    double a1{};
    double a2{};

    double b0{};
    double b1{};
    double b2{};


};


int main()
{

// define actual values in actual instances of your struct type
LoadcellCoefs loadCellType1{0.001, 0.3, -1.2, 5, 0.004, 0.012};

// or:

LoadcellCoefs loadCellType2;

loadCellType2.a0 = 0.1; 
loadCellType2.a1 = 0.23;
// etc.

LoadcellCoefs a;

return 0;
}
