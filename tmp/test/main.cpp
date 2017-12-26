/*************************************************************************
	> File Name: main.cpp
	> Author: YinJianxiang
	> Mail: YinJianxiang123@gmail.com
	> Created Time: 2017年12月25日 星期一 21时24分32秒
 ************************************************************************/

#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<cstring>

using namespace std;

const int maxn = 256;
#define STRING_LEN 1024
#define INF 0x3f3f3f3f

//struct of frequancy
typedef struct FREQ {
    char data;
    int freq;
} FREQ;


//struct of huffmantree
typedef struct HuffManNode {
    int weight;
    char *code;
    char data;
    int lChild;
    int rChild;
    int parent;
} HuffManNode,*HuffManTree;

FREQ* get_frequancy(char *a,int len,int &cnt);
void createHuffManTree(HuffManTree &tree,FREQ fq,int cnt);
void destoryHuffManTree(HuffManTree &tree,int cnt);
void select(HuffManTree &tree,int len,int &t_1,int &t_2);
void getHuffManCoding(HuffManTree tree,int cnt);
char* decodeHuffManCode(HuffManTree tree,int cnt); 

char* decodeHuffManCode(HuffManTree tree,int cnt) {
    char input[STRING_LEN];
    char* res = (char *)malloc(sizeof(char)*STRING_LEN);

    cin.getline(input,STRING_LEN);  

    int root = 2 * cnt - 2;
    int i = 0;
    int index = 0;
    while(input[i]) {
        if(input[i] == '0') {
            root = tree[root].lChild;
        } else if(input[i] == '1') {
            root = tree[root].rChild;
        } else {
            printf("input error %d \n",i);
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

        printf("select %d %d\n",t_1,t_2);
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


int main() {
    char a[STRING_LEN];
    fgets(a,STRING_LEN,stdin);

    int len = strlen(a) - 1;

    int cnt = 0;
    FREQ *t = get_frequancy(a,len,cnt);
    
    HuffManTree tree;

    createHuffManTree(tree,t,cnt);
    for(int i = 0;i < 2*cnt - 1;i++) {
        printf("%d %c: %d %d\n",i,tree[i].data,tree[i].lChild,tree[i].rChild);
    }
    printf("----------------------------\n");
    getHuffManCoding(tree,cnt);
    for(int i = 0;i < cnt;i++) {
        printf("%c: %s\n",tree[i].data,tree[i].code);
    }

    printf("----------------------------\n");

    char *res = decodeHuffManCode(tree,cnt);
    if(NULL != res) {
        printf("%s\n",res);
    }
    
    
    free(res);

    destoryHuffManTree(tree,cnt);
 
    free(t);
    return 0;
}

