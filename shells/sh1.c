/*=============================================================================
#
# Author:Yuxiu Zeng
#
# Copyright (C) 2020 All rights reserved.
#
# Last modified: 2020-06-08 10:10
#
# Filename: mysys.c
#
# Description: 
#
=============================================================================*/
#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<stdlib.h>
#include<errno.h>
#include<sys/wait.h>

#define MAX 1024

char * get_cmd(char * cmd){
	printf(">");
	fgets(cmd,MAX,stdin);
	return cmd;
}

int mysys(char *command){
	pid_t pid;
	int status;
	if (command == NULL)
		return 1;
	if ((pid = fork()) < 0)
		status = -1;
	else if (pid == 0)
	{
		execl("/bin/sh", "sh", "-c", command, NULL);
		exit(127);
	}
	else
	{
		while (waitpid(pid, &status, 0) < 0)
		{
			if (errno == EINTR)
				continue;
			status = -1;
			break;
		}
	}
	return status;
}



int main()
{
	char  cmd[MAX];
	while(1){
		mysys(get_cmd(cmd));
	}
	return 0;

}
