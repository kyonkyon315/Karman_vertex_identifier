#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>
#include "make_HD_v_field.hpp"
#include "loadcsv.hpp"
#include "KDtree.hpp"
#include "gadget.hpp"
#define LL long long

using namespace std;

void VPC::Get_data(string foldername){
        cout<<"\n-------------------------------------------------------\n";
        cout<<"getting data in folder named "<<foldername<<". "<<flush;

        CSVLOADER csvloader(foldername);

        csvloader.GETVALUE(px,py,vx,vy,size_of_each_frame,num_of_frame);
        cout<<"->succeed\n";

}

void VPC::Delete(){
        cout<<"deleting VPC. "<<flush;
        for(int i=0;i<num_of_frame;i++){
            delete[] px[i];
            delete[] py[i];
            delete[] vx[i];
            delete[] vy[i];
        }
        delete[] px;
        delete[] py;
        delete[] vx;
        delete[] vy;
        delete[] size_of_each_frame;
        cout<<"->succeed\n";
}

void vfield::init(VPC* &vpc,int cell_length_){
        vpcptr = vpc;
        num_of_frame=vpcptr->num_of_frame;
        cout<<"initializing vfield. "<<flush;
        cell_length=cell_length_;
        cell_size_x=(movie_x_size+cell_length)/cell_length;
        cell_size_y=(movie_y_size+cell_length)/cell_length;
        v_field_x=new int**[num_of_frame];
        v_field_y=new int**[num_of_frame];
        for(int i=0;i<num_of_frame;i++){
            v_field_x[i]=new int*[cell_size_y];
            v_field_y[i]=new int*[cell_size_y];
            for(int j=0;j<cell_size_y;j++){
                v_field_x[i][j]=new int[cell_size_x];
                v_field_y[i][j]=new int[cell_size_x];
            }
        }
        cout<<"->succeed\n";

}

void vfield::calc_velocity(){
    cout<<"calculating HD velocity field. "<<flush;
    KDTree kdtree;
    gadget Gadget;
    cout<<Gadget.bar(0.,30)<<flush;
    Gadget.setsize(30);
    for(int i=0;i<num_of_frame;i++){
        cout<<Gadget.back<<Gadget.bar((double) (i+1) / (double)num_of_frame ,30)<<flush;
        kdtree.init(vpcptr,vpcptr->px[i],vpcptr->py[i],vpcptr->size_of_each_frame[i]);
        kdtree.CreateTree();
        kdtree.Query(cell_size_x,cell_size_y,v_field_x[i],v_field_y[i],i,cell_length);
        kdtree.Delete();
    }
    cout<<"->succeed\n";
}

void vfield::find_vertex_center(string save_foldername){

    cout<<"searching vertex center. ";

    filesystem::path dir = save_foldername;
    if(!filesystem::exists(dir)){
        filesystem::create_directory(dir);
    }else{
        cout<<"The directory named "<<save_foldername<<" already exists.\n";
    }


    int r=radius/cell_length;
/*
渦評価関数
   x x x x x x x
   x x x x x x x
   x x x x x x x
   x x x o x x x
   x x x x x x x
   x x x x x x x
   x x x x x x x
          <- r ->
*/

    int    calc_board_vec_x[cell_size_y][cell_size_x];
    int    calc_board_vec_y[cell_size_y][cell_size_x];
    double calc_board_scalar[cell_size_y][cell_size_x];

    double ans_board_vec[cell_size_y][cell_size_x];
    double ans_board_scalar[cell_size_y][cell_size_x];


    for(int i=0;i<num_of_frame;i++){

        for(int j=0;j<cell_size_y;j++){
            int x_sum=0;
            int y_sum=0;
            double r_sum=0.;
            for(int k=0;k<cell_size_x;k++){
                x_sum += v_field_x[i][j][k];
                y_sum += v_field_y[i][j][k];
                r_sum += pow( v_field_x[i][j][k]*v_field_x[i][j][k]
                             +v_field_y[i][j][k]*v_field_y[i][j][k],
                             0.5);
                calc_board_vec_x[j][k]=x_sum;
                calc_board_vec_y[j][k]=y_sum;
                calc_board_scalar[j][k]=r_sum;
            }
        }

        for(int j=1;j<cell_size_y;j++){
            for(int k=0;k<cell_size_x;k++){
                calc_board_vec_x[j][k]+=calc_board_vec_x[j-1][k];
                calc_board_vec_y[j][k]+=calc_board_vec_y[j-1][k];
                calc_board_scalar[j][k]+=calc_board_scalar[j-1][k];
            }
        }

        for(int j=r;j<cell_size_y-r;j++){
            for(int k=r;k<cell_size_x-r;k++){
                ans_board_vec[j][k]
                        =pow(
                           (double)pow(  (calc_board_vec_x[j+r][k+r]+calc_board_vec_x[j-r][k-r]
                                -calc_board_vec_x[j-r][k+r]-calc_board_vec_x[j+r][k-r]) / (double) (4*r*r+4*r+1),2)
                          +(double)pow(  (calc_board_vec_y[j+r][k+r]+calc_board_vec_y[j-r][k-r]
                                -calc_board_vec_y[j-r][k+r]-calc_board_vec_y[j+r][k-r]) / (double) (4*r*r+4*r+1),2)
                        ,0.5);

                ans_board_scalar[j][k]=( calc_board_scalar[j+r][k+r]+calc_board_scalar[j-r][k-r]
                                        -calc_board_scalar[j-r][k+r]-calc_board_scalar[j+r][k-r]) 
                                        / (double) (4*r*r+4*r+1) ;
            }
        }


        ofstream ofs_csv_file_v(save_foldername+"/v"+to_string(i)+".csv");
        for(int j=0;j<cell_size_y;j++){
            for(int k=0;k<cell_size_x;k++){

                ofs_csv_file_v<<ans_board_vec[j][k]<<",";

            }
            ofs_csv_file_v<<endl;
        }

        ofstream ofs_csv_file_s(save_foldername+"/s"+to_string(i)+".csv");
        for(int j=0;j<cell_size_y;j++){
            for(int k=0;k<cell_size_x;k++){

                ofs_csv_file_s<<ans_board_scalar[j][k]<<",";

            }
            ofs_csv_file_s<<endl;
        }
    }
    cout<<"->succeed\n";
}

void vfield::Delete(){
        cout<<"deleting vfield. "<<flush;
        for(int i=0;i<num_of_frame;i++){
            for(int v=0;v<cell_size_y;v++){
                delete[] v_field_x[i][v];
                delete[] v_field_y[i][v];
            }
            delete[] v_field_x[i];
            delete[] v_field_y[i];
        }
        delete[] v_field_x;
        delete[] v_field_y;
        cout<<"->succeed\n";
}



