#include <bits/stdc++.h>
#include <string>

#include "API.h"
using namespace std;
void log(const std::string& text) {
    std::cerr << text << std::endl;
}
class spot{
    public:
        int cost;
        bool l;
        bool r;
        bool u;
        bool d;
        spot(){
            cost=INT_MAX;
            l=false;
            r=false;
            u=false;
            d=false;
        }
};

int mazeWidth=15; //horizontal
int mazeHeight=15; //vertical

void check_wall(int mouse[],int face,vector<vector<spot>> &grid){
        if(API::wallFront()){
            if(face==0){
                grid[mouse[0]][mouse[1]].u=true;
                if(mouse[0]<mazeHeight-1) grid[mouse[0]+1][mouse[1]].d=true;
                API::setWall(mouse[1],mouse[0],'n');
            }
            else if(face==1){
                grid[mouse[0]][mouse[1]].r=true;
                if(mouse[1]<mazeWidth-1) grid[mouse[0]][mouse[1]+1].l=true;
                API::setWall(mouse[1],mouse[0],'e');
            }
            else if(face==2){
                grid[mouse[0]][mouse[1]].d=true;
                if(mouse[0]>0) grid[mouse[0]-1][mouse[1]].u=true;
                API::setWall(mouse[1],mouse[0],'s');
            }
            else{
                grid[mouse[0]][mouse[1]].l=true;
                if(mouse[1]>0) grid[mouse[0]][mouse[1]-1].r=true;
                API::setWall(mouse[1],mouse[0],'w');
            }
        }
        if(API::wallLeft()){
            if(face==0){
                grid[mouse[0]][mouse[1]].l=true;
                if(mouse[1]>0) grid[mouse[0]][mouse[1]-1].r=true;
                API::setWall(mouse[1],mouse[0],'w');
            }
            else if(face==1){
                grid[mouse[0]][mouse[1]].u=true;
                if(mouse[0]<mazeHeight-1) grid[mouse[0]+1][mouse[1]].d=true;
                API::setWall(mouse[1],mouse[0],'n');
            }
            else if(face==2){
                grid[mouse[0]][mouse[1]].r=true;
                if(mouse[1]<mazeWidth-1) grid[mouse[0]][mouse[1]+1].l=true;
                API::setWall(mouse[1],mouse[0],'e');
            }
            else{
                grid[mouse[0]][mouse[1]].d=true;
                if(mouse[0]>0) grid[mouse[0]-1][mouse[1]].u=true;
                API::setWall(mouse[1],mouse[0],'s');
            }
        }
        if(API::wallRight()){
            if(face==0){
                grid[mouse[0]][mouse[1]].r=true;
                if(mouse[1]<mazeWidth-1) grid[mouse[0]][mouse[1]+1].l=true;
                API::setWall(mouse[1],mouse[0],'e');
            }
            else if(face==1){
                grid[mouse[0]][mouse[1]].d=true;
                if(mouse[0]>0) grid[mouse[0]-1][mouse[1]].u=true;
                API::setWall(mouse[1],mouse[0],'s');
            }
            else if(face==2){
                grid[mouse[0]][mouse[1]].l=true;
                if(mouse[1]>0) grid[mouse[0]][mouse[1]-1].r=true;
                API::setWall(mouse[1],mouse[0],'w');
            }
            else{
                grid[mouse[0]][mouse[1]].u=true;
                if(mouse[0]<mazeHeight-1) grid[mouse[0]+1][mouse[1]].d=true;
                API::setWall(mouse[1],mouse[0],'n');
            }
        }
}
void turn(int mouse[],int face,int move){
    if(move==0){
        if(face==1) API::turnLeft();
        else if(face==2){
            API::turnLeft();
            API::turnLeft();
        }
        else if(face==3) API::turnRight();

        mouse[0]+=1;
    }
    else if(move==1){
        if(face==0) API::turnRight();
        else if(face==3){
            API::turnLeft();
            API::turnLeft();
        }
        else if(face==2) API::turnLeft();

        mouse[1]+=1;
    }
    else if(move==2){
        if(face==1) API::turnRight();
        else if(face==0){
            API::turnLeft();
            API::turnLeft();
        }
        else if(face==3) API::turnLeft();

        mouse[0]-=1;
    }
    else{
        if(face==2) API::turnRight();
        else if(face==1){
            API::turnLeft();
            API::turnLeft();
        }
        else if(face==0) API::turnLeft();

        mouse[1]-=1;
    }
    API::moveForward(1);
}
void floodfill(int centers[4][2],vector<vector<spot>> &grid){
    queue<pair<int,int>> q;
    set<pair<int,int>> s;
    for(int i=0;i<mazeHeight;i++){
        for(int j=0;j<mazeWidth;j++){
            grid[i][j].cost=INT_MAX;
        }
    }
    if(centers[0][0]==0){
        q.push({0,0});
        s.insert({0,0});
        grid[0][0].cost=0;
    }
    else{
    for(int i=0;i<4;i++){
        q.push({centers[i][0],centers[i][1]});
        s.insert({centers[i][0],centers[i][1]});
        grid[centers[i][0]][centers[i][1]].cost=0;
    }
    }
    while(!q.empty()){
        pair<int,int> temp = q.front();
        q.pop();
        s.erase(temp);
        int cur_y=temp.first;
        int cur_x=temp.second;
        int cur_cost = grid[cur_y][cur_x].cost + 1;
        if(cur_y>0 && !grid[cur_y][cur_x].d && grid[cur_y-1][cur_x].cost > cur_cost && s.find({cur_y-1,cur_x})==s.end()){
            grid[cur_y-1][cur_x].cost=cur_cost;
            q.push({cur_y-1,cur_x});
            s.insert({cur_y-1,cur_x});
            API::setText(cur_x,cur_y-1,to_string(cur_cost));
        }
        if(cur_x>0 && !grid[cur_y][cur_x].l && grid[cur_y][cur_x-1].cost > cur_cost && s.find({cur_y,cur_x-1})==s.end()){
            grid[cur_y][cur_x-1].cost=cur_cost;
            q.push({cur_y,cur_x-1});
            s.insert({cur_y,cur_x-1});
            API::setText(cur_x-1,cur_y,to_string(cur_cost));
        }
        if(cur_y<mazeHeight-1 && !grid[cur_y][cur_x].u && grid[cur_y+1][cur_x].cost > cur_cost && s.find({cur_y+1,cur_x})==s.end()){
            grid[cur_y+1][cur_x].cost=cur_cost;
            q.push({cur_y+1,cur_x});
            s.insert({cur_y+1,cur_x});
            API::setText(cur_x,cur_y+1,to_string(cur_cost));
        }
        if(cur_x<mazeWidth-1 && !grid[cur_y][cur_x].r && grid[cur_y][cur_x+1].cost > cur_cost && s.find({cur_y,cur_x+1})==s.end()){
            grid[cur_y][cur_x+1].cost=cur_cost;
            q.push({cur_y,cur_x+1});
            s.insert({cur_y,cur_x+1});
            API::setText(cur_x+1,cur_y,to_string(cur_cost));
        }
    }
}
void algorithm(int mouse[],int &face,vector<vector<spot>> &grid,int centers[4][2]){
    //int centers[4][2]={{7,7},{8,7},{7,8},{8,8}};
    floodfill(centers,grid);
    while((mouse[0]!=centers[0][0] || mouse[1]!=centers[0][1]) && (mouse[0]!=centers[1][0] || mouse[1]!=centers[1][1]) && (mouse[0]!=centers[2][0] || mouse[1]!=centers[2][1]) && (mouse[0]!=centers[3][0] || mouse[1]!=centers[3][1])){
        //wall check karo
        int cur_y=mouse[0],cur_x=mouse[1];
        check_wall(mouse,face,grid);
        int cur_cost=grid[cur_y][cur_x].cost;
        int move=-1;
        if(cur_y>0 && !grid[cur_y][cur_x].d && grid[cur_y-1][cur_x].cost <= cur_cost){
            if(grid[cur_y-1][cur_x].cost == cur_cost){
                if(move!=-1 && (face==2 || abs(face-move)==2)) move=2;
            }
            else{
                cur_cost=grid[cur_y-1][cur_x].cost;
                move=2;
            }
        }
        if(cur_x>0 && !grid[cur_y][cur_x].l && grid[cur_y][cur_x-1].cost <= cur_cost){
            if(grid[cur_y][cur_x-1].cost == cur_cost){
                if(move!=-1 && (face==3 || abs(face-move)==2)) move=3;
            }
            else{
                cur_cost=grid[cur_y][cur_x-1].cost;
                move=3;
            }
        }
        if(cur_y<mazeHeight && !grid[cur_y][cur_x].u && grid[cur_y+1][cur_x].cost <= cur_cost){
            if(grid[cur_y+1][cur_x].cost == cur_cost){
                if(move!=-1 && (face==0 || abs(face-move)==2)) move=0;
            }
            else{
                cur_cost=grid[cur_y+1][cur_x].cost;
                move=0;
            }
        }
        if(cur_x<mazeWidth && !grid[cur_y][cur_x].r && grid[cur_y][cur_x+1].cost <= cur_cost){
            if(grid[cur_y][cur_x+1].cost == cur_cost){
                if(move!=-1 && (face==1 || abs(face-move)==2)) move=1;
            }
            else{
                cur_cost=grid[cur_y][cur_x+1].cost;
                move=1;
            }
        }
        if(move==-1){
            floodfill(centers,grid);
        }
        else{
            turn(mouse,face,move);
            face=move;
        }
    }
}
int main(int argc, char* argv[]) {
    log("Running...");
    API::setColor(0, 0, 'G');
    API::setText(0, 0, "abc");

    mazeWidth=API::mazeWidth(); //horizontal
    mazeHeight=API::mazeHeight(); //vertical

    vector<vector<spot>> grid;
    vector<spot> temp;
    spot a_spot;
    for(int i=0;i<mazeWidth;i++) temp.push_back(a_spot);
    for(int i=0;i<mazeHeight;i++) grid.push_back(temp);
    int centers[4][2]={{7,7},{8,7},{7,8},{8,8}};
    int home[4][2]={{0,0},{0,0},{0,0},{0,0}};
    int mouse[2]={0,0};
    int face=0; //0:up , 1:right , 2:down , 3:left

    algorithm(mouse,face,grid,centers);
    algorithm(mouse,face,grid,home);
    //path select karke fast run karna hai
    algorithm(mouse,face,grid,centers);
    algorithm(mouse,face,grid,home);

    return 0;
}
