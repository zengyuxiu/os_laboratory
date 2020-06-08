/*=============================================================================
#
# Author:Yuxiu Zeng
#
# Copyright (C) 2020 All rights reserved.
#
# Last modified: 2020-05-20 08:21
#
# Filename: copy.c
#
# Description: 
#
=============================================================================*/
#include "copy_file.h"





FILE_ENUM check_file(char *var, struct stat *st)

{

	if( stat(var, st) )    //if stat function error(renturn nonzero)

	{

		if( ENOENT == errno)    //No such file or directory

		{

			return NOT_EXIST;

		}

		else

		{

			perror("stat");

			exit(EXIT_FAILURE);

		}

	}



	else    // stat() ok, no error

	{

		//check file attr(dir or file)

		if( S_ISDIR(st->st_mode ))

			return DIRECTORY;

		else if( S_ISREG(st->st_mode) )

			return FILES;

		else

		{

			fprintf(stderr, "%s(%d):file type error", __FILE__ , __LINE__);

			exit(EXIT_FAILURE);

		}

	}

}





//-----------------------------------------------------



int cp_file(char *src_var, char *dest_var, struct stat *st)

{

	FILE *src = NULL;

	FILE *dest = NULL;



	if( S_ISREG(st->st_mode) )  //if dest is file

	{

		//1. open src and dest file

		if( NULL == (src = fopen(src_var, "r")) )

		{

			perror("fopen");

			exit(EXIT_FAILURE);

		}



		if( NULL == (dest = fopen(dest_var, "w+")) )

		{

			perror("fopen");

			exit(EXIT_FAILURE);

		}



		//2. copy the context from src to dest

		char buf[1024];

		int num;



		while(1)

		{

			// if at the end of file or an error occured

			if( 1024 != (num = fread(buf, 1,1024, src)))

			{

				if( !feof(src))

				{

					perror("fread");

					exit(EXIT_FAILURE);

				}



				else

				{

					fwrite(buf, 1, num, dest);

					fclose(dest);   //3. close dest file

					break;

				}

			}

			fwrite(buf, 1, 1024, dest);



		}



		//3. close src file

		fclose(src);

		return 0;

	}



	if( S_ISDIR(st->st_mode) )

	{

		char buf[100]="";



		//make the relative path to absolute path

		strncpy(buf, dest_var, sizeof(dest_var));

		strcat(buf, src_var);



		//if dest file doesn't exist, creat it first

		char cmd[100]="";

		sprintf(cmd, "touch %s",buf);

		system(cmd);



		struct stat new_dest_stat;



		if( stat(buf, &new_dest_stat))

		{

			perror("stat");

			exit(EXIT_FAILURE);

		}



		cp_file(src_var, buf, &new_dest_stat);

	}



	return 0;

}



//----------------------------------------------

//if src file is a dir

int cp_dir(char *src, char *dest)

{

	DIR *dirp = NULL;



	//1. open the dir

	if( NULL == (dirp = opendir(src)) )

	{

		perror("opendir");

		exit(EXIT_FAILURE);

	}



	struct dirent *entp = NULL;



	//2. read the dir

	while( NULL != (entp = readdir(dirp)))      //read the dir context

	{

		if( 0 == (strcmp(entp->d_name,"..")) || 0 == (strcmp(entp->d_name, ".")))

		{

			continue;

		}



		char src_buf[100] = "";

		char dest_buf[100] = "";



		sprintf(src_buf, "%s/%s\0", src, entp->d_name);

		sprintf(dest_buf, "%s/%s\0", dest, entp->d_name);





		struct stat src_stat;



		if( stat(src_buf,&src_stat) )

		{

			perror("stat");

			exit(EXIT_FAILURE);

		}



		if( S_ISREG(src_stat.st_mode) )

		{

			cp_file(src_buf, dest_buf, &src_stat);

		}



		else if( S_ISDIR(src_stat.st_mode) )

		{

			if( -1 == mkdir(dest_buf, src_stat.st_mode) )

			{

				perror("mkdir");

				exit(EXIT_FAILURE);

			}



			cp_dir(src_buf, dest_buf);  //if subdir, recursive call itself

		}

	}



	return 0;

}
