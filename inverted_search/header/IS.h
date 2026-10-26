#ifndef IS_H
#define IS_H

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <sys/stat.h>

// #define DEBUG

#ifdef DEBUG
#define TRACE(fmt, ...) \
    fprintf(debug_fptr, "%s:%d:%s(): " fmt, __FILE__, __LINE__, __func__, ##__VA_ARGS__)
#else
#define TRACE(fmt, ...)
#endif  

#define MAGIC_PIN "#4IS_#4_DATABASE4#"
#define FAILURE 1
#define SUCCESS 0
#define DATA_NOT_FOUND -2
#define LIST_EMPTY -4
#define POSITION_NOT_FOUND -3

static unsigned int i ;
extern FILE * debug_fptr;

typedef struct s_node{  //linked list of inputed filename
    char * file_name;
    struct s_node * link;
}l_list;


typedef struct sub_node{    //filename and wordcount table
    char f_name[50];
    unsigned int word_count;
    struct sub_node * sublink;
}subnode;


typedef struct main_node{   //main node table
    char word[50];
    unsigned int file_count;
    subnode * sub_link;
    struct main_node * main_link; 
}mainnode;


typedef struct hashtable    //array of hashtable
{
    int ind;    
    mainnode * link;
}hash;


typedef struct s_file_data{
    char filename[50];
    char word[50];
}f_d;


int validate_CLA(int argc,char*argv[],l_list ** head);
int validate_FILE_LIST(int argc,char* file_name,l_list ** head);
unsigned char str_len(char * str);
void reset_data();


int create_database(l_list * head , hash * arr_hash);
int display_database(hash * arr_hash);
int search_database(hash * arr_hash);
int save_database(l_list ** head,l_list ** update_head,hash *arr_hash);
int update_database(l_list ** head, l_list ** update_head,hash * hash_arr);

int insert_at_last(l_list **head, char *file_name);
int delete_at_first(l_list ** head);
void filter_list(l_list ** head,char * temp_file);

void print_list(l_list *head);
int remove_duplicates( l_list **head );
int to_upper(char * str);
int get_index(char * word);



mainnode * mainnode_insert_after(hash * head,int data,f_d*file_data);
mainnode * mainnode_insert_at_first(hash * head,f_d *file_data);

int subnode_insert_after(mainnode ** head,int data,f_d*file_data);
int subnode_insert_at_first(mainnode ** head,f_d *file_data);
#endif
