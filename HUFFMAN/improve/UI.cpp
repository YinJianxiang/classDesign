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

char *get_file_name(char *file_name);

void Compress_UI();
void UnCompress_UI();

char *get_file_name(char *file_name) {
    int len = strlen(file_name);
    char *p = file_name + len- 1;
    while(*p == ' ') 
        *(p--) = 0;

    return file_name;
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
    if(len <= 0) {
        return -1;
    }

    for(int i = 0;i < len;i++) {
        if(s[i] >= '0' && s[i] <= '9') {
            int t = s[i] - '0';
            res = res * 10 + t;
        } else {
            return -1;
        }
    }

    return res;
}

void get_Size(int &width,int &depth) {
    struct winsize info;
    
    ioctl(STDIN_FILENO,TIOCGWINSZ,&info);
    width = info.ws_col;
    depth = info.ws_row;
}

void Compress_UI() {
    char str_1[256];
    char str_2[256];
    char *name;
    char *rename;

    int width;
    int depth;
    get_Size(width,depth);

    system("clear");
    printf("\n");
    printf("\33[1A");
    sprintf(str_1,"input the file you want to compress ^_^\n        ");
    sprintf(str_2,"input the target file ^_^\n      ");
    for(int i = 0;i < 0.2 * width;i++) {
        printf(" ");
    }
    name = readline(str_1);
    for(int i = 0;i < 0.2 * width;i++) {
        printf(" ");
    }
    rename = readline(str_2);

    Compress(name,rename);
}

void UnCompress_UI() {
    char str_1[256];
    char str_2[256];
    char *name;
    char *rename;

    int width;
    int depth;
    get_Size(width,depth);

    system("clear");
    printf("\n");
    printf("\33[1A");
    
    sprintf(str_1,"input the file you want to uncompress ^_^\n      ");  
    sprintf(str_2,"input the target file ^_^\n\33[%dC       ");
    for(int i = 0;i < 0.2 * width;i++) {
        printf(" ");
    }
    name = readline(str_1);
    for(int i = 0;i < 0.2 * width;i++) {
        printf(" ");
    }
    rename = readline(str_2);
    
    UnCompress(name,rename);
}



void input_file_coding() {
    int fd;
    int choice;

    //do {
    
    int width;
    int depth;

    get_Size(width,depth);
    for(int i = 0;i < width * 0.3;i++) {
        printf(" ");
    }
    //printf("");
    char *file_name;
    
    char str[256];
    
    sprintf(str,"please input the file_name ^_^\n\33[%dC",0.3 * width);
    file_name = readline(str);
    //printf("file name(%s)\n",file_name);
    file_name = get_file_name(file_name);

    fd = open(file_name,O_RDONLY);

    if(fd == -1) {
        for(int i = 0;i < width * 0.3;i++) {
            printf(" ");
        }
        printf("open the file failed!\n");
        for(int i = 0;i < width * 0.3;i++) {
            printf(" ");
        }
        printf("press any button to exit!\n");
        for(int i = 0;i < width * 0.4;i++) {
            printf(" ");
        }
        getchar();
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
    char hint[256];
    for(int i = 0;i < width * 0.3;i++) {
        printf(" ");
    }
    sprintf(hint,"press any button to exit\n\33[%dC",0.4 * width);
    
    printf("%s",hint);
    
    //} while(choice);

    close(fd);
}

void input_file_decoding() {
    int choice;
    int fd;
    //do {
    
    system("clear");
    if(tree == NULL) {
        printf("\033[45;31m you should coding first\n \033[0m");
        getchar();
        return;
    }
    int width;
    int depth;
    get_Size(width,depth);
    for(int i = 0;i < width * 0.3;i++) {
        printf(" ");
    }
    //printf("please input the file_name ^_^\n");
    char str[256];
    sprintf(str,"please input the file_name ^_^\n\33[%dC",0.4*width);

    char input[STRING_LEN];

    //char file_name[30];
    char *file_name;
    //scanf("%s",file_name);
    file_name = readline(str);
    file_name = get_file_name(file_name);
    fd = open(file_name,O_RDONLY);
    
    if(fd == -1) {
        printf("open the file failed\n");
        getchar();
        close(fd);
        return;
    }
    read(fd,input,sizeof(input));

    //printf("input:%s\n",input);

    char *res = decodeHuffManCode(tree,cnt,input);

    printf("\n\n");
    if(res) {
        printf("%s\n",res);
        free(res);
    } 
    
    for(int i = 0;i < width;i++) {
        printf("-");
    }
    printf("\n");

    char hint[256];
    for(int i = 0;i < 0.3 * width;i++) {
        printf(" ");
    }
    sprintf(hint,"press any button to exit\n\33[%dC",0.4*width);
    printf("%s",hint);
    getchar();

    //} while(choice);
    
    close(fd);
}

void input_string_decoding() {
    char *res;
    int choice;
    
    //do {
    system("clear");
    printf("\n");
    printf("\33[1A");
    if(tree == NULL) {
        printf("\033[45;31m you should coding first\n \033[0m");
        getchar();
        return;
    }
    int width;
    int depth;
    get_Size(width,depth);
    for(int i = 0;i < width * 0.1;i++) {
        printf(" ");
    }
    
    char str[256];
        
    sprintf(str,"please input the bincode you want to decode^_^\n\33[%dC",0.3 * width);
    //char input[STRING_LEN];
    char *input;
    
    //cin.getline(input,STRING_LEN);
    input = readline(str);
    //printf("input:%s\n",input);

    res = decodeHuffManCode(tree,cnt,input);

    //printf("(%s)\n",res);
    printf("\n\n");
    if(res != NULL) {
        printf("%s\n",res);
    } 
    //printf("----------------------------\n");
    for(int i = 0;i < width;i++) {
        printf("-");
    }
    printf("\n");

    //char *t;
    char hint[256];
    for(int i = 0;i < 0.3 * width;i++) {
        printf(" ");
    }
    //sprintf(hint,"input 0 to exit\n\33[%dC",0.4*width);
    sprintf(hint,"press any button to exit\n\33[%dC",0.4 * width);

    printf("%s",hint);
    getchar();
        
    //}while(choice);
	
	free(res);
}




void input_string_coding() {
	system("clear");
	int width;
	int depth;
	get_Size(width,depth);
	for(int i = 0;i < 0.3*width;i++) {
		printf(" ");
	}
    char str[256];
    //printf("please input the string ^_^\n");
    //char a[STRING_LEN];
    sprintf(str,"please input the string ^_^\n\33[%dC",0.2*width);
    char *a;

    a = readline(str);
    //cin.getline(a,STRING_LEN);
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
        int width;
        int depth;
        get_Size(width,depth);
        printf("\n");
        printf("\33[1A");
        for(int i = 0;i < width;i++) {
            printf("-");
        }
        printf("\n");
        
        for(int i = 0;i < cnt;i++) {
            for(int j = 0;j < 0.3 * width;j++) {
                printf(" ");
            }
            printf("%c: %s\n",tree[i].data,tree[i].code);
        }

        for(int i = 0;i < width;i++) {
            printf("-");
        }
        printf("\n");
        for(int i = 0;i < 0.3*width;i++) {
            printf(" ");
        }
        char hint[256] = "input 0 to exit\n";
        //scanf("%s",t);
        printf("%s",hint);
        for(int i = 0;i < 0.2*width;i++) {
            printf(" ");
        }
        char str[256];
        sprintf(str,"please input your choice ^_^\n\33[%dC",0.4*width);
        char *t;
        t = readline(str);
        //printf("\33[%dC",adjust);
        //t = readline(str);
        choice = get_input(t);
    } while(choice);
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
        printf("\n");
        printf("\33[1A");
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
        //printf("please input your choice ^_^\n");

        //char t[20];
        char str[256];
        sprintf(str,"please input your choice ^_^\n\33[%dC",adjust);
        char *t;
        t = readline(str);
        //scanf("%s",t);

        choice = get_input(t);
        //getchar();
        printf("\n");
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

void Huffcode_UI() {
    int winSize;
    int winDepth;

    char s[] = "Huffman coding";

    get_Size(winSize,winDepth);
    printf("\n");
    printf("\33[1A");
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
        for(int i = 0;i < height;i++) {
            printf("\n");
        }
        //printf("\33[%dB",height);
        //printf("\33[1A");
        for(int i = 0;i < 0.3 * winSize;i++) {
            printf(" ");
        }
        //printf("please input your choice ^_^\n");
        char str[256];
        sprintf(str,"please input your choice ^_^\n\33[%dC",adjust);
        //printf("\33[%dC",adjust);
        //char t[20];
        char *t;
        //scanf("%s",t);
        t = readline(str);

        choice = get_input(t);
        //getchar();
        switch(choice) {
            case 1:
                input_string_coding();
                break;
            case 2:
                input_file_coding();
                //getchar();
                break;
            default:
                break;
        }
    } while(choice);
}

void main_UI() {
    screen_signal();
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
        //printf("please input your choice ^_^\n");
        
        //printf("\33[%dC",adjust);
        
        
        //char t[20];
        
        char str[256];
        sprintf(str,"please input your choice ^_^\n\33[%dC",adjust);
        char *t;
        t = readline(str);
        //printf("\33[%dC",adjust);

        //scanf("%s",t);

        choice = get_input(t);
        //printf("(%s)\n",t);

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
                Compress_UI();
			    //getchar();
                break;
            case 5:
                UnCompress_UI();
			    //getchar();
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

