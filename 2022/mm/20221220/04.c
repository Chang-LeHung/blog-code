#include <stdio.h>

int main(void)
{
    // 定义数组用于存放5个同学的成绩信息
    int scores[5];

    // 输入每个同学的成绩
    for (int i = 0; i < 5; i++) {
        printf("输入第 %d 个同学的成绩: ", i + 1);
        scanf("%d", &scores[i]);
    }

    // 初始化最高分为数组中的第一个元素
    int max = scores[0];

    // 遍历数组，找出最高分
    for (int i = 1; i < 5; i++) {
        if (scores[i] > max) {
            max = scores[i];
        }
    }

    // 输出最高分
    printf("最高分: %d\n", max);

    return 0;
}
