/*
 * To compile this tool : gcc -Wall -Wextra -Werror main.c -o Wii_U_ticket_patcher
 */

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

static void	print_header()
{
	printf(GREEN"\n############## Wii_U_ticket_patcher ###########\n");
	printf("#                                             #\n");
	printf("#          Developped by SegFault42           #\n");
	printf("#       https://github.com/SegFault42/        #\n");
	printf("#                                             #\n");
	printf("###############################################\n\n"END);
}

static void	print_title(char *title, size_t len_title)
{
	char	str[] = " ------------------title.tik---------------------\n";

	printf(BLUE"%s"END, str);
	for (size_t i = 0; i < len_title; ++i)
	{
		if (i % 16 == 0)
			printf(BLUE"|"END);
		if (i == 1 || i == 15)
			printf(GREEN"%02x "END, title[i] & 0xff);
		else
			printf("%02x ", title[i] & 0xff);
		if (i % 16 == 15 )
			printf(BLUE"|\n"END);
	}
	printf(BLUE"%s\n"END, str);
}

static void	backup_original_title(char *title, size_t len_title)
{
	int	fd = 0;

	if (access("title_orig.tik", F_OK) != 0)
	{
		fd = open("title_orig.tik", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
		write(fd, title, len_title);
		if (close(fd) < 0)
			printf(RED"close failure ! : %s\n"END, strerror(errno));
	}
	else
		dprintf(2, RED"title_orig.tik exist !\n"END);
}

static void	create_patched_title(char *title, size_t len_title)
{
	int	fd = 0;

	fd = open(TITLE, O_RDWR, O_TRUNC);
	write(fd, title, len_title);
	if (close(fd) < 0)
		printf(RED"close failure ! : %s\n"END, strerror(errno));
}

static bool patch_title(char *title, size_t len_title)
{
	if (title[1] == 0x01)
	{
		dprintf(2, RED"title.tik already patched !\n"END);
		return (false);
	}
	else
	{
		backup_original_title(title, len_title);
		title[1] = 0x01;
		title[15] -= 0x02;
		printf(GREEN"title.tik patch success !\n"END);
		create_patched_title(title, len_title);
	}
	return (true);
}

int	main()
{
	int			fd = 0;
	int			ret;
	size_t		len_title = 0;
	struct stat	st;

	print_header();
	fd = open(TITLE, O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	if (fd < 0)
	{
		printf(RED"Open title.tik failure ! : %s\n"END, strerror(errno));
		return (EXIT_FAILURE);
	}
	if (fstat(fd, &st) < 0)
	{
		printf(RED"fstat title.tik failure ! : %s\n"END, strerror(errno));
		return (EXIT_FAILURE);
	}
	len_title = st.st_size;

	char	title[len_title];

	ret = read(fd, title, len_title);
	if (ret < 0)
	{
		printf(RED"Read title.tik failure ! : %s\n"END, strerror(errno));
		if (close(fd) < 0)
			printf(RED"close failure ! : %s\n"END, strerror(errno));
		return (EXIT_FAILURE);
	}

	print_title(title, len_title);

	if (patch_title(title, len_title) == false)
	{
		if (close(fd) < 0)
			printf(RED"close failure ! : %s\n"END, strerror(errno));
		return (EXIT_FAILURE);
	}

	return (EXIT_SUCCESS);
}
