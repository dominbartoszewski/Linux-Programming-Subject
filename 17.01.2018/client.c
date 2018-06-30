#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>

int main( int argc, char *argv[] )
{
	int sockfd;
	char datagram[] = { 7,16,125,0,65,66,67,12 };
	char path_tab[] = "/home/student/socketUNIXclient";
	struct sockaddr_un from;
	sockfd = socket( AF_UNIX, SOCK_DGRAM, 0 );

	if( sockfd == -1)
	{
		perror("Error making socket! \n");
		exit(-1);
	}
	
	from.sun_family = AF_UNIX;
	strncpy(from.sun_path, path_tab, sizeof(path_tab));

	int bind_flag = bind( sockfd, (struct sockaddr*)(&from), sizeof(from)); 

	if( bind_flag == -1)
	{
		perror("Error binding socket! \n");
		exit(-1);
	}

	char buffer[108]; // hehehe
	socklen_t addr_len;
	addr_len = sizeof(addr_len);
	printf("przed \n");
	int status = recvfrom(sockfd, buffer, strlen(buffer)+1, 0, (struct sockaddr *)(&from), &addr_len);
	printf("po \n");
	if( status == -1 )
	{
		perror("recvfrom failed \n");
		return -1;
	}



	//recvfrom
	
	//zamiast sockaddr - sockaddr_un
	// bind(...) - podajemy sciezke i ta sciezka nie moze wskazywac na
	// istniejacy obiekt musi byc osiagalna z prawa do zapisu

	// listen - nie teraz 
	// accept - nie teraz
	// shutdown - nie teraz 

	return 0;
}
