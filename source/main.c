#include "../include/common.h"

static void	print_header()
{
	printf(GREEN"\n############## Wii_U_ticket_patcher ###########\n");
	printf("#                                             #\n");
	printf("#          Developped by SegFault42           #\n");
	printf("#       https://github.com/SegFault42/        #\n");
	printf("#                                             #\n");
	printf("###############################################\n\n"END);
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
