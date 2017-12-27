/*************************************************************************
	> File Name: thread.cpp
	> Author: YinJianxiang
	> Mail: YinJianxiang123@gmail.com
	> Created Time: 2017年12月26日 星期二 07时18分11秒
 ************************************************************************/

#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<pthread.h>
#include<sys/ioctl.h>
#include<sys/stat.h>
#include<unistd.h>



using namespace std;
void get_Size(int &width,int &depth);

void get_Size(int &width,int &depth) {
	struct winsize info;
    
    ioctl(STDIN_FILENO,TIOCGWINSZ,&info);
    width = info.ws_col;
    depth = info.ws_row;
}


void* main_UI(void *arg) {
	int winSize;
	int winDepth;

	get_Size(winSize,winDepth);
	int len = 0.1 * winSize;
	int cnt = 0.2 * winSize;
	int height =  0.2 * winDepth;
	int adjust = 0.3 * winSize + 6; 
	int choice;
	char s[] = "hello world";

	//do {
	while(1) {
		for(int i = 0;i < len;i++) {
            printf(" ");
        }
        for(int i = 0;i < cnt;i++) {
            printf("*");
        }
        printf("%s",s);
        for(int i = 0;i < cnt;i++) {
            printf("*");
        }
       
		printf("\n");
		int t;
		//scanf("%d",&t);
		sleep(1);
	}
	//} while(choice);
}

int main() {
    pthread_t t_1;

	if (pthread_create(&t_1,NULL,main_UI,NULL) != 0) {  
        printf("Creat thread 1 failed.\n");  
        exit(1);  
    }  

    return 0;
}

