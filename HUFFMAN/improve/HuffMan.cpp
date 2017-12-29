/*************************************************************************
	> File Name: HuffMan.cpp
	> Author: YinJianxiang
	> Mail: YinJianxiang123@gmail.com
	> Created Time: 2017年12月25日 星期一 16时16分39秒
 ************************************************************************/

#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<cstring>
 
#include"HuffMan.h"
using namespace std;


char* decodeHuffManCode(HuffManTree tree,int cnt,char *input) {
    //char input[STRING_LEN];

    if(tree == NULL) {
        printf("you should coding first\n");
        return NULL;
    }
    /*
    for(int i = 0;i < cnt;i++) {
        printf("%c:%s\n",tree[i].data,tree[i].code);
    }
    */

    char* res = (char *)malloc(sizeof(char)*STRING_LEN);
    //cin.getline(input,STRING_LEN);     

    int root = 2 * cnt - 2;
    int i = 0;
    int index = 0;
    //printf("tree:%c\n",tree[root].data);
    while(input[i]) {  
        if(input[i] == '0') {
            root = tree[root].lChild;
        } else if(input[i] == '1') {
            root = tree[root].rChild;
        } else {
            printf("\033[45;41m input error at %d \033[0m\n",i);
            //getchar();
            return NULL;
        }
        if(tree[root].lChild == 0 && tree[root].rChild == 0) {
            res[index] = tree[root].data;
            index++;
            //printf("%c",tree[root].data);
                       
            root = 2 * cnt - 2;
        }
        i++;
    }
    res[index] = 0;
    //printf("(%s)\n",res);

    return res;
}

void codeString(char *input,HuffManTree tree,int cnt) {
    int i = 0;
    int j;
    int index = 0;
    char str[STRING_LEN];
    bool flag = true;

    if(tree == NULL) {
        printf("\033[45;31m you should coding first\n \033[0m");
        getchar();
        return;
    }
    /*
    for(int i = 0;i < cnt;i++) {
        printf("%c :%s\n",tree[i].data,tree[i].code);
    }
    printf("\n");
    */
    
    memset(str,0,sizeof(str));
    while(input[i]) {
        for(j = 0;j <= cnt;j++) {
            if(tree[j].data == input[i]) {
                strcat(str,tree[j].code);
            }
        }
        if(j == cnt) {
            printf("(%c) cannot not be code\n",tree[j].data);
            flag = false;
            break;
        }
        i++;
    }
    if(flag == false) {
        return;
    }
    printf("%s\n",str);

    printf("\n");
}

void getHuffManCoding(HuffManTree tree,int cnt) {
    int start;
    char code[cnt];
   
    code[cnt-1] = 0;

    if(tree == NULL) {
        printf("\033[45;41m cannot coding! \033[0m\n");
        getchar();
        return;
    }

    for(int i = 0;i <= cnt;i++) {
        start = cnt - 1;
        int cur = i;
        int parent = tree[i].parent;
        
        while(parent) {
            start--;
            if(tree[parent].lChild == cur) {
                code[start] = '0'; 
            } else {
                code[start] = '1';
            }
            cur = parent;
            parent = tree[cur].parent;
        }
        //printf("%s",code + start);
        tree[i].code = (char *)malloc(sizeof(char)*(cnt-start));
        strcpy(tree[i].code,code + start);
    }

}

void select(HuffManTree &tree,int len,int &t_1,int &t_2) { 
    int Min = INF;
    int index;

    for(int i = 0;i <= len;i++) {
        if(tree[i].parent == 0 && tree[i].weight < Min) {
            index = i;
            Min = tree[i].weight;
        }
    }
    tree[index].parent = 1;
    t_1 = index;
    Min = INF;
    for(int i = 0;i <= len;i++) {
        if(!tree[i].parent && tree[i].weight < Min) {
            index = i;
            Min = tree[i].weight;
        }
    }
    tree[index].parent = 1;
    t_2 = index;
}

void destoryHuffManTree(HuffManTree &tree,int cnt) {
    if(tree == NULL) { //安全检查
        return;
    }

    for(int i = 0;i < cnt;i++) {
        free(tree[i].code);
    }
    free(tree);
}

//create the huffmantree
void createHuffManTree(HuffManTree &tree,FREQ* fq,int cnt) {
    if(cnt <= 1) {
        return;
    }

    int m = cnt * 2 - 1;

    if(tree) {
        destoryHuffManTree(tree,cnt);
    }

    tree = (HuffManNode *)malloc(sizeof(HuffManNode)*(m+1));
    //init the tree

    for(int i = 0;i < cnt;i++) {
        tree[i].weight = fq[i].freq;
        tree[i].data = fq[i].data;
        tree[i].code = 0;
        tree[i].parent = 0;
        tree[i].lChild = 0;
        tree[i].rChild = 0;
    }
    for(int i = cnt;i <= m;i++) {
        tree[i].weight = 0;
        tree[i].data = ' ';
        tree[i].code = 0;
        tree[i].parent = 0;
        tree[i].lChild = 0;
        tree[i].rChild = 0;
    }
    
    //select the node and create the tree
    for(int i = cnt;i < m;i++) {
        int t_1;
        int t_2;
        select(tree,i-1,t_1,t_2);

        //printf("select %d %d\n",t_1,t_2);
        tree[i].lChild = t_1;
        tree[i].rChild = t_2;
        tree[i].weight = tree[t_1].weight + tree[t_2].weight;
        tree[t_1].parent = i;
        tree[t_2].parent = i;
    }
}


//get the frequancy of string
FREQ* get_frequancy(char *a,int len,int &cnt) {
    int f[maxn];

    memset(f,0,sizeof(f));
    for(int i = 0;i < len;i++) {
        f[a[i]]++;
    }

    for(int i = 0;i < maxn;i++) {
        if(f[i]) {
            cnt++;
        }
    }
    FREQ* t = (FREQ *)malloc(sizeof(FREQ)*cnt);
    int index = 0;
    for(int i = 0;i < maxn;i++) {
        if(f[i]) {
            t[index].data = i;
            t[index++].freq = f[i];
        }
    }

    return t;
}


