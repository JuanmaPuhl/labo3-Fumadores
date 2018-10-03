#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

int papel,tabaco,cerillas;

sem_t mutex;
sem_t sem_prove;
sem_t sem_fuma[3];
sem_t fumando;

typedef struct auxiliar{
    int id;
} aux;

void* fAgente(void* arg){
    while(1){
    printf("Espero.\n");
    //sem_wait(&sem_prove);
    //printf("Ahora tengo el control.\n");
    int i=rand()%3;
    int j=i;
    while(j==i){
        j=rand()%3;
    }
    //printf("Tengo i=%d y j=%d.\n",i,j);
    sem_wait(&mutex);
    if(i==0){
        papel=1;
        sleep(1);
        printf("[AGENTE] Dejo papel.\n");
    }
    else
    if(i==1){
        tabaco=1;
        sleep(1);
        printf("[AGENTE] Dejo tabaco.\n");
    }
    else
    if(i==2){
        cerillas=1;
        sleep(1);
        printf("[AGENTE] Dejo cerillas.\n");
    }


    if(j==0){
        papel=1;
        sleep(1);
        printf("[AGENTE] Dejo papel.\n");
    }
    else
    if(j==1){
        tabaco=1;
        sleep(1);
        printf("[AGENTE] Dejo tabaco.\n");
    }
    else
    if(j==2){
        cerillas=1;
        sleep(1);
        printf("[AGENTE] Dejo cerillas.\n");
    }

    sem_post(&mutex);
    int x;

    for(x=0; x<3; x++){
        sem_post(&sem_fuma[x]);
        }
     sem_wait(&sem_prove);
    }
}

void* fFumadores(void* arg){
    int papel_f,tabaco_f,cerillas_f;
    aux* est=(aux*)arg;
    int id=est->id;
    if(id==0){
        papel_f=1;
        tabaco_f=0;
        cerillas_f=0;
        printf("[FUMADOR #%d] Tengo papel.\n",id);
    }
    else
    if(id==1){
        papel_f=0;
        tabaco_f=1;
        cerillas_f=0;
        printf("[FUMADOR #%d] Tengo tabaco.\n",id);
    }
    else
    if(id==2){
        papel_f=0;
        tabaco_f=0;
        cerillas_f=1;
        printf("[FUMADOR #%d] Tengo cerillas.\n",id);
    }
    while(1){
        //printf("[FUMADOR #%d]Estoy al principio. Papel=%d, tabaco=%d, cerillas=%d.\n",id,papel_f,tabaco_f,cerillas_f);

        if(tabaco_f!=0 && papel_f!=0 && cerillas_f!=0){

            printf("[FUMADOR #%d] Estoy fumando.\n",id);

            tabaco_f=0;
            papel_f=0;
            cerillas_f=0;
            sem_post(&sem_prove);
        }

        sem_wait(&sem_fuma[id]);
        sem_wait(&mutex);
            if(id==0){ //Tengo papel
                if(tabaco==1 && tabaco_f==0 && cerillas==1 && cerillas_f==0){
                    tabaco_f=1;
                    tabaco=0;
                    cerillas_f=1;
                    cerillas=0;
                    papel_f=2;
                    //printf("[FUMADOR #%d]Me abastezco de cerillas.\n",id);
                    //printf("[FUMADOR #%d]Me abastezco de tabaco.\n",id);
                }
            }
            else
            if(id==1){//Tengo tabaco
                if(papel==1 && papel_f==0 && cerillas==1 && cerillas_f==0){
                    papel_f=1;
                    papel=0;
                    cerillas_f=1;
                    cerillas=0;
                    tabaco_f=2;
                    //printf("[FUMADOR #%d]Me abastezco de cerillas.\n",id);
                    //printf("[FUMADOR #%d]Me abastezco de papel.\n",id);
                }
            }
            else
            if(id==2){ //Tengo cerillas
                if(papel==1 && papel_f==0 && tabaco==1 && tabaco_f==0){
                    papel_f=1;
                    papel=0;
                    tabaco_f=1;
                    tabaco=0;
                    cerillas_f=2;
                    //printf("[FUMADOR #%d]Me abastezco de tabaco.\n",id);
                    //printf("[FUMADOR #%d]Me abastezco de papel.\n",id);
                }
            }
            sem_post(&mutex);
        }
    }



int main(){
    pthread_t fumadores[3];
    pthread_t agente;
    sem_init(&mutex,0,1);
    sem_init(&sem_prove,0,0);
    int x;
    for(x=0;x<3;x++){
        sem_init(&sem_fuma[x],0,0);
    }
    papel=tabaco=cerillas=0;

    pthread_create(&agente,NULL,fAgente,(void*)&x);
    for(x=0; x<3; x++){
        aux* est=(aux*)malloc(sizeof(aux));
        est->id=x;
        pthread_create(&fumadores[x],NULL,fFumadores,(void*)est);
    }

    for(x=0;x<3;x++){
        //pthread_join(ayuda[x],NULL);
        pthread_join(fumadores[x],NULL);
    }
    pthread_join(agente,NULL);
    return 0;
}





































//    for(x=0; x<3; x++){
//        aux* est=(aux*)malloc(sizeof(aux));
//        est->id=x;
//        pthread_create(&fumadores[x],NULL,fFumadores,(void*)est);
//    }
//
//    for(x=0;x<3;x++){
//        pthread_join(ayuda[x],NULL);
//        pthread_join(fumadores[x],NULL);
//    }
//    pthread_join(agente,NULL);





//sem_t ingredientes[3];
//sem_t s[6];
//sem_t mutex;
//sem_t sem;
//int t;
//typedef struct auxiliar{
//    int id;
//} aux;
//
//void* fAgente(void* arg){
//    int i,j;
//    while(1){
//        i=rand()%3;
//        printf("[AGENTE] Deje el elemento %d.\n",i);
//        j=i;
//        while(j==i)
//            j=rand()%3;
//        printf("[AGENTE] Deje el elemento %d.\n",j);
//        //sleep(1);
//        sem_wait(&sem);
//        sem_post(&ingredientes[i]);
//        sem_post(&ingredientes[j]);
//
//    }
//}
//
//void* fAyuda(void* arg){
//    aux* est=(aux*) arg;
//    int id=est->id;
//    int incr[3]={1,2,4};
//    while(1){
//        sem_wait(&ingredientes[id]);
//        sem_wait(&mutex);
//        t=t+incr[id];
//        sem_post(&s[t]);
//        sem_post(&mutex);
//    }
//}
//
//void* fFumadores(void* arg){
//    aux* est=(aux*)arg;
//    int id=est->id;
//    int necesito[3]={6,5,3};
//    while(1){
//        sem_wait(&s[necesito[id]]);
//        t=0;
//
//
//    printf("[Fumador #%d]Estoy fumando.\n",id);
//    sleep(2);
//    sem_post(&sem);
//    }
//}
//
//int main(){
//    pthread_t fumadores[3];
//    pthread_t agente;
//    pthread_t ayuda[3];
//    t=0;
//    int x;
//    for(x=0; x<3 ;x++){
//        sem_init(&ingredientes[x],0,0);
//    }
//    for(x=0; x<6; x++){
//        sem_init(&s[6],0,0);
//    }
//    sem_init(&mutex,0,1);
//    sem_init(&sem,0,1);
//
//    pthread_create(&agente,NULL,fAgente,(void*)&x);
//    for(x=0; x<3; x++){
//        aux* est=(aux*)malloc(sizeof(aux));
//        est->id=x;
//        pthread_create(&ayuda[x],NULL,fAyuda,(void*)est);
//    }
//    for(x=0; x<3; x++){
//        aux* est=(aux*)malloc(sizeof(aux));
//        est->id=x;
//        pthread_create(&fumadores[x],NULL,fFumadores,(void*)est);
//    }
//
//    for(x=0;x<3;x++){
//        pthread_join(ayuda[x],NULL);
//        pthread_join(fumadores[x],NULL);
//    }
//    pthread_join(agente,NULL);
//
//
//
//
//    printf("Hello world!\n");
//    return 0;
//}
