/*************************************************************************
	> File Name: haha.cpp
	> Author: YinJianxiang
	> Mail: YinJianxiang123@gmail.com
	> Created Time: 2017年12月27日 星期三 14时31分21秒
 ************************************************************************/

#include<iostream>
#include<cstdlib>
#include <readline/readline.h>
#include <readline/history.h>

using namespace std;

int main(int argc,char *argv[]) {
	char * a;
	char b[256];
	
	a = readline(b);
    
	free(a);
	return 0;
}

