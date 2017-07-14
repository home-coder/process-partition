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

static void cal_flash(char *buf)
{
	int ret = 0, i = 0;
	char *pbuf = buf;
	char *sdaname = NULL;
	char *psda = NULL;
	char *ppsda = NULL;
	unsigned long sdasize = 0UL;

	sdaname = strstr(pbuf, "sda");
	psda = sdaname-2;
	psda[1] = '\0';
	while (psda[i] != ' ') {
		i--;
		ppsda = psda + i;
	}
	dbgprint("psda %s\n", ppsda + 1);
	sdasize = strtoul(ppsda+1, NULL, 10);
	//TODO 计算block与GB的对应关系
	dbgprint("sda size %ldM\n", sdasize);
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

	cal_flash(buf);
}

int main()
{
#ifdef PARTTION_TEST
	parttion_parcel();
#endif

#ifdef COMDLINE_TEST
	commandline_parcel();
#endif

	return 0;
}
