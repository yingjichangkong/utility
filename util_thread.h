// util_thread.h

#ifndef ______UTIL_THREAD_H______
#define ______UTIL_THREAD_H______

#ifdef _WIN32_ENV_
  #define THREAD_T		DWORD WINAPI
#else
  #define THREAD_T		void*
#endif

#ifdef _WIN32_ENV_
 static var_4 create_thread(LPTHREAD_START_ROUTINE in_function, void* in_argv)
 {
	DWORD dwThreadID;
	HANDLE hThreadHandle = CreateThread(NULL, 0, in_function, in_argv, 0, &dwThreadID);
	if (hThreadHandle == NULL)
		return -1;
	CloseHandle(hThreadHandle);
	return 0;
 }
#else
 static var_4 create_thread(void* (in_function)(void*), void* in_argv)
 {
	pthread_t pid;
	if (pthread_create(&pid, 0, in_function, in_argv))
		return -1;
	return 0;
 }

static var_4 create_thread(void* (in_function)(void*), void* in_argv, var_4 create_num)
{
	pthread_t pid;
	for (var_4 i = 0; i < create_num; i++)
	{
		if (pthread_create(&pid, 0, in_function, in_argv))
			return -1;
	}
	return 0;
}
#endif

static var_u1 BM_RULE_BIT[] = { 0x80, 0x40, 0x20, 0x10, 0x8, 0x4, 0x2, 0x1 };
static var_u1 BM_RULE_ZERO_BIT[] = { 0x7F, 0xBF, 0xDF, 0xEF, 0xF7, 0xFB, 0xFD, 0xFE };

#define BM_SET_BIT(bm, val)			(((var_u1*)(bm))[(val)>>3] |= BM_RULE_BIT[(val)&0x7])
#define BM_SET_BIT_ZERO(bm, val)	(((var_u1*)(bm))[(val)>>3] &= BM_RULE_ZERO_BIT[(val)&0x7])
#define BM_GET_BIT(bm, val)			(((var_u1*)(bm))[(val)>>3] & BM_RULE_BIT[(val)&0x7])

#define BM_LEN_1BYTE(num)	(((num) + 7) / 8)
#define BM_LEN_2BYTE(num)	(((num) + 15) / 16)
#define BM_LEN_4BYTE(num)	(((num) + 31) / 32)
#define BM_LEN_8BYTE(num)	(((num) + 63) / 64)

// ʱ��ͳ��
typedef struct stat_time_t
{
	struct timeval tv_1, tv_2;

	inline void set_time_begin()
	{
		gettimeofday(&tv_1, 0);
	}

	inline void set_time_end()
	{
		gettimeofday(&tv_2, 0);
	}

	inline var_4 get_time_s()
	{
		return (var_4)((tv_2.tv_sec - tv_1.tv_sec) + (tv_2.tv_usec - tv_1.tv_usec) / 1000000);
	}

	inline var_4 get_time_ms()
	{
		return (var_4)((tv_2.tv_sec - tv_1.tv_sec) * 1000 + (tv_2.tv_usec - tv_1.tv_usec) / 1000);
	}

	inline var_4 get_time_us()
	{
		return (var_4)((tv_2.tv_sec - tv_1.tv_sec) * 1000000 + (tv_2.tv_usec - tv_1.tv_usec));
	}

	static var_8 get_time()
	{
		struct timeval tv_T;
		gettimeofday(&tv_T, 0);

		return tv_T.tv_sec * 1000000 + tv_T.tv_usec;
	}

	inline void clear()
	{
		memset(&tv_1, 0, sizeof(struct timeval));
		memset(&tv_2, 0, sizeof(struct timeval));
	}

} STAT_TIME_T;



static var_vd cp_warning_no_use()
{
	BM_RULE_BIT[0] = BM_RULE_BIT[0];
	BM_RULE_ZERO_BIT[0] = BM_RULE_ZERO_BIT[0];
}

/*
�ӽ��̵Ľ���״̬���غ����status���м�������б�������
WIFEXITED(status)����ӽ�������������Ϊ��0ֵ��
WEXITSTATUS(status)ȡ���ӽ���exit()���صĽ������룬һ�������WIFEXITED ���ж��Ƿ�������������ʹ�ô˺ꡣ
WIFSIGNALED(status)����ӽ�������Ϊ�źŶ�������˺�ֵΪ��
WTERMSIG(status)ȡ���ӽ������źŶ���ֹ���źŴ��룬һ�������WIFSIGNALED ���жϺ��ʹ�ô˺ꡣ
WIFSTOPPED(status)����ӽ��̴�����ִͣ�������˺�ֵΪ�档һ��ֻ��ʹ��WUNTRACED ʱ�Ż��д������
WSTOPSIG(status)ȡ�������ӽ�����ͣ���źŴ��룬
*/
#endif ______UTIL_THREAD_H______