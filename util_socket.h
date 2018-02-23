// util_socket.h

#ifndef ______UTIL_SOCKET_H______
#define ______UTIL_SOCKET_H______

/*
 * 通讯相关
 */

#ifdef _WIN32_ENV_
  #define SOCKET_T		SOCKET
#else
  #define SOCKET_T		int
#endif

#ifdef _WIN32_ENV_
  #define close_socket(x)		(::closesocket(x))
#else
  #define close_socket(x)		(::close(x))
#endif

static var_4 set_overtime (SOCKET_T in_socket, var_4 in_time_ms)
{
#ifdef _WIN32_ENV_
	setsockopt(in_socket, SOL_SOCKET, SO_RCVTIMEO, (var_1*)&in_time_ms, sizeof(in_time));
	setsockopt(in_socket, SOL_SOCKET, SO_SNDTIMEO, (var_1*)&in_time_ms, sizeof(in_time));
#else
	struct timeval over_time;
	over_time.tv_sec = in_time_ms / 1000;
	over_time.tv_usec = (in_time_ms % 1000) * 1000;
	setsockopt(in_socket, SOL_SOCKET, SO_RCVTIMEO, (var_1*)&over_time, sizeof(over_time));
	setsockopt(in_socket, SOL_SOCKET, SO_SNDTIMEO, (var_1*)&over_time, sizeof(over_time));
#endif
	return 0;
}

static var_4 init_socket()
{
#ifdef _WIN32_ENV_
	WSAData cWSAData;
	if (WSAStartup(MAKEWORD(2, 2), &cWSAData)) {
		printf("cp_init_socket: init socket failure\n");
		return -1;
	}
#else
	sigset_t signal_mask;
	sigemptyset(&signal_mask);
	sigaddset(&signal_mask, SIGPIPE);
	if (pthread_sigmask(SIG_BLOCK, &signal_mask, NULL))
	{
		printf("init_socket: block sigpipe failure\n");
		return -1;
	}
#endif

	return 0;
}

static var_4 listen_socket(SOCKET_T& in_listen, var_u2 in_port)
{
	struct sockaddr_in my_server_addr;
	my_server_addr.sin_family = AF_INET;
	my_server_addr.sin_port = htons(in_port);
	my_server_addr.sin_addr.s_addr = htons(INADDR_ANY);

	in_listen = socket(AF_INET, SOCK_STREAM, 0);
	if (in_listen < 0)
	{
		printf("listen_socket: create socket failure\n");
		return -1;
	}
	if (bind(in_listen, (struct sockaddr*)&my_server_addr, sizeof(my_server_addr)) < 0)
	{
		close_socket(in_listen);
		printf("listen_socket: bind to port %d failure!\n", in_port);
		return -1;
	}
	if (listen(in_listen, SOMAXCONN) < 0)
	{
		close_socket(in_listen);
		printf("listen_socket: listen to port %d failure\n", in_port);
		return -1;
	}

	return 0;
}

static var_4 connect_socket(SOCKET_T& in_socket, var_1* in_ip, var_u2 in_port)
{
	struct sockaddr_in my_server_addr;
	my_server_addr.sin_family = AF_INET;
	my_server_addr.sin_port = htons(in_port);
	my_server_addr.sin_addr.s_addr = inet_addr(in_ip);

	in_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (in_socket < 0)
	{
		printf("connect_socket: create socket failure\n");
		return -1;
	}
	if (connect(in_socket, (struct sockaddr*)&my_server_addr, sizeof(my_server_addr)) < 0)
	{
		close_socket(in_socket);

		printf("connect_socket: connect %s:%d failure\n", in_ip, in_port);
		return -1;
	}

	return 0;
}

static var_4 accept_socket(SOCKET_T in_listen, SOCKET_T& out_socket)
{
	struct sockaddr_in my_client_addr;
#ifdef _WIN32_ENV_
	int length = sizeof(my_client_addr);
#else
	socklen_t length = sizeof(my_client_addr);
#endif

	out_socket = accept(in_listen, (struct sockaddr*)&my_client_addr, &length);
	if (out_socket == -1)
		return -1;

	return 0;
}

static var_4 sendbuf(SOCKET_T in_socket, var_1* in_buffer, var_4 in_buflen)
{
	var_4 retval = 0, finlen = 0;
	do {
		retval = (var_4)send(in_socket, in_buffer + finlen, in_buflen - finlen, 0);
		if (retval > 0)
			finlen += retval;
#ifdef _WIN32_ENV_
#else
		if (retval < 0 && errno == EINTR)
			continue;
#endif
	} while (retval > 0 && finlen < in_buflen);
	if (retval < 0 || finlen < in_buflen)
		return -1;

	return 0;
}

static var_4 recvbuf(CP_SOCKET_T in_socket, var_1* in_buffer, var_4 in_buflen)
{
	var_4 retval = 0, finlen = 0;
	do {
		retval = (var_4)recv(in_socket, in_buffer + finlen, in_buflen - finlen, 0);
		if (retval > 0)
			finlen += retval;
	} while (retval > 0 && finlen < in_buflen);
	if (retval < 0 || finlen < in_buflen)
		return -1;

	return 0;
}

static var_4 recvbuf_onebyte(SOCKET_T in_socket, var_1 end_char, var_1* in_buffer, var_4& in_buflen)
{
	in_buflen = 0;
	var_4 once_len = 0;

	for (;;)
	{
		once_len = (var_4)recv(in_socket, in_buffer + in_buflen, 1, 0);
		if (once_len <= 0)
			break;
		in_buflen += once_len;
		if (in_buffer[in_buflen - 1] == end_char)
			break;
	}
	if (once_len <= 0)
		return -1;

	in_buflen--;

	return 0;
}

#endif // ______UTIL_SOCKET_H______