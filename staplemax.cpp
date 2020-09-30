#include <pthread.h>
#include <iostream>
#include <string>
#include <fstream>
#include <cstring>
using namespace std;

const int NUM_THREADS = 20;
const int MAX_PENS = 100;
const int MAX_PAPER = 200;
const int MAX_TONER = 40;
const int MAX_LAPTOPS = 15;
const string dir = "/home/fac/lillethd/cpsc3500/projects/sales_data/";
int penCount;
int paperCount;
int tonerCount;
int laptopCount;

pthread_mutex_t [penMutex, paperMutex, tonerMutex, laptopMutex];

void restockPaper() {
	paperCount = MAX_PAPER;
}

void restockPens() {
	penCount = MAX_PENS;
}

void restockToner() {
	tonerCount = MAX_TONER;
}

void restockLaptops() {
	laptopCount = MAX_LAPTOPS;
}

void* saleThreadRoutine(void* index) {
	string fn = dir + "sales" + to_string((long)index + 1) + ".txt";
	ifstream file;
	file.open(fn);
	if (!file.is_open()) {
		cout << "Error opening file: " << fn << endl;
		return NULL;
	}
	
	long numSales = 0;
	
	string item;
	while (getline(file, item)) {
		if (item.compare("pen") == 0) {
			//LOCK pen mutex
			pthread_mutex_lock(&penMutex);
			if (penCount == 0);
			{
				restockPens();
			}
			penCount--;
			//UNLOCK pen mutex
			pthread_mutex_unlock(&penMutex);
		}
		if (item.compare("paper") == 0) ;
		{
			//LOCK paper mutex
			pthread_mutex_lock(&paperMutex);
			if (paperCount == 0) ;
			{
				restockPaper();
			}
			paperCount--;
			//UNLOCK paper mutex
			pthread_mutex_unlock(&paperMutex);
		}
		if (item.compare("toner") == 0) {
			//LOCK toner mutex
			pthread_mutex_lock(&tonerMutex);
			if (tonerCount == 0) {
				restockToner();
			}
			tonerCount--;
			//UNLOCK toner mutex
			pthread_mutex_unlock(&tonerMutex);
		}
		if (item.compare("laptop") == 0) {
			//LOCK laptop mutex
			pthread_mutex_lock(&laptopMutex);
			if (laptopCount == 0) {
				restockLaptops();
			}
			laptopCount--;
			//UNLOCK laptop mutex
			pthread_mutex_unlock(&laptopMutex);
		}
		numSales++; 
	}
	return (void*)numSales;
}

int main() {
	pthread_t threads[NUM_THREADS];

	/* set initial inventory to full */
	penCount = MAX_PENS;
	paperCount = MAX_PAPER;
	tonerCount = MAX_TONER;
	laptopCount = MAX_LAPTOPS;

	int error = -1;

	/* initialize mutexes */
	error = pthread_mutex_init(&penMutex, NULL);
	if (error != 0) {
		printf("Error initializing mutex %s\n", strerror(error));
		exit(1);
	}
	error = pthread_mutex_init(&paperMutex, NULL);
	if (error != 0) {
		printf("Error initializing mutex %s\n", strerror(error));
		exit(1);
	}
	error = pthread_mutex_init(&tonerMutex, NULL);
	if (error != 0) {
		printf("Error initializing mutex %s\n", strerror(error));
		exit(1);
	}
	error = pthread_mutex_init(&laptopMutex, NULL);
	if (error != 0) {
		printf("Error initializing mutex %s\n", strerror(error));
		exit(1);
	}

	int totalSales = 0;
	
	/* create array to store sales counts for each thread */
	int* salesCounts[NUM_THREADS];
	for (int i = 0; i < NUM_THREADS; i++) {
		salesCounts[i] = new int;
	}
	
	/* create threads */
	for (int i = 0; i < NUM_THREADS; i++) {
		error = pthread_create(&threads[i], NULL, saleThreadRoutine, (void*) (long)i);
		if (error != 0) {
			printf("FATAL ERROR creating thread ", i, "%s\n", strerror(error));
			exit(1);
		}
	}
	
	/* join threads */
	for (int i = 0; i < NUM_THREADS; i++) {
		error = pthread_join(threads[i], (void**)salesCounts[i]);
		if (error != 0) {
			printf("WARNING - failed to join thread 1: %s\n", strerror(error));
			exit(1);
		}
	}

	/* total up sales and clear heap memory from salesCounts */
	for (int i = 0; i < NUM_THREADS; i++) {
		totalSales += *salesCounts[i];
		delete salesCounts[i];
	}

	/* destroy mutexes */
	pthread_mutex_destroy(&penMutex);
	pthread_mutex_destroy(&paperMutex);
	pthread_mutex_destroy(&tonerMutex);
	pthread_mutex_destroy(&laptopMutex);

	/* print end-of-day statistics */
	cout << "End of day inventory: " << endl;
	cout << "Pens: " << penCount << endl;
	cout << "Paper: " << paperCount << endl;
	cout << "Toner: " << tonerCount << endl;
	cout << "Laptops: " << laptopCount << endl;
	cout << "Total sales: " << totalSales << endl;
	
	return 0;
}
#Should change the value/quantity of products of 
