/*************************************************************************
	> File Name: Compress.cpp
	> Author: YinJianxiang
	> Mail: YinJianxiang123@gmail.com
	> Created Time: 2017年12月26日 星期二 23时33分34秒
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
 #include<unistd.h>
 #include<fcntl.h>

#include"Compress.h"
#include"Queue.h"

static void selectMin(HTNode *ht,int n,int *k);
static int SortTree(HTNode *ht);
static HTNode *CreatHFTreeFromFile(FILE * fp, short *leafNum, WeightType *FileLength);
static char **getHuffManCode(HTNode *ht, short leafNum);
static type GetBits(Queue *Q);
static void getMaxMinLen(FILE *fp,HTNode *ht,char **hc,short leafNum,type *Max,type *Min);
static short WriteCodeToFile(FILE *fp,char **hc,short leafNum,Queue *Q,type *length);
static void InToQueue(Queue *Q,type ch);


void selectMin(HTNode *ht,int n,int *k) {
	WeightType Min = 0x3f3f3f3f;
	int index;
	for(int i = 0;i <= n;i++) {
		if (0 == ht[i].parent && ht[i].weight < Min) {
			Min = ht[i].weight;
			index = i;
		}
	}
	*k = index;
}


// 对哈夫曼树排序，并统计叶子数量
int SortTree(HTNode *ht) {
	HTNode tmp;

	for (int i = N - 1; i >= 0; i--) {
		for (int j = 0; j < i; j++)
			if (ht[j].weight < ht[j + 1].weight) {
				tmp = ht[j];
				ht[j] = ht[j + 1];
				ht[j + 1] = tmp;
			}
	}
	int i;
	for(i = 0; i < N; i++) {
		if (0 == ht[i].weight)
			return i;
	}
	return i;		//返回叶子个数
}

//文件指针  叶子节点个数 文件长度
HTNode *CreatHFTreeFromFile(FILE * fp, short *leafNum, WeightType *FileLength) {
	HTNode *ht = NULL;

	type ch;

	ht = (HTNode *)malloc(2 * N * sizeof(HTNode));
	if(!ht) {
		printf("malloc failed\n");
		exit(1);
	}

	//初始化字符
	for(int i = 0;i < N;i++) {
		ht[i].weight = 0;
		ht[i].ch = (type)i;
	}

	//读取文件
	for(*FileLength = 0;!feof(fp);(*FileLength)++) {
		ch = fgetc(fp);
		ht[ch].weight++;
	}
	
	//减去文件末尾来获得文件长度
	(*FileLength)--;

	*leafNum = SortTree(ht);
	short m = 2 * *leafNum - 1;
	//得到叶子节点和总结节点数目

	

	if (1 == *leafNum) {
		ht[0].parent = 1;
		return ht;
	} else if (0 > *leafNum) {
		return NULL;
	}
	//特殊情况的判定

	//创建叶子节点数目
	for(int i = 0;i < m;i++) {
		ht[i].lChild = 0;
		ht[i].rChild = 0;
		ht[i].parent = 0;
	}
	
	for(int i = *leafNum;i < m;i++) {
		int s1,s2;
		selectMin(ht, i - 1, &s1);
		ht[s1].parent = i;
		ht[i].lChild = s1;

		selectMin(ht, i - 1, &s2);
		ht[s2].parent = i;
		ht[i].rChild = s2;

		ht[i].weight = ht[s1].weight + ht[s2].weight;
	}

	return ht;
}

//得到哈弗曼编码表
char **getHuffManCode(HTNode *ht, short leafNum) {
	char *s;
	char **hc;

	hc = (char **)malloc((leafNum) * sizeof(char *)); //申请编码表空间

	if(1 == leafNum) {
		hc[0] = (char *)malloc((leafNum + 1) * sizeof(char));
		strcpy(hc[0], "0");
		return hc;
	}
	s = (char *)malloc(sizeof(char) * (leafNum + 1));//申请空间
	s[leafNum] = '\0';	//逆向求编码
	for(int i = 0; i < leafNum; i++) {	/*求n个叶子结点对应的哈夫曼编码 */
		int start = leafNum;	
		int cur = i;
		for (int p = ht[i].parent; p; p = ht[p].parent) {	/*从叶子到根结点求编码 */
			if (ht[p].lChild == cur)
				s[--start] = '0';	/*左分支标0 */
			else
				s[--start] = '1';	/*右分支标1 */
			cur = p;
		}
		hc[i] = (char *)malloc((leafNum - start) * sizeof(char));	/*为第i个编码分配空间 */
		strcpy(hc[i], &s[start]);
		/*
		printf("%3d号 %3c 码长:%2d;编码:%s\n", ht[i].ch, ht[i].ch,
			   LeafNum - start, &cd[start]);
		*/
	}	
	printf("%s",hc['7']);		
	free(s);		
	return hc;
}

//从队列里取8个字符（0、1），转换成一个字节
type GetBits(Queue *Q) {
	type bits = 0;
	char t;

	for(int i = 0;i < 8;i++) {
		if(Out_Queue(Q,&t) != Empty) {
			if(t == '0') {
				bits = bits << 1;
			} else {
				bits = (bits << 1) | 1;
			}
		} else {
			break;//不足则退出
		}
	} 

	return bits;
}

//求最长（最短）编码长度
void getMaxMinLen(FILE *fp,HTNode *ht,char **hc,short leafNum,type *Max,type *Min) {
	type len;

	*Max = *Min = strlen(hc[0]);
	for(int i = 0;i < leafNum;i++) {
		len = strlen(hc[i]);
		fwrite(&ht[i].ch, sizeof(type), 1,fp);	//字符和对应的
		fwrite(&len, sizeof(type), 1, fp);	//编码长度写进文件
		if (len > *Max)
			*Max = len;
		if (len < *Min)
			*Min = len;
	}
}

//把出现过的字符编码表经过压缩写进文件
short WriteCodeToFile(FILE *fp,char **hc,short leafNum,Queue *Q,type *length) {
	char *p;
	short Count = 0;

	for(int i = 0;i < leafNum;i++) {
		for(p = hc[i];*p;p++) {
			In_Queue(Q,*p);
		}
		while(Q->len > 8) {
			type bits = GetBits(Q);
			fputc(bits,fp);
			Count++;
		}
	}

	*length = Q->len;
	int i = 8 - *length;
	type bits = GetBits(Q);
	for (int j = 0;j < i;j++) {
		bits = bits << 1;	//补0
	}
	fputc(bits, fp);	
	Count++;		

	InitQueue(Q);
	return Count;
}


void Compress(char *name,char *rename) {
	//char name[80];
	//char rename[80];
	FILE *fp_1;
	FILE *fp_2;

	//打开文件
	/*
	printf("filename to be compressed:");
	scanf("%s", name);
	
	printf("filename after compressed:");
	scanf("%s", rename);
	*/
	fp_1 = fopen(name, "rb");	//原文件
	fp_2 = fopen(rename, "wb");
	

	if (!fp_1 || !fp_2) {
		printf("Cannot open file/(ㄒoㄒ)/~~\n\n");
		return;
	}

	//创建哈夫曼树,统计叶子个数和原文件长度
	short leafNum;
	WeightType FileLength;
	HTNode *ht = CreatHFTreeFromFile(fp_1, &leafNum, &FileLength);	
	
	printf("FileLenth:%ld",FileLength);
	//初始化队列
	Queue *Q = (Queue *) malloc(sizeof(Queue));
	InitQueue(Q);	

	//得到编码表
	char **hc = getHuffManCode(ht, leafNum);	
	
	char **Map = (char **)malloc(N * sizeof(char *));	//字符编码表
	

	for(int i = 0; i < N; i++)	//初始化
		Map[i] = NULL;

	for(int i = 0; i < leafNum; i++) {	// 定位，编码指针数组Map[256]
		//printf("%c %s\n",ht[i].ch,hc[i]);
		Map[(int)(ht[i].ch)] = hc[i];
	}

	//printf("first\n");
	
	fseek(fp_2, sizeof(WeightType) + sizeof(short) + 6 * sizeof(type), SEEK_SET);	
	//先占个位置,等下填压缩叶子编码剩几个和最长编码长//getchar();
	
	type maxLen,minLen;
	getMaxMinLen(fp_2, ht, hc, leafNum, &maxLen, &minLen);	
	//获得最长码串长度,顺便填写字符对应编码长
	
	
	//printf("最长码串长度: %d\n",maxLen);printf("ftell = %d\n",ftell(fp_2));
	free(ht);		
	type lastLen;
	//printf("second\n");

	short codeNum = WriteCodeToFile(fp_2,hc,leafNum,Q,&lastLen);	
	//把字符转成其二进制编码写入文件,返回压成多少个

	rewind(fp_2);	//使文件指针移到开始
	fseek(fp_2, sizeof(WeightType) + sizeof(type), SEEK_SET);
	fwrite(&leafNum, sizeof(short), 1, fp_2);	//写入叶子个数
	fwrite(&maxLen, sizeof(type), 1, fp_2);	//最长码串长度
	fwrite(&minLen, sizeof(type), 1, fp_2);	//最短码串长度
	fwrite(&codeNum, sizeof(short), 1, fp_2);	//填写叶子编码压多少个
	fwrite(&lastLen, sizeof(type), 1, fp_2);	//最后剩
	//printf("叶子共压：%d个,最后剩%d个\n\n",codeNum,lastLen);

	fseek(fp_2, 2 * leafNum * sizeof(type) + codeNum, SEEK_CUR);
	//printf("开始正文ftell= %d 原文长%d\n",ftell(fp_2),FileLength);
	fseek(fp_1, 0, SEEK_SET);
	//printf("源文件ftell= %d\n",ftell(fp_1));
	
	WeightType Count = 0;
	WeightType Length = 0;

	//printf("third\n");
	while(Count < FileLength) {
		type ch;
		fread(&ch,sizeof(type),1,fp_1);
		//printf("Read读 : %c 值 %d %s\n",ch,ch,Map[ch]);	  
		char *p;     
		type bits;
		++Count;	
		for (p = Map[ch]; *p != '\0'; p++) {
			In_Queue(Q, *p);
		}
		
		while (Q->len > 8) {
			bits = GetBits(Q);	//出队8个元素,合成一个字节
			//printf("压：%c\n",bits);
			fputc(bits, fp_2);	    
			Length++;	
		}
	}		

	//printf("fourth\n");
	//最后一个bits ;
	lastLen = Q->len;	
	type n = 8 - lastLen;
	type bits = GetBits(Q);
	
	for (int i = 0; i < n; i++)
		bits = bits << 1;	//以‘0’补

	fwrite(&bits, sizeof(type), 1, fp_2);	
	Length++;

	rewind(fp_2);	
	fwrite(&Length, sizeof(WeightType), 1, fp_2);	//压缩后的长
	fwrite(&lastLen, sizeof(char), 1, fp_2);	//最后的串长
	
	fclose(fp_1);
	fclose(fp_2);
	free(Q);
	free(hc);
	free(Map);
}


//把读出的字符，转换成8个0、1字符串并入队
void InToQueue(Queue *Q,type ch) {
	type tmp;

	for(int i = 0;i < 8;i++) {
		tmp = ch << i;
		tmp = tmp >> 7;
		if(tmp == 1) {
			In_Queue(Q,'1');
		} else {
			In_Queue(Q,'0');
		}
	}
}

//解压缩
void UnCompress(char *name,char *rename) {
	FILE *fp_1;
	FILE *fp_2;
	/*
	fflush(stdin);		//清空输入缓冲区域
	printf("filename to be uncompressed:");
	scanf("%s", name);
	fflush(stdin);		//清空输入缓冲区域
	printf("filename after uncompressed:");
	scanf("%s", rename);
	*/
	fp_1 = fopen(name, "rb");
	fp_2 = fopen(rename, "wb");
	if (!fp_1 || !fp_2) {
		printf("Cannot open files./(ㄒoㄒ)/~~");
		return;
	}

	WeightType FileLength;
	type lastLen;
	short leafNum;
	type MaxLength,MinLength;
	fread(&FileLength, sizeof(WeightType), 1, fp_1);	
	fread(&lastLen, 1, 1, fp_1);	
	fread(&leafNum, sizeof(short), 1, fp_1);	
	fread(&MaxLength, sizeof(type), 1, fp_1);
	fread(&MinLength, sizeof(type), 1, fp_1);

	Queue *Q = (Queue *) malloc(sizeof(Queue));
	InitQueue(Q);

	char *buf = (char *)malloc((2 + MaxLength * sizeof(char)));
	type *str = (type *) malloc(leafNum * sizeof(type));
	type *num = (type *) malloc(leafNum * sizeof(type));
	//压缩叶子数量x和最后剩长
	
	
	short Ncom;
	type Final;
	fread(&Ncom, sizeof(short), 1, fp_1);
	fread(&Final, sizeof(type), 1, fp_1);

	//读取叶子及其码长
	for (int i = 0; i < leafNum; i++)	{
		fread(&str[i], sizeof(type), 1, fp_1);
		fread(&num[i], sizeof(type), 1, fp_1);
	}
	
	char **hc = (char **)malloc((leafNum) * sizeof(char *));	//hc为编码表的指针数组
	

	--Ncom;			//printf("开始取叶子编码ftell = %d Ncom=%d\n",ftell(cf),Ncom);

	int i,j;
	char *p;
	for (j = i = 0; i < Ncom; i++) {
		char ch = fgetc(fp_1);	
		InToQueue(Q, ch);	
		while (Q->len > MaxLength) {
			hc[j] = p = (char *)malloc(1 + num[j]);
			for (ch = 0; ch < num[j]; ch++) {
				char x;
				Out_Queue(Q, &x);
				*p++ = x;
			}
			*p = '\0';	//printf("%c编 码%s\n",str[j],hc[j]);
			j++;
		}
	}
	char ch = fgetc(fp_1);		//fread(&ch,1,1,cf);//最后一个
	InToQueue(Q, ch);
	Final = 8 - Final;

	while (Q->len > Final) {
		char x;
		p = hc[j] = (char *)malloc(1 + num[j]);
		for (ch = 0; ch < num[j]; ch++) {
			Out_Queue(Q, &x);
			*p++ = x;
		}
		*p = '\0';	//printf("%c编码%s\n",str[j],hc[j]);getchar();
		j++;
	}

	InitQueue(Q);
	--FileLength;
	--MinLength;

	WeightType thisFile = 0;

	while (thisFile < FileLength) {
		ch = fgetc(fp_1);	
		InToQueue(Q, ch);
		thisFile++;	
		//完了后队列长于码串的最大值       

		while (Q->len > MaxLength) {
			char x;			
			for (i = 0; i < MinLength; i++) {
				Out_Queue(Q, &x);
				buf[i] = x;
			}
			for (; i < MaxLength; i++) {
				Out_Queue(Q, &x);
				buf[i] = x;
				buf[i + 1] = '\0';
				for (j = 0; j < leafNum; j++) {
					if (i + 1 == num[j]
					    && 0 == strcmp(hc[j], buf)) {
						ch = str[j];	
						fputc(ch, fp_2);	
						break;
					}
				}
				if (j < leafNum)
					break;
			}
		}		
	}

	ch = fgetc(fp_1);		//fread(&ch,1,1,cf);//读取最后一个压缩字getchar();
	InToQueue(Q, ch);

	lastLen = 8 - lastLen;

	while (Q->len > lastLen) {
		int i,j;
		char x;
		for (i = 0; i < MinLength; i++) {
			Out_Queue(Q, &x);
			buf[i] = x;
		}
		for (; i < MaxLength; i++) {
			Out_Queue(Q, &x);
			buf[i] = x;
			buf[i + 1] = '\0';
			for (j = 0; j < leafNum; j++) {
				if (i + 1 == num[j] && 0 == strcmp(hc[j], buf)) {
					ch = str[j];	//printf("出 %s\n",buf);
					fputc(ch, fp_2);	//fwrite(&ch,1,1,uf);//printf("解   出：%c\n",ch);
					break;
				}
			}
			if (j < leafNum)
				break;
		}
	}
	free(Q);
	free(str);
	free(num);
	free(hc);
	fclose(fp_1);
	fclose(fp_2);
}
