#include <stdio.h>
#include <unistd.h>  
#include <dirent.h>  
#include <string.h>  
#include <sys/stat.h>  
#include <stdlib.h>

#define UBUNTU

#ifdef ALLWINNER
#define PARTTION_PATH "/dev/block/by-name/"
#elif defined UBUNTU
#define PARTTION_PATH "/dev/"
#endif

void match_partition()
{
	
}

void printdir(char *dir, int depth)  
{  
	DIR *dp;  
	struct dirent *entry;  
	struct stat statbuf;  

	if ((dp = opendir(dir)) == NULL) {  
		fprintf(stderr, "Can`t open directory %s\n", dir);  
		return ;  
	}  

	chdir(dir);  
	while ((entry = readdir(dp)) != NULL) {  
		lstat(entry->d_name, &statbuf);  
		if (S_ISDIR(statbuf.st_mode)) {  
			if (strcmp(entry->d_name, ".") == 0 ||   
					strcmp(entry->d_name, "..") == 0 )    
				continue;     
			printf("%*s%s/\n", depth, "", entry->d_name);  
			printdir(entry->d_name, depth+4);  
		} else  
			printf("%*s%s\n", depth, "", entry->d_name);  
			match_partition();
	}  
	chdir("..");  
	closedir(dp);     
}  

int main(int argc, char **argv)
{
	char *topdir = PARTTION_PATH;  
	if (argc >= 2)  
		topdir = argv[1];  

	printf("Directory scan of %s\n", topdir);  
	printdir(topdir, 0);  
	printf("done.\n");  
	exit(0);  
	
	return 0;
}

#include <stdio.h>
#include <sys/statfs.h>
#include <sys/vfs.h>
#include <errno.h>

int main(int argc, char *argv[])
{
	struct statfs disk_info;
	char *path = "/";
	int ret = 0;
	if (argc == 2) {
		path = argv[1];
	}
	if (ret == statfs(path, &disk_info) == -1) {
		//              　　 fprintf(stderr, "Failed to get file disk infomation, \
errno: %u, reason: %s \ n ", errno, strerror(errno));
	   return -1;
	}
	long long total_size = disk_info.f_blocks * disk_info.f_bsize;
	long long available_size = disk_info.f_bavail * disk_info.f_bsize;
	long long free_size = disk_info.f_bfree * disk_info.f_bsize;
	//输出每个块的长度，linux下内存块为4KB
	printf(" block size: %ld bytes \n ", disk_info.f_bsize);
	//输出块个数
	printf(" total data blocks: %ld \n ", disk_info.f_blocks);
	//输出path所在磁盘的大小
	printf(" total file disk size: %lld MB \n ",total_size >> 20);
	//输出非root用户可以用的磁盘空间大小
	printf(" avaiable size: %lld MB \n ",available_size >> 20);
	//输出硬盘的所有剩余空间
	printf(" free size: %lld MB \n ",free_size >> 20);
	//输出磁盘上文件节点个数
	printf(" total file nodes: %ld \n ", disk_info.f_files);
	//输出可用文件节点个数
	printf(" free file nodes: %ld \n ", disk_info.f_ffree);
	//输出文件名最大长度
	printf(" maxinum length of file name:%ld \n", disk_info.f_namelen);
	return 0;
}
