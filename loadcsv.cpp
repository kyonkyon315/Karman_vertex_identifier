#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include "loadcsv.hpp"


using namespace std;


CSVLOADER::CSVLOADER(string dirpath_){
    dirpath=dirpath_;
    string ANALYSELIST_path = dirpath+"/read_by_python.txt";
    ifstream ifs_csv_file(ANALYSELIST_path);
    string str_buf;
    string str_conma_buf;

    getline(ifs_csv_file,str_buf);
    getline(ifs_csv_file,str_buf);
    getline(ifs_csv_file,str_buf);

    gridsize_T=stoi(str_buf.substr(13,(int)str_buf.size()-13));

    getline(ifs_csv_file,str_buf);

    max_id_length=stoi(str_buf.substr(13,(int)str_buf.size()-13));
}

void CSVLOADER::GETVALUE(int** &px_,int** &py_,int** &vx_,int** &vy_,int* &size_of_each_frame_,int &num_of_frame_){
        string str_buf;
        string str_conma_buf;


        string vpc_px_path=dirpath+"/px.csv";

        ifstream ifs_csv_file_px(vpc_px_path);

        for(int i=0;i<gridsize_T;i++){
            getline(ifs_csv_file_px,str_buf);
            istringstream i_stream_px(str_buf);
            getline(i_stream_px,str_conma_buf,delimiter);

            int length=stoi(str_conma_buf);

            if(length<1){continue;}

            prepare_size_of_each_frame.push_back(length);

            int* data_line=new int[length];


            for(int i=0;i<length;i++){
                getline(i_stream_px,str_conma_buf,delimiter);
                data_line[i]=stoi(str_conma_buf);
            }
            prepare_px.push_back(data_line);
        }
//--------------------------------------------------------------------------------------

        string vpc_py_path=dirpath+"/py.csv";

        ifstream ifs_csv_file_py(vpc_py_path);

        for(int i=0;i<gridsize_T;i++){
            getline(ifs_csv_file_py,str_buf);
            istringstream i_stream_py(str_buf);
            getline(i_stream_py,str_conma_buf,delimiter);

            int length=stoi(str_conma_buf);

            if(length<1){continue;}

            int* data_line=new int[length];


            for(int i=0;i<length;i++){
                getline(i_stream_py,str_conma_buf,delimiter);
                data_line[i]=1920-stoi(str_conma_buf);///////1920 = pixel y size
            }
            prepare_py.push_back(data_line);
        }
//----------------------------------------------------------------------------


        string vpc_vx_path=dirpath+"/vx.csv";

        ifstream ifs_csv_file_vx(vpc_vx_path);

        for(int i=0;i<gridsize_T;i++){
            getline(ifs_csv_file_vx,str_buf);
            istringstream i_stream_vx(str_buf);
            getline(i_stream_vx,str_conma_buf,delimiter);

            int length=stoi(str_conma_buf);

            if(length<1){continue;}

            int* data_line=new int[length];


            for(int i=0;i<length;i++){
                getline(i_stream_vx,str_conma_buf,delimiter);
                data_line[i]=stoi(str_conma_buf);
            }
            prepare_vx.push_back(data_line);
        }
//----------------------------------------------------------------------------


        string vpc_vy_path=dirpath+"/vy.csv";

        ifstream ifs_csv_file_vy(vpc_vy_path);

        for(int i=0;i<gridsize_T;i++){
            getline(ifs_csv_file_vy,str_buf);
            istringstream i_stream_vy(str_buf);
            getline(i_stream_vy,str_conma_buf,delimiter);

            int length=stoi(str_conma_buf);

            if(length<1){continue;}

            int* data_line=new int[length];


            for(int i=0;i<length;i++){
                getline(i_stream_vy,str_conma_buf,delimiter);
                data_line[i]=-stoi(str_conma_buf);
            }
            prepare_vy.push_back(data_line);
        }
//----------------------------------------------------------------------------




        int num_of_frame=(int)prepare_size_of_each_frame.size();

        px=new int*[num_of_frame];
        py=new int*[num_of_frame];
        vx=new int*[num_of_frame];
        vy=new int*[num_of_frame];

        size_of_each_frame=new int[num_of_frame];

        for(int i=0;i<num_of_frame;i++){
            px[i]            = prepare_px[i];
            py[i]            = prepare_py[i];
            vx[i]            = prepare_vx[i];
            vy[i]            = prepare_vy[i];

            size_of_each_frame[i] = prepare_size_of_each_frame[i];
        }

        num_of_frame_       = num_of_frame;
        size_of_each_frame_ = size_of_each_frame;
        px_                = px;
        py_                = py;
        vx_                = vx;
        vy_                = vy;


        if (num_of_frame==0){
            cout<<"error: there is no data in csv_file:"<<filename<<"./n";
        }

        prepare_px.clear();
        prepare_py.clear();
        prepare_vx.clear();
        prepare_vy.clear();

        prepare_size_of_each_frame.clear();
}


