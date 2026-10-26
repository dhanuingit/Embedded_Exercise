#include "../header/IS.h"

int create_database(l_list *file_head, hash *arr_hash)
{
    int exit_in = 0;

    int data_base_flag = 0;

    f_d file_data;
    l_list *temp = file_head;
    int traversal_count = 0;
    while (temp != NULL)
    {
        data_base_flag = 0;
        if (exit_in++ > 5000)
        {
            return 1;
        }
        strcpy(file_data.filename, temp->file_name);
        FILE *fptr = NULL;
        if (!(fptr = fopen(file_data.filename, "r")))
        {
            printf("Error : unable to open the file.\n Terminating the execution...\n");
            return FAILURE;
        }
        while (fscanf(fptr, "%s", file_data.word) == 1)
        {
            // if (exit_in++ > 5000)
            // {
            //     return 1;
            // }
            // printf("%s",file_data.word);
            // exit(0);
            if (strcmp(file_data.word, MAGIC_PIN) == 0)
            { // its stored IS Database...
                l_list *temp = file_head;
                char *file_list = NULL;
                size_t len = 0;
                getc(fptr);
                getline(&file_list, &len, fptr);
                // printf("%s",file_list);
                char *temp_file = NULL;
                strtok(file_list, ":");
                while ((temp_file = strtok(NULL, ":")) != NULL)
                {
                    if (strcmp(temp_file, "#") != 0)
                        temp = file_head;
                    while (temp != NULL)
                    {
                        if (strcmp(temp->file_name, temp_file) == 0)
                        {
                            printf("Warning: The Filename already exists in the database\n Hence the %s wont be added to the database\n", temp->file_name);
                            return FAILURE;
                        }
                        temp = temp->link;
                    }
                    // printf("%s\n",temp_file);
                }
                data_base_flag = 1;
                free(file_list);
            }

            if (data_base_flag)
            {
                mainnode *mainnode_temp = NULL;
                subnode *subnode_temp = NULL;
                static int b_ind = -1;
                char *line_data = NULL;
                size_t len = 0;
                while (getline(&line_data, &len, fptr) != -1)
                {
                    // printf("%s",line_data);
                    char *data = NULL;
                    data = strtok(line_data, ":");

                    if (strcmp(data, "#") == 0)
                    {
                        data = strtok(NULL, ":");
                        // printf("data - %s\n",data);
                        if (sscanf(data, "%d", &b_ind) == 1)
                        {
                            // printf("b_ind - %d\n",b_ind);
                        }
                    }
                    else
                    { // data line
                        if (arr_hash[b_ind].link == NULL)
                        {
                            arr_hash[b_ind].link = mainnode_temp = (mainnode *)malloc(sizeof(mainnode));
                            if (arr_hash[b_ind].link == NULL)
                            {
                                printf("Error : Failed to allocate mainnode memory\n");
                                return FAILURE;
                            }
                        }
                        else
                        {
                            mainnode_temp = arr_hash[b_ind].link;
                            while (mainnode_temp->main_link != NULL)
                            {
                                mainnode_temp = mainnode_temp->main_link;
                            }
                            mainnode_temp->main_link = (mainnode *)malloc(sizeof(mainnode));

                            mainnode_temp = mainnode_temp->main_link;
                        }

                        strcpy(mainnode_temp->word, data);
                        if (mainnode_temp->word != NULL)
                        {
                            //  printf("%s\n",mainnode_temp->word);
                        }
                        int i = 0;
                        while ((data = strtok(NULL, ":")) != NULL && strcmp(data, "#\n") != 0)
                        {
                            // printf("data - %s\n",data);
                            if (i++ == 0)
                            {
                                if (sscanf(data, "%d", &(mainnode_temp->file_count)) != 1)
                                {
                                    printf("Error: Failed to assign value to the mainnodes filecount\n");
                                }
                                // printf("file count - %d\n",mainnode_temp->file_count);
                                continue;
                            }
                            if (mainnode_temp->sub_link == NULL)
                            {
                                mainnode_temp->sub_link = subnode_temp = (subnode *)malloc(sizeof(subnode));
                            }
                            else
                            {
                                while (subnode_temp->sublink != NULL)
                                    subnode_temp = subnode_temp->sublink;
                                subnode_temp->sublink = (subnode *)malloc(sizeof(subnode));
                                subnode_temp = subnode_temp->sublink;
                            }
                            strcpy(subnode_temp->f_name, data);
                            if (subnode_temp->f_name != NULL)
                            {
                                // printf("Subnode fname - %s\n",subnode_temp->f_name);
                            }
                            data = strtok(NULL, ":");
                            // printf("data - %s\n",data);
                            sscanf(data, "%d", &(subnode_temp->word_count));
                            // printf("Subnode wordcount - %d\n",subnode_temp->word_count);
                        }
                    }
                }
            }
            else
            {
                TRACE("\n\n%s\n\n", file_data.word);
                int index = get_index(file_data.word);
                mainnode *subnode_head = NULL;
                traversal_count = 0;
                if (arr_hash[index].link == NULL)
                {
                    TRACE("\n");
                    subnode_head = mainnode_insert_at_first(arr_hash + index, &file_data);
                }
                else
                {
                    TRACE("\n");
                    mainnode *mainnode_temp = arr_hash[index].link;

                    char word[50], stored_word[50]; // word -> to hold upper case of the word data...
                                                    // stored_word  -> to hold the upper case of the word data stored in the index database...

                    while (mainnode_temp != NULL)
                    {
                        if (exit_in++ > 5000)
                        {
                            return 1;
                        }

                        TRACE("\n");

                        strcpy(word, file_data.word);
                        to_upper(word);

                        strcpy(stored_word, mainnode_temp->word);
                        to_upper(stored_word);
                        char str_comp_ret_val = strcmp(word, stored_word);

                        if (str_comp_ret_val == 0)
                        {
                            if (strcmp(file_data.word, mainnode_temp->word) == 0)
                            {
                                TRACE("\n");
                                subnode_head = mainnode_temp;
                                break;
                            }
                            else if (strcmp(file_data.word, mainnode_temp->word) < 0)
                            {
                                TRACE("\n");
                                if (traversal_count == 0)
                                {
                                    subnode_head = mainnode_insert_at_first(arr_hash + index, &file_data);
                                }
                                else
                                {
                                    subnode_head = mainnode_insert_after(arr_hash + index, traversal_count, &file_data);
                                }
                                break;
                            }
                        }
                        else if (str_comp_ret_val < 0)
                        {
                            TRACE("\n");
                            TRACE("Traversal count = %d\n", traversal_count);
                            if (traversal_count == 0)
                            {
                                subnode_head = mainnode_insert_at_first(arr_hash + index, &file_data);
                            }
                            else
                            {
                                subnode_head = mainnode_insert_after(arr_hash + index, traversal_count, &file_data);
                            }
                            break;
                        }
                        traversal_count++;
                        mainnode_temp = mainnode_temp->main_link;
                    }
                    if (mainnode_temp == NULL)
                    {
                        TRACE("\n");
                        TRACE("traversal_count = %d\n", traversal_count);
                        subnode_head = mainnode_insert_after(arr_hash + index, traversal_count, &file_data);
                    }
                }

                if (subnode_head != NULL)
                {
                    TRACE("\n");
                    if (subnode_head->sub_link == NULL)
                    {
                        TRACE("\n");
                        subnode_insert_at_first(&subnode_head, &file_data);
                    }
                    else
                    {
                        TRACE("\n");
                        traversal_count = 0;
                        char filename[50], stored_filename[50];
                        subnode *subnode_temp = subnode_head->sub_link;
                        while (subnode_temp != NULL)
                        {
                            if (exit_in++ > 5000)
                            {
                                return 1;
                            }
                            TRACE("\n");

                            strcpy(filename, file_data.filename);
                            to_upper(filename);
                            strcpy(stored_filename, subnode_temp->f_name);
                            to_upper(stored_filename);
                            char str_comp_ret_val = strcmp(filename, stored_filename);
                            if (str_comp_ret_val == 0)
                            {
                                if (strcmp(file_data.filename, subnode_temp->f_name) == 0)
                                {
                                    subnode_temp->word_count++;
                                    TRACE("\n");
                                    break;
                                }
                                else if (strcmp(file_data.filename, subnode_temp->f_name) < 0)
                                {
                                    TRACE("\n");

                                    if (traversal_count == 0)
                                    {
                                        subnode_insert_at_first(&subnode_head, &file_data);
                                    }
                                    else
                                    {
                                        subnode_insert_after(&subnode_head, traversal_count, &file_data);
                                    }
                                    break;
                                }
                            }
                            else if (str_comp_ret_val < 0)
                            {
                                TRACE("\n");
                                if (traversal_count == 0)
                                {
                                    subnode_insert_at_first(&subnode_head, &file_data);
                                }
                                else
                                {
                                    subnode_insert_after(&subnode_head, traversal_count, &file_data);
                                }
                                break;
                            }
                            traversal_count++;
                            subnode_temp = subnode_temp->sublink;
                        }
                        if (subnode_temp == NULL)
                        {
                            TRACE("\n");
                            if (subnode_insert_after(&subnode_head, traversal_count, &file_data))
                            {
                                printf("Error while updating the subnode in line 93\n");
                                return FAILURE;
                            }
                        }
                    }
                }
            }
        }

        fclose(fptr);
        temp = temp->link;
    }
    fflush(debug_fptr);
    return SUCCESS;
}

int display_database(hash *arr_hash)
{
    char flag = 0;
    printf("\n[index]\t[word\t][file_count] File:[File_count][word_count]\n\n");
    for (int ind = 0; ind < 27; ind++)
    {
        if (arr_hash[ind].link != NULL)
        {
            flag = 1;
            mainnode *mainnode_temp = arr_hash[ind].link;
            subnode *subnode_temp = NULL;

            while (mainnode_temp != NULL)
            {
                subnode_temp = mainnode_temp->sub_link;
                printf("[  %-5d]   [ %-10s ]  %-3d  file/s: File : ", ind, mainnode_temp->word, mainnode_temp->file_count);

                while (subnode_temp != NULL)
                {
                    printf("%-10s %-5d ", subnode_temp->f_name, subnode_temp->word_count);
                    subnode_temp = subnode_temp->sublink;
                }

                printf("\n");
                mainnode_temp = mainnode_temp->main_link;
            }
        }
    }
    if (!flag)
    {
        printf("No data to display\n");
    }

    return SUCCESS;
}

int search_database(hash *arr_hash)
{
    char word[50];
    char word_upper[50];
    char word_present = 0;
    printf("Enter the word you want to search : ");
    getchar();
    scanf("%s", word);

    strcpy(word_upper, word);
    to_upper(word_upper);
    int index = get_index(word);

    if (arr_hash[index].link == NULL)
    {
        printf("cheackpoint 1\n");
        printf("The entered word doesnt exist in the database\n");
        return SUCCESS;
    }
    else
    {
        char stored_word[50];

        mainnode *temp = arr_hash[index].link;
        while (temp != NULL)
        {
            strcpy(stored_word, temp->word);
            to_upper(stored_word);
            // printf("up_word = %s    act_word - %s\n",word_upper,word);
            if (strcmp(word_upper, stored_word) < 0)
            {
                if (!word_present)
                {
                    printf("cheackpoint 2\n");
                    printf("The entered word doesnt exist in the database\n");
                    return SUCCESS;
                }
            }
            else if (strcmp(word_upper, stored_word) == 0)
            {
                if (strcmp(word, temp->word) == 0)
                {
                    printf("The word \"%s\" is present in \"%d\" File/s\n", temp->word, temp->file_count);
                    subnode *subnode_temp = NULL;
                    for (int i = 0; i < temp->file_count; i++)
                    {
                        printf("In file : %s - %d time/s\n", temp->sub_link->f_name, temp->sub_link->word_count);
                        if (subnode_temp != NULL)
                            subnode_temp = subnode_temp->sublink;
                    }
                }
                else
                {
                    printf("The word \"%s\" is present as \"%s\"  in \"%d\" File/s\n", word, temp->word, temp->file_count);
                    subnode *subnode_temp = NULL;
                    for (int i = 0; i < temp->file_count; i++)
                    {
                        printf("In file : %s - %d time/s\n", temp->sub_link->f_name, temp->sub_link->word_count);
                        if (subnode_temp != NULL)
                            subnode_temp = subnode_temp->sublink;
                    }
                }
                word_present = 1;
            }
            if (temp != NULL)
            {
                temp = temp->main_link;
            }
        }
    }
    if (!word_present)
    {
        printf("cheackpoint 3\n");
        printf("The entered word doesnt exist in the database\n");
    }
    return SUCCESS;
}

int save_database(l_list **head, l_list **update_head, hash *arr_hash)
{
    char file_name[50] = {0};

    chmod("local_data.txt", S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP | S_IXGRP | S_IROTH | S_IWOTH | S_IXOTH);

    FILE *local_file_fptr = fopen("local_data.txt", "w+");
    if (local_file_fptr == NULL)
    {
        printf("Failed to open \"local_data.txt\" file\n");
        perror("fopen");
        return FAILURE;
    }
label1:
    getchar();
    while (1)
    {
        printf("Enter the Filename with .txt extension\n");
        scanf("%s", file_name);

        if (strstr(file_name, ".txt") == NULL)
        {
            printf("The enter File is not in .txt format");
        }
        else
        {
            break;
        }
    }

    chmod(file_name, S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP | S_IXGRP | S_IROTH | S_IWOTH | S_IXOTH);
    char temp_choice;
    FILE *fptr = NULL;
    while (1)
    {
        fptr = fopen(file_name, "r");
        if (fptr != NULL)
        {
            printf("Warning : The File with name %s already exist and its content will be written.\n1.Do you want to overwrite the content - ENTER(Y/y)\n2.If you willing to re-enter a new filename ENTER(R/r)\n3.If you are willing to Abort the save operation - ENTER(A/a)\n", file_name);
            getchar();
            scanf("%c", &temp_choice);
            switch (temp_choice)
            {
            case 'Y':
            case 'y':
                fclose(fptr);
                fptr = NULL;
                goto label2;

            case 'A':
            case 'a':
                return SUCCESS;

            case 'R':
            case 'r':
                goto label1;

            default:
                printf("Entered Invalid option\n");
                // getchar();
            }
        }
        else
        {
            break;
        }
    }

label2:

    fptr = fopen(file_name, "w+");

    if (fptr == NULL)
    {
        perror("fopen");
        return FAILURE;
    }

    fprintf(local_file_fptr, "%s", file_name);

    fflush(local_file_fptr);

    if (local_file_fptr != NULL)
    {
        fclose(local_file_fptr);
        local_file_fptr = NULL;
    }

    chmod("local_data.txt", S_IRUSR | S_IRGRP | S_IROTH);

    fprintf(fptr, MAGIC_PIN);
    fprintf(fptr, "\n");

    // to store filename
    l_list *temp = *head;
    fprintf(fptr, "#:FILE_LIST");
    while (temp != NULL)
    {
        fprintf(fptr, ":%s", temp->file_name);
        temp = temp->link;
    }
    temp = *update_head;
    while (temp != NULL)
    {
        fprintf(fptr, ":%s", temp->file_name);
        temp = temp->link;
    }
    fprintf(fptr, ":#\n");

    for (int ind = 0; ind < 27; ind++)
    {
        if (arr_hash[ind].link != NULL)
        {
            TRACE("\n");
            mainnode *temp_main_link = arr_hash[ind].link;
            subnode *temp_file_link = NULL;

            fprintf(fptr, "#:%d\n", ind);
            while (temp_main_link != NULL)
            {
                temp_file_link = temp_main_link->sub_link;
                TRACE("\n");
                fprintf(fptr, "%s:%d", temp_main_link->word, temp_main_link->file_count);
                for (int i = 0; i < temp_main_link->file_count; i++)
                {
                    TRACE("\n");
                    fprintf(fptr, ":%s:%d", temp_file_link->f_name, temp_file_link->word_count);
                    temp_file_link = temp_file_link->sublink;
                }
                fprintf(fptr, ":#\n");
                temp_main_link = temp_main_link->main_link;
            }
        }
    }
    TRACE("\n");
    fflush(fptr);

    if (fptr != NULL)
    {
        fclose(fptr);
        fptr = NULL;
    }

    chmod(file_name, S_IRUSR | S_IRGRP | S_IROTH);
    return SUCCESS;
}

int update_database(l_list **head, l_list **update_head, hash *arr_hash)
{
    char *inp_buf = NULL;
    char buf = 0;
    FILE *fptr_file_list = fopen("file_list.txt", "w+");
    if (!fptr_file_list)
    {
        return FAILURE;
    }
    printf("Enter the filename to update the data :\n");


    int file_count = 0;
    unsigned int char_count = 0;
    getchar();
    while (buf != '\n')
    {
        scanf("%c", &buf);
        if (buf == ' ')
        {
            file_count++;
        }
        char_count++;
        fprintf(fptr_file_list, "%c", buf);
    }
    fflush(fptr_file_list);

    if (fptr_file_list != NULL)
    {
        fclose(fptr_file_list);
        // printf("fptr_file_list closed\n");
    }

    if (char_count == 0)
    {
        printf("No arguments passed\n");
        return FAILURE;
    }

    // printf("head - %p\n",*head);
    // l_list * update_head = NULL;

    if (validate_FILE_LIST(file_count, "file_list.txt", update_head))
    {
        printf("Failed at validating file list\n");
        return FAILURE;
    }

    // fptr_file_list = fopen(

    FILE *temp_fptr = fopen("local_data.txt", "r");
    if (!temp_fptr)
    {
        printf("failed to open local_data file\n");
        return FAILURE;
    }

    char local_file[50] = {0};
    FILE *backup_filelist_fptr = NULL;
    char *backup_file_list = NULL;
    if (fscanf(temp_fptr, "%s", local_file) != 1)
    {
        printf("Database doesnt exist.\n");
    }
    else
    {
        backup_filelist_fptr = fopen(local_file, "r");

        if (!backup_filelist_fptr)
        {
            printf("Failed to open the file lists backup\n");
            return 0;
        }
        if (temp_fptr != NULL)
        {
            fclose(temp_fptr);
            temp_fptr = NULL;
        }

        size_t n = 0;
        getline(&backup_file_list,&n,backup_filelist_fptr);
        if(strcmp(backup_file_list,MAGIC_PIN)){
            create_database(*update_head,arr_hash);
            return SUCCESS;
        }
        getline(&backup_file_list, &n, backup_filelist_fptr);
        // if(backup_file_list!=NULL)
        // // printf("%s\n",backup_file_list);
    }

    if (backup_filelist_fptr != NULL)
    { ////To Skip the files which already present in the database which is saved.
        char *temp_file = strtok(backup_file_list, ":");
        while (temp_file != NULL)
        {
            // printf("%s\n", temp_file);
            filter_list(update_head, temp_file);
            temp_file = strtok(NULL, ":"); // ← NULL after the first call
        }
    }

    if (*head != NULL)
    { // To Skip the files which already present in the main LL
        l_list *temp = *head;
        while (temp != NULL)
        {
            // printf("database_filename - %s\n",temp->file_name);
            filter_list(update_head, temp->file_name);
            temp = temp->link;
        }
    }

    if (create_database(*update_head, arr_hash))
    {
        printf("Failed to update the file's content in the database\n");
        return FAILURE;
    }

    return SUCCESS;
}