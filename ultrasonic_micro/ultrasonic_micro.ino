// ye libraries download kar lena
#include <ArduinoQueue.h>
#include<Vector.h>
#include "gridBox.h"

#define INT_MAX 2147483647
#define tl 1
#define el 2
#define tr 3
#define er 4
#define tf 5
#define ef 6

int mazeWidth=16; //horizontal
int mazeHeight=16; //vertical
Vector<Vector<spot>> grid;

void setup(){
  // put your setup code here, to run once:
  pinMode(tl,OUTPUT);
  pinMode(tr,OUTPUT);
  pinMode(tf,OUTPUT);

  Vector<spot> temp;
  spot a_spot;
  for(int i=0;i<mazeWidth;i++) temp.push_back(a_spot);
  for(int i=0;i<mazeHeight;i++) grid.push_back(temp);
  
  Serial.begin(9600);
}
bool wl=false,wr=false,wf=false;
float disl=0.0,disr=0.0,disf=0.0;
float dis=11.0;
void sense(){
   digitalWrite(tl,LOW);
   digitalWrite(tr,LOW);
   digitalWrite(tf,LOW);
   delay(10);
   digitalWrite(tl,HIGH);
   digitalWrite(tr,HIGH);
   digitalWrite(tf,HIGH);
   delay(10);
   digitalWrite(tl,LOW);
   digitalWrite(tr,LOW);
   digitalWrite(tf,LOW);
   
   disl=pulseIn(el,HIGH);
   disr=pulseIn(er,HIGH);
   disf=pulseIn(ef,HIGH);
   disl/=2;
   disl=disl*0.0343;
   disr/=2;
   disr=disr*0.0343;
   disf/=2;
   disf=disf*0.0343;

   if(disl<dis){
      wl=true;
   }
   else{
      wl=false;
   }
   if(disr<dis){
      wr=true;
   }
   else{
      wr=false;
   }
   if(disf<dis){
      wf=true;
   }
   else{
      wf=false;
   }
}
void check_wall(int mouse[],int face,Vector<Vector<spot>> &grid){
        sense();
        if(wf){
            if(face==0){
                grid[mouse[0]][mouse[1]].u=true;
                if(mouse[0]<mazeHeight-1) grid[mouse[0]+1][mouse[1]].d=true;
            }
            else if(face==1){
                grid[mouse[0]][mouse[1]].r=true;
                if(mouse[1]<mazeWidth-1) grid[mouse[0]][mouse[1]+1].l=true;
            }
            else if(face==2){
                grid[mouse[0]][mouse[1]].d=true;
                if(mouse[0]>0) grid[mouse[0]-1][mouse[1]].u=true;
            }
            else{
                grid[mouse[0]][mouse[1]].l=true;
                if(mouse[1]>0) grid[mouse[0]][mouse[1]-1].r=true;
            }
        }
        if(wl){
            if(face==0){
                grid[mouse[0]][mouse[1]].l=true;
                if(mouse[1]>0) grid[mouse[0]][mouse[1]-1].r=true;
            }
            else if(face==1){
                grid[mouse[0]][mouse[1]].u=true;
                if(mouse[0]<mazeHeight-1) grid[mouse[0]+1][mouse[1]].d=true;
            }
            else if(face==2){
                grid[mouse[0]][mouse[1]].r=true;
                if(mouse[1]<mazeWidth-1) grid[mouse[0]][mouse[1]+1].l=true;
            }
            else{
                grid[mouse[0]][mouse[1]].d=true;
                if(mouse[0]>0) grid[mouse[0]-1][mouse[1]].u=true;
            }
        }
        if(wr){
            if(face==0){
                grid[mouse[0]][mouse[1]].r=true;
                if(mouse[1]<mazeWidth-1) grid[mouse[0]][mouse[1]+1].l=true;
            }
            else if(face==1){
                grid[mouse[0]][mouse[1]].d=true;
                if(mouse[0]>0) grid[mouse[0]-1][mouse[1]].u=true;
            }
            else if(face==2){
                grid[mouse[0]][mouse[1]].l=true;
                if(mouse[1]>0) grid[mouse[0]][mouse[1]-1].r=true;
            }
            else{
                grid[mouse[0]][mouse[1]].u=true;
                if(mouse[0]<mazeHeight-1) grid[mouse[0]+1][mouse[1]].d=true;
            }
        }
}
//ye sab likho
void turnLeft(){
  return;
}
void turnRight(){
  return;
}
void moveForward(){
  return;
}
void turn(int mouse[],int face,int Move){
    if(Move==0){
        if(face==1) turnLeft();
        else if(face==2){
            turnLeft();
            turnLeft();
        }
        else if(face==3) turnRight();

        mouse[0]+=1;
    }
    else if(Move==1){
        if(face==0) turnRight();
        else if(face==3){
            turnLeft();
            turnLeft();
        }
        else if(face==2) turnLeft();

        mouse[1]+=1;
    }
    else if(Move==2){
        if(face==1) turnRight();
        else if(face==0){
            turnLeft();
            turnLeft();
        }
        else if(face==3) turnLeft();

        mouse[0]-=1;
    }
    else{
        if(face==2) turnRight();
        else if(face==1){
            turnLeft();
            turnLeft();
        }
        else if(face==0) turnLeft();

        mouse[1]-=1;
    }
    moveForward();
}

void floodfill(int centers[4][2],Vector<Vector<spot>> &grid){
    ArduinoQueue<Vector<int>> q(256);
    
    Vector<Vector<int>> s;
    Vector<int> initializer;
    for(int i=0;i<mazeWidth;i++) initializer.push_back(0);
    for(int i=0;i<mazeHeight;i++) s.push_back(initializer);
    
    for(int i=0;i<mazeHeight;i++){
        for(int j=0;j<mazeWidth;j++){
            grid[i][j].cost=INT_MAX;
        }
    }
    if(centers[0][0]==0){
        Vector<int> initializer2;
        initializer2.push_back(0);
        initializer2.push_back(0);
        q.enqueue(initializer2);
        s[0][0]=1;
        grid[0][0].cost=0;
    }
    else{
    for(int i=0;i<4;i++){
        Vector<int> initializer2;
        initializer2.push_back(centers[i][0]);
        initializer2.push_back(centers[i][1]);
        q.enqueue(initializer2);
        s[centers[i][0]][centers[i][1]]=1;
        grid[centers[i][0]][centers[i][1]].cost=0;
    }
    }
    while(!q.isEmpty()){
        Vector<int> temp = q.dequeue();
        s[temp[0]][temp[1]]=0;
        int cur_y=temp[0];
        int cur_x=temp[1];
        int cur_cost = grid[cur_y][cur_x].cost + 1;
        if(cur_y>0 && !grid[cur_y][cur_x].d && grid[cur_y-1][cur_x].cost > cur_cost && s[cur_y-1][cur_x]==0){
            grid[cur_y-1][cur_x].cost=cur_cost;
            Vector<int> initializer2;
            initializer2.push_back(cur_y-1);
            initializer2.push_back(cur_x);
            q.enqueue(initializer2);
            s[cur_y-1][cur_x]=1;
        }
        if(cur_x>0 && !grid[cur_y][cur_x].l && grid[cur_y][cur_x-1].cost > cur_cost && s[cur_y][cur_x-1]==0){
            grid[cur_y][cur_x-1].cost=cur_cost;
            Vector<int> initializer2;
            initializer2.push_back(cur_y);
            initializer2.push_back(cur_x-1);
            q.enqueue(initializer2);
            s[cur_y][cur_x-1]=1;
        }
        if(cur_y<mazeHeight-1 && !grid[cur_y][cur_x].u && grid[cur_y+1][cur_x].cost > cur_cost && s[cur_y+1][cur_x]==0){
            grid[cur_y+1][cur_x].cost=cur_cost;
            Vector<int> initializer2;
            initializer2.push_back(cur_y+1);
            initializer2.push_back(cur_x);
            q.enqueue(initializer2);
            s[cur_y+1][cur_x]=1;
        }
        if(cur_x<mazeWidth-1 && !grid[cur_y][cur_x].r && grid[cur_y][cur_x+1].cost > cur_cost && s[cur_y][cur_x+1]==0){
            grid[cur_y][cur_x+1].cost=cur_cost;
            Vector<int> initializer2;
            initializer2.push_back(cur_y);
            initializer2.push_back(cur_x+1);
            q.enqueue(initializer2);
            s[cur_y][cur_x+1]=1;
        }
    }
}

void algorithm(int mouse[],int &face,Vector<Vector<spot>> &grid,int centers[4][2]){
    //int centers[4][2]={{7,7},{8,7},{7,8},{8,8}};
    floodfill(centers,grid);
    while((mouse[0]!=centers[0][0] || mouse[1]!=centers[0][1]) && (mouse[0]!=centers[1][0] || mouse[1]!=centers[1][1]) && (mouse[0]!=centers[2][0] || mouse[1]!=centers[2][1]) && (mouse[0]!=centers[3][0] || mouse[1]!=centers[3][1])){
        //wall check karo
        int cur_y=mouse[0],cur_x=mouse[1];
        check_wall(mouse,face,grid);
        int cur_cost=grid[cur_y][cur_x].cost;
        int Move=-1;
        if(cur_y>0 && !grid[cur_y][cur_x].d && grid[cur_y-1][cur_x].cost <= cur_cost){
            if(grid[cur_y-1][cur_x].cost == cur_cost){
                if(Move!=-1 && (face==2 || abs(face-Move)==2)) Move=2;
            }
            else{
                cur_cost=grid[cur_y-1][cur_x].cost;
                Move=2;
            }
        }
        if(cur_x>0 && !grid[cur_y][cur_x].l && grid[cur_y][cur_x-1].cost <= cur_cost){
            if(grid[cur_y][cur_x-1].cost == cur_cost){
                if(Move!=-1 && (face==3 || abs(face-Move)==2)) Move=3;
            }
            else{
                cur_cost=grid[cur_y][cur_x-1].cost;
                Move=3;
            }
        }
        if(cur_y<mazeHeight && !grid[cur_y][cur_x].u && grid[cur_y+1][cur_x].cost <= cur_cost){
            if(grid[cur_y+1][cur_x].cost == cur_cost){
                if(Move!=-1 && (face==0 || abs(face-Move)==2)) Move=0;
            }
            else{
                cur_cost=grid[cur_y+1][cur_x].cost;
                Move=0;
            }
        }
        if(cur_x<mazeWidth && !grid[cur_y][cur_x].r && grid[cur_y][cur_x+1].cost <= cur_cost){
            if(grid[cur_y][cur_x+1].cost == cur_cost){
                if(Move!=-1 && (face==1 || abs(face-Move)==2)) Move=1;
            }
            else{
                cur_cost=grid[cur_y][cur_x+1].cost;
                Move=1;
            }
        }
        if(Move==-1){
            floodfill(centers,grid);
        }
        else{
            turn(mouse,face,Move);
            face=Move;
        }
    }
}

int centers[4][2]={{7,7},{8,7},{7,8},{8,8}};
int Home[4][2]={{0,0},{0,0},{0,0},{0,0}};
int mouse[2]={0,0};
int face=0; //0:up , 1:right , 2:down , 3:left


void loop() {
    //ON KEY PRESS ka code likho
    algorithm(mouse,face,grid,centers);
    algorithm(mouse,face,grid,Home);
}
