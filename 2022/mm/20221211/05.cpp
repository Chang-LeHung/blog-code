#include <iostream>
#include <string.h>
#include <algorithm>
using namespace std;
 
int main()
{
	int a[7], ans=0, area=36;
	bool flag = true;
	while (1) {
		flag = true;
		for (int i=1; i<=6; i++) {
			scanf ("%d", &a[i]);
			if (a[i] != 0) {
				flag = false;
			}
		}
		if (flag == true) {
			break;
		}
		ans=0;
		while(1) {
			flag = true;
			if (a[6]>0) {
				flag = false;
				a[6]--;
				ans++;
				area=36;
			} else if (a[5]>0) {
				a[5]--;
				if (a[1]>=11) {
					a[1] -= 11;
				} else {
					a[1] = 0;
				}
				flag = false;
				ans++;
				area = 36;
			} else if (a[4]>0) {
				a[4]--;
				area -= 16;
				if (a[2]>=5) {
					a[2] -= 5;
				} else {
					a[2] = 0;
					area -= a[2]*4;
						while (a[1]>0 && area>0) {
						area -= 1;
						a[1]--;
					}
				}
				flag = false;
				ans++;
				area=36;
			} else if (a[3]>0) {
				if (a[3]>=4) {
					a[3] -= 4;
				} else {
					area -= (9*a[3]);
					a[3] = 0;
					while (a[2]>0 && area>7) {
						area -= 4;
						a[2]--;
					}
					while (a[1]>0 && area>0) {
						area -= 1;
						a[1]--;
					}
				}
				flag = false;
				ans++;
				area=36;
			} else if (a[2]>0) {
				while (area>0 && a[2]>0) {
					area -= 4;
					a[2]--;
				}
				while (a[1]>0 && area>0) {
					area -= 1;
					a[1]--;
				}
				flag = false;
				ans++;
				area=36;
			} else if (a[1]>0) {
				while (area>0 && a[1]>0) {
					area -= 1;
					a[1]--;
				}
				flag = false;
				ans++;
				area=36;
			}
			if (flag == true)	
				break;
		}
		printf ("%d\n", ans);
	}
	return 0;
}