#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

#define IP "192.168.1.134"
#define PORT 8999			//http

int main()
{
	int 		s_fd, file_fd;		
	int 		segment;
	int 		total_received = 0;
	struct sockaddr_in sock;	//instance of type struct with port and family as members
	char 		buf[512];
	char 		*msg;
	char		*file_location = "./output.png";

	msg = "1";
		
	s_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (s_fd < 0)
	{
		perror("socket() error");
		return -1;
	}
	// Giving values to members of the struct
	sock.sin_addr.s_addr = inet_addr(IP);
	sock.sin_port = htons(PORT);
	sock.sin_family = AF_INET;

	if (connect(s_fd, (const struct sockaddr *) &sock, sizeof(sock)) != 0)
	{
		perror("connect() error");
		close(s_fd);
		return -1;
	}
	
	msg_size = read(s_fd, buf, 10);
	read(s_fd, buf, msg_size);
	write(s_fd, msg, 1);
	if ( (file_fd = open(file_location, O_RDWR | O_CREAT | O_APPEND, S_IWUSR) ) == -1 ) perror("open() error");
	else
	// receive file from server
	while ( (segment = read(s_fd, buf, 512) ) > 0 )
	{
		write(file_fd, buf, segment);
		total_received += segment;
		printf("[%d] received\n", total_received);
	}

	printf("Closing connection...\n");	
	close(file_fd);
	close(s_fd);

	return 0;
}
