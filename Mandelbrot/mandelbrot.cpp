#include <complex>
#include <iostream>
#include <pthread.h>

using namespace std;

struct thread_data {
	int max_row;
	int max_column;
	int max_n;
	int n_threads;
	int t_id;
	char ** mat;
};

void *mandel_thread(void * a) {
	struct thread_data *data = (struct thread_data *) a;
	int max_row = data->max_row;
	int max_column = data->max_column;
	int max_n = data->max_n;
	int n_threads = data->n_threads;
	int t_id = data->t_id;
	char ** mat = data->mat;

	int inicio = t_id;
	int step = n_threads;

	//abre os dados ..
	 for(int r = inicio; r < max_row; r+= step){ //escalonamento
         for(int c = 0; c < max_column; ++c){
             //para cada celula da matriz
             complex<float> z;
                         int n = 0;
                         while(abs(z) < 2 && ++n < max_n)
                                 z = pow(z, 2) + decltype(z)(
                                         (float)c * 2 / max_column - 1.5,
                                         (float)r * 2 / max_row - 1
                                 );
                         mat[r][c]=(n == max_n ? '#' : '.');
                 }
         }
	 pthread_exit(NULL);
}


int main(int argc, char * argv[]){
	int max_row, max_column, max_n, n_threads;
	max_row = 2300;
	max_column = 790;
	max_n = 4800;
	n_threads = (argc > 1) ? std::stoi(argv[1]) : 1;
	
	//cin >> max_row;
	//qcin >> max_column;
	//cin >> max_n;
	//cin >> n_threads;
	/// n_threads = 1;

	pthread_t threads[n_threads];

	char **mat = (char**)malloc(sizeof(char*)*max_row);
	char *block = (char *)malloc(sizeof(char)*max_row*max_column);

	for (int i=0; i<max_row;i++) {
		mat[i]=&block[i * max_column];
//		mat[i]=(char*)malloc(sizeof(char)*max_column);
	}
	// alimentar a memoria

	// struct thread_data  dados;
	
	struct thread_data dados[n_threads];
	// criar as threads
	int rc;

	for(int i = 0; i < n_threads;i++){
		dados[i].max_column = max_column;
		dados[i].max_row = max_row;
		dados[i].max_n = max_n;
		dados[i].n_threads = n_threads;
		dados[i].mat = mat;
		dados[i].t_id = i;

		rc = pthread_create(&threads[i], NULL, mandel_thread, (void *)&dados[i]);
		
		if (rc) {
			printf("ERROR; return code from pthread_create() is %d\n", rc);
			exit(-1);
		}
	}
	
	for(int i =0; i < n_threads; i++){
		pthread_join(threads[i], NULL);
	}


	//for(int r = 0; r < max_row; ++r){
	//	for(int c = 0; c < max_column; ++c)
	// 		std::cout << mat[r][c];
	// 	cout << '\n';
	//}
	free(mat);
		
	pthread_exit(NULL);
}


