/*************************************************************************
	> File Name: haha.cpp
	> Author: YinJianxiang
	> Mail: YinJianxiang123@gmail.com
	> Created Time: 2017年12月27日 星期三 14时31分21秒
 ************************************************************************/
 #include<iostream>
 #include<cstdio>
 #include<cstdlib>
 #include<cstring>
 #include<sys/ioctl.h>
 #include<sys/signal.h>
 #include<sys/wait.h>
 #include<sys/time.h>
 #include<sys/types.h>
 #include<sys/stat.h>
 #include<readline/readline.h>
 #include<readline/history.h>
 #include<unistd.h>
 #include<fcntl.h>
using namespace std;

int main(int argc,char *argv[]) {
	char str_1[256] = "first\n";
    char str_2[256] = "second\n";
    char *t_1;
    char *t_2;
    
    t_1 = readline(str_1);
    t_2 = readline(str_2);
    printf("(%s) (%s)\n",t_1,t_2);

    return 0;
}

