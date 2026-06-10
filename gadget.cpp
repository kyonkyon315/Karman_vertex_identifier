#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <limits>
#include <iomanip>
#include <ios>
#include "loadcsv.hpp"
#include "KDtree.hpp"
#include "gadget.hpp"

using namespace std;

void gadget::setsize(int size_){
    size=size_;
    back="";
    for(int i=0;i<size;i++){
        back+="\b";
    }
}

string gadget::bar(double value,int size){

    string ans="";

    if(value<0 || value>1){
        cout<<"error:  value="<<value<<" value must be in [0,1].\n";
        return ans;
    }

    if(size < 9){
        cout<<"error: size must be bigger than 8.\n";
        return ans;
    }

    ans+="|";

    for(int i=0;i<size-8;i++){

        if((double)i / (double)(size-8) <value){
            ans+=">";
        }
        else{
            ans+="-";
        }
    }

    ans+="|";


    string value_ =to_string( ( (int)(value*1000.)  )/1);

    int value_size=(int)value_.size();

    for(int i=0;i<4-value_size;i++){
        ans+=" ";
    }

    for(int i=0;i<value_size-1;i++){
        ans+=value_[i];
    }

    ans+=".";

    ans+=value_[value_size-1];

    ans+="%";

    return ans;

}







