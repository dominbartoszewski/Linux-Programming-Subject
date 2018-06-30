// vim: sts=4 sw=4 et :
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>

//budzik..
//
sigset_t mask;

timer_t timerid;
timer_t timerid_exit;

struct sigevent sev;
struct sigevent sev_exit;

struct itimerspec its;
struct itimerspec its_exit;

time_t sec = 1;
long nsec = 0;

float lambda;

const long NANO = 1000000000L;
const long initial = 250000000L;
struct timespec volatile dlay;

char ROTOR_TAB[] = "-\\|/";
int ROTOR_CHR = 0;

const char * SAVEPOS = "";
const char * RESTPOS = "";
const char * JUMPPOS = "[%d;%df";

// pozycja na ekranie
struct { int W, K; } pos;

void sigHandler( int sigNum, siginfo_t *siginfo, void *ptrVoid)
{
    int diff = 0.05 * NANO;
    int initial = 0.25 * NANO;
    if( !dlay.tv_sec && !dlay.tv_nsec )
    {
        dlay = (struct timespec) { 0,initial };
        return;
    }
    
    if( sigNum == SIGUSR1)
    {
        if( siginfo->si_pid == getppid() )
        {
           if( pos.W < 57) 
               pos.W++;
        }
        else
        {
            dlay.tv_nsec += diff;
            if( dlay.tv_nsec > NANO )
            {
                dlay.tv_sec++;
                dlay.tv_nsec -= NANO;
            }
        }
    }
    else if( sigNum == SIGUSR2 )
    {
        if( siginfo->si_pid == getppid() )
        {
            if( pos.W > 0 )
                pos.W--;
        }
        else
        {
            dlay.tv_nsec -= diff;
            if( dlay.tv_nsec < 0)
            {
                dlay.tv_sec--;
                dlay.tv_nsec += NANO;
            }
        }
    }
    else if( sigNum == SIGRTMIN )
    {

        if(getppid() == 1)
        {
            timer_create(CLOCK_MONOTONIC, &sev_exit, &timerid_exit);
            
            its_exit.it_value.tv_sec = rand() % (2) + 1;  
            its_exit.it_value.tv_nsec = rand() % (500000000 +1 );
            its_exit.it_interval.tv_sec = 0;
            its_exit.it_interval.tv_nsec = 0;
            
            sigemptyset(&mask);
            sigaddset(&mask, SIGRTMIN);
            sigprocmask(SIG_SETMASK, &mask, NULL);
            
            timer_settime(timerid_exit, 0, &its_exit, NULL);
        }
    }
    else if( sigNum == SIGRTMIN+1 )
    {
        raise(SIGTERM);
    }
}

void sigFunc()
{
    struct sigaction sa;
    memset( &sa, '\0', sizeof(sa));
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = sigHandler;
    
    if( sigaction( SIGUSR1, &sa, NULL ) == -1 
            || sigaction( SIGUSR2, &sa, NULL ) == -1 
            || sigaction( SIGRTMIN, &sa, NULL) == -1
            || sigaction( SIGTERM, &sa, NULL) == -1)
    {
        perror("sigaction error");
        exit(9);
    }

}

void delay( void ) {
    /* spanie na czas określony przez dlay
     * jeżeli nie określony (== 0), to aż do obsługi sygnału,
     * który coś zmieni */
    int status;

super_loop:    
    if( (dlay.tv_sec*NANO + dlay.tv_nsec) == 0L ) { 
        
        // czekanie na sygnal
        // oczekiwanie na zmianę 
        
        while( (dlay.tv_sec*NANO + dlay.tv_nsec) == 0L ) {
            pause();
        }

        return;
    }

    /* spanie z dosypianiem */
    struct timespec rest = dlay;
    while( (status=nanosleep(&rest,&rest)) == -1 ) {
        if( errno != EINTR ) {
            // błąd => zawieszenie rotora
            dlay = (struct timespec){0L,0L};
            goto super_loop;
        }

        // w tym miejscu wiadomo, że  errno == EINTR 
        // mogła nastąpić zmiana prędkości obrotów
        // czas dosypiania to uwzględni - możliwy krótszy czas dosypiania
        int R = rest.tv_sec*NANO + rest.tv_nsec;
        int V = dlay.tv_sec*NANO + dlay.tv_nsec;
        if( V < R ) rest = dlay;
    }
} 

/* zmienia wizerunek rotora 
 * używa kodów sterujących konsoli */
void step_rotor( void ) {
    fputs(SAVEPOS,stdout);
    printf(JUMPPOS,pos.W,pos.K);
    ROTOR_CHR %= 4;
    fputc(ROTOR_TAB[ROTOR_CHR++],stdout);
    fputs(RESTPOS,stdout);
    fflush(stdout);
}

void usage( char * name, int status ) {
    char * kody = "kody błędów:\n"
        "  1 - zła ilość parametrów\n"
        "  2 - błąd parsowania <wiersza>\n"
        "  3 - błąd parsowania <kolumny>\n";
    fprintf((status == 0)? stdout : stderr,
            "%s <wiersz> <kolumna>\n\t oba parametry typu int\n",
            name);
    if( status ) 
        fprintf(stderr,kody);
    
    exit(status);
}

void parseargs( char ** argv ) {
    char * end;
    pos.W = strtol(argv[1],&end,10);
    
    if( *end ) 
        usage(argv[0],2);
    
    pos.K = strtol(argv[2],&end,10);
    
    if( *end ) 
        usage(argv[0],3);
}

int main(int argc, char ** argv) {
 
    srand((unsigned int)time(NULL));

    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = SIGRTMIN;
    sev.sigev_value.sival_ptr = &timerid;

    sev_exit.sigev_notify = SIGEV_SIGNAL;
    sev_exit.sigev_signo = SIGRTMIN+1;
    sev_exit.sigev_value.sival_ptr = &timerid_exit;

    its.it_value.tv_sec = its.it_interval.tv_sec = sec;
    its.it_value.tv_nsec = its.it_interval.tv_nsec = nsec;
    
    timer_create(CLOCK_MONOTONIC, &sev, &timerid);
    timer_create(CLOCK_MONOTONIC, &sev_exit, &timerid_exit);
    timer_settime(timerid, 0, &its, NULL);

    /* argumenty: dwie liczby calkowite = wspołrzędne ekranowe */
    
    if(argc!=3) usage(argv[0],1);
    parseargs(argv);        /* samo opuszcza program */
    sigFunc();
    step_rotor();
    while( 1 ) {
        delay();
        step_rotor();
    }
    
    return 0;
}
