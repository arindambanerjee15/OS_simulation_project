//Explanation of first code 
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>

sem_t paper, pen, question_paper, teacher;

void* student1(void* arg)
{
    sem_wait(&pen);
    printf("Student 1 has pen\n");
    sem_wait(&question_paper);
    printf("Student 1 has question paper\n");
    printf("Student 1 is making the assignment\n");
    sem_post(&teacher);
    return NULL;
}

void* student2(void* arg)
{
    sem_wait(&paper);
    printf("Student 2 has paper\n");
    sem_wait(&pen);
    printf("Student 2 has pen\n");
    printf("Student 2 is making the assignment\n");
    sem_post(&teacher);
    return NULL;
}

void* student3(void* arg)
{
    sem_wait(&question_paper);
    printf("Student 3 has question paper\n");
    sem_wait(&paper);
    printf("Student 3 has paper\n");
    printf("Student 3 is making the assignment\n");
    sem_post(&teacher);
    return NULL;
}

void* teacher_process(void* arg)
{
    while(1)
    {
        sem_wait(&teacher);
        printf("Teacher has the completed assignment\n");
        sem_post(&paper);
        sem_post(&pen);
        sem_post(&question_paper);
    }
    return NULL;
}

int main()
{
    sem_init(&pen,0,1);
    sem_init(&paper,0,1);
    sem_init(&question_paper,0,1);
    sem_init(&teacher,0,0);
    pthread_t tid1,tid2,tid3,tid4;
    pthread_create(&tid1,NULL,student1,NULL);
    pthread_create(&tid2,NULL,student2,NULL);
    pthread_create(&tid3,NULL,student3,NULL);
    pthread_create(&tid4,NULL,teacher_process,NULL);
    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);
    pthread_join(tid3,NULL);
    pthread_join(tid4,NULL);
    sem_destroy(&pen);
    sem_destroy(&paper);
    sem_destroy(&question_paper);
    sem_destroy(&teacher);
    return 0;
}


//Solution to ensure non-chaotic manner at the library:
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>

sem_t student_queue, teacher_queue, cpu, mutex;
int student_count = 0, teacher_count = 0;

void* student(void* arg)
{
    sem_wait(&mutex);
    student_count++;
    printf("Student %d enters the library\n", student_count);
    sem_post(&mutex);
    sem_wait(&student_queue);
    sem_wait(&cpu);
    printf("Student %d is getting the books issued\n", student_count);
    sem_post(&cpu);
    sem_post(&student_queue);
    sem_wait(&mutex);
    student_count--;
    printf("Student %d leaves the library\n", student_count+1);
        if(student_count == 0 && teacher_count > 0) 
    {
        sem_post(&teacher_queue);
    }
    sem_post(&mutex);
    return NULL;
}

void* teacher(void* arg)
{
    sem_wait(&mutex);
    teacher_count++;
    printf("Teacher %d enters the library\n", teacher_count);
    sem_post(&mutex);
    sem_wait(&teacher_queue);
    sem_wait(&cpu);
    printf("Teacher %d is getting the books issued\n", teacher_count);
    sem_post(&cpu);
    sem_wait(&mutex);
    teacher_count--;
    printf("Teacher %d leaves the library\n", teacher_count+1);
    if(student_count > 0)
    {
        sem_post(&student_queue);
    }
    else if(teacher_count > 0)
    {
        sem_post(&teacher_queue);
    }
    sem_post(&mutex);
    return NULL;
}

int main()
{
    sem_init(&student_queue,0,1);
    sem_init(&teacher_queue,0,1);
    sem_init(&cpu,0,1);
    sem_init(&mutex,0,1);
    pthread_t tid[10];
    for(int i=0; i<5; i++)
    {
        pthread_create(&tid[i], NULL, student, NULL);
    }
    for(int i=0; i<5; i++)
    {
        pthread_create(&tid[i+5], NULL, teacher, NULL);
    }
    for(int i=0; i<10; i++)
    {
        pthread_join(tid[i], NULL);
    }
    sem_destroy(&student_queue);
    sem_destroy(&teacher_queue);
    sem_destroy(&cpu);
    sem_destroy(&mutex);
    return 0;
}


//Explanation of part B
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

sem_t student_queue, teacher_queue, cpu, mutex, teacher_turn;
int student_count = 0, teacher_count = 0;

void* student(void* arg)
{
    sem_wait(&mutex);
    student_count++;
    printf("Student %d enters the library\n", student_count);
    sem_post(&mutex);

    sem_wait(&student_queue);
    sem_wait(&cpu);
    printf("Student %d is getting the books issued\n", student_count);
    sem_post(&cpu);

    sem_wait(&mutex);
    student_count--;
    printf("Student %d leaves the library\n", student_count+1);

    if(student_count == 0 && teacher_count > 0) 
    {
        sem_post(&teacher_turn);
    }

    sem_post(&student_queue);
    sem_post(&mutex);

    return NULL;
}

void* teacher(void* arg)
{
    sem_wait(&mutex);
    teacher_count++;
    printf("Teacher %d enters the library\n", teacher_count);
    sem_post(&mutex);

    if (teacher_count == 1) {
        sem_wait(&teacher_turn);
    }

    sem_wait(&teacher_queue);
    sem_wait(&cpu);
    printf("Teacher %d is getting the books issued\n", teacher_count);
    sem_post(&cpu);

    sem_wait(&mutex);
    teacher_count--;
    printf("Teacher %d leaves the library\n", teacher_count+1);

    if (teacher_count > 0) {
        sem_post(&teacher_turn);
    } else if(student_count > 0) {
        sem_post(&student_queue);
    }

    sem_post(&mutex);

    return NULL;
}

int main()
{
    sem_init(&student_queue,0,1);
    sem_init(&teacher_queue,0,1);
    sem_init(&cpu,0,1);
    sem_init(&mutex,0,1);
    sem_init(&teacher_turn,0,1);

    pthread_t tid[10];

    for(int i=0; i<5; i++) {
        pthread_create(&tid[i], NULL, student, NULL);
    }

    for(int i=0; i<5; i++) {
        pthread_create(&tid[i+5], NULL, teacher, NULL);
    }

    for(int i=0; i<10; i++) {
        pthread_join(tid[i], NULL);
    }

    sem_destroy(&student_queue);
    sem_destroy(&teacher_queue);
    sem_destroy(&cpu);
    sem_destroy(&mutex);
    sem_destroy(&teacher_turn);

    return 0;
}

