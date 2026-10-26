#include "../header/IS.h"

int insert_at_last(l_list **head, char *file_name)
{
    l_list *new = (l_list*)malloc(sizeof(l_list));
    if (new == NULL)
    {
        printf("Error : Unable to allocate memory\n");
        return FAILURE;
    }
    new->file_name = malloc((sizeof(strlen(file_name)) + 1));
    if (new->file_name == NULL)
    {
        return FAILURE;
    }
    strcpy(new->file_name, file_name);
    new->link = NULL;

    if (*head == NULL)
    {
        *head = new;
        return SUCCESS;
    }
    l_list *temp = *head;
    while (temp->link != NULL)
    {
        temp = temp->link;
    }
    temp->link = new;
    return SUCCESS;
}

int delete_at_first(l_list ** head){
if((*head)==NULL){
return FAILURE;
}
l_list * temp = *head;
*head = (*head) -> link;
free(temp);
return SUCCESS;
}

void print_list(l_list *head)
{
    if (head == NULL)
    {
        printf("INFO : List is empty\n");
    }
    else
    {
        while (head)
        {
            printf("%s -> ", head->file_name);
            head = head->link;
        }

        printf("NULL\n");
    }
}

int remove_duplicates(l_list **head)
{
    if (*head == NULL)
    {
        return FAILURE;
    }

    // if(temp->link==NULL){
    //     return SUCCESS;
    // }

    l_list *fast = (*head)->link, *slow = *head, *temp = *head;

    while (fast != NULL)
    {
        if (!strcmp(fast->file_name, slow->file_name))
        {
            printf("\033[33m Warning :\033[0m %s file is being passed multiple times\n", slow->file_name);
            temp->link = fast->link;
            free(fast);
            fast = temp->link;
            ;
        }
        else
        {
            temp = fast;
            fast = fast->link;
        }
        if (fast == NULL)
        {
            slow = slow->link;
            if (slow == NULL)
            {
                return SUCCESS;
            }
            fast = slow->link;
            temp = slow;
        }
    }
}


void filter_list(l_list ** head,char * temp_file){

    l_list * temp_cur = *head;
    l_list * temp_prev = NULL;
    while(temp_cur!=NULL){
        if(!strcmp(temp_cur->file_name,temp_file)){
            printf("The %s file's database/file's name Already exists in the existing database\nThis Files contents will be skipped\n",temp_cur->file_name);
            if(temp_prev == NULL){
                l_list * temp = *head;
                *head = temp_cur->link;
                free(temp);
                temp_cur = *head;
                
            }
            else{
                 temp_prev->link= temp_cur->link;
                free(temp_cur);
                temp_cur = temp_prev->link;
                
            }
        }
else{

        temp_prev = temp_cur;
        temp_cur = temp_cur->link;
    }
    }
}



mainnode * mainnode_insert_after(hash * head,int data,f_d*file_data){
if(head->link == NULL){
    return NULL;
}
mainnode * temp = head -> link;
for(int i = 0;i<data-1;i++){
    temp = temp -> main_link;
}

    mainnode * new = (mainnode*)malloc(sizeof(mainnode));
    if(!new){
        printf("Memory allocation failed at creating mainnode for the first place\n");
        return NULL;
    }
    strcpy(new -> word,file_data->word);
    new -> file_count = 0;
    new -> main_link = temp -> main_link;
    new -> sub_link = NULL;
    // subnode_insert_at_first(&new,file_data);
    temp -> main_link = new;
    return new;

}


mainnode * mainnode_insert_at_first(hash * head,f_d *file_data){
    mainnode * new = (mainnode*)malloc(sizeof(mainnode));
    if(!new){
        printf("Memory allocation failed at creating mainnode for the first place\n");
        return NULL;
    }
    strcpy(new -> word,file_data->word);
    new -> file_count = 0;
    new -> sub_link = NULL;
    new ->main_link = head -> link;
    // subnode_insert_at_first(&new,file_data);
    head -> link = new;
    return new;
}

int subnode_insert_after(mainnode ** head,int data,f_d*file_data){
        // printf("traversal value in subnode_insert_after= %d\n",data);
    if((*head) == NULL){
    return FAILURE;
}
subnode * temp = (*head)->sub_link;
for(int i = 0;i<data-1;i++){
    temp = temp -> sublink;
    // printf("debug - index = %d\n",i);
}

    subnode * new = (subnode*)malloc(sizeof(subnode));
    if(!new){
        printf("Memory allocation failed at creating mainnode for the first place\n");
        return FAILURE;
    }
    (*head)->file_count++;
    strcpy(new -> f_name,file_data->filename);
    new -> word_count = 1;
    new -> sublink = temp -> sublink;
    temp -> sublink = new;
    return SUCCESS;
}



int subnode_insert_at_first(mainnode ** head,f_d *file_data){
    subnode * new = (subnode*)malloc(sizeof(subnode));
    if(!new){
        printf("Memory allocation failed at creating subnode for the first place\n");
        return FAILURE;
    }
    (*head)->file_count++;
    strcpy(new -> f_name,file_data->filename);
    new -> word_count = 1;
    new -> sublink = (*head)->sub_link;
    (*head) -> sub_link = new;
    return SUCCESS;
}



