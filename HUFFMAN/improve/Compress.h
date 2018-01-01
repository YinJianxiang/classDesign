/*************************************************************************
	> File Name: Compress.h
	> Author: YinJianxiang
	> Mail: YinJianxiang123@gmail.com
	> Created Time: 2017年12月26日 星期二 23时33分24秒
 ************************************************************************/

 #ifndef _COMPRESS_H
 #define _COMPRESS_H
 
 //---------------------------------------------------------------------------
// ASCII码最大数量
//---------------------------------------------------------------------------
 #define N 256
 

 typedef unsigned char type;
 typedef long int WeightType;
 
 /*
 *@brief:哈弗曼树节点
 *
 */
 typedef struct {
	 type ch; ///<节点数据类型
	 WeightType weight;///<
	 int parent; ///<
	 int lChild; ///<
	 int rChild; ///<
 } HTNode;
 
 void Compress(char *name,char *rename);
 void UnCompress(char *name,char *rename);
 
 #endif
 