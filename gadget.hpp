#ifndef Gadget_h_
#define Gadget_h_

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <limits>
#include "loadcsv.hpp"
#include "KDtree.hpp"


using namespace std;


class gadget{
    private:
    int size;

    public:
    string back;
    void setsize(int size_);
    string bar(double value,int size);

};


#endif













