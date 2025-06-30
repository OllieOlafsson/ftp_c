#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <fcntl.h>

#define PORT 8999
#define IP INADDR_ANY

int main()
{
	int 		server_fd, conn_fd;
	int		fin_fd, fout_fd;
	int 		segment;
	char 		*matched;
	int 		file_size = 0;
	int		written;
	struct sockaddr_in sock, conn;
	socklen_t 	address_len = sizeof(sock);
	char 		buf[512];
	char		chunks[512] = "";
	char		*msg = "Welcome to server. Please select an option";
	char 		*file_location = "/home/franco/Pictures/meditate.png";

	server_fd = socket(AF_INET, SOCK_STREAM, 0);

	if (server_fd == -1)
	{
		printf("Socket error\n");
		return -1;
	}
	setsockopt( server_fd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int) );

	sock.sin_family = AF_INET;
	sock.sin_port = htons(PORT);
	sock.sin_addr.s_addr = htonl(IP);

	ssize_t bind_ret = bind (
				server_fd, 
				(const struct sockaddr *) &sock, 
				sizeof(sock) 
				);

	if ( bind_ret == -1 ) 
		perror("bind() error");

/*	printf("%s", getpeername(
				server_fd, 
				(struct sockaddr *) &buf,
				
				);*/

	if ( listen(server_fd, 1) == 0 )
	{
		printf("Listening for connections...\n");
		conn_fd = accept(
				server_fd, 
				(struct sockaddr *) &sock, 
				&address_len 
				);
		if ( conn_fd == -1 )
		{
			printf("Connection socket error\n");
			return -1;
		}

		// Receive input from client
		read(conn_fd, buf, 1);

		// Client uploads
		if ( strchr(buf, '1') ) 
		{
			fout_fd = open("./output.png", O_RDWR | O_CREAT, S_IRWXU);
			if ( fout_fd < 0 ) perror("fout open() error");

			while ( (segment = read(conn_fd, buf, 511)) > 0 )
			{
				write(fout_fd, buf, segment);
				file_size += segment;
				//printf("[%d] received\n", file_size);
			}
			printf("[%d] bytes received\n", file_size);

		}
		// Client donwloads
		else if ( strchr(buf, '2') ) 
		{
			fin_fd = open(file_location, O_RDONLY);
			if ( fin_fd == -1 ) perror("open() error");	
			else
			while ( (segment = read(fin_fd, buf, 512)) > 0 ) 
			{
				write(conn_fd, buf, 512);
				file_size += segment;
			}
			printf("%d attempted to send\n", file_size);
		}
		else printf("Wrong input from client\n");

			
	}
	close(fin_fd);
	close(fout_fd);
	close(conn_fd);		
	close(server_fd);
}
