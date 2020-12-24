#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<dirent.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
int read_fileattr(const char *path)
{
		int fd = open(path,O_CREAT|O_RDWR,0666);
		if(-1 == fd){
			perror("open file error");
			return -1;
		}
		struct stat fileattr;
		fstat(fd,&fileattr);
		printf("file size is %d\n",fileattr.st_size);
		printf("file last access time is %d\n",fileattr.st_atim.tv_nsec);
		close(fd);
		return 0;
}
int read_filelist(const char *dirpath)
{
		DIR *dir = NULL;
		struct dirent *dent;
		char base[1024] = {0};
		if(NULL == (dir = opendir(dirpath))){
			perror("open dir error...");
			return -1;
		}
		while(NULL != (dent = readdir(dir))){
			if(!strcmp(dent->d_name,".")||!strcmp(dent->d_name,".."))
				continue;
			else if(8 == dent->d_type)
				printf("d_name = %s/%s ",dirpath,dent->d_name);
			else if(4 == dent->d_type){
				printf("d_name = %s/%s ",dirpath,dent->d_name);
				memset(base,0,sizeof(base));
				strcat(base,"/");
				strcat(base,dent->d_name);
				read_filelist(base);
			}
			else if(10 == dent->d_type)
				printf("d_name = %s/%s ",dirpath,dent->d_name);
			else{
				printf("d_name = %s/%s ",dirpath,dent->d_name);
				read_fileattr(dent->d_name);
			}		
		}
		return 0;
}
int main(int argc, char *argv[])
{
    DIR *dir = NULL;
    char basepath[1024] = {0};
    getcwd(basepath,999);
    printf("the current dir is %s\n",basepath);
    read_filelist(basepath);
    return 0;
}
