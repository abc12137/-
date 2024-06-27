#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_ROWS 100
#define MAX_COLS 200
#define INF INT_MAX
#define MAX 200

typedef struct {
    int num;      // 景点序号
    char *name;   // 景点名称
    char *blurb;  // 景点简介
} Viewpoint;

typedef struct {
    int numEdges, numVertices; // 顶点数和边数
    int Edge[14][14];          // 邻接矩阵
    Viewpoint point[14];       // 景点信息数组
} Graph;

// 创建景点
Viewpoint create_point(int num1, const char *name, const char *blurb) {
    Viewpoint v;
    v.num = num1;
    v.name = strdup(name); 
    v.blurb = strdup(blurb); 
    return v;
}

// 创建图
void create(Graph* G) {
    G->numVertices = 14;
    G->numEdges = 18;
    G->point[0] = create_point(0, "北门", "很少有人走的地方");
    G->point[1] = create_point(1, "听海苑", "信息学院宿舍，独立卫浴");
    G->point[2] = create_point(2, "国旗处", "五星红旗迎风飘扬");
    G->point[3] = create_point(3, "东操场", "打篮球、跑步不错的地方");
    G->point[4] = create_point(4, "综合体", "图书馆、学习的去处，山字形大楼");
    G->point[5] = create_point(5, "信院南", "本专业的楼");
    G->point[6] = create_point(6, "信院北", "电子信息专业的楼");
    G->point[7] = create_point(7, "听海餐厅", "已经吃腻了的食堂");
    G->point[8] = create_point(8, "东门", "出门的地方、大部分走的地方、连接望海");
    G->point[9] = create_point(9, "正门", "没开放的门、还在修啊");
    G->point[10] = create_point(10, "西操场", "上体育课的地方，能再修大一点吗？");
    G->point[11] = create_point(11, "望海餐厅", "西区同学的吃饭处");
    G->point[12] = create_point(12, "工程楼", "工程学院的专业楼");
    G->point[13] = create_point(13, "望海苑", "工程学院的学生住所");

    // 初始化邻接矩阵
    for (int i = 0; i < 14; i++) {
        for (int j = 0; j < 14; j++) {
            if (i == j) {
                G->Edge[i][j] = 0;
            } else {
                G->Edge[i][j] = -1;
            }
        }
    }

    G->Edge[0][1] = 8;
    G->Edge[1][2] = 15;
    G->Edge[1][7] = 7;
    G->Edge[2][4] = 6;
    G->Edge[3][4] = 3;
    G->Edge[3][6] = 1;
    G->Edge[3][7] = 3;
    G->Edge[4][5] = 8;
    G->Edge[5][6] = 10;
    G->Edge[5][9] = 1;
    G->Edge[6][8] = 12;
    G->Edge[7][8] = 7;
    G->Edge[8][10] = 8;
    G->Edge[10][11] = 4;
    G->Edge[10][12] = 18;
    G->Edge[11][12] = 7;
    G->Edge[11][13] = 3;
    G->Edge[12][13] = 15;

    for (int i = 1; i < 14; i++) {
        for (int j = i + 1; j < 14; j++) {
            G->Edge[j][i] = G->Edge[i][j];
        }
    }
}

// 打印地图
void print_map(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }
    char map[MAX_ROWS][MAX_COLS];
    int row = 0;
    while (fgets(map[row], MAX_COLS, file)) {
        row++;
    }
    fclose(file);
    for (int i = 0; i < row; i++) {
        printf("%s", map[i]);
    }
}

// 查询景点信息
void viewpoint_information(Graph* g) {
    printf("请选择你想要查询的景点(选择景点前的选项)\n"); 
    printf(" 0,北门     1,听海苑     2,国旗处    3,东操场  4,综合体\n");
    printf(" 5,信院南   6,信院北     7,听海餐厅  8,东门    9,正门\n");
    printf(" 10,西操场  11,望海餐厅  12,工程楼   13,望海苑\n");
    int choice;
    scanf("%d", &choice);
    printf("%s\n", g->point[choice].name);//输出名称
    printf("%s\n", g->point[choice].blurb);//展示相关信息
}

// Dijkstra算法求最短路径
void dijkstra(Graph* G) {
    int n = G->numVertices;
    int start, end;
    printf("请选择你的起点和终点(选择景点前的选项)\n"); 
    printf(" 0,北门     1,听海苑     2,国旗处    3,东操场  4,综合体\n");
    printf(" 5,信院南   6,信院北     7,听海餐厅  8,东门    9,正门\n");
    printf(" 10,西操场  11,望海餐厅  12,工程楼   13,望海苑\n");
    scanf("%d %d", &start, &end);
    int distance[MAX], previous[MAX], visited[MAX];
    int i, count, next_node, min_distance;

    // 初始化
    for (i = 0; i < n; i++) {
        distance[i] = INF;
        previous[i] = -1;
        visited[i] = 0;
    }
    distance[start] = 0;

    for (count = 0; count < n - 1; count++) {
        min_distance = INF;

        // 找到距离最小的未访问节点
        for (i = 0; i < n; i++)
            if (!visited[i] && distance[i] < min_distance) {
                min_distance = distance[i];
                next_node = i;
            }

        // 标记为已访问
        visited[next_node] = 1;

        // 更新相邻节点的距离
        for (i = 0; i < n; i++)
            if (!visited[i] && G->Edge[next_node][i] != -1 && distance[next_node] != INF
                && distance[next_node] + G->Edge[next_node][i] < distance[i]) {
                distance[i] = distance[next_node] + G->Edge[next_node][i];
                previous[i] = next_node;
            }
    }

    // 输出路径
    int path[MAX];
    int path_index = 0;
    int current = end;
    while (current != -1) {
        path[path_index++] = current;
        current = previous[current];
    }

    if (distance[end] == INF) {
        printf("没有路径从 %d 到 %d\n", start, end);
        return;
    }

    printf("最短路径从 %s 到 %s: ", G->point[start].name, G->point[end].name);
    for (i = path_index - 1; i >= 0; i--) {
        printf("%s", G->point[path[i]].name);
        if (i > 0) printf(" -> ");
    }
    printf("\n");
    printf("总距离: %d(单位:10m)\n", distance[end]);
}

int main() {
    const char *filename = "D:\\code\\c\\formal_test6\\map.txt"; 
    Graph *g = (Graph *)malloc(sizeof(Graph));
    create(g); 
    printf("****************************************************\n");
    printf("欢迎来到中国海洋大学景点查询功能\n");
    printf("请选择功能\n");
    printf("1.查看校园平面图\n");
    printf("2.查询任意景点信息\n");
    printf("3.查询任意景点之间路径\n");
    printf("其他任意键退出\n");
    printf("****************************************************\n");
    int choice;
    scanf("%d", &choice);
    while (choice == 1 || choice == 2 || choice == 3) {
        switch (choice) {
            case 1:
                print_map(filename); 
                break;
            case 2:
                viewpoint_information(g);
                break;
            case 3:
                dijkstra(g);
                break;
            default:
                break;
        }
        printf("请选择功能\n");
        printf("1.查看校园平面图\n");
        printf("2.查询任意景点信息\n");
        printf("3.查询任意景点之间路径\n");
        printf("其他任意键退出\n");
        printf("****************************************************\n");
        scanf("%d", &choice);
    }
    return 0;
}
