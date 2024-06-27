#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include<windows.h>

#define STACK_INIT_SIZE 20//存储空间初始分配量
#define STACKINCREMENT 10//存储空间配增量

typedef struct {
    int i;
    int j;   //通道在迷宫的坐标
} PosType;

typedef struct {
    int ord;   //通道序号
    PosType seat; //坐标位置
    int di;    //下一块通道方向
} SElemType;  //元素类型

typedef struct {
    SElemType *base;   //栈底指针
    SElemType *top;   //栈顶指针
    int stacksize;  //容量
} SqStack;

typedef struct {
    int **map;     //迷宫地图（二维数组建立）
    int row, col;   //迷宫的行列
} Mazetype;

SqStack *InitStack(SqStack *S) {
    S->base = (SElemType *)malloc(STACK_INIT_SIZE * sizeof(SElemType));
    if (!S->base)
        exit(0);
    S->top = S->base;
    S->stacksize = STACK_INIT_SIZE;
    return S;
}

int IsEmpty(SqStack *S) {
    return (S->top == S->base);
}

void DestroyStack(SqStack *S) {
    free(S->base);
    S->base = NULL;
    S->top = NULL;
    S->stacksize = 0;
}

SqStack *Push(SqStack *S, SElemType e) {
    if (S->top - S->base >= S->stacksize) {
        S->base = (SElemType *)realloc(S->base, (S->stacksize + STACKINCREMENT) * sizeof(SElemType));
        if (!S->base)
            exit(0);
        S->top = S->base + S->stacksize;
        S->stacksize += STACKINCREMENT;
    }

    *(S->top) = e;
    S->top++;
    return S;
}

SqStack *Pop(SqStack *S, SElemType *e) {
    if (IsEmpty(S))
        return S;

    *e = *(--S->top);
    return S;
}

SElemType GetTop(SqStack *S) {
    if (!IsEmpty(S))
        return *(S->top - 1);
    else
        exit(0); // 如果栈空直接返回
}
int determine_Maze(Mazetype *M, int m, int n){   //返回地图坐标m、n处是通路还是墙
     if(M->map[m][n]==0){   //墙返回1
        return 0;
     }
     return 1;              //通路返回0          
}
void Create_Maze(Mazetype *M, int m, int n) { //创建迷宫地图
    int i, j;
    M->col = n;
    M->row = m;

    M->map = (int **)malloc(M->col * sizeof(int *)); //申请迷宫内存
    if (!M->map)           
        exit(0);

    for (i = 0; i < M->col; i++) {
        M->map[i] = (int *)malloc(M->row * sizeof(int));
        if (!M->map[i])
            exit(0);
    }
    
     /*srand(123);*/ srand((unsigned)time(NULL)); 
    for (i = 0; i < M->col; i++) {    //生成地图，默认0为墙，1可以行走
        for (j = 0; j < M->row; j++) {
            if (i == 0 || j == 0 || i == M->col - 1 || j == M->row - 1)
                M->map[i][j] = 0; // 设置边缘，迷宫边缘必须为墙
            else
             M->map[i][j] = (rand() % 3 == 0) ? 0 : 1; //随机生成地图 
        }
    }
}

void Print_Maze(Mazetype *M) {
    int i, j;

    printf("  ");
    for (i = 0; i < M->row; i++)
        printf("%-2d", i);
    printf("\n");

    for (i = 0; i < M->col; i++) {
        printf("%-2d", i);
        for (j = 0; j < M->row; j++) {
            /*if((i==3||(i==2))&&j==3){
               printf("  ");
            }
            else{*/switch (M->map[i][j]) {
                case 0: printf("* "); break; // 障碍
                case 1: printf("  "); break; // 通路
                case 2: printf("+ "); break; // 足迹
                case 4: printf("S "); break; // 起点
                case 5: printf("E "); break; // 终点
                default: printf("  "); break;}
            //}
        }
        printf("\n");
    }
}

PosType NextPos(PosType pos, int di) {
    switch (di) {
        case 1: pos.j++; break; // 右
        case 2: pos.i++; break; // 下
        case 3: pos.j--; break; // 左
        case 4: pos.i--; break; // 上
    }
    return pos;
}

void FindOut(int **maze, PosType Start, PosType End) {
    
    SqStack S;
    SElemType e;
    PosType position = Start;
    int curstep = 1;
    int count=0;
    InitStack(&S);
    do {
        if (maze[position.i][position.j] == 1) {  //1表面是未走过的可通路径
            maze[position.i][position.j] = 2;   //   把路径标记为2，然后入栈 
            count ++;
            e.di = 1;
            e.seat.i = position.i;
            e.seat.j = position.j;
            e.ord = curstep;
            Push(&S, e);

            if (position.i == End.i && position.j == End.j)//终点
                break;

            position = NextPos(position, 1);
            curstep++;      //继续探索
        } else {
            if (!IsEmpty(&S)) {   //空栈直接退出
                Pop(&S, &e);
                curstep--;

                while (e.di == 4 && !IsEmpty(&S)) {
                    maze[e.seat.i][e.seat.j] = 3; //   
                    Pop(&S, &e);
                    curstep--;
                }

                if (e.di < 4) {
                    e.di++;
                    Push(&S, e);
                    curstep++;
                    position = NextPos(e.seat, e.di);
                }
            }
        }
    } while (!IsEmpty(&S));

 
    if (IsEmpty(&S)) {
        maze[Start.i][Start.j] = 1; // 
        printf("\nNo solution.\n");
    } else {
        printf("\nPath found:\n");
        while (!IsEmpty(&S)) {
            e = GetTop(&S);
            printf("(%d,%d) -> ", e.seat.i, e.seat.j);  //打印路径
            Pop(&S, &e);
        }
        printf("\n");
        printf("试过的次数为%d",count);
    }

    DestroyStack(&S);
}

int main() {
    int Si, Sj, Ei, Ej;
    int LSize, RSize;
    Mazetype M;
    PosType Start, End;

    printf("Enter Maze Size (LSize RSize): ");
    scanf("%d %d", &LSize, &RSize);

    Create_Maze(&M, LSize, RSize);
    Print_Maze(&M);

    printf("Enter Start (Si Sj): ");
    scanf("%d %d", &Si, &Sj);

    while (Si >= LSize || Sj >= RSize || M.map[Si][Sj] == 0) {
        printf("Invalid Start. Enter Start (Si Sj): ");
        scanf("%d %d", &Si, &Sj);
    }

    printf("Enter End (Ei Ej): ");
    scanf("%d %d", &Ei, &Ej);

    while (Ei >= LSize || Ej >= RSize || M.map[Ei][Ej] == 0) {
        printf("Invalid End. Enter End (Ei Ej): ");
        scanf("%d %d", &Ei, &Ej);
    }
    LARGE_INTEGER frequency;
    LARGE_INTEGER start, end;
    double cpu_time_used;

    // 获取计时器频率
    QueryPerformanceFrequency(&frequency);

    // 记录开始时间
    QueryPerformanceCounter(&start);


    Start.i = Si;
    Start.j = Sj;
    End.i = Ei;
    End.j = Ej;

    FindOut(M.map, Start, End);

    M.map[Start.i][Start.j] = 4; // Mark start
    M.map[End.i][End.j] = 5; // Mark end

  // 记录结束时间
    QueryPerformanceCounter(&end);

    // 计算执行时间（单位：秒）
    cpu_time_used = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;

    printf("程序运行时间为 %.6f 秒\n", cpu_time_used);


    printf("\n'*' is an obstacle, '+' is the path.\n");
    Print_Maze(&M);

    // Free dynamically allocated memory
    for (int i = 0; i < M.col; i++)
        free(M.map[i]);
    free(M.map);

    return 0;
}

