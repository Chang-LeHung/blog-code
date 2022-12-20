#include <stdio.h>

struct student {
    int id;
    char name[20];
    float grade1;
    float grade2;
    float grade3;
};

int main() {
    struct student s1 = {1, "Alice", 85.0, 90.0, 95.0};
    struct student s2 = {2, "Bob", 75.0, 80.0, 85.0};

    printf("学生 1:\nID: %d\n姓名: %s\n分数: %.1f %.1f %.1f\n", s1.id, s1.name, s1.grade1, s1.grade2, s1.grade3);
    printf("学生 2:\nID: %d\n姓名: %s\n分数: %.1f %.1f %.1f\n", s2.id, s2.name, s2.grade1, s2.grade2, s2.grade3);

    float total1 = s1.grade1 + s1.grade2 + s1.grade3;
    float average1 = total1 / 3;
    printf("总分: %.1f\n平均分: %.1f\n", total1, average1);
    float total2 = s2.grade1 + s2.grade2 + s2.grade3;
    float average2 = total2 / 3;
    printf("总分: %.1f\n平均分: %.1f\n", total2, average2);

    return 0;
}
