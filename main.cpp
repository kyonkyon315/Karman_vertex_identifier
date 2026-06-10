#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <filesystem>
#include "make_HD_v_field.hpp"
using namespace std;

vector<vector<int>>directions={
                               {1,0},
                               {1,1},
                               {1,-1},
                               {-1,0},
                               {-1,1},
                               {-1,-1},
                               {0,1},
                               {0,-1},};
int min_x_in_cp;
int min_y_in_cp;
double min_value_in_cp;

void dfs_in_connected_pixels(vector<vector<int>> &ans,vector<vector<double>> &info,double kyokai,int label,int x,int y){
    if(min_value_in_cp>info[y][x]){
        min_value_in_cp=info[y][x];
        min_x_in_cp=x;
        min_y_in_cp=y;
    }
    ans[y][x]=label;
    info[y][x]=kyokai+1;
    for(int i=0;i<8;i++){
        if(     y+directions[i][0]>=0 && y+directions[i][0]<1920
             && x+directions[i][1]>=0 && x+directions[i][1]<1080
             && info[y+directions[i][0]][x+directions[i][1]]<kyokai){
            dfs_in_connected_pixels(ans,info,kyokai,label,x+directions[i][1],y+directions[i][0]);
        }
    }
}

vector<vector<int>> connected_pixels(vector<vector<double>>&info,double kyokai){
    vector<vector<int>>ans((int)info.size(),vector<int>((int)info[0].size(),0));

    vector<vector<int>> real_ans(0,vector<int>(2,0));
    int label=1;
    for(int i=0;i<(int)info.size();i++){
        for(int j=0;j<(int)info[0].size();j++){
            if(info[i][j]<kyokai){
                min_x_in_cp=j;
                min_y_in_cp=i;
                min_value_in_cp=info[i][j];
                dfs_in_connected_pixels(ans,info,kyokai,label,j,i);
                label+=1;
                vector<int> ans_p={min_x_in_cp,min_y_in_cp};
                real_ans.push_back(ans_p);
            }
        }
    }

    //return ans;
    return real_ans;
};


int main(){

    double kyokai=0.3;
    int min_num=20;
    int r=50;


    vector<string> namelist={
 "0620_1525",
 "0619_1554",
 "0703_1351",
 "0619_1532",
 "0619_1535",
 "0619_1600",
 "0620_1517",
 "0620_1520",
 "0620_1522",
 "0625_1553",
 "0625_1624",
 "0702_1419",
 "0702_1421",
 "0702_1423",
 "0702_1429",
 "0702_1430",
 "0702_1431",
 "0702_1441",
 "0702_1443",
 "0702_1445",
 "0702_1457",
 "0702_1453",
 "0702_1459",
 "0702_1508",
 "0702_1510",
 "0702_1512",
 "0702_1515",
 "0702_1517",
 "0702_1521",
 "0702_1537",
 "0702_1539",
 "0702_1540",
 "0703_1329",
 "0703_1330",
 "0703_1334",
 "0703_1340",
 "0703_1342",
 "0703_1344",
 "0703_1347",
 "0710_1616",
 "0710_1619",
 "0710_1622",
 "0710_1625",
 "0710_1629",
 "0711_1751",
 "0711_1754",
 "0711_1758",
};


    for(int movie=0;movie<(int)namelist.size();movie++){


    string name=namelist[movie];


    string foldername="../chibutu2_v2/PVC/VPC_"+name;
    string save_foldername="chushin/"+name;

    VPC* vpc=new VPC;
    vpc->Get_data(foldername);

    //vpc->px[frame]
    //vpc->py[frame]
    //vpc->vx[frame]
    //vpc->vy[frame]
    //vpc->num_of_frame
    //vpc->size_of_each_frame
    int NF=vpc->num_of_frame;
    vector<vector<vector<double>>> v_dot_r(NF,vector<vector<double>>(1920,vector<double>(1080,0.)));
    vector<vector<vector<int>>> num_dot(NF,vector<vector<int>>(1920,vector<int>(1080,0)));
    vector<vector<vector<int>>> num_plus_dot(NF,vector<vector<int>>(1920,vector<int>(1080,0)));

    int x_max=1080;
    int y_max=1920;
    for(int i=0;i<NF;i++){
        for(int j=0;j<vpc->size_of_each_frame[i];j++){
            if(vpc->px[i][j]<r || vpc->px[i][j]>=x_max-r||vpc->py[i][j]<r || vpc->py[i][j]>=y_max-r){
                continue;
            }

            for(int dx=-r;dx<r+1;dx++){
                for(int dy=-r;dy<r+1;dy++){
                    double v_r=pow((double)
                                     (vpc->vx[i][j]*vpc->vx[i][j]+vpc->vy[i][j]*vpc->vy[i][j])*
                                     (dx*dx+dy*dy),
                                 0.5);
                    if(v_r>0.){

                        v_dot_r[i][vpc->py[i][j]+dy][vpc->px[i][j]+dx]
                                 +=(((double)abs(-vpc->vx[i][j]*dx-vpc->vy[i][j]*dy))/v_r);

                        num_dot[i][vpc->py[i][j]+dy][vpc->px[i][j]+dx]+=1;

                        if(-dx*vpc->vy[i][j]+dy*vpc->vx[i][j] > 0){

                            num_plus_dot[i][vpc->py[i][j]+dy][vpc->px[i][j]+dx]+=1;

                        }

                    }
                }
            }
        }
    }

    cout<<"a\n"<<flush;

    for(int i=0;i<NF;i++){
        for(int j=0;j<y_max;j++){
            for(int k=0;k<x_max;k++){
                if(num_dot[i][j][k]<min_num){
                    v_dot_r[i][j][k]=50.;
                }
                else if((double)num_plus_dot[i][j][k]/(double)num_dot[i][j][k]>0.375 &&
                        (double)num_plus_dot[i][j][k]/(double)num_dot[i][j][k]<0.625 ){
                    v_dot_r[i][j][k]=50.;
                }
                else{
                    v_dot_r[i][j][k]/=(double)num_dot[i][j][k];
                }
            }
        }
    }
    cout<<"a\n"<<flush;

    vector<vector<int>> center_x(NF,vector<int>(0,0));
    vector<vector<int>> center_y(NF,vector<int>(0,0));

    for(int i=0;i<NF;i++){
        vector<vector<double>> info=v_dot_r[i];
        vector<vector<int>> ans=connected_pixels(info,kyokai);
        for(int j=0;j<(int)ans.size();j++){
            center_x[i].push_back(ans[j][0]);
            center_y[i].push_back(ans[j][1]);
        }
    }
/*
    cout<<"a\n"<<flush;

    for(int i=0;i<NF;i++){
        cout<<"frame:"<<i<<"\n";
        for(int j=0;j<(int)center_x[i].size();j++){
            cout<<"("<<center_x[i][j]<<","<<center_y[i][j]<<") ";
        }
        cout<<"\n";
    }
*/
    cout<<"saving place of center of each vertex.";

    filesystem::path dir =save_foldername;

    if(!filesystem::exists(dir)){
        filesystem::create_directory(dir);
    }else{
        cout<<"The directory named "<<save_foldername<<" already exists.\n";
    }

    int max_length=(int)center_x[0].size();
    for(int i=0;i<(int)center_x.size();i++){
        max_length=max(max_length,(int)center_x[i].size());
    }

    ofstream ofs_txt_file(save_foldername+"/read_by_python.txt");
    ofs_txt_file <<"pvcname:"<<foldername<<endl;
    ofs_txt_file <<"type:chushin"<<endl;
    ofs_txt_file <<"gridsize_T:"<<NF<<endl;
    ofs_txt_file <<"max_length:"<<max_length<<endl;
    ofs_txt_file <<"kyokai:"<<kyokai<<endl;
    ofs_txt_file <<"min_num:"<<min_num<<endl;
    ofs_txt_file <<"r:"<<r<<endl;

    ofstream ofs_csv_file_c_x(save_foldername+"/c_x.csv");
    for(int i=0;i<(int)center_x.size();i++){
        ofs_csv_file_c_x<<center_x[i].size()<<',';
        for(int j=0;j<(int)center_x[i].size();j++){
            ofs_csv_file_c_x<<center_x[i][j]<<',';
        }
        for(int j=(int)center_x[i].size();j<max_length;j++){
            ofs_csv_file_c_x<<0<<',';
        }
        ofs_csv_file_c_x<<endl;
    }

    ofstream ofs_csv_file_c_y(save_foldername+"/c_y.csv");
    for(int i=0;i<(int)center_y.size();i++){
        ofs_csv_file_c_y<<center_y[i].size()<<',';
        for(int j=0;j<(int)center_y[i].size();j++){
            ofs_csv_file_c_y<<center_y[i][j]<<',';
        }
        for(int j=(int)center_y[i].size();j<max_length;j++){
            ofs_csv_file_c_y<<0<<',';
        }
        ofs_csv_file_c_y<<endl;
    }
    cout<<"->succeed\n";

    vpc->Delete();
    delete vpc;

    }///end of for(int movie・・・・


    return 0;
}
