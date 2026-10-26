#include"../header/IS.h"

int validate_CLA(int argc,char*argv[],l_list ** head){
    if(argc<2){
        printf("\033[31m Error :\033[0m Minimum arguments not passed\n");
        return 1;
    }
    FILE * fptr = NULL;
    for(int i=1;i<argc;i++){
        int file_flag = 0;
        if(!(strstr(argv[i],".txt"))){
            printf("\033[33m Warning :\033[0m Invalid File - %s\n",argv[i]);
            file_flag=1;
        }
        else if(fopen(argv[i],"r")==NULL){
            printf("\033[33m Warning :\033[0m %s file doesn't exist in directory\n",argv[i]);
            file_flag = 1;
        }
        
        
        
        //printf("position - %ld\n",ftell(fptr));
        else {
            fptr=fopen(argv[i],"r");
        fseek(fptr,0,SEEK_END);
            if((ftell(fptr)==0)){
            printf("\033[33m Warning :\033[0m %s file content is empty\n",argv[i]);
            file_flag = 1;
        }
    }
        if(!file_flag){
             insert_at_last(head,argv[i]);
        }
        
        if(fptr != NULL)
        fclose(fptr);
    }
    remove_duplicates(head);
    printf("Linked list ==> ");
    print_list(*head);
    if(*head == NULL){
        return FAILURE;
    }
    return SUCCESS;
}


int validate_FILE_LIST(int argc,char* file_name,l_list ** head){
    FILE * FILE_ptr = fopen(file_name,"r");
    if(FILE_ptr == NULL){
        printf("Failed in open the file\n");
        perror("fopen");
        return FAILURE;
    }
    char argv[50] = {0};
    FILE * fptr = NULL;
    for(int i=0;i<=argc;i++){
        int file_flag = 0;
        if(fscanf(FILE_ptr,"%s",argv)==1){
            
        
        if(!(strstr(argv,".txt"))){
            printf("\033[33m Warning :\033[0m Invalid File - %s\n",argv);
            file_flag=1;
        }
        else if(fopen(argv,"r")==NULL){
            printf("\033[33m Warning :\033[0m %s file doesn't exist in directory\n",argv);
            file_flag = 1;
        }
        
        
        
        //printf("position - %ld\n",ftell(fptr));
        else {
            fptr=fopen(argv,"r");
        fseek(fptr,0,SEEK_END);
            if((ftell(fptr)==0)){
            printf("\033[33m Warning :\033[0m %s file content is empty\n",argv);
            file_flag = 1;
        }
    }
        if(!file_flag){
             insert_at_last(head,argv);
        }
        
        
    }
    else{
        printf("failed to read the sting\n");
        return FAILURE;
    }
    if(fptr != NULL){
        fclose(fptr);
        fptr = NULL;
    }
    }
    remove_duplicates(head);
    printf("Linked list ==> ");
    print_list(*head);
    if(*head == NULL){
        printf("No files in the linked list\n");
        return FAILURE;
    }
    if(FILE_ptr!=NULL){
        fclose(FILE_ptr);
    }
    return SUCCESS;
}



unsigned char str_len(char * str){
    unsigned char i = 0;
    for(i = 0;str[i]!='\0';i++);
    return i;
} 



int to_upper(char * str){
    int i = 0;
    while(str[i]!='\0'){
        // printf("1\n");
        if(str[i]>='a'&&str[i]<='z'){
           str[i] = str[i]-'a'+'A';
        }
        i++;
    }
    return 1;
}

int get_index(char * word){
        //to get the index of the word...
    if (word[0] >= 'A' && word[0] <= 'Z')
    {
        return word[0] - 'A';
    }
    else if (word[0] >= 'a' && word[0] <= 'z')
    {
        return word[0] - 'a';
    }
    else
    {
        return 26;
    }
}

void reset_data(){
    chmod("local_data.txt",S_IRUSR	| S_IWUSR | S_IXUSR	|  S_IRGRP	|  S_IWGRP	|   S_IXGRP	|   S_IROTH	|   S_IWOTH |   S_IXOTH);
    FILE * fptr = fopen("local_data.txt","w+");//To erase the contents of the local_data file
    if(fptr!=NULL){
        fclose(fptr);
        fptr = NULL;
    }
    chmod("local_data.txt",S_IRUSR|S_IRGRP|S_IROTH);
}