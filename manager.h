#ifndef MANAGER
#define MANAGER

#include <stdio.h>

#define TRUE 1
#define FALSE 0
#define MAX_SIZE 100
#define MAX_SV 100
#define MAX_CLASS 1000

extern int count;

int isFull();
int isEmpty();
int get_data(char *name_file);
void show();
int check_int(char *text, int *res);
int check_birthday(char *text);
void add();
void save(char *name_file);
void sort_list(int start, int stop);
int *find(char *name_find, int *n);
int *find_class(char *name_file, int *n);
void find_by_name();
void find_and_delete();
void assign_id(int id_faculty);
void assign_email();
void get_class();
void sort_by_name();
void find_students();
void add_class();
void delete_class();
void select_create_class();
void menu(int id_faculty, int check);

#endif