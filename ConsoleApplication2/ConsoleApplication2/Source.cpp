 #include <iostream>
 #include <mpi.h>
 using namespace std;

 const int SIZE = 30000; //dimensiunea maxima a grafului
 char G[SIZE][SIZE]; //matricea
 bool OK[SIZE]; 
 int D[SIZE]; //distanta
 int path[SIZE]; 
 const int infini = 9999; //numar mai mare ca orice distanta

 int nproc, id;
 MPI_Status status;
 int N;
 //int Read_G_Matrice() {
 //}
 void dijk(int s) {
	    int i, j;
	    int tmp, x;
	    int pair[2];
	    int tmp_pair[2];
	    for (i = id; i<N; i += nproc) {
		      D[i] = G[s][i];
		      OK[i] = false;
		      path[i] = s;		 
	 }
	    OK[s] = true;
	    path[s] = -1;
	    for (j = 1; j<N; j++) {		 
		  tmp = infini;
		      for (i = id; i<N; i += nproc) {
			        if (!OK[i] && D[i]<tmp) {
				  	x = i;
				  	tmp = D[i];				 
			 }			 
		 }		 
		  pair[0] = x;
		      pair[1] = tmp;		 
		  if (id != 0) { 
			        MPI_Send(pair, 2, MPI_INT, 0, 1, MPI_COMM_WORLD);			 
		 }
		 else { 
			        for (i = 1; i<nproc; ++i) {
				  	MPI_Recv(tmp_pair, 2, MPI_INT, i, 1, MPI_COMM_WORLD, &status);
				  	if (tmp_pair[1]<pair[1]) {
					  	  pair[0] = tmp_pair[0];
					  	  pair[1] = tmp_pair[1];					 
				 }				 
			 }			 
		 }
		     MPI_Bcast(pair, 2, MPI_INT, 0, MPI_COMM_WORLD);
		      x = pair[0];
		      D[x] = pair[1];
		      OK[x] = true;
		      for (i = id; i<N; i += nproc) {
			        if (!OK[i] && D[i]>D[x] + G[x][i]) {
				  	D[i] = D[x] + G[x][i];
				  	path[i] = x;				 
			 }			 
		 }		 
	 }	 
 }
int main(int argc, char** argv) {
	    double t1, t2;	 
	    MPI_Init(&argc, &argv);
	    MPI_Comm_size(MPI_COMM_WORLD, &nproc); 
	    MPI_Comm_rank(MPI_COMM_WORLD, &id);     
	 
	 // N = Read_G_Matrice();
	    //citeste matricea
	    //seteaza size ul
		 
		  if (id == 0) {
		      t1 = MPI_Wtime();
		 
	 }
		//apelul fct
		  dijk(200);
	  
	 
		  if (id == 0) {
		      t2 = MPI_Wtime();			 
			  cout << "  " << (t2 - t1) << endl;		 
	 }	 
	  MPI_Finalize();
	 
 }