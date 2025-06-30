#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

int main()
{
	int sock = socket(AF_UNIX, SOCK_STREAM, 0);

	printf(*sock);
}

