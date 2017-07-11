#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

#define PARTTIONLINE = "/proc/partitions"

int main()
{
	int fd;

	fd = open(PARTTIONLINE, O_RDONLY);
	if (fd < 0) {
		perror("open failed!\n");
		exit(-1);
	}

	

	return 0;
}
