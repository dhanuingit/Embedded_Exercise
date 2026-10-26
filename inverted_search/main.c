#include "header/IS.h"
int main(int argc, char *argv[]){
    #ifdef DEBUG
    extern FILE *debug_fptr;
    debug_fptr = fopen("Debug_log.txt", "w+");
    if (!debug_fptr)
    {
        return FAILURE;
    }
    #endif

    reset_data();
    l_list * head=NULL;
    l_list * update_head = NULL;

    if(validate_CLA(argc,argv,&head)==0){
        printf("Validate of input is done\n");
    }

    hash hash_arr[27];
    for(int i = 0 ; i < 27;i++){
        hash_arr[i].ind = i;
        hash_arr[i].link = NULL;
        //printf("ind - %d add - %p\n",hash_arr[i].ind,hash_arr[i].link);
    }

    int option;
    while(1){
        printf("\n--- choose an operation ---\n");
        printf("1.Create Database.\n2.Display database.\n3.Search Database.\n4.Save Database.\n5.Update Databse.\n6.Exit\n");
        scanf("%d",&option);
        switch(option){
            case 1://Creating database
            
            if(create_database(head,hash_arr)){
                printf("1\n");
                printf("\033[32m- INFO :\033[0m Failed to create Database\n");
            }
            else
            {
            printf("\033[32m-INFO :\033[0m Database created successfully\n");
            }
            break;
            
            case 2://Display Database
            if(display_database(hash_arr))
            {
                // printf("1\n");
                printf("\033[32m- INFO :\033[0m Failed to Display Database\n");
            }
        
            break;
            
            case 3://search Database
            if(search_database(hash_arr)){
                printf("\033[32m- INFO :\033[0m Failed to Search the data in the Database\n");
            }
            break;
            
            case 4://save Database
            if(save_database(&head,&update_head,hash_arr)){
                printf("\033[32m - INFO :\033[0m Failed to save the Database\n");
            }
            else
            {
                printf("\033[32m - INFO :\033[0m Database saved into the file Successfully!!!\n");
            }
            break;
            
            case 5:
            if(update_database(&head,&update_head,hash_arr)){
                printf("\033[32m - INFO :\033[0m Failed to update the Database\n");
            }
            else
            {
                printf("\033[32m - INFO :\033[0m Database updated Successfully!!!\n");
            }
            break;

            case 6:
            return 0;

            default:
            printf("Invalid option!!!\n");
            getchar();
        }
    }
    
return 0;
}
