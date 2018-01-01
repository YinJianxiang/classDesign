/*************************************************************************
	> File Name: HuffMan.h
	> Author: YinJianxiang
	> Mail: YinJianxiang123@gmail.com
	> Created Time: 2017年12月25日 星期一 16时14分47秒
 ************************************************************************/

#ifndef _HUFFMAN_H
#define _HUFFMAN_H

using namespace std;


const int maxn = 256; ///< 最大ASCII码值


//---------------------------------------------------------------------------
// 哈弗曼编码译码需要的宏
//---------------------------------------------------------------------------
#define STRING_LEN 100001
#define INF 0x3f3f3f3f



/*
 *@brief: 字符频度结构体
 * 
 */
typedef struct FREQ {
    char data; ///< 字符
    int freq;  ///< 对应字符出现的频度
} FREQ;

/*
 *@brief: 哈弗曼树节点
 * 
 */
typedef struct HuffManNode {
    int weight; ///< 节点的权重
    char *code; ///< 对应的编码
    char data; ///< 字符
    int lChild; ///< 节点的左孩子
    int rChild; ///< 节点的右孩子
    int parent; ///< 节点的父节点
} HuffManNode,*HuffManTree;

FREQ* get_frequancy(char *a,int len,int &cnt);
void createHuffManTree(HuffManTree &tree,FREQ* fq,int cnt);
void destoryHuffManTree(HuffManTree &tree,int cnt);
void select(HuffManTree &tree,int len,int &t_1,int &t_2);
void getHuffManCoding(HuffManTree tree,int cnt);
void codeString(char *input,HuffManTree tree,int cnt);
char* decodeHuffManCode(HuffManTree tree,int cnt,char *input); 

#endif
