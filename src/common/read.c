#include "../ft_ssl.h"

void	readFromFile(SSL *ssl, ContentList *content)
{
	string	buff = myAlloc(sizeof(char) * (1024 + 1));
	string	str = NULL;
	string	tmp;

	int	fd = openFile(ssl, content->key);
	if (fd == -1)
		return ;
	while (1)
	{
		int		ret = read(fd, buff, 1024);
		if (ret == 0)
			break ;
		buff[ret] = '\0';
		tmp = str;
		str = strJoin(str, buff);
		free(tmp);
	}
	content->value = str;
	close(fd);
}

void	readFromStdin(SSL *ssl)
{
	string	buff = myAlloc(sizeof(char) * 1);
	string	tempRead = myAlloc(sizeof(char) * 2);
	string	tmp;
	buff[0] = '\0';

	char	c;
	while(read(STDIN_FILENO, &c, 1) > 0)
	{
		tempRead[0] = c;
		tempRead[1] = '\0';

		tmp = buff;
		buff = strJoin(buff, tempRead);
		free(tmp);
	}
	addFrontContent(ssl, strDup(buff), strDup(buff), false);
	free(buff);
}

int		openFile(SSL *ssl, char *fileName) {
	int	fd = open(fileName, O_DIRECTORY);
	if (fd > 0) {
		printf("ft_ssl: %s: %s: is a directory.\n", ssl->command, fileName);
		return (-1);
	}
	fd = open(fileName, O_RDONLY);
	if (fd == -1)
		printf("ft_ssl: %s: %s: No such file or directory.\n", ssl->command, fileName);
	return (fd);
}