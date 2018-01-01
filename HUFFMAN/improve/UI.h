/*************************************************************************
	> File Name: UI.h
	> Author: YinJianxiang
	> Mail: YinJianxiang123@gmail.com
	> Created Time: 2017年12月27日 星期三 13时09分45秒
 ************************************************************************/

#ifndef _UI_H
#define _UI_H



void main_UI();//主界面
void showHuffManCode();//显示哈弗曼编码
void input_string_coding();//手动输入字符串编码
void Huffcode_UI();//解码界面
static void get_Size(int &width,int &depth);//获取窗口尺寸
static int get_input(char *s);//得到
void screen_signal();//屏蔽信号
void Huffcode_UI();//编码界面
void input_string_decoding();//手动输入字符串解码
void input_file_decoding();//用文件内容解码
void input_file_coding();//用文件内容编码

#endif
