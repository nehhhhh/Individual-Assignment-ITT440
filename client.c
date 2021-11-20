#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>


int main(int argc, char *argv[])
{
	int sd;
	struct sockaddr_in server;
	char *address;
	address = argv[1];

	//create socket
	sd = socket(AF_INET, SOCK_STREAM, 0);

	server.sin_family = AF_INET;
	server.sin_port = htons(8080);
	inet_aton(address, &(server.sin_addr));

	//connect
	connect(sd, (struct sockaddr *)&server, sizeof(server));
	char request[4096] = "GET/index.htmlHTTP/1.1\r\n\r\n";

	//send and receive
	//send(sd,request,sizeof(request),0);
	send(sd,request,sizeof(request),0);
	recv(sd,request,sizeof(request),0);


	printf("Server response:\n %s",request );
	close(sd);

	return 0;
}

