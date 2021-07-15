# include "stdio.h"
# include "fcntl.h"
# include "unistd.h"
# include "string.h"
# include "stdlib.h"

# define BUFF_LEN 101

typedef struct lst
{
	char buff[201];
	size_t size;
	struct lst *next;
}		lst;

void Widify(lst *elem)
{
	char buff[101];
	size_t i;
	size_t n;

	i = 0;
	n = 0;

	memcpy(buff, elem->buff, elem->size);
	while (i < elem->size)
	{
		elem->buff[n] = buff[i];
		elem->buff[n + 1] = ' ';
		n += 2;
		i++;
	}
	elem->size = n;

}

void Spongify(lst *elem)
{
	int i;
	int s;

	i = 0;
	s = 0;
	while (i < elem->size)
	{
		if (elem->buff[i] >= 'a' && elem->buff[i] <= 'z')
		{
			elem->buff[i] = elem->buff[i] - (32 * s);
			s ^= 1;
		}
		i++;
	}
	printf("SpOnGe %s \n", elem->buff);
}

void parse(char *arg, lst *elem)
{
	int i;

	i = 0;
	while (arg[i])
	{
		elem->buff[ i - ((i / 100) * 100)] = arg[i];
		printf("this %d, %d %d\n",i, i - ((i / 100) * 100), i %100);
		if (i > 0 && i % 100 == 0)
		{
			elem->size = 100;
			elem->next = calloc(1, sizeof(*elem));
			elem = elem->next;
			printf("Over 100 %s\n", elem->buff);
		}
		i++;
	}
	elem->size =  i % 100;
}

int main(int argc, char **argv)
{
	int fd;
	int bytes;
	lst *head;
	lst *elem;
	char buffer[201];

	if (argc > 3)
	{
		printf("Usage: ./sponge [file] or type text\n");
		exit(1);
	}

	head = calloc(1, sizeof(*head));
	if (argv[1][0] == '-' && argv[1][1] == 't')
	{
		parse(argv[2], head);
	}
	else
	{
		fd = open(argv[1], O_RDONLY);
		elem = head;
		while((bytes = read(((fd != -1) * fd), buffer, BUFF_LEN - 1)) > 0)
		{
			buffer[bytes] = '\0';
			memcpy(elem->buff, buffer, bytes);
			elem->size = bytes;
			elem->next = calloc(1, sizeof(*elem));
		}
	}

	fd = ((argc == 3) ? (open(argv[2], O_WRONLY | O_CREAT, 0777)) : (0));

	while (head)
	{
		Widify(head);
		Spongify(head);
		write(fd, head->buff, head->size);
		head = head->next;
	}
	exit(0);
}
