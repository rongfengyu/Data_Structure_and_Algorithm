//from lm-sensors

#include <dirent.h>
#include <stdio.h>

const char *hwmon_path = "/sys/devices/virtual/hwmon";


int get_file_content(const char *file_path, char *file_str)
{
	FILE *fp = NULL;
	
	if((fp = fopen(file_path, "r")) == NULL)
		return -1;
				
	fgets(file_str, 64, fp);
	fclose(fp);
	
	return 0;
}


int is_dev_path(const char *dev_path)
{
	char  dev_path_buff[64];
	FILE *fp = NULL;
	DIR *dir;
	struct dirent *ent_dev;
	
	//读取cpu-dev文件夹下目录
	if (!(dir = opendir(dev_path)))
		return -1;
	
	while ((ent_dev = readdir(dir))) {

		//跳过文件夹与链接文件
		if (ent_dev->d_type != DT_REG)
			continue;
			
		if(!strcmp(ent_dev->d_name, "name"))
		{
			snprintf(dev_path_buff, 64, "%s/%s", dev_path, ent_dev->d_name);
			char  breakfile[64];
						
			if(get_file_content(dev_path_buff, breakfile) != 0)
				return -1;
				
			printf("%s : |%s|\n", __FUNCTION__, breakfile);
			
			if(!strcmp(breakfile, "cpu-hwmon\n"))
				return 0;
		}
	}
	closedir(dir);
	
	return -1;
}

int read_dev_temp(const char *dev_path)
{
	DIR *dir;
	DIR *dir_in;
	struct dirent *ent;
	struct dirent *ent_dev;
	
	char  dev_path_buff[64];
	char  dev_temp_buff[64];

	int count = 0;
	int count_cpu = 0;
	int temp_tmp = 0;//用于存放每个cpu输出的温度值
	
	
	//遍历hwmon文件夹下目录
	if (!(dir = opendir(dev_path)))
		return -1;

	while ((ent = readdir(dir))) {
		//跳过非文件夹
		if (ent->d_type != DT_DIR || ent->d_name[0] == '.')
			continue;
		
		snprintf(dev_path_buff, 64, "%s/%s", dev_path, ent->d_name);
		
		
		printf("%s : path : %s\n", __FUNCTION__, dev_path_buff);
		
		//如果是cpu-hwmon设备
		if(is_dev_path(dev_path_buff) == 0)
			break;
	}	
	closedir(dir);
	
	//读取cpu-dev文件夹下目录
	if (!(dir_in = opendir(dev_path_buff)))
		return -1;
	
	while ((ent_dev = readdir(dir_in))) 
	{
		//跳过文件夹与链接文件
		if (ent_dev->d_type != DT_REG)
			continue;

		char c;
		int nr;
		count = sscanf(ent_dev->d_name, "temp%d%c", &nr, &c);
		
		if (count != 2 || c != '_')
			continue;
			
		char  breakfile[64];
		
		snprintf(dev_temp_buff, 64, "%s/%s", dev_path_buff, ent_dev->d_name);
		printf("%s : %s\n", __FUNCTION__, dev_temp_buff);
		
		if(get_file_content(dev_temp_buff, breakfile) != 0)
			return -1;
		
		printf("%s : %s\n", __FUNCTION__, breakfile);
		
		temp_tmp =(atoi(breakfile)/1000 < temp_tmp)? temp_tmp: atoi(breakfile)/1000;
		count_cpu++;
	}	
	closedir(dir_in);

	return temp_tmp;
}

int main()
{
	char  breakfile[64];
	
	get_file_content("/root/test.txt", breakfile);
	printf("dev temp is : %s\n", breakfile);
	
	int temp = read_dev_temp(hwmon_path);
	printf("dev temp is : %d\n", temp);
	return 0;
}