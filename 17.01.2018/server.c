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
	char datagram[] = { 7, 16,125,0,65,66,67,12 };

	struct sockaddr_un sun;
	sockfd = socket( AF_UNIX, SOCK_DGRAM, 0 );

	if( sockfd == -1)
	{
		perror("Error making socket! \n");
		exit(-1);
	}
	
	sun.sun_family = AF_UNIX;
	char path_tab[] = "/home/student/socketUNIXserver";
	strncpy(sun.sun_path, path_tab, sizeof(path_tab));

	int bind_flag = bind( sockfd, (struct sockaddr*)(&sun), sizeof(sun)); 

	if( bind_flag == -1)
	{
		perror("Error binding socket! \n");
		exit(-1);
	}

	char buffer[] = "jestem";
	int status = sendto(sockfd, datagram, strlen(datagram)+1, 0, (struct sockaddr*)(&sun), sizeof(sun));
	if( status == -1 )
	{
		perror("sendto failed \n");
		return -1;
	}
	else
	{
		printf("sendto status: %d \n", status);
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
