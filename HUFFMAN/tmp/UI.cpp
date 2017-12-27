/*************************************************************************
	> File Name: UI.cpp
	> Author: YinJianxiang
	> Mail: YinJianxiang123@gmail.com
	> Created Time: 2017年12月25日 星期一 16时45分49秒
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


#include"HuffMan.h"
#include"Queue.h"
#include"Compress.h"
#include"UI.h"

using namespace std;

HuffManTree tree = NULL;
int cnt;

void input_file_coding() {
    int fd;
    int choice;

    do {
		int width;
		int depth;
		get_Size(width,depth);
        printf("please input the file_name ^_^\n");
        char file_name[30];
        scanf("%s",file_name);
        int fd = open(file_name,O_RDONLY);

        if(fd == -1) {
            printf("open the file failed!\n");
            close(fd);
            return;
        }

        char input[STRING_LEN];
        read(fd,input,STRING_LEN);
        //printf("input:%s\n",input);

        int len = strlen(input) - 1;

        cnt = 0;
        FREQ *t = get_frequancy(input,len,cnt);    

        createHuffManTree(tree,t,cnt);
        getHuffManCoding(tree,cnt);
        free(t);
        //printf("----------------------------\n");
        printf("input 0 to exit\n");
        char tmp[20];
        scanf("%s",tmp);
        choice = get_input(tmp);
    } while(choice);

    close(fd);
}

void input_file_decoding() {
    int choice;
    int fd;
    do {
        system("clear");
        printf("please input the file_name ^_^\n");
        char input[STRING_LEN];

        char file_name[30];
        scanf("%s",file_name);
        fd = open(file_name,O_RDONLY);

        if(fd == -1) {
            close(fd);
            return;
        }
        read(fd,input,sizeof(input));
        //printf("input:%s\n",input);

        char *res = decodeHuffManCode(tree,cnt,input);

        //printf("(%s)\n",res);
        printf("\n\n");
        if(res) {
            printf("%s\n",res);
            free(res);
        } 
        printf("----------------------------\n");
        printf("input 0 to exit\n");
        char t[20];
        scanf("%s",t);
        choice = get_input(t);
    } while(choice);
    
    close(fd);
}

void input_string_decoding() {
    char *res;
    int choice;
    
    do {
        system("clear");
        printf("please input the bincode you want to decode^_^\n");
        char input[STRING_LEN];

        
        cin.getline(input,STRING_LEN);
		//printf("input:%s\n",input);

        res = decodeHuffManCode(tree,cnt,input);

        //printf("(%s)\n",res);
        printf("\n\n");
        if(res != NULL) {
            printf("%s\n",res);
        } 
        printf("----------------------------\n");
        printf("input 0 to exit\n");
        char t[20];
        scanf("%s",t);
        choice = get_input(t);
    }while(choice);
	
	free(res);
}

void HUffdecode_UI() {
    int winSize;
    int winDepth;

    char s[] = "Huffman decoding";

    get_Size(winSize,winDepth);
    int len = 0.1 * winSize;
    int cnt = 0.2 * winSize;
    int height =  0.2 * winDepth;
    int adjust = 0.3 * winSize + 6; 
    int choice;

    do {
        system("clear");
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

        for(int i = 0; i < 0.3*winDepth;i++) {
            printf("\n");
        }
        for(int i = 0;i < 0.3 * winSize;i++) {
            printf(" ");
        }
        printf("1.input string\n");
        for(int i = 0;i < 0.3 * winSize;i++) {
            printf(" ");
        }
        printf("2.input file name\n");
        for(int i = 0;i < 0.3 * winSize;i++) {
            printf(" ");
        }
        printf("0.exit\n");


        printf("\n");
        printf("\33[%dB",height);
        printf("\33[1A");
        for(int i = 0;i < 0.3 * winSize;i++) {
            printf(" ");
        }
        printf("please input your choice ^_^\n");

        printf("\33[%dC",adjust);
        char t[20];
        scanf("%s",t);

        choice = get_input(t);
        getchar();
        switch(choice) {
            case 1:
                input_string_decoding();
                break;
            case 2:
                input_file_decoding();
                break;
            default:
                break;
        }
    } while(choice);
}

void screen_signal() {
    signal(SIGINT,SIG_IGN);
    signal(SIGQUIT,SIG_IGN);
    signal(SIGSTOP,SIG_IGN);
    signal(SIGTSTP,SIG_IGN);
}

int get_input(char *s) {
    int len = strlen(s);
    int res = 0;

    for(int i = 0;i < len;i++) {
        int t = s[i] - '0';
        res = res * 10 + t;
    }

    return res;
}

void get_Size(int &width,int &depth) {
    struct winsize info;
    
    ioctl(STDIN_FILENO,TIOCGWINSZ,&info);
    width = info.ws_col;
    depth = info.ws_row;
}

void input_string_coding() {
	system("clear");
	int width;
	int depth;
	get_Size(width,depth);
	for(int i = 0;i < 0.3*width;i++) {
		printf(" ");
	}

    printf("please input the string ^_^\n");
    char a[STRING_LEN];
    
    cin.getline(a,STRING_LEN);
    //printf("yes\n");

    int len = strlen(a);

    cnt = 0;
    FREQ *t = get_frequancy(a,len,cnt);
    
    //HuffManTree tree;

    createHuffManTree(tree,t,cnt);
    getHuffManCoding(tree,cnt);

    free(t);
}

void showHuffManCode() {
    int choice;
    do {
        system("clear");
        printf("----------------------------\n");
        for(int i = 0;i < cnt;i++) {
            printf("%c: %s\n",tree[i].data,tree[i].code);
        }

        printf("----------------------------\n");
        printf("input 0 to exit\n");
        char t[20];
        scanf("%s",t);
        choice = get_input(t);
    } while(choice);
}

void Huffcode_UI() {
    int winSize;
    int winDepth;

    char s[] = "Huffman coding";

    get_Size(winSize,winDepth);
    int len = 0.1 * winSize;
    int cnt = 0.2 * winSize;
    int height =  0.2 * winDepth;
    int adjust = 0.3 * winSize + 6; 
    int choice;

    do {
        system("clear");
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

        for(int i = 0; i < 0.3*winDepth;i++) {
            printf("\n");
        }
        for(int i = 0;i < 0.3 * winSize;i++) {
            printf(" ");
        }
        printf("1.input string\n");
        for(int i = 0;i < 0.3 * winSize;i++) {
            printf(" ");
        }
        printf("2.input file name\n");
        for(int i = 0;i < 0.3 * winSize;i++) {
            printf(" ");
        }
        printf("0.exit\n");


        printf("\n");
        printf("\33[%dB",height);
        printf("\33[1A");
        for(int i = 0;i < 0.3 * winSize;i++) {
            printf(" ");
        }
        printf("please input your choice ^_^\n");

        printf("\33[%dC",adjust);
        char t[20];
        scanf("%s",t);

        choice = get_input(t);
        getchar();
        switch(choice) {
            case 1:
                input_string_coding();
                break;
            case 2:
                input_file_coding();
                break;
            default:
                break;
        }
    } while(choice);
}

void main_UI() {
    system("clear");
    int choice;
    do { 
        system("clear");
        
        char s[] = "welcome to huffman";
       
        /*winsize adjust*/
        int winSize;
        int winDepth;

        get_Size(winSize,winDepth);
        
        int len = 0.1 * winSize;
        int cnt = 0.2 * winSize;
        int height =  0.2 * winDepth;
        int adjust = 0.3 * winSize + 8; 
        
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
        for(int i = 0; i < 0.3*winDepth;i++) {
            printf("\n");
        }
        for(int i = 0;i < 0.3 * winSize;i++) {
            printf(" ");
        }
        printf("1.coding\n");
        for(int i = 0;i < 0.3 * winSize;i++) {
            printf(" ");
        }
        printf("2.show coding\n");
        for(int i = 0;i < 0.3 * winSize;i++) {
            printf(" ");
        }
        printf("3.decoding\n");
        for(int i = 0;i < 0.3 * winSize;i++) {
            printf(" ");
        }
        printf("4.compress file\n");
        for(int i = 0;i < 0.3 * winSize;i++) {
            printf(" ");
        }
        printf("5.uncompress file\n");
        for(int i = 0;i < 0.3 * winSize;i++) {
            printf(" ");
        }
    
        printf("0.exit\n");

        printf("\33[%dB",height);
        printf("\33[1A");
        for(int i = 0;i < 0.3 * winSize;i++) {
            printf(" ");
        }
        printf("please input your choice ^_^\n");
        
        printf("\33[%dC",adjust);
        
        
        char t[20];
        scanf("%s",t);
        choice = get_input(t);
        
        switch(choice) {
            case 1:
                Huffcode_UI();
                break;
            case 2:
                showHuffManCode();
                break;
            case 3:
                HUffdecode_UI();
                break;
            case 4:
                Compress();
			    getchar();
                break;
            case 5:
                UnCompress();
			    getchar();
                break;
            default:
                break;
        } 
    } while(choice);
}


int main(int argc,char *argv[]) {
	main_UI();

	destoryHuffManTree(tree,cnt);
	return 0;
}

