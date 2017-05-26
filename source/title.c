#include "../include/common.h"

void	print_title(char *title, size_t len_title)
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

void	backup_original_title(char *title, size_t len_title)
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

void	create_patched_title(char *title, size_t len_title)
{
	int	fd = 0;

	fd = open(TITLE, O_RDWR, O_TRUNC);
	write(fd, title, len_title);
	if (close(fd) < 0)
		printf(RED"close failure ! : %s\n"END, strerror(errno));
}

bool patch_title(char *title, size_t len_title)
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
