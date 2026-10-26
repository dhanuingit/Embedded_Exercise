# Inverted Search

A command-line application written in **C** that builds an **inverted index** from a collection of text files. Once the database is built, you can instantly find **which files contain a given word and how many times it appears in each**, without rescanning the files.

> Inverted indexing is the core idea behind search engines: instead of asking "what words are in this file?", you store "which files contain this word?".

---

## Table of Contents

- [Features](#features)
- [How It Works](#how-it-works)
- [Data Structures](#data-structures)
- [Project Structure](#project-structure)
- [Requirements](#requirements)
- [Build and Run](#build-and-run)
- [Usage](#usage)
- [Example Session](#example-session)
- [Input File Validation](#input-file-validation)
- [Backup File Format](#backup-file-format)
- [Error Handling](#error-handling)
- [Testing](#testing)
- [Concepts Used](#concepts-used)
- [Known Limitations](#known-limitations)
- [Future Improvements](#future-improvements)
- [Author](#author)
- [License](#license)

---

## Features

- **File validation:** accepts only existing, non-empty `.txt` files and rejects duplicates.
- **Create database:** reads every valid file and indexes each unique word.
- **Display database:** prints every word with the files it appears in and its count per file.
- **Search:** look up any word and see the files and number of occurrences.
- **Save database:** writes the in-memory database to a backup file.
- **Update database:** loads a previously saved backup and lets you add new files without re-indexing old ones.
- **Dynamic memory:** all nodes are allocated at runtime and freed on exit.

---

## How It Works

1. The program receives a list of file names as command-line arguments.
2. Each file is validated, and the valid ones are stored in a **linked list**.
3. For every file in the list, the program reads word by word.
4. Each word is mapped to a **hash table index** using its first character.
5. At that index, the program searches the word's linked list:
   - **Word not present:** create a new *main node* for the word and a *sub node* for the file with count `1`.
   - **Word present, file not present:** add a new *sub node* for this file with count `1`, and increase the word's file count.
   - **Word and file both present:** increment that file's word count.
6. Searching a word means hashing it, walking one short list, and reading its sub nodes.

### Hash function

The table has **27 buckets**:

| Index | Holds words starting with |
|-------|---------------------------|
| 0 to 25 | `a` to `z` (case-insensitive) |
| 26 | Digits and any other character |

```c
int index = isalpha(word[0]) ? tolower(word[0]) - 'a' : 26;
```

---

## Data Structures

The database is an **array of linked lists of linked lists**.

```
hash_table[27]
   │
   ├─ [0]  'a' ─► [ apple | files:2 ] ─► [ ant | files:1 ] ─► NULL
   │                    │                       │
   │                    ▼                       ▼
   │             [ f1.txt | 3 ]          [ f2.txt | 1 ]
   │                    │
   │                    ▼
   │             [ f3.txt | 1 ]
   │                    │
   │                    ▼
   │                  NULL
   ├─ [1]  'b' ─► ...
   │
   └─ [26] other ─► ...
```

### Structures

```c
/* One file in the list of validated input files */
typedef struct file_node
{
    char filename[50];
    struct file_node *link;
} File_node;

/* Sub node: one file in which a word appears */
typedef struct sub_node
{
    int word_count;                /* occurrences of the word in this file */
    char filename[50];
    struct sub_node *sub_link;
} Sub_node;

/* Main node: one unique word */
typedef struct main_node
{
    int file_count;                /* number of files containing the word  */
    char word[50];
    Sub_node *sub_link;            /* list of files                        */
    struct main_node *main_link;   /* next word in the same bucket         */
} Main_node;
```

> Adjust the names and sizes above to match your own headers.

---

## Project Structure

```
inverted-search/
├── README.md
├── LICENSE
├── Makefile
├── .gitignore
├── inc/
│   ├── types.h                 # typedefs, status enums, constants
│   └── inverted_search.h       # structures and function prototypes
├── src/
│   ├── main.c                  # argument handling and menu loop
│   ├── validate.c              # file validation
│   ├── file_list.c             # linked list of input files
│   ├── create_db.c             # build the hash table
│   ├── display_db.c            # print the database
│   ├── search_db.c             # word lookup
│   ├── save_db.c               # write the backup file
│   └── update_db.c             # load backup and add new files
├── data/
│   ├── file1.txt               # sample input files
│   ├── file2.txt
│   ├── file3.txt
│   └── sample_backup.txt       # example saved database
├── docs/
│   ├── design.md
│   └── screenshots/
└── tests/
    └── run_tests.sh
```

| Source file | Responsibility |
|-------------|----------------|
| `main.c` | Reads command-line arguments, shows the menu, calls the right function |
| `validate.c` | Checks that files exist, end in `.txt`, are not empty, and are not duplicates |
| `file_list.c` | Inserts valid file names into a linked list |
| `create_db.c` | Reads words from each file and fills the hash table |
| `display_db.c` | Walks every bucket and prints words with their file counts |
| `search_db.c` | Finds a word and prints the files and counts |
| `save_db.c` | Writes the database to a backup file |
| `update_db.c` | Reads a backup file back into the hash table |

---

## Requirements

- Linux (developed and tested on Ubuntu)
- GCC compiler
- GNU Make

Install on Ubuntu or Debian:

```bash
sudo apt update
sudo apt install build-essential
```

---

## Build and Run

Clone the repository:

```bash
git clone https://github.com/<your-username>/inverted-search.git
cd inverted-search
```

Build:

```bash
make
```

Run with one or more input files:

```bash
./inverted_search data/file1.txt data/file2.txt data/file3.txt
```

Clean build output:

```bash
make clean
```

### Without Make

```bash
gcc -Wall -Wextra -Iinc src/*.c -o inverted_search
```

---

## Usage

If no file is given, the program prints a usage message and exits:

```
Usage: ./inverted_search <file1.txt> <file2.txt> ...
```

After validation, the main menu appears:

```
1. Create database
2. Display database
3. Search database
4. Update database
5. Save database
6. Exit
```

| Option | Action |
|--------|--------|
| **1. Create** | Builds the database from the validated file list. Can be done only once per run unless you update |
| **2. Display** | Shows every word, the number of files containing it, and the per-file counts |
| **3. Search** | Asks for a word and shows where it appears |
| **4. Update** | Loads a saved backup file. Files already indexed in the backup are skipped |
| **5. Save** | Writes the current database to a file you name |
| **6. Exit** | Frees all memory and quits |

---

## Example Session

```
$ ./inverted_search data/file1.txt data/file2.txt data/file3.txt

Validating files...
  data/file1.txt  : OK
  data/file2.txt  : OK
  data/file3.txt  : OK

1. Create database
2. Display database
3. Search database
4. Update database
5. Save database
6. Exit
Enter your choice: 1
Database created successfully from 3 file(s).

Enter your choice: 3
Enter the word to search: apple
Word "apple" is present in 2 file(s):
  file1.txt : 3 time(s)
  file3.txt : 1 time(s)

Enter your choice: 5
Enter backup file name: backup.txt
Database saved to backup.txt

Enter your choice: 6
Goodbye.
```

---

## Input File Validation

A file is accepted only if **all** of these are true:

1. The file exists and can be opened for reading.
2. The name ends with the `.txt` extension.
3. The file is not empty.
4. The file has not already been given (no duplicates).

Invalid files are skipped with a message, and the program continues with the valid ones. If **no** valid file remains, the program exits.

---

## Backup File Format

The saved database uses one line per word:

```
#<index>;<word>;<file_count>;<file1>;<count1>;<file2>;<count2>;...;#
```

Example:

```
#0;apple;2;file1.txt;3;file3.txt;1;#
#0;ant;1;file2.txt;1;#
#1;banana;1;file1.txt;2;#
```

| Field | Meaning |
|-------|---------|
| `#` | Start and end marker of a record |
| `index` | Hash table bucket |
| `word` | The indexed word |
| `file_count` | Number of files containing the word |
| `file;count` | File name and occurrences, repeated `file_count` times |

> Update this section to match the exact format your `save_db.c` writes.

---

## Error Handling

| Situation | Behaviour |
|-----------|-----------|
| No command-line arguments | Prints usage and exits |
| File does not exist | Prints an error and skips the file |
| Wrong extension | Prints an error and skips the file |
| Empty file | Prints an error and skips the file |
| Duplicate file | Prints a message and skips the file |
| Database already created | Refuses to create it a second time |
| Search before create | Reports that the database is empty |
| Word not found | Prints a clear "not found" message |
| `malloc` failure | Reports the error and exits cleanly |

---

## Testing

Run the test script:

```bash
bash tests/run_tests.sh
```

Suggested manual test cases:

- No arguments
- A missing file
- A file named `notes.pdf` (wrong extension)
- An empty `.txt` file
- The same file passed twice
- A word present in all files, in one file, and in none
- Save, restart, then update from the backup
- Words with mixed case and words starting with a digit

Check for memory leaks and invalid accesses:

```bash
valgrind --leak-check=full --show-leak-kinds=all ./inverted_search data/file1.txt
```

---

## Concepts Used

- Hash tables
- Singly linked lists (nested: word list and file list)
- File handling (`fopen`, `fscanf`, `fprintf`, `fclose`)
- String handling (`strcmp`, `strcpy`, `strstr`, `strlen`)
- Dynamic memory allocation (`malloc`, `free`)
- Command-line arguments (`argc`, `argv`)
- Modular programming with header files
- Makefiles

---

## Known Limitations

- Only plain `.txt` files are supported.
- Words are split on whitespace, so attached punctuation (for example `hello,`) may be treated as part of the word unless stripped.
- Word length is limited by the fixed buffer size in the structures.
- The entire database is held in memory.

> Edit this list so it honestly reflects your implementation.

---

## Future Improvements

- Strip punctuation and normalize case for better matching
- Support multi-word (phrase) search
- Sort search results by occurrence count
- Add a command to remove a file from the database
- Support additional file types
- Add unit tests

---

## Author

**Your Name**
GitHub: [@dhanuingit](https://github.com/dhanuingit)

---

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
