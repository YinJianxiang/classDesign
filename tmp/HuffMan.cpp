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
        printf("you should code first\n");
        return NULL;
    }
    char* res = (char *)malloc(sizeof(char)*STRING_LEN);

    //cin.getline(input,STRING_LEN);  
    

    int root = 2 * cnt - 2;
    int i = 0;
    int index = 0;
    while(input[i] == 0) {
        if(input[i] == '0') {
            root = tree[root].lChild;
        } else if(input[i] == '1') {
            root = tree[root].rChild;
        } else {
            printf("input error at %d \n",i);
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

    return res;
}

void getHuffManCoding(HuffManTree tree,int cnt) {
    int start;
    char code[cnt];
   
    code[cnt-1] = 0;

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


