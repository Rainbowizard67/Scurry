#include "../include/scurry_utils.h"

/*
This static function is for creating a work node and adding to the linked list.
The arguments are thread_funt_t structure function and void pointer arg, returns tpool_work_t structure.
*/
static tpool_work_t* tpool_work_create(thread_func_t func, void* arg) {
    tpool_work_t* work;

    if(func==NULL) {
        return NULL;
    }

    work = malloc(sizeof(*work));
    work->func = func;
    work->arg = arg;
    work->next = NULL;

    return work;
}

/*
This static function is for freeing a work node from the tpool linked list.
The argument is tpool_work_t structure, returns void.
*/
static void tpool_work_destroy(tpool_work_t* work) {
    if(work==NULL) {
        return;
    }
    free(work);
}

/*
This static function is for returning specific work objects from the list.
The argument is tpool_t structure, returns tpool_work_t structure.
*/
static tpool_work_t* tpool_work_get(tpool_t* tm) {
    tpool_work_t* work;

    if(tm==NULL) {
        return NULL;
    }

    work = tm->work_first;
    if(work==NULL) {
        return NULL;
    }

    if(work->next==NULL) {
        tm->work_first = NULL;
        tm->work_last = NULL;
    }
    else {
        tm->work_first = work->next;
    }

    return work;
}

/*
Main worker static function that waits for work for the pool and the actual worker.
The argument is void pointer arg, returns a void pointer.
*/
static void* tpool_worker(void* arg) {
    tpool_t* tm = arg;
    tpool_work_t* work;

    while(true) {
        pthread_mutex_lock(&(tm->work_mutex));

        while(tm->work_first == NULL && !tm->stop) {
            pthread_cond_wait(&(tm->work_cond), &(tm->work_mutex));
        }
        if(tm->stop) {break;}

        work = tpool_work_get(tm);
        tm->working_cnt++;
        pthread_mutex_unlock(&(tm->work_mutex));

        if(work!=NULL) {
            work->func(work->arg);
            tpool_work_destroy(work);
        }

        pthread_mutex_lock(&(tm->work_mutex));
        tm->working_cnt--;
        if(!tm->stop && tm->working_cnt == 0 && tm->work_first == NULL) {
            pthread_cond_signal(&(tm->working_cond));
        }
        pthread_mutex_unlock(&(tm->work_mutex));
    }

    tm->thread_cnt--;
    pthread_cond_signal(&(tm->working_cond));
    pthread_mutex_unlock(&(tm->work_mutex));
    return NULL;
}

/*
This function is to create the entire thread pool.
The argument is size_t arg, return tpool_t structure.
*/
tpool_t* tpool_create(size_t num) {
    if(num > 15) {
        return NULL;
    }
    
    tpool_t* tm;
    pthread_t thread;
    size_t i;

    if(num==0) {
        num=2;
    }
    tm=calloc(1, sizeof(*tm));
    tm->thread_cnt = num;

    pthread_mutex_init(&(tm->work_mutex), NULL);
    pthread_cond_init(&(tm->work_cond), NULL);
    pthread_cond_init(&(tm->working_cond), NULL);

    tm->work_first = NULL;
    tm->work_last = NULL;

    for(i=0; i<num; i++) {
        pthread_create(&thread, NULL, tpool_worker, tm);
        pthread_detach(thread);
    }

    return tm;
}

/*
This function is to destroy the thread pool.
The argument is tpool_t structure, returns void.
*/
void tpool_destroy(tpool_t* tm) {
    tpool_work_t* work;
    tpool_work_t* work2;

    if(tm==NULL) {
        return;
    }

    pthread_mutex_lock(&(tm->work_mutex));
    work = tm->work_first;
    while(work!=NULL) {
        work2 = work->next;
        tpool_work_destroy(work);
        work = work2;
    }
    tm->work_first = NULL;
    tm->stop = true;
    pthread_cond_broadcast(&(tm->work_cond));
    pthread_mutex_unlock(&(tm->work_mutex));

    tpool_wait(tm);

    pthread_mutex_destroy(&(tm->work_mutex));
    pthread_cond_destroy(&(tm->work_cond));
    pthread_cond_destroy(&(tm->working_cond));

    free(tm);
}

/*
This function is to add work to tpool.
The arguments are tpool_t structure, thread_func_t function structure, and
*/
bool tpool_add_work(tpool_t* tm, thread_func_t func, void* arg) {
    tpool_work_t* work;

    if(tm==NULL) {
        return false;
    }

    work = tpool_work_create(func, arg);
    if(work==NULL) {
        return false;
    }

    pthread_mutex_lock(&(tm->work_mutex));
    if(tm->work_first == NULL) {
        tm->work_first = work;
        tm->work_last = tm->work_first;
    }
    else {
        tm->work_last->next = work;
        tm->work_last = work;
    }

    pthread_cond_broadcast(&(tm->work_cond));
    pthread_mutex_unlock(&(tm->work_mutex));

    return true;
}

/*
Blocking function that will return when there is no work.
The argument is tpool_t structure, returns void.
*/
void tpool_wait(tpool_t* tm) {
    if(tm==NULL) {
        return;
    }

    pthread_mutex_lock(&(tm->work_mutex));
    while(true) {
        if(tm->work_first != NULL || (!tm->stop && tm->working_cnt != 0) || (tm->stop && tm->thread_cnt != 0)) {
            pthread_cond_wait(&(tm->working_cond), &(tm->work_mutex));
        }
        else {
            break;
        }
    }
    pthread_mutex_unlock(&(tm->work_mutex));
}
