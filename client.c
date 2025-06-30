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
	int 		s_fd;
	int		fin_fd, fout_fd;
	int 		segment;
	int 		file_size = 0;
	int 		option;
	struct sockaddr_in sock;	//instance of type struct with port and family as members
	char 		buf[512];
	char 		*msg;
	int		msg_size;
	char		*file_location = "./output.png";
		
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
	
	msg = "1";

	write( s_fd, msg, 1 );


	// download file from server
	if ( strchr(msg, '2') )
	{
		if ( (fout_fd = open(file_location, O_RDWR | O_CREAT, S_IRWXU) ) == -1 ) perror("fout open() error");
		while ( (segment = read(s_fd, buf, 512)) > 0 )
		{
			write(fout_fd, buf, segment);
			file_size += segment;
			//printf("[%d] received\n", file_size);
		}
		printf("received %d bytes\n", file_size);
	}
	// upload file to server
	else if ( strchr(msg, '1') )
	{
		if ( (fin_fd = open("./fallout.jpg", O_RDONLY)) < 0 ) perror("fin open() error");
		while ( (segment = read(fin_fd, buf, 512)) > 0)
		{
			write(s_fd, buf, segment);
			file_size += segment;
		}
		printf("%d attempted to send\n", file_size);
	}
	else printf("Wrong selection \n");
				
	printf("Closing connection...\n");	
	close(fin_fd);
	close(fout_fd);
	close(s_fd);

	return 0;
}
