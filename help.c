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
			//printf("%*s%s/\n", depth, "", entry->d_name);  
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
