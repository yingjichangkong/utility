// global_define.h

#ifndef ______GLOBAL_DEFINE_H______
#define ______GLOBAL_DEFINE_H______

// 系统类型定义
#define _SYSTEM_X64_

// 头文件包含定义
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <stdarg.h>
#include <iconv.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <signal.h>
#include <netdb.h>
#include <net/if.h>
#include <errno.h>
#include <unistd.h>
#include <uuid/uuid.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <algorithm>

// 变量类型定义
typedef               char var_1;
typedef      unsigned char var_u1;
typedef              short var_2;
typedef     unsigned short var_u2;
typedef                int var_4;
typedef       unsigned int var_u4;
typedef              float var_f4;
typedef             double var_d8;
typedef				  void var_vd;
typedef				  bool var_bl;

#ifdef _SYSTEM_X86_
  typedef          long long var_8;
  typedef unsigned long long var_u8;
#else
  typedef               long var_8;
  typedef      unsigned long var_u8;
#endif // _SYSTEM_X86_

#ifdef _WIN32_ENV_
  #define P64		"%I64d"
  #define PU64		"%I64u"
#else
  #define P64		"%ld"
  #define PU64		"%lu"
#endif

// 64bit string to value
#define str2uint64(x)		strtoul(x, NULL, 10)
#define str2int64(x)		strtol(x, NULL, 10)


typedef struct MutexLock
{
    pthread_mutex_t mutex;

	CP_MutexLock() {
	    pthread_mutex_init(&mutex, NULL);
	}

	~CP_MutexLock() {
	    pthread_mutex_destroy(&mutex);
	}

	inline void lock() {
		pthread_mutex_lock(&mutex);
	}

	inline void unlock() {
		pthread_mutex_unlock(&mutex);
	}
} MUTEXLOCK;

#endif // ______GLOBAL_DEFINE_H______
