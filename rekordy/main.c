#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

int read_all_file(int fd)
{
        printf("reading all file started. \n");
        char *buffer = (char *)malloc(1);
        ssize_t read_status;
        lseek(fd, SEEK_SET, 0);
        while( ( read_status = read( fd, buffer, 1) ) != 0 )
        {
		int act_pos = lseek( fd, 0, SEEK_CUR );
 		printf("buffer%d: %s \n", act_pos, buffer);	
	}
        lseek(fd, SEEK_SET,0);
        printf("reading all file ended \n");
	return 1;
}


int open_file(char *file_name, int flag)
{
        int fd = open(file_name, flag);
        if( fd < 0 )
        {
               perror("Open file error! \n");	
	       return -1;
        }

        printf("Opened file: %s with flag: %d \n", file_name, flag);
        return fd;

}

long count_bits(char *file_name)
{
        struct stat st;
        int status = stat(file_name, &st);
        printf("File %s size: %ld \n", file_name, st.st_size);
        return st.st_size;
}

int rozdziel_klucz_i_wartosc(char *tekst, char *sciezka)
{
	printf("Tekst przed podziałem: %s \n", tekst);
	char klucz[61];
	char dane[64000];

	int fd = open_file( sciezka, O_RDWR);
	int pozycja = lseek(fd, 0, SEEK_END);
	printf("Pozycja kursora przed dodawaniem: %d \n", pozycja);
	char *tmp = strsep(&tekst, ":");
	strcpy(klucz,tmp);
	klucz[61] = '\0';
	strcpy(dane,tekst);
	printf("Klucz: %s Długość: %ld Wielkość: %ld \n", klucz, strlen(klucz), sizeof(klucz));
	printf("Wartość: %s Długość %ld Wielkość: %ld \n", dane, strlen(dane), sizeof(dane));
	unsigned short dlg = strlen(dane)/8;
	printf("Dlg: %d \n", dlg);
	ssize_t write_status = write(fd, klucz, sizeof(klucz));
	char *str_dlg = (char *)malloc(6);
	sprintf(str_dlg, "%d", dlg);
	printf("Ilość bajtów zajętych przez dane: %s \n", str_dlg);
	write_status = write( fd, str_dlg, strlen(str_dlg));
	write_status = write( fd, dane, sizeof(dane));
	
	
	
	/*
	char *klucz = strsep( &tekst, ":");
	printf("Klucz: %s \n", klucz);
	printf("Wartość: %s \n", tekst);
	
	int fd = open_file( sciezka, O_RDWR);
	int pozycja = lseek(fd, 0, SEEK_END);
	printf("Pozycja kursora przed dodawaniem: %d \n", pozycja);
	ftruncate(fd, pozycja+strlen(tekst)+strlen(klucz)+5);
	unsigned short dlg = strlen(tekst);
	ssize_t write_status = write( fd, klucz, strlen(klucz));
	write( fd, "\0\n", 2);
	char *str_dlg = (char *)malloc(6);
	sprintf(str_dlg, "%d", dlg);
	printf("Ilość bajtów zajętych przez dane: %s \n", str_dlg);
	write_status = write( fd, str_dlg, strlen(str_dlg));
	write( fd, "\n", 1);
	write_status = write( fd, tekst, strlen(tekst));
	write(fd, "\n", 1);
	*/
	//read_all_file(fd);
	return 1;

}

int print_data_from_key( char *sciezka, char *key )
{
	int fd = open_file( sciezka, O_RDWR);
	
}

int main( int argc, char *argv[] )
{
       	int opt;
       	char *sciezka = (char *)malloc(255);
	
       	while( (opt = getopt( argc, argv, "f:a:d:p:lrc")) != -1 )
        {
		switch(opt)
		{
			case 'f':
				sciezka = strcpy(sciezka, optarg);
				break;
			case 'a':
				printf("Dodawanie klucza: %s do bazy. \n", optarg);
				rozdziel_klucz_i_wartosc(optarg, sciezka);	
				break;
			case 'd':
				printf("Usunięcie zawartości związanej z kluczem: %s. \n", optarg);
				
				break;
			case 'p':
				printf("Wyświetlenie zawartości podanego klucza: %s. \n", optarg);
				int status = print_data_from_key(sciezka, optarg);
				break;
			case 'l':
				printf("Wylistowanie kluczy: \n");
				break;
			case 'r':
				printf("Raport wypełnienia bazy: \n");
				break;
			case 'c':
				printf("Kompresja. \n");
				break;
			default:
				perror("Zły parametr! \n");
				exit(1);
		}
	}
	return 0;
}
