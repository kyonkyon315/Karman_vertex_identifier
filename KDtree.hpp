#ifndef kyoya_h_
#define kyoya_h_

#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <time.h>
#include "make_HD_v_field.hpp"

using namespace std;

class Node{
    public:
    int *x;
    int *y;
    int size;
    int y_max;
    int y_min;
    int x_max;
    int x_min;
    int depth;
    int *id;

    Node *left;// left is bigger
    Node *right;
    Node *parent;
    void generatechild();

    int distance_between_point(int _x,int _y);

    void Delete();

    void print();
};

class KDTree{
    int maxvalue=1e8;
    public:
    Node *root;
    bool initialized = false;
    int min_distance;
    Node *exist;
    Node *ans;

    VPC* vpcptr;
    vfield* vfieldptr;

    void init(VPC* &vpcptr,int* _x,int* _y,int _size);

    void DFS_in_CreateTree(Node *node);

    void CreateTree();

    void DFS_in_search(Node* &node,int x,int y);

    void search_node_exist(int x,int y);

    void DFS_in_nearestnode(Node* node_now,int &x,int &y);

    Node* nearestnode(int x,int y);

    void Query(int gridsize_x,int gridsize_y,int** &vx_,int** &vy_,int frame,int cell_length);

    void Delete();//動的確保する予定はないので自分自身は消去しない。


    void print();
};
/*使用例
    int main(){


    //frame1とframe2の定義(std::vector<int> を用いる。)
    vector<int> data1_x;
    vector<int> data1_y;
    vector<int> data2_x;
    vector<int> data2_y;


    n=4000;
    m=4000;
    for(int i=0;i<n;i++){
        data1_x.push_back(i*(i/17)*(i/19));
        data1_y.push_back(i*(i/21)*(i/23));
    }
    for(int i=0;i<m;i++){
        data2_x.push_back(100*i);
        data2_y.push_back(150*i);
    }



    //kdtreeを定義
    KDTree kdtree;

    //frame1の挿入
    kdtree.init(data1_x,data1_y);

    //木の構築
    kdtree.CreateTree();


    //frame2を入れて、答えを出力
    vector<int> ans=kdtree.Query(data2_x,data2_y);

    //使用後は必ず削除(かなり容量をくう)
    kdtree.Delete();

    return 0;
}
*/


#endif
