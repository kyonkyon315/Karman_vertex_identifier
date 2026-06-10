#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <time.h>
#include "KDtree.hpp"
#include "make_HD_v_field.hpp"

using namespace std;

int variance(int *data,int datasize){//分散を求める
    int mean=0;
    int v=0;
    for(int i=0; i < datasize; i++){
        mean+=data[i];
        v   +=data[i]*data[i];
    }
    mean /= datasize;
    v    /= datasize;
    v    -= mean*mean;
    return v;
}

void Node:: generatechild(){
        if(size <= 1){
            cout<<"error: this Node has only one value, but you attempt to generate this children.\n"<<flush;
            return ;
        }
        left = new Node;
        right= new Node;

        left  -> size =  size   /2;
        right -> size = (size+1)/2;
        left  -> depth= this->depth+1;
        right -> depth= this->depth+1;

        left  -> x    = new int[left  -> size];
        left  -> y    = new int[left  -> size];
        left  -> id   = new int[left  -> size];
        right -> x    = new int[right -> size];
        right -> y    = new int[right -> size];
        right -> id   = new int[right -> size];

        left  -> parent = this;
        right -> parent = this;

        left  -> left   =nullptr;
        left  -> right  =nullptr;
        right -> left   =nullptr;
        right -> right  =nullptr;

        if(variance(x,size) > variance(y,size)){
            int it[size];
            for(int i=0;i<size;i++)it[i]=i;
            sort(it,it+size,[&](int p,int q){return x[p] > x[q];});

            for(int i=0           ; i < left  -> size; i++){
                left  ->x[i]              = this -> x[it[i]];
                left  ->y[i]              = this -> y[it[i]];
                left  ->id[i]             = this ->id[it[i]];
            }

            for(int i=left -> size; i < this  -> size; i++){
                right ->x[i - left->size]  = this -> x[it[i]];
                right ->y[i - left->size]  = this -> y[it[i]];
                right ->id[i - left->size] = this ->id[it[i]];
            }

            left  -> y_max = this -> y_max;
            right -> y_max = this -> y_max;
            left  -> y_min = this -> y_min;
            right -> y_min = this -> y_min;

            left  -> x_max = this -> x_max;
            right -> x_min = this -> x_min;
            left  -> x_min = this -> x[it[left -> size]];
            right -> x_max = this -> x[it[left -> size]];
        }
        else{
            int it[size];
            for(int i=0;i<size;i++)it[i]=i;
            sort(it,it+size,[&](int p,int q){return y[p] > y[q];});

            for(int i=0           ; i < left  -> size; i++){
                left  ->x[i]              = this -> x[it[i]];
                left  ->y[i]              = this -> y[it[i]];
                left  ->id[i]             = this ->id[it[i]];
            }

            for(int i=left -> size; i < this  -> size; i++){
                right ->x[i - left->size] = this -> x[it[i]];
                right ->y[i - left->size] = this -> y[it[i]];
                right ->id[i - left->size]= this ->id[it[i]];
            }

            left  -> x_max = this -> x_max;
            right -> x_max = this -> x_max;
            left  -> x_min = this -> x_min;
            right -> x_min = this -> x_min;

            left  -> y_max = this -> y_max;
            right -> y_min = this -> y_min;
            left  -> y_min = this -> y[it[left -> size]];
            right -> y_max = this -> y[it[left -> size]];
        }
}

int Node::distance_between_point(int _x,int _y){
        int delta_x,delta_y;
        if     (_x > this->x_max)delta_x=_x-this->x_max;
        else if(_x > this->x_min)delta_x=0;
        else                     delta_x=_x-this->x_min;

        if     (_y > this->y_max)delta_y=_y-this->y_max;
        else if(_y > this->y_min)delta_y=0;
        else                     delta_y=_y-this->y_min;
        return delta_x*delta_x+delta_y*delta_y;
}

void Node::Delete(){
        if(left  != nullptr)left  -> Delete();
        if(right != nullptr)right -> Delete();
        delete[] x;
        delete[] y;
        delete[] id;
        delete this;
}

void Node::print(){
        for(int i=0;i<this->depth;i++){
            cout<<"   ";
        }
        for(int i=0;i<this->size;i++){
            cout<<"("<<this->x[i]<<","<<this->y[i]<<") ";
        }
        cout<<"\n";
        if(this->right != nullptr)this->right->print();
        if(this->left  != nullptr)this->left ->print();
}


void KDTree::init(VPC* &vpcptr_,int* _x,int* _y,int size_){
        vpcptr=vpcptr_;
        if(size_ < 1){

            cout<<"input_data's size is 0\n";
            return;

        }

        if(initialized){

            cout<<"error: in void KDTree::init() in KDtree.cpp.\n";
            cout<<"you need to Delete kdtree before initializing it.\n";
            return;

        }

        initialized = true;

        root = new Node;
        root -> size = size_;

        root-> x=new int[root->size];
        root-> y=new int[root->size];
        root->id=new int[root->size];
        root-> depth=0;

        for(int i=0; i < root->size; i++){
            root -> x[i] = _x[i];
            root -> y[i] = _y[i];
            root -> id[i]= i;
        }
        root -> parent =nullptr;
        root -> left   =nullptr;
        root -> right  =nullptr;

        root -> x_max =  maxvalue;
        root -> x_min = -maxvalue;
        root -> y_max =  maxvalue;
        root -> y_min = -maxvalue;
}

void KDTree::DFS_in_CreateTree(Node *node){
        if(node->size < 2){
            return;
        }
        node->generatechild();
        DFS_in_CreateTree(node->left );
        DFS_in_CreateTree(node->right);
}

void KDTree::CreateTree(){
        DFS_in_CreateTree(root);
}

void KDTree::DFS_in_search(Node* &node,int x,int y){
        if(node->size == 1){
            return;
        }
        else{
            if(node->left->x_min < x && node->left->y_min < y){
                node = node->left;
            }
            else{
                node = node->right;
            }
            DFS_in_search(node,x,y);
        }
}

void KDTree::search_node_exist(int x,int y){
        this->exist=root;
        this->DFS_in_search(exist,x,y);
}

void KDTree::DFS_in_nearestnode(Node* node_now,int &x,int &y){
        if(node_now->size == 1){
            if((node_now->x[0] - x)*(node_now->x[0]-x) + (node_now->y[0] - y)*(node_now->y[0]) < min_distance){
                this->min_distance =(node_now->x[0]-x)*(node_now->x[0]-x) + (node_now->y[0] - y)*(node_now->y[0]-y);
                this->ans = node_now;
            }
        }
        else{
            if(node_now->left->distance_between_point(x,y)<min_distance){
                DFS_in_nearestnode(node_now->left,x,y);
            }

            if(node_now->right->distance_between_point(x,y)<min_distance){
                DFS_in_nearestnode(node_now->right,x,y);
            }
        }
}

Node* KDTree::nearestnode(int x,int y){

        if(!initialized){

            cout<<"error: even though it is not initialized, KDTree::nearestnode() was called\n"<<flush;

        }

        if(x<-maxvalue || maxvalue<x || y<-maxvalue || maxvalue<y){
            cout<<"error: |input query| is bigger than maxvalue("<<maxvalue<<")\n";
            cout<<"(x,y)= ("<<x<<","<<y<<")\n";
            cout<<"this is skipped and  given nullptr.\n";
            return nullptr;
        }
        search_node_exist(x,y);
        ans   = exist;
        Node* start = exist;
        min_distance=pow(exist->x[0] - x,2)+pow(exist->y[0] - y,2);
        DFS_in_nearestnode(root,x,y);
        return this->ans;
}

void KDTree::Query(int gridsize_x,int gridsize_y,int** &ans_vx,int** &ans_vy,int frame,int cell_length){
        if(! initialized){

            cout<<"it is not initialized or input query's size = 0\n";
            cout<<"it can cause error later\n"<<flush;

        }
        for(int i=0;i<gridsize_y;i++){
            for(int v=0;v<gridsize_x;v++){
                Node* ansnode=nearestnode(cell_length*v,cell_length*i);
                ans_vx[i][v]=vpcptr->vx[frame][ansnode->id[0]];
                ans_vy[i][v]=vpcptr->vy[frame][ansnode->id[0]];
            }
        }
}

void KDTree::Delete(){//動的確保する予定はないので自分自身は消去しない。

        if(initialized){
            root->Delete();
        }

        root = nullptr;
        initialized = false;
}

void KDTree::print(){
        this->root->print();
}



