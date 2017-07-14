#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

#ifdef ANDROID
#include <android/log.h>
#endif

#include <string.h>
#include <errno.h>

#ifdef LOG_TAG
#undef LOG_TAG
#define LOG_TAG "Parttion Test"
#endif

#ifdef ANDROID
#define  LOGI(...) __android_log_print (ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define  LOGE(...) __android_log_print (ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#endif

#define PARTTIONLINE  "/proc/partitions"
#define COMDLINE	  "/proc/cmdline"
#define BUFSIZE	      256
const char* time = __DATE__" " __TIME__;
const char* dbinfo = "__LINE__,__func__";

static void commandline_parcel()
{
	char *rootkey = "root";
	char buf[BUFSIZE];
	char *rootloc = NULL;
	char *valueloc = NULL;
	char rootvalue[BUFSIZE];
	int ret = 0;

	int fd;
	fd = open(COMDLINE, O_RDONLY);
	if (fd < 0) {
		perror("open failed!\n");
		exit(-1);
	}

	if ((ret = read(fd, buf, BUFSIZE)) < 0) {
		fprintf(stderr, "read err");
		exit(-1);
	}
	buf[ret] = '\0';
	printf("--buf:%s\n", buf);
	rootloc = strstr(buf, rootkey);
	if (rootloc) {
		printf("--rootloc:%s\n", rootloc);
	} else {
		perror("--rootloc is null\n");
		exit(-1);
	}

	valueloc = rootloc + strlen(rootkey) - 1;
	if (!valueloc) {
		perror("--valueloc is not exsit\n");
		exit(-1);
	}
	memset(rootvalue, 0, BUFSIZE);
	memccpy(rootvalue, valueloc, ' ', BUFSIZE);
	ret = strlen(rootvalue);
	printf("--ret:%d\n", ret);
	rootvalue[ret] = '\0';
	printf("rootvalue:%s\n", rootvalue);
}

int main()
{
#ifdef PARTTION
	int fd;
	fd = open(PARTTIONLINE, O_RDONLY);
	if (fd < 0) {
		perror("open failed!\n");
		exit(-1);
	}
#endif

#ifdef COMDLINE
	commandline_parcel();
#endif

	return 0;
}
