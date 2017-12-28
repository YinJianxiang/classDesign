/*************************************************************************
	> File Name: HuffMan.h
	> Author: YinJianxiang
	> Mail: YinJianxiang123@gmail.com
	> Created Time: 2017年12月25日 星期一 16时14分47秒
 ************************************************************************/

#ifndef _HUFFMAN_H
#define _HUFFMAN_H

using namespace std;

const int maxn = 256;
#define STRING_LEN 100001
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
void createHuffManTree(HuffManTree &tree,FREQ* fq,int cnt);
void destoryHuffManTree(HuffManTree &tree,int cnt);
void select(HuffManTree &tree,int len,int &t_1,int &t_2);
void getHuffManCoding(HuffManTree tree,int cnt);
char* decodeHuffManCode(HuffManTree tree,int cnt,char *input); 

#endif
