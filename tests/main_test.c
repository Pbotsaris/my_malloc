#include <criterion/criterion.h>
#include "../include/my_malloc.h"

bool are_same(int *arr, int *arr2)
{
  int i = 0;
  while(arr[i] != 0 && arr[i] != 0)
  {
      if(arr[i] != arr2[i])
        return false;

    i++; 
  }
   return true;
}


Test(asserts, allocating_string)
{
  char *string = "hello,world!\n";
  char *new_string = "ola,mundo!\n";

  char *malloc_result = (char*)my_malloc(sizeof(char) * (strlen(string) + 1));
  char *calloc_result = (char*)my_calloc(sizeof(char), (strlen(string) + 1));

  strcpy(malloc_result, string);
  strcpy(calloc_result, string);

  cr_assert(strcmp(malloc_result, string) == 0, "copying string to allocated buffer was not successfull with my_malloc");
  cr_assert(strcmp(calloc_result, string) == 0, "copying string to allocated buffer was not successfull with my_calloc");

  char *realloc_result = (char*)my_realloc(malloc_result, (strlen(new_string) + 1));
  strcpy(realloc_result, new_string);

  cr_assert(strcmp(realloc_result, new_string) == 0, "copying string to allocated buffer was not successfull with my_realloc");

  dealloc();
}


Test(asserts, allocating_ints)
{
  int ints[7] = {10, 230, 34039, 4059 , 20394, 499, 0};


  int *malloc_result = (int*)my_malloc(sizeof(int) * 7);
  int *calloc_result = (int*)my_calloc(sizeof(int), 7);

  memcpy(malloc_result, ints, sizeof(int)* 7);
  memcpy(calloc_result, ints, sizeof(int) * 7);

  cr_assert(are_same(ints, malloc_result), "copying int buffer to allocated buffer was not successfull with my_malloc");
  cr_assert(are_same(ints, calloc_result), "copying int buffer to allocated buffer was not successfull with my_calloc");

//  char *realloc_result = (char*)my_realloc(malloc_result, (strlen(new_string) + 1));
//  strcpy(realloc_result, new_string);
//
//  cr_assert(strcmp(realloc_result, new_string) == 0, "copying string to allocated buffer was not successfull with my_realloc");
//
  dealloc();
}
