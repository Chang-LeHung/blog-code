#include<iostream>
#include<queue>
using namespace std;
int movex[8] = { -2,-1,1,2,2,1,-1,-2 }, movey[8] = { 1,2,2,1,-1,-2,-2,-1 };
//Point类记录坐标与当前步数
class Point{
	public:
		int x,y;
		int pathN;//将当前步数与坐标绑定
		Point(int xx,int yy,int n):x(xx),y(yy),pathN(n){}
};
int bfs(Point &end,queue<Point> &list,int a[][9])
{
	while(!list.empty())
	{
		Point nowVertex(list.front());
		list.pop();
		if(nowVertex.x==end.x&&nowVertex.y==end.y)
		{
			return nowVertex.pathN;
		}else{
			a[nowVertex.x][nowVertex.y]=1;
			for(int i=0;i<8;i++)
			{
				Point temp(nowVertex.x+movex[i],nowVertex.y+movey[i],nowVertex.pathN+1);
				if((a[temp.x][temp.y]==0)&&(temp.x>=1&&temp.x<=9)&&(temp.y>=1&&temp.y<=9))
				{
					cout<<temp.x<<temp.y<<" ";
					list.push(temp);
				} 
			}
		}
	}
}
int main()
{
	while(1)
	{
		int a[9][9];//棋盘状态
		for (int i = 1; i < 9; i++)
		{
			for (int j = 1; j < 9; j++)
			{
				a[i][j] = 0;
			}
		}
		queue<Point> list;
		string str1,str2;
		cin >> str1 >> str2;
		Point start(str1[0]-96,str1[1]-48,0);
		Point end(str2[0]-96,str2[1]-48,0);
		list.push(start);
		cout<<"min is:"<<bfs(end,list,a)<<endl;
	}
	return 0;
}