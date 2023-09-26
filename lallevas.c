#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

void espera(int seg);
void seccion_critica();
void seccion_critica_debug();


void fin_programa() {
    char mensaje_fin[] = "\nPrograma acabado correctamente\n";
    while ((wait(NULL)) > 0) {}
    write(1, mensaje_fin, strlen(mensaje_fin));
    exit(0); 
}

void nada() {}

void manejadora_SIGUSR1() {      
    srand(time(NULL));  
    int num;
    num = rand() % 2;
    while (num == 0) {                    
        seccion_critica();
        num = rand() % 2;
    }
} 

void manejadora_SIGUSR1_debug() {
    srand(time(NULL));  
    int num;
    num = rand() % 2;
    while (num == 0) {     
        espera(1);                 
        seccion_critica_debug();
        num = rand() % 2;
    }
}


int main(int argc, char *argv[]) {
    int valor_devuelto_fork, num_procesos, i = 0;
    num_procesos = atoi(argv[1]);
    int vector_PID_hijos [num_procesos];
    int padre = getpid();
    sigset_t mascara_con_SIGALRM, mascara_vacia;
    struct sigaction muerte, hijos, testigo, alarma;
    struct timespec tim, tim2; 
    tim.tv_sec = 1;
    tim.tv_nsec = 150000000;
  
    if (argc != 2) {
        if (argc == 3 && strcmp(argv[2], "debug") != 0) {
            char mensaje_error1[] = "Error en la ejecucion del programa\nEjemplo de ejecucion: lallevas 3 [debug]\n";
            write(1, mensaje_error1, strlen(mensaje_error1));
            return 1;
        }
    }
    
    
    if (num_procesos < 3 || num_procesos > 33) {
        char mensaje_error2[] = "El numero de procesos debe estar comprendido entre 3 y 33\n";
        write(1, mensaje_error2, strlen(mensaje_error2));
        return 2;
    }
    
    if((sigemptyset(&mascara_con_SIGALRM)) == -1) {
        perror("sigemptyset");
        return 6;
    }
    if((sigaddset(&mascara_con_SIGALRM, SIGALRM)) == -1) {
        perror("sigaddset");
        return 8;
    }    
    if((sigaddset(&mascara_con_SIGALRM, SIGUSR1)) == -1) {
        perror("sigaddset");
        return 8;
    }
    if((sigprocmask(SIG_SETMASK, &mascara_con_SIGALRM, &mascara_vacia)) == -1) {
        perror("sigprocmask");
        return 7;
    } 
            
    if (argc == 2) {        
        testigo.sa_handler = manejadora_SIGUSR1;
        if((sigemptyset(&testigo.sa_mask)) == -1) {
            perror("sigemptyset");
            return 6;
        }
        testigo.sa_flags = SA_RESTART;
        if ((sigaction(SIGUSR1, &testigo, NULL))==-1) {
            perror("sigaction");
            return 3;
        }
    }
    else if (argc == 3) {
        testigo.sa_handler = manejadora_SIGUSR1_debug;
        if((sigemptyset(&testigo.sa_mask)) == -1) {
            perror("sigemptyset");
            return 6;
        }
        testigo.sa_flags = SA_RESTART;
        if ((sigaction(SIGUSR1, &testigo, NULL))==-1) {
            perror("sigaction");
            return 3;
        }
    }
    
    
    alarma.sa_handler = nada;
    if((sigemptyset(&alarma.sa_mask)) == -1) {
        perror("sigemptyset");
        return 6;
    }
    alarma.sa_flags = SA_RESTART;
    if ((sigaction(SIGALRM, &alarma, NULL))==-1) {
        perror("sigaction");
        return 3;
    }
      
    valor_devuelto_fork = fork();
    if (valor_devuelto_fork == -1) {
            perror("fork");
            return 4;
    }
    if (valor_devuelto_fork != 0) {
        vector_PID_hijos[0] = getpid();
        vector_PID_hijos[1] = valor_devuelto_fork;
    }
    while (valor_devuelto_fork != 0 && i < (num_procesos - 2)) { 
        valor_devuelto_fork = fork();
        i++;     
        if (valor_devuelto_fork == -1) {
            perror("fork");
            return 4;
        }  
        if (valor_devuelto_fork != 0) {
            vector_PID_hijos[i+1] = valor_devuelto_fork;
        } 
    }
                   
    switch (valor_devuelto_fork) {
        case -1:
            perror("fork");
            return 4;
        case 0:            
            for(;;) {
                sigsuspend(&mascara_vacia);
                if ((kill(padre, SIGALRM)) == -1) {
                    perror("kill: Pulsa Control + C para finalizar");
                    sigsuspend(&mascara_vacia);
                }
            }
            break;
        default:       
            muerte.sa_handler = fin_programa;
            if((sigemptyset(&muerte.sa_mask)) == -1) {
                perror("sigemptyset: Pulsa Control + C para finalizar");
                sigsuspend(&mascara_vacia);
            }
            muerte.sa_flags = SA_RESTART;
            if ((sigaction(SIGINT, &muerte, NULL))==-1) {
                perror("sigaction: Pulsa Control + C para finalizar");
                sigsuspend(&mascara_vacia);
            }                 
                         
            for(;;) {       
            for (i = 0; i < num_procesos; i++) {
                if ((kill(vector_PID_hijos[i], SIGUSR1)) == -1) {
                    perror("kill: Pulsa Control + C para finalizar");
                    sigsuspend(&mascara_vacia);              
                }
                sigsuspend(&mascara_vacia);
            }
            }            
            break;
    }   
}
    
void espera(int seg) {
    sigset_t mascara_vacia;
    if((sigemptyset(&mascara_vacia)) == -1) {
        perror("sigemptyset: Pulsa Control + C para finalizar");
        sigsuspend(&mascara_vacia);
    }
    alarm(seg);
    sigsuspend(&mascara_vacia);
}    

void seccion_critica() {
    write(1, "E", 1);
    write(1, "S", 1);
}

void seccion_critica_debug() {
    char cadena[20];
    sprintf(cadena, "E(%d)", getpid());
    write(1, cadena, strlen(cadena));
    espera(2);
    sprintf(cadena, "S(%d)", getpid());
    write(1, cadena, strlen(cadena));
}
