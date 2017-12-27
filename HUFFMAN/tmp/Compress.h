/*************************************************************************
	> File Name: Compress.h
	> Author: YinJianxiang
	> Mail: YinJianxiang123@gmail.com
	> Created Time: 2017年12月26日 星期二 23时33分24秒
 ************************************************************************/

#ifndef _COMPRESS_H
#define _COMPRESS_H

#define N 256

typedef unsigned char type;
typedef long int WeightType;

typedef struct {
	type ch;
	WeightType weight;
	int parent;
	int lChild;
	int rChild;
} HTNode;

void Compress();
void UnCompress();

#endif
