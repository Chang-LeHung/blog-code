#include <stdio.h>

int main() {
  float depositAmount, interest1, interest2, total1, total2;
  int numYears;

  printf("请输入存储金额: ");
  scanf("%f", &depositAmount);
  printf("请输入存储年限: ");
  scanf("%d", &numYears);

  if (numYears == 1) {
    interest1 = depositAmount * 0.015;
  } else if (numYears == 2) {
    interest1 = depositAmount * 0.025;
  } else {
    interest1 = depositAmount * 0.035;
  }
  total1 = depositAmount + interest1;

  if (numYears <= 3) {
    interest2 = depositAmount * 0.035;
  } else {
    interest2 = depositAmount * 0.045;
  }
  total2 = depositAmount + interest2;

  printf("2年总额: %.2f\n", total1);
  printf("4年总额 %.2f\n", total2);

  return 0;
}
