#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 定义哈夫曼树节点结构体
typedef struct {
    char ch; // 字符
    int weight; // 权重
    int parent, lchild, rchild;
} HTNode, *HuffmanTree;

typedef char **HuffmanCode; // 存放哈夫曼编码

struct Char_frequency {
    char s; // 字符
    int f; // 字符出现的频率
};

HuffmanTree HT; // 全局变量存储赫夫曼树
HuffmanCode HC; // 全局变量存储赫夫曼编码
int count1; // 非零字符类型个数

void Select(HuffmanTree HT, int end, int *s1, int *s2);
void creat_HuffmanTree(HuffmanTree *HT, HuffmanCode *HC, struct Char_frequency *w, int n);
int calculate_frequency();
void write_encoded_text(struct Char_frequency* array, HuffmanCode HC, int count1);
void decode_text(HuffmanTree HT, int count1);
void compare_files();

int main() {
      // 计算字符频率
    if (calculate_frequency() != 0) {
        // 解码文本
        decode_text(HT, count1);
        // 比较文件内容
        compare_files();
    }
    return 0;
}

void write_encoded_text(struct Char_frequency* array, HuffmanCode HC, int count1) {
    FILE *inputFile = fopen("D:\\test.txt", "r");
    FILE *outputFile = fopen("D:\\text2.txt", "w");

    if (inputFile == NULL || outputFile == NULL) {
        fprintf(stderr, "无法打开文件\n");
        return;
    }

    char ch;
    while ((ch = fgetc(inputFile)) != EOF) {
        for (int i = 0; i < count1; i++) {
            if (ch == array[i].s) { // 在哈夫曼编码数组中找到对应的字符
                fprintf(outputFile, "%s", HC[i + 1]); // 写入对应的哈夫曼编码
                break;
            }
        }
    }

    fclose(inputFile);
    fclose(outputFile);
}

void Select(HuffmanTree HT, int end, int *s1, int *s2) {
    int min1 = -1, min2 = -1;
    for (int i = 1; i <= end; i++) {
        if (HT[i].parent == 0) {
            if (min1 == -1 || HT[i].weight < HT[min1].weight) {
                min2 = min1;
                min1 = i;
            } else if (min2 == -1 || HT[i].weight < HT[min2].weight) {
                min2 = i;
            }
        }
    }
    *s1 = min1;
    *s2 = min2;
}
//创建哈夫曼树
void creat_HuffmanTree(HuffmanTree *HT, HuffmanCode *HC, struct Char_frequency *w, int n) {
    if (n <= 1) return;
    int m = 2 * n - 1;
    *HT = (HuffmanTree)malloc((m + 1) * sizeof(HTNode)); // 0号单元未用
    // 初始化哈夫曼树的n个叶子节点的权值和字符
    for (int i = 1; i <= n; i++) {
        (*HT)[i].ch = w[i - 1].s; // 字符
        (*HT)[i].weight = w[i - 1].f; // 字符频率数组下标是从0开始的
        (*HT)[i].lchild = 0;
        (*HT)[i].rchild = 0;
        (*HT)[i].parent = 0;
    }
    // 初始化哈夫曼树的n+1到m个分支节点
    for (int i = n + 1; i <= m; i++) {
        (*HT)[i].weight = 0;
        (*HT)[i].lchild = 0;
        (*HT)[i].rchild = 0;
        (*HT)[i].parent = 0;
    }
    // 构建哈夫曼树
    for (int i = n + 1; i <= m; i++) {
        int s1, s2;
        Select(*HT, i - 1, &s1, &s2); // 选择两个权值最小的节点
        (*HT)[s1].parent = i;
        (*HT)[s2].parent = i;
        (*HT)[i].lchild = s1;
        (*HT)[i].rchild = s2;
        (*HT)[i].weight = (*HT)[s1].weight + (*HT)[s2].weight;
    }
    // 生成哈夫曼编码,逆向求编码
    *HC = (HuffmanCode)malloc((n + 1) * sizeof(char *));
    char *cd = (char *)malloc(n * sizeof(char));
    cd[n - 1] = '\0';
    for (int i = 1; i <= n; i++) {
        int start = n - 1;//编码结束位置
        for (int c = i, f = (*HT)[c].parent; f != 0; c = f, f = (*HT)[f].parent) {
            if ((*HT)[f].lchild == c) {
                cd[--start] = '0';
            } else {
                cd[--start] = '1';
            }
        }
        (*HC)[i] = (char *)malloc((n - start) * sizeof(char));
        strcpy((*HC)[i], &cd[start]);
         // 打印字符及其对应的哈夫曼编码
        printf("字符: %c  哈夫曼编码: %s\n", (*HT)[i].ch, (*HC)[i]);
    }
   
    free(cd);
}

int calculate_frequency() {
    FILE *file;
    int count = 0;
    count1 = 0; // 
    int frequency[127] = {0}; // ASCII码来映射到每个字符的，统计其相应的概率
    int ch;
    // 打开文件
    file = fopen("D:\\test.txt", "r");
    if (file == NULL) {
        fprintf(stderr, "无法打开文件\n");
        return 0;
    } else {
        while ((ch = fgetc(file)) != EOF) {
                 printf("%c",ch);
            if (ch >= 0 && ch < 127) {
                frequency[ch]++; // 更新相应字符的频率
            }
            count++;
        }
    }
    fclose(file);

    printf("原文字符数：%d\n", count);

    for (int i = 0; i < 127; i++) {
        printf("%c:%d ", i, frequency[i]);
        if (frequency[i] != 0)
            count1++;
    }
    printf("\n");
    printf("非零字符类型：%d\n", count1); // 统计非0字符个数，优化数组方便构建哈夫曼树

    struct Char_frequency *array = (struct Char_frequency *)malloc(sizeof(struct Char_frequency) * count1);
    if (array == NULL) {
        fprintf(stderr, "内存分配失败\n");
        return 0;
    }
    int j = 0;
    for (int i = 0; i < 127; i++) {
        if (frequency[i] != 0) {
            array[j].f = frequency[i];
            array[j].s = i;
            j++;
        }
    }
    for (int i = 0; i < count1; i++) {
        printf("%c:%d  ", array[i].s, array[i].f);
    }
    printf("\n");

    // 接下来调用哈夫曼树的创建函数生成哈夫曼编码
    creat_HuffmanTree(&HT, &HC, array, count1);

    // 将原文根据哈夫曼编码写入文件
    write_encoded_text(array, HC, count1);

    // 释放内存
    free(array);

    return 0;
}

void decode_text(HuffmanTree HT, int count1) {
    FILE *encodedFile = fopen("D:\\text2.txt", "r");
    FILE *outputFile = fopen("D:\\text3.txt", "w");

    if (encodedFile == NULL || outputFile == NULL) {
        fprintf(stderr, "无法打开文件\n");
        return;
    }

    int root = 2 * count1 - 1; // 赫夫曼树根节点的索引
    int current = root;
    char bit;

    while ((bit = fgetc(encodedFile)) != EOF) {
        if (bit == '0') {
            current = HT[current].lchild;
        } else if (bit == '1') {
            current = HT[current].rchild;
        }

        if (HT[current].lchild == 0 && HT[current].rchild == 0) { // 叶子节点
            fputc(HT[current].ch, outputFile); // 输出字符
            current = root; // 重置为根节点，开始解码下一个字符
        }
    }

    fclose(encodedFile);
    fclose(outputFile);
}
void compare_files() {
    FILE *file1;
    FILE *file2;
    int ch1, ch2;
    // 打开文件
    file1 = fopen("D:\\test.txt", "r");
    file2 = fopen("D:\\test3.txt", "r");
    if (file1 == NULL || file2 == NULL) {
        fprintf(stderr, "无法打开文件\n");
        return;
    } else {
        while ((ch1 = fgetc(file1)) != EOF && (ch2 = fgetc(file2)) != EOF) {
            if (ch1 != ch2) {
                printf("文件存在不同\n");
                fclose(file1);
                fclose(file2);
                return;
            }
        }
        
        if (feof(file1) && feof(file2)) {
            printf("文件完全一致\n");
        } else {
            printf("文件长度不同\n");
        }
    }
    
    // 关闭文件
    fclose(file1);
    fclose(file2);
}
