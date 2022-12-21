#include <stdio.h>

int main(void)
{
    char name[100]; // 定义字符数组用于存放姓名
    char student_id[100]; // 定义整型变量用于存放学号
    int age;        // 定义整型变量用于存放年龄

    // 从键盘输入姓名、学号和年龄
    printf("输入你的姓名: ");
    scanf("%s", name); // 使用%s格式字符输入字符数组
    printf("输入你的学号: ");
    scanf("%s", student_id); // 使用%d格式字符输入整型变量
    printf("输入你的年龄: ");
    scanf("%d", &age); // 使用%d格式字符输入整型变量

    // 输出姓名、学号和年龄
    printf("你的姓名是%s，学号是%s，年龄是%d\n", name, student_id, age);

    return 0;
}
