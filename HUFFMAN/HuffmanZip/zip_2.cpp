#include "./HuffmanTree.h"
#define NAMESIZE 500

void zip(const char*s)
{
	//s	为要压缩的地址和文件名。 
	FILE*pr,*pw;	
	int a,i=0 ,w[256],j,n=256,m=2*n-1;
	HuffmanCode HC,hc;HuffmanTree HT;
	char zipname[NAMESIZE]="zip",c=' ';//压缩后的压缩文件名为zip+原文件名+.txt. 
	while(s[i]!='.'){zipname[3+i]=s[i];i++;}
	zipname[3+i]='.';zipname[4+i]='t';zipname[5+i]='x';zipname[6+i]='t';zipname[7+i]='\0';


	pr=fopen(s,"rb");

	if(!pr){
		printf("文件打开失败！");exit(1);
	}
	for(int i=0;i<256;i++)w[i]=0;
	a=fgetc(pr);
	while(!feof(pr))
	{
		w[a]++;a=fgetc(pr);
	}
	fclose(pr);

	HuffmanCoding(HT,HC,w,256);//求出霍夫曼书，下面将文件的扩展名和霍夫曼树存入压缩文件中；
	 
	pw=fopen(zipname,"wb");
	


	//先写入扩展名 ，扩展名占 4 个字节。 不足4个用空格' '补齐; 
	j=0;
	while(s[++i]){
		fwrite(s+i,1,1,pw);j++;
	}
	while(j++<4)fwrite(&c,1,1,pw);
	//下面是写入霍夫曼树。 
	for(int i=n+1;i<=m;i++) {
		fwrite(&((HT+i)->lchild),2,1,pw);
		fwrite(&((HT+i)->rchild),2,1,pw);
	}
	//此时霍夫曼树已经存入压缩文件中。 
	
	pr=fopen(s,"rb");//重新打开文件遍历文件进压缩。 
	a=fgetc(pr);
	i=0;int b=0;hc=HC;char*p=*(hc+a+1);
	while(!feof(pr)||*p!='\0')
	{
		for(i=0;i<8;i++){
			if(*p=='\0'){
				a=fgetc(pr);
				if(a>=0)p=hc[a+1];
				else {
						int h=1;
						for(int j=0;j<8-i;j++)h=h*2;
						b=b*h;break;
					}
			}
			int h=1;
			for(int j=0;j<7-i;j++)h=h*2;
			b=b+(*p-48)*h;
			++p;
		}
		fwrite(&b,1,1,pw);b=0;
	}
	printf("压缩成功！\n");
	
	fclose(pr);fclose(pw);
}

void rezip(const char*s)
{
	//s	为压缩文件的地址和文件名。 
	printf("进行文件解压\n");
	FILE*pr,*pw;	
	int a,i=0,j,n=256,m=2*n-1;UInt32 w[256];
	HuffmanCode HC,hc;HuffmanTree HT;
	char rezipname[NAMESIZE]="re";
	i=0;
	while(s[i]!='.'){rezipname[2+i]=s[i];i++;}
	rezipname[2+i]='.';i++;
	
	pr=fopen(s,"rb");
	j=0;a=fgetc(pr);
	while(j<4){
		rezipname[2+i]=a;if(j<3)a=fgetc(pr);j++;i++;
	}
	HT=(HuffmanTree)malloc(sizeof(HTNode)*(m+1));
	HT[0].weight=0;HT[0].parent=0;HT[0].lchild=0;HT[0].rchild=0;
	for(i=1;i<=n;i++){
		HT[i].weight=0;//此时不需要权重，所以可以随便赋值。 
		HT[i].lchild=0;
		HT[i].rchild=0;
	}
	
	for(i=n+1;i<=m;i++)
	{
		HT[i].weight=0;//此时不需要权重，所以可以随便赋值。 
		fread(&a,2,1,pr);HT[i].lchild=a;HT[a].parent=i;
		fread(&a,2,1,pr);HT[i].rchild=a;HT[a].parent=i;
	}
	HT[m].parent=0;
	//此时HT为霍夫曼树， 



	int gm=0;
	char aa[80000],b[8];int x[10000],d;a=fgetc(pr);
	while(!feof(pr))
	{
			for(int i=0;i<8;i++){
				if(a){b[7-i]=48+a%2;a=a/2;}
				else b[7-i]='0';
			}
			for(int i=0;i<8;i++)
			aa[8*j+i]=b[i];
			j++;d=a;
			a=fgetc(pr);
	}
	for(i=0;i<8;i++)if(a){b[7-i]=48+a%2;a=a/2;}else break;
	for(int k=0;k<i;k++)
		aa[8*j-8+k]=b[8-i+k];
//	for(;i<7;i++)aa[8*j-8+i]='0';
	aa[8*j-8+i]='0';aa[8*j-8+i+1]='#';

	Rewin(HT,aa,x,n,gm);
	FILE *fw;
	
	
	
	fw=fopen(rezipname,"wb");
	for(int i=0;i<gm;i++)
	{x[i]=x[i]-1;fwrite(x+i,1,1,fw);}
	fclose(fw);fclose(pr);
	printf("解压成功！\n");
}
int main()
{
	const char*s="mystring.txt";
	const char*str2="zipmystring.txt";
	zip(s);
	rezip(str2);
	return 0; 
}
