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

#include <windows.h>
#include <sys\stat.h>

#define TITLE "title.tik"

enum Color
{
	BLACK = 0,
	BLUE = 3,
	RED = 12,
	GREEN = 2,
	WHITE = 15
};


static void    press_enter()
{
    printf("\nPress enter to exit\n");
    getchar();
}

static void color(int text,int background)
{
    HANDLE H=GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(H, background * 16 + text);
}

static void	print_header()
{
	color(GREEN, BLACK);
	printf("\n############## Wii_U_ticket_patcher ###########\n");
	printf("#                                             #\n");
	printf("#          Developped by SegFault42           #\n");
	printf("#       https://github.com/SegFault42/        #\n");
	printf("#                                             #\n");
	printf("###############################################\n\n");
	color(WHITE, BLACK);
}

static void	print_title(char *title, size_t len_title)
{
	char	str[] = " ------------------title.tik---------------------\n";

	color(BLUE, BLACK);
	printf("%s", str);
	color(WHITE, BLACK);
	for (size_t i = 0; i < len_title; ++i)
	{
		if (i % 16 == 0)
		{
			color(BLUE, BLACK);
			printf("|");
			color(WHITE, BLACK);
		}
		if (i == 1 || i == 15)
		{
			color(GREEN, BLACK);
			printf("%02x ", title[i] & 0xff);
			color(WHITE, BLACK);
		}
		else
			printf("%02x ", title[i] & 0xff);
		if (i % 16 == 15 )
		{
			color(BLUE, BLACK);
			printf("|\n");
			color(WHITE, BLACK);
		}
	}
	color(BLUE, BLACK);
	printf("%s\n", str);
	color(WHITE, BLACK);
}

static void	backup_original_title(char *title, size_t len_title)
{
	int	fd = 0;

	if (access("title_orig.tik", F_OK) != 0)
	{
		#ifdef _WIN32
			fd = open("title_orig.tik", O_CREAT | O_RDWR, S_IREAD | _S_IWRITE);
		#else
			fd = open("title_orig.tik", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
		#endif
		
		write(fd, title, len_title);
		if (close(fd) < 0)
		{
			color(RED, BLACK);
			printf("close failure ! : %s\n", strerror(errno));
			color(WHITE, BLACK);
		}
	}
	else
	{
		color(RED, BLACK);
		fprintf(stderr, "title_orig.tik exist !\n");
		color(WHITE, BLACK);
	}
}

static void	create_patched_title(char *title, size_t len_title)
{
	int	fd = 0;

	fd = open(TITLE, O_RDWR, O_TRUNC);
	write(fd, title, len_title);
	if (close(fd) < 0)
	{
		color(RED, BLACK);
		printf("close failure ! : %s\n", strerror(errno));
		color(WHITE, BLACK);
	}
}

static bool patch_title(char *title, size_t len_title)
{
	if (title[1] == 0x01)
	{
		color(RED, BLACK);
		fprintf(stderr, "title.tik already patched !\n");
		color(WHITE, BLACK);
		return (false);
	}
	else
	{
		backup_original_title(title, len_title);
		title[1] = 0x01;
		title[15] -= 0x02;
		color(GREEN, BLACK);
		printf("title.tik patch success !\n");
		color(WHITE, BLACK);
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

	fd = open(TITLE, O_RDWR, _S_IREAD | _S_IWRITE);
	if (fd < 0)
	{
		color(RED, BLACK);
		printf("Open title.tik failure ! : %s\n", strerror(errno));
		color(WHITE, BLACK);
		press_enter();
		return (EXIT_FAILURE);
	}
	if (fstat(fd, &st) < 0)
	{
		color(RED, BLACK);
		printf("fstat title.tik failure ! : %s\n", strerror(errno));
		color(WHITE, BLACK);
		press_enter();
		return (EXIT_FAILURE);
	}
	len_title = st.st_size;

	char	title[len_title];

	ret = read(fd, title, len_title);
	if (ret < 0)
	{
		color(RED, BLACK);
		printf("Read title.tik failure ! : %s\n", strerror(errno));
		color(WHITE, BLACK);
		if (close(fd) < 0)
		{
			color(RED, BLACK);
			printf("close failure ! : %s\n", strerror(errno));
			color(WHITE, BLACK);
		}
		press_enter();
		return (EXIT_FAILURE);
	}

	print_title(title, len_title);

	if (patch_title(title, len_title) == false)
	{
		if (close(fd) < 0)
		{
			color(RED, BLACK);
			printf("close failure ! : %s\n", strerror(errno));
			color(WHITE, BLACK);
		}
		press_enter();
		return (EXIT_FAILURE);
	}
			press_enter();
	return (EXIT_SUCCESS);
}