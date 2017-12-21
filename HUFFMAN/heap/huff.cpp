/*************************************************************************
	> File Name: huff.cpp
	> Author: YinJianxiang
	> Mail: YinJianxiang123@gmail.com
	> Created Time: 2017年12月20日 星期三 00时38分35秒
 ************************************************************************/

#include<iostream>
#include<cstdio>
#include<cstring>
#include<map>

using namespace std;

#define INF 0x3f3f3f3f

const int maxn = 1024;

typedef struct node {
	int weight;
	char data;
	char *code;
	int lChild;
	int rChild;
	int parent;
}Huffnode,*HuffTree;


int getMin(HuffTree &tree,int len) {
	int index;
	int k = INF;

	for(int i = 0;i <= len;i++) {
		if(tree[i].weight < k && tree[i].parent == 0) {    
			k = tree[i].weight;    
			index = i;    
		}    
	}
	tree[index].parent = 1;

	return index;
}

void selectNode(HuffTree &tree,int len,int &t_1,int &t_2) {
	t_1 = getMin(tree,len);
	t_2 = getMin(tree,len);
}

void createHuffTree(HuffTree &tree,int n,char *ele,int *w) {
	if(n <= 1) {
		return;
	}
	int m = 2*n - 1;
	tree = new Huffnode[m+1];

	for(int i = 0;i < n;i++) {
		tree[i].data = ele[i];
		tree[i].weight = w[i];
		tree[i].code = 0;
		tree[i].parent = 0;
		tree[i].lChild = 0;
		tree[i].rChild = 0;
	}
	for(int i = n;i <= m;i++) {
		tree[i].data = ' ';
		tree[i].weight = 0;
		tree[i].code = 0;
		tree[i].parent = 0;
		tree[i].lChild = 0;
		tree[i].rChild = 0;
	}
	for(int i = n;i < m;i++) {
		int t_1;
		int t_2;
		selectNode(tree,i-1,t_1,t_2);
		tree[t_1].parent = i;
		tree[t_2].parent = i;
		
		tree[i].lChild = t_1;
		tree[i].rChild = t_2;
		tree[i].weight = tree[t_1].weight + tree[t_2].weight;
	}
}

  

void createHuffCode(HuffTree tree,int n) {
	int start;

	char code[n];
	code[n-1] = 0;

	//for(int i = 0; i < 2*n; i++)  {
	//	cout << tree[i].data << ":" << tree[i].lChild << ":" << tree[i].rChild << " " << tree[i].weight << endl;
	//}
    cout << "---------------------------------------" << endl;
	for(int i = 0;i < n;i++) {
		start = n - 1;
		int cur = i;
		int parent = tree[i].parent;
		//cout << "------------------------------" << endl;
		//cout << "start" << start << endl;
		
		while(parent) {		
			--start;
			if(tree[parent].lChild == cur) {
				code[start] = '0';
			} 
			if(tree[parent].rChild == cur) {
				code[start] = '1';
			}
			cur = parent;
			parent = tree[cur].parent;
		} 

		//cout << start << "(" << code << ")" << endl;
		tree[i].code = new char[n-start];
		strcpy(tree[i].code,code+start); 
		
		cout << tree[i].data << " " << tree[i].code << endl;
	}
	cout << "------------------------------" << endl;
}

bool getBinInput(char *buf,int len) {
	bool flag = true;

	for(int i = 0;i < len;i++) {
		if(!(buf[i] == '0' || buf[i] == '1')) {
			//cout << "buf[i]" << buf[i] << endl;
			flag = false;
			break;
		}
	}

	return flag;
}



void decodeHuffCode(HuffTree tree,int n) {
	char buf[maxn];

	cout << "input bincode" << endl;
	cin.getline(buf,1024);
	int len = strlen(buf);

	if(getBinInput(buf,len) == false) {
		return;
	}
	int i = 0;
	int k = 2 * n - 2; 

	while(buf[i]) {
		if(buf[i] == '0') {
			k = tree[k].lChild;
			//cout << "left:k" << k << " ";
		}
		if(buf[i] == '1') {
			k = tree[k].rChild;
			//cout << "right:k" << k << " ";
		}
		if(tree[k].lChild == 0 && tree[k].rChild == 0) {
			//cout << endl;
			//cout << "---------------------" << endl;
			//cout << "data" << "(" << tree[k].data << ")" << endl;
			cout << tree[k].data;
			k = 2 * n - 2;
		}
		i++;

	}
	cout << endl;
}



int main() {
	HuffTree tree;
	char buf[maxn];
	map<char,int> m;
	cin.getline(buf,1024);
	char ele[maxn];
	int weight[maxn];
	int n;

	int len = strlen(buf);
	for(int i = 0;i < len;i++) {
		m[buf[i]]++;
	}
	map<char,int>::iterator it;
	n = m.size();
	int i;
	for(it = m.begin(),i = 0;it != m.end();++it,i++) {
		//cout << it->first << " " << it->second << endl;
		ele[i] = it->first;
		weight[i] = it->second;
	}

	cout << n << endl;
	createHuffTree(tree,n,ele,weight);
	createHuffCode(tree,n);
	for(int i = 0;i < 2*n - 1;i++) {
		cout << tree[i].data << ":" << tree[i].lChild << " " << tree[i].rChild << endl;
	}
	decodeHuffCode(tree,n);

	cout << endl;
	return 0;
}

