#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 计算模式串的部分匹配表（next数组）
void get_next(const char pattern[], int next[]) {
    int len = strlen(pattern);
    int i = 0;
    int j = -1;
    next[0] = -1;

    while (i < len - 1) {
        if (j == -1 || pattern[i] == pattern[j]) {
            ++i;
            ++j;
            next[i] = j;
        } else {
            j = next[j];
        }
    }
}

// 使用KMP算法在文本串S中查找模式串T的位置
int Index_KMP(const char S[], const char T[], const int next[]) {
    int i = 0;
    int j = 0;
    int sLen = strlen(S);
    int tLen = strlen(T);

    while (i < sLen && j < tLen) {
        if (j == -1 || S[i] == T[j]) {
            ++i;
            ++j;
        } else {
            j = next[j];
        }
    }

    if (j == tLen) {
        return i - j; // 返回模式串在文本串中的起始位置
    } else {
        return -1; // 未找到匹配位置
    }
}

int main() {
    FILE *file;
    char *pi_digits = NULL;
    long file_size;
    const char *pattern = "20021108"; // 要查找的数字序列
    int pattern_length = strlen(pattern);
    int *next = (int *)malloc(sizeof(int) * pattern_length);

    // 打开文件
    file = fopen("D:\\pi.txt", "r");
    if (file == NULL) {
        fprintf(stderr, "无法打开文件\n");
        return 1;
    }

    // 获取文件大小
    fseek(file, 0, SEEK_END);
    file_size = ftell(file);
    rewind(file);

    // 分配内存并读取文件内容
    pi_digits = (char *)malloc(file_size + 1);
    if (pi_digits == NULL) {
        fprintf(stderr, "内存分配失败\n");
        fclose(file);
        return 1;
    }

    fread(pi_digits, 1, file_size, file);
    pi_digits[file_size] = '\0'; // 添加字符串结束符

    fclose(file);

    // 计算模式串的部分匹配表（next数组）
    get_next(pattern, next);

    // 在圆周率字符串中查找目标数字序列
    int pos = Index_KMP(pi_digits, pattern, next);

    if (pos != -1) {
        printf("找到目标数字序列在圆周率中的位置：%d\n", pos+1);
    } else {
        printf("未找到目标数字序列在圆周率中\n");
    }

    free(pi_digits);
    free(next);

    return 0;
}
