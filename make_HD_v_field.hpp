#ifndef make_HD_v_field_h_
#define make_HD_v_field_h_

#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include "loadcsv.hpp"


using namespace std;

class VPC{

    public:

    string foldername;

    int** px;
    int** py;
    int** vx;
    int** vy;

    int*  size_of_each_frame;

    int num_of_frame;


    void Get_data(string foldername);

    void Delete();
};

class vfield{

    public:

    int movie_x_size=1080;
    int movie_y_size=1920;
    VPC* vpcptr;
    int num_of_frame;

    int cell_length;

    int cell_size_x;
    int cell_size_y;

    int radius;

    int*** v_field_x;
    int*** v_field_y;

    void init(VPC* &vpc,int cell_length_);

    void calc_velocity();

    void find_vertex_center(string save_foldername);

    void Delete();

};


#endif
