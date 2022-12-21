
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdio>

using namespace std;
 

struct Node {
    int coeff;
    int pow;
    struct Node* next;
};
 
void create_node(int x, int y, struct Node** temp)
{
    struct Node *r, *z;
    z = *temp;
    if (z == NULL) {
        r = (struct Node*)malloc(sizeof(struct Node));
        r->coeff = x;
        r->pow = y;
        *temp = r;
        r->next = (struct Node*)malloc(sizeof(struct Node));
        r = r->next;
        r->next = NULL;
    }
    else {
        r->coeff = x;
        r->pow = y;
        r->next = (struct Node*)malloc(sizeof(struct Node));
        r = r->next;
        r->next = NULL;
    }
}
 
void polyadd(struct Node* poly1, struct Node* poly2,
             struct Node* poly)
{
    while (poly1->next && poly2->next) {
        if (poly1->pow > poly2->pow) {
            poly->pow = poly1->pow;
            poly->coeff = poly1->coeff;
            poly1 = poly1->next;
        }
 
        else if (poly1->pow < poly2->pow) {
            poly->pow = poly2->pow;
            poly->coeff = poly2->coeff;
            poly2 = poly2->next;
        }
        else {
            poly->pow = poly1->pow;
            poly->coeff = poly1->coeff + poly2->coeff;
            poly1 = poly1->next;
            poly2 = poly2->next;
        }
         poly->next
            = (struct Node*)malloc(sizeof(struct Node));
        poly = poly->next;
        poly->next = NULL;
    }
    while (poly1->next || poly2->next) {
        if (poly1->next) {
            poly->pow = poly1->pow;
            poly->coeff = poly1->coeff;
            poly1 = poly1->next;
        }
        if (poly2->next) {
            poly->pow = poly2->pow;
            poly->coeff = poly2->coeff;
            poly2 = poly2->next;
        }
        poly->next
            = (struct Node*)malloc(sizeof(struct Node));
        poly = poly->next;
        poly->next = NULL;
    }
}
 
void show(struct Node* node)
{
  vector<string> data;
  while (node->next != NULL) {
    char s[1024];
    sprintf(s, "%dx^%d", node->coeff, node->pow);
    printf("%dx^%d", node->coeff, node->pow);
    node = node->next;
    if (node->coeff >= 0) {
        if (node->next != NULL)
        {
          printf("+");
          s[strlen(s)] = '+';
          s[strlen(s)] = '\0';
        }
    }
    data.push_back(string(s));
  }
  reverse(data.begin(), data.end());
  printf("\n");
  string final;
  for(int i = 0; i < data.size(); ++i)
  {
    if(final.size() > 0 && (final[final.size() - 1] == '+' ||
      data[i][0] == '-'))
      final.append(data[i]);
    else
    {
      if(i != 0)
        final.append("+");
      final.append(data[i]);
    }
  }
  if(final[final.size() - 1] == '+' || final[final.size() - 1] == '-')
  {
    final = final.substr(0, final.length() - 1);
  }
  cout << "閫嗗簭杈撳嚭" << final << endl;
}

int main()
{
    struct Node *poly1 = NULL, *poly2 = NULL, *poly = NULL;
 
    // Create first list of 5x^2 + 4x^1 + 2x^0
    create_node(5, 2, &poly1);
    create_node(4, 1, &poly1);
    create_node(2, 0, &poly1);
 
    // Create second list of -5x^1 - 5x^0
    create_node(-5, 1, &poly2);
    create_node(-5, 0, &poly2);
 
    printf("1st Number: ");
    show(poly1);
 
    printf("\n2nd Number: ");
    show(poly2);
 
    poly = (struct Node*)malloc(sizeof(struct Node));
 
    polyadd(poly1, poly2, poly);
 
    printf("\nAdded polynomial: ");
    show(poly);

    int n;
    cout << "璇疯緭鍏ヤ綘瑕佹帓搴忕殑鏁版嵁鐨勪釜鏁癨n";
    cin >> n;
    vector<int> nums;
    for(int i = 0; i < n; ++i)
    {
      int n;
      cin >> n;
      nums.push_back(n);
    }
    sort(nums.begin(), nums.end());
    for(auto d : nums)
      cout << d << " " << endl;
    return 0;
}