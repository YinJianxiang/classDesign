/*************************************************************************
	> File Name: compressAnduncompress.cpp
	> Author: YinJianxiang
	> Mail: YinJianxiang123@gmail.com
	> Created Time: 2017年12月26日 星期二 13时02分16秒
 ************************************************************************/

#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>

using namespace std;

#include"../HuffMan.h"
#include"../Queue.h"

HuffManTree tree = NULL;
int cnt;



int main() {
	compress();
	
	printf("检测哈弗曼树创建情况\n");
	for(int i = 0;i < cnt;i++) {
		printf("%c :%s\n",tree[i].data,tree[i].code);
	}
	
	destoryHuffManTree(tree,cnt);

	
	return 0;
}

