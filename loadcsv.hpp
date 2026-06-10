#ifndef loadcsv_h_
#define loadcsv_h_

#include <string>
#include <vector>
#include <iostream>
#include <cstdio>

using namespace std;

class CSVLOADER{
    public:
    string dirpath;
    string filename;
    char delimiter=',';

    int gridsize_T;

    int max_id_length;

    int** px;
    int** py;
    int** vx;
    int** vy;
    int*  size_of_each_frame;

    vector<int*> prepare_px;
    vector<int*> prepare_py;
    vector<int*> prepare_vx;
    vector<int*> prepare_vy;

    vector<int> prepare_size_of_each_frame;


    CSVLOADER(string filename_);

    void GETVALUE(int** &px_,int** &py_,int** &vx_,int** &vy,int* &size_of_each_frame,int &num_of_frame);
};

#endif




