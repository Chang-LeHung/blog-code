#include <stdio.h>
#include <stdlib.h> // 包含rand函数所在的头文件
#include <time.h>   // 包含time函数所在的头文件

int main(void)
{
    int num1, num2, num3; // 定义三个整型变量用于存放随机数

    // 设置随机数种子，这样每次生成的随机数就不一样了
    srand((unsigned)time(NULL));

    // 随机生成三个50到100的随机数
    num1 = 50 + rand() % (100 - 50 + 1);
    num2 = 50 + rand() % (100 - 50 + 1);
    num3 = 50 + rand() % (100 - 50 + 1);

    // 输出这三个随机数
    printf("随机数1: %d\n", num1);
    printf("随机数2: %d\n", num2);
    printf("随机数3: %d\n", num3);

    // 找出这三个随机数中值最大的数
    int max = num1;
    if (num2 > max) {
        max = num2;
    }
    if (num3 > max) {
        max = num3;
    }

    // 输出最大值
    printf("最大值: %d\n", max);

    return 0;
}
