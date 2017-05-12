#include <stdio.h>
   #include <mpi.h>
   
   #define max_rows 100000
   #define send_data_tag 2001
   #define return_data_tag 2002

   int array[max_rows];
   int array2[max_rows];
   
   main(int argc, char **argv) 
   {
      long int sum, partial_sum;
      MPI_Status status;
      int my_id, root_process, ierr, i, num_rows, num_procs,
         an_id, num_rows_to_receive, avg_rows_per_process, 
         sender, num_rows_received, start_row, end_row, num_rows_to_send;

      //initialisation de MPI    
      ierr = MPI_Init(&argc, &argv);
      
      root_process = 0;
      
      //pour recuperer l'id du process et le nombre de process total    
      ierr = MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
      ierr = MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

      //instruction executé seulement par le root process id=0   
      if(my_id == root_process) {

         printf("please enter the number of numbers to sum: ");
         scanf("%i", &num_rows);
         
         //si jamais le nombre demandé est supérieur au nombre max de ligne on coupe le programme  
         if(num_rows > max_rows) {
            printf("Too many numbers.\n");
            exit(1);
         }

         //calcule du nombre moyen de ligne par process 
         avg_rows_per_process = num_rows / num_procs;

         //intialisation d'un des tableaux 
         for(i = 0; i < num_rows; i++) {
            array[i] = i + 1;
         }

         //boucle sur tout les process sauf le root_process ( 0 ) pour distribuer à chaque process une portion du tableau à calculer 
         for(an_id = 1; an_id < num_procs; an_id++) {
            start_row = an_id*avg_rows_per_process + 1;
            end_row   = (an_id + 1)*avg_rows_per_process;

            if((num_rows - end_row) < avg_rows_per_process)
               end_row = num_rows - 1;

            num_rows_to_send = end_row - start_row + 1;

            ierr = MPI_Send( &num_rows_to_send, 1 , MPI_INT,
                  an_id, send_data_tag, MPI_COMM_WORLD);

            ierr = MPI_Send( &array[start_row], num_rows_to_send, MPI_INT,
                  an_id, send_data_tag, MPI_COMM_WORLD);
         }

         //calcule de la sum du segment assigner au root process 
        
         sum = 0;
         for(i = 0; i < avg_rows_per_process + 1; i++) {
            sum += array[i];   
         } 

         printf("sum %i calculated by root process\n", sum);

         //on récupère les sommes calculées par les autres process 

         for(an_id = 1; an_id < num_procs; an_id++) {
            
            ierr = MPI_Recv( &partial_sum, 1, MPI_LONG, MPI_ANY_SOURCE,
                  return_data_tag, MPI_COMM_WORLD, &status);
  
            sender = status.MPI_SOURCE;

            printf("Partial sum %i returned from process %i\n", partial_sum, sender);
     
            sum += partial_sum;
         }

         printf("The grand total is: %i\n", sum);
      } 

      //instructions exécutées par tout les process différents du root process 

      else {

         //recois le segment assigné

         ierr = MPI_Recv( &num_rows_to_receive, 1, MPI_INT, 
               root_process, send_data_tag, MPI_COMM_WORLD, &status);
          
         ierr = MPI_Recv( array2, num_rows_to_receive, MPI_INT, 
               root_process, send_data_tag, MPI_COMM_WORLD, &status);

         num_rows_received = num_rows_to_receive;

         //calcule de la somme du segment reçus 

         partial_sum = 0;
         for(i = 0; i < num_rows_received; i++) {
            partial_sum += array2[i];
         }

         //envoie de la somme calculé au root process 

         ierr = MPI_Send( &partial_sum, 1, MPI_LONG, root_process, 
               return_data_tag, MPI_COMM_WORLD);
      }
      //libération des ressources utilisées par MPI 
      ierr = MPI_Finalize();
   }


