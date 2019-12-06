#include<iostream>
#include<cstdlib>
#include<ctime>
#include<windows.h>

#define M 30    //围墙的宽
#define N 40    //围墙的高
#define LEN sizeof(struct snake)
#define KEY_DOWN(VK_NONAME)((GetAsyncKeyState(VK_NONAME)&0x8000)?1:0)

using namespace std;

typedef struct snake{ //蛇的某一节身体的坐标链节
    int x;
    int y;
    struct snake *next;
}snake,*Snake;
//全局变量
Snake head;     //蛇头
int m,n;        //食物坐标
int sec=200;    //移动的时间间隔初始值为200毫秒
int grade=0;    //分数初始值0分
//需要编写的函数
void cursor(bool flag){        //隐藏或显示光标
    HANDLE h_GAME =GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursor_info;
    GetConsoleCursorInfo(h_GAME,&cursor_info);
    cursor_info.bVisible=flag;
    SetConsoleCursorInfo(h_GAME,&cursor_info);
    return ;
}

void move_pos(int x,int y){     //移动光标
    COORD pos;
    pos.X=x;
    pos.Y=y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),pos);
    return ;
}

void CreateSnake(void){         //产生最初的蛇
    Snake p,q;

    head = new snake;
    p = new snake;
    q = new snake;

    p->x=N/2;
    p->y=M/2;
    p->x=N/2-1;
    p->y=M/2;

    head->next=p;
    p->next=q;
    q->next=NULL;

    move_pos(N/2-1,M/2);
    cout<<"**";
}

void CreateHead(int x,int y){   //构建头部
    Snake p = new snake;
    p->x=x;
    p->y=y;
    p->next=head->next;
    head->next=p;
}

void delTail(void){             //消去尾部
    int x,y;
    Snake p,q;

    q=head;
    p=head->next;
    while(p->next){
        q=q->next;
        p=p->next;
    }

    x=p->x;
    y=p->y;
    q->next=NULL;
    delete p;
    move_pos(x,y);
    cout<<" ";  //用空格覆盖尾部
    return ;
}

void ProFood(void){             //产生食物
    time_t t;
    Snake p;
    srand(time(&t));
    while(true){
        n=rand()%N;
        m=rand()%M;

        if(n<=1||n>=N-2||m<=0||m>=M-1)
            continue;
        for(p=head->next;p;p=p->next)
            if(p->x==n&&p->y==m)
                continue;

        move_pos(n,m);
        cout<<"*";
        return ;
    }
}

void Draw(void){                //画出围墙，蛇，食物
    move_pos(0,0);
    for(int i=0;i<N;i++)        //上墙
        cout<<"#";
    move_pos(0,M-1);
    for(int i=0;i<N;i++)        //下墙
        cout<<"#";
    move_pos(0,0);
    for(int i=0;i<M;i++)        //左墙
        cout<<"##"<<endl;
    for(int i=0;i<M;i++){       //右墙
        move_pos(N-2,i);
        cout<<"##";
    }

    char menu[7][50]={          //菜单
        "Gluttonous Snake",
        "The Developer:Love of Butterfly",
        " ",
        "up   : W",
        "down : S",
        "left : A",
        "right: D"};
    for(int i=0;i<7;i++){
        move_pos(N+20,M/2-5+i);
        cout<<menu[i];
    }

    CreateSnake();
    ProFood();

    move_pos(N+20,M/2+5);
    cout<<"Grade:0";
    return ;
}

bool Move(int dx,int dy){       //蛇的移动
    int x,y;
    Snake p;
    p=head->next;
    x=p->x+dx;
    y=p->y+dy;

    if(x<=1||x>=N-2||y<=0||y>=M-1)  //是否撞墙
            return false;
    for(p=p->next;p;p=p->next)      //是否咬尾
        if(p->x==x&&p->y==y)
            return false;

    CreateHead(x,y);
    if(x==n&&y==m){     //是否吃到食物
        if(sec>50)
            sec-=5;
        grade++;
        move_pos(N+26,M/2+5);
        cout<<grade;
        ProFood();
    }
    else{
        move_pos(x,y);
        cout<<"*";
        delTail();
    }
    return true;
}

void Play(void){                //游戏中
    int dx=1,dy=0;

    int now,last;
    now=last=clock();

    while(true){
        now=clock();
        if(now-last>=sec){      //隔sec毫秒移动一次
            if(!Move(dx,dy))
                return ;
            last=now;
        }
        //改变方向
        if(KEY_DOWN('D')&&dx==0){
            dx=1;
            dy=0;
        }
        else if(KEY_DOWN('A')&&dx==0){
            dx=-1;
            dy=0;
        }
        else if(KEY_DOWN('W')&&dy==0){
            dx=0;
            dy=-1;
        }
        else if(KEY_DOWN('S')&&dy==0){
            dx=0;
            dy=1;
        }
    }
}

void GameOver(void){            //游戏结束
    move_pos(N/2-7,M/2-1);
    cout<<"Grade:"<<grade;
    move_pos(N/2-7,M/2);
    cout<<"Game Over!";
    move_pos(N/2-7,M/2+1);
    system("pause");
    return ;
}

int main(void){
    system("color 7A");     //设置界面颜色
    cursor(false);          //隐藏光标
    Draw();
    Play();
    GameOver();
    system("cls");
    return 0;
}
