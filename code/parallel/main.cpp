/****************************************************************
 * File: main.cpp
 * Description: Main file for Indexing Product Reviews.
 *		This file used Inverted Index data structure.
 *              This file used InvertedIndex class.
 *		This file used <pthread.h> for multi-thread programming.
 *
 * System: Linux
 * Status: Parallel Program, Version 1.0
 * Lauguage: C++
 * Author: Yueying Cui
 * Date: 01/20/2013
******************************************************************/
#include <iostream>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include "inverted_index.h"
// Number of threads
#define THREAD_NUM 10
// pthread initial
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
// InvertedIndex object
InvertedIndex invidx;
// all files' name
std::queue<std::string> files;
// counter of files
int counter = 0;

/*
 * Function:	run
 * Description: thread task function
 *
 * Parameter:	
 *		* id: thread_id
 * Return:	
*/
void * run(void * id){
	std::string file;

	// debug info
	/*while(true) {
		printf("%x\n", id);
		sleep(10);
	}
	printf("%x\n", id);
		sleep(10);*/

	while(1){

		if(!files.empty()){
			pthread_mutex_lock(&mutex);
			file = files.front();
			files.pop();
			pthread_mutex_unlock(&mutex);
			invidx.parse_file(file);
			
			// debug info
			//counter++;
			//std::cout << "Thread id = " << *(int *)id << std::endl;
			//std::cout << "counter = " << counter << std::endl;

			//sleep(10);
		}
		else{			
			//sleep(10);
			pthread_exit(0); 
		}		
	}		
}

/*
 * Function:	main
 * Description: main function
 *
 * Parameter:	
 * Return:	
*/
int main(){
	// timer begin
	time_t seconds_begin, seconds_end;
	seconds_begin = time (NULL);

	// group files
	invidx.parse_directory("./download", files);

	// multiple thread
	pthread_t tid[THREAD_NUM];
	int * thread_id = new int[THREAD_NUM];
	int i;
	for(i = 0; i < THREAD_NUM; i++){
		thread_id[i] = i;
	}

	for(i = 0; i < THREAD_NUM; i++){
		pthread_create(&tid[i], NULL, &run, &thread_id[i]);
	}

	for(i = 0; i < THREAD_NUM; i++)
		pthread_join(tid[i], NULL);

	free(thread_id);

	// timer end	
	seconds_end = time (NULL);
	std::cout << "timer = " << seconds_end - seconds_begin << std::endl;

	// search
	std::string searchword;
	std::string ans;
	while(1){
		std::cout << "Please input a word for search: ";
		std::cin >> searchword;

		// debug info
		//std::cout << "searchword = " << searchword << std::endl;

		invidx.search(searchword);

		std::cout << "Do you want to search another word (Y/N): ";
		std::cin >> ans;
		if(ans == "N")
			break;
	}

	return 0;
}
