/*=============================================================================
#
# Author:Yuxiu Zeng
#
# Copyright (C) 2020 All rights reserved.
#
# Last modified:	2020-05-21 20:56
#
# Filename:		cat.c
#
# Description: 
#
=============================================================================*/
#include<stdio.h>

int main(int argc, char *argv[])

{

	FILE *fp = fopen(argv[1], "r");//打开文件

	int read_ret;

	if(argc < 2)

	{

		printf("please input src file!\n");

	}

	if(fp == NULL)

	{

		printf("open src %s failed!\n", argv[1]);

		return -1;

	}



	while(1)

	{

		read_ret = fgetc(fp);//读取文件

		if(feof(fp))//判断是否读取完毕

		{

			printf("read file %s endl\n",argv[1]);

			break;

		}

		fputc(read_ret,stdout);//将数据输出到屏幕

	}
}
