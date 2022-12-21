#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_STUDENTS 5
#define NUM_SUBJECTS 3

int generate_score() {
  return rand() % 100 + 1;
}

int main() {
  srand(time(0));  

  for (int i = 0; i < NUM_STUDENTS; i++) {
    int total_score = 0;

    printf("学生 %d:\n", i + 1);
    for (int j = 0; j < NUM_SUBJECTS; j++) {
      int score = generate_score();
      printf("\t科目 %d: %d\n", j + 1, score);
      total_score += score;
    }
    printf("\t总分： %d\n", total_score);
    printf("\t平均分： %.2f\n", (float) total_score / NUM_SUBJECTS);
  }

  return 0;
}
