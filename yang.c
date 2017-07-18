#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/vfs.h>

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
//TODO 将ANDROID和Linux的debug合并成同名的方式，通过红开关控制
#define DEBUG
#ifdef DEBUG
#define dbgprint(format,args...) ({ \
	fprintf (stderr, "[%s] <%d>-->", __func__, __LINE__); \
	fprintf(stderr, format, ##args);})
#else
#define dbgprint(format,args...)
#endif

#define PARTTIONLINE  "/proc/partitions"
#define COMDLINE	  "/proc/cmdline"
#define BUFSIZE	      1024

#define PARTTION_TEST
//#define COMDLINE_TEST

const char* time = __DATE__" " __TIME__;
const char* overinfo = "----byebye----";

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

//never used
static int statfs_getblc2byte()
{
	struct statfs disk_info;
	char *path = "/home";
	int ret = 0;

	if (ret == statfs(path, &disk_info) == -1) {
	   return -1;
	}
	dbgprint(" block size: %ld bytes \n ", disk_info.f_bsize);
	dbgprint(" block num: %ld  \n ", disk_info.f_blocks);
	return disk_info.f_bsize;
}

static int getblk2byte()
{
	return 1024;
}

static void cal_flash(char *buf, char *part)
{
	int ret = 0, i = 0;
	char *pbuf;
	char *partname = NULL;
	char *ppart = NULL;
	char *pppart = NULL;
	unsigned long partsize = 0UL;
	int blk2byte = 0, partshow = 0;
	int bufsize = strlen(buf);

	pbuf = (char *)malloc(bufsize * sizeof(char));
	memset(pbuf, 0, bufsize);
	memcpy(pbuf, buf, bufsize);

	partname = strstr(pbuf, part);
	ppart = partname-2;
	ppart[1] = '\0';
	while (ppart[i] != ' ') {
		i--;
		pppart = ppart + i;
	}
	dbgprint("%s %s\n", part, pppart + 1);
	partsize = strtoul(pppart+1, NULL, 10);
	dbgprint("%s size %ld blocks\n", part, partsize);
	//TODO 计算block与GB的对应关系
	//blk2byte = statfs_getblc2byte();4096是在statfs的函数下得到的.如果用df或者proc/parttions的块大小是1024byte
	blk2byte = getblk2byte();
	partshow = (partsize * blk2byte) >> 30;
	dbgprint("%sshow %dGB\n", part, partshow);
}

static void printbuildversion()
{
	printf("%s\n", time);
}

static void parttion_parcel()
{
	int ret = 0;
	char buf[BUFSIZE];
	int fd;

	fd = open(PARTTIONLINE, O_RDONLY);
	if (fd < 0) {
		perror("open failed!\n");
		exit(-1);
	}
	if ((ret = read(fd, buf, BUFSIZE)) < 0) {
		dbgprint("read error\n");
		exit(-1);
	}

	buf[ret] = '\0';
	dbgprint("\nbuf is %s\n", buf);

	cal_flash(buf, "sda");
	cal_flash(buf, "sdb");
}

int main()
{
#ifdef PARTTION_TEST
	parttion_parcel();
#endif

#ifdef COMDLINE_TEST
	commandline_parcel();
#endif
	/*test standard*/
	statfs_getblc2byte();
	printbuildversion();

	printf("\n%s\n", overinfo);

	return 0;
}
