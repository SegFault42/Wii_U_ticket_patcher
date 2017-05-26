/*
 * To compile this tool : gcc -Wall -Wextra -Werror main.c -o Wii_U_ticket_patcher
 */

#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

#define TITLE "title.tik"
#define GREEN "\033[32m"
#define RED "\033[31m"
#define BLUE "\033[34m"
#define END "\033[0m"


void	print_title(char *title, size_t len_title);
void	backup_original_title(char *title, size_t len_title);
void	create_patched_title(char *title, size_t len_title);
bool	patch_title(char *title, size_t len_title);

#endif
