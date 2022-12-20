
#include <iostream>
#include <unordered_map>
#include <string>

using namespace std;
unordered_map<string, size_t> map;
bool isNum = false;
string all;

void remove_(string s)
{
  size_t pos;
  while((pos = all.find(s)) != s.npos)
  {
    all = all.erase(pos, s.size());
  }
}

int count_(string s)
{
  int cnt = 0;
  int index = 0;
  while( (index = all.find(s, index)) != all.npos ){
		cnt++;
		index++;
	}
  return cnt;
}

int main()
{
  cout << "\033[31m请输入文章内容 # 表示结束数据的输入\033[0m\n";
  map["blank"] = 0;
  map["char"]  = 0;
  map["all"]   = 0;
  map["num"]   = 0;
  char c;
  while((c = getchar()) != '#')
  {
    all.push_back(c);
    if(c == ' ')
    {
      map["blank"]++;
      continue;
    }
    else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
    {
      map["char"]++;
    }
    else if (c >= '0' && c <= '9')
    {
      map["num"]++;
    }
    map["all"]++;
  }
  cout << "\033[31m空格的数量为 " << map["blank"] << " \033[0m\n";
  cout << "\033[31m数字数量为 " << map["num"] << " \033[0m\n";
  cout << "\033[31m英文字母的数量为 " << map["char"] << " \033[0m\n";
  cout << "\033[31m文章总数为 " << map["all"] << " \033[0m\n";
  cout << "请输入要统计的子串\n";
  string t;
  cin >> t;
  cout << "\033[31m出现次数等于 = " << count_(t) << "\033[0m"<< endl;
  cout << "请输入要删除的子串\n";
  cin >> t;
  remove_(t);
  cout << "\033[31m最终的文章如下所示\033[0m\n";
  cout << all << endl;
  return 0;
}
