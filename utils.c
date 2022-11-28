#include "ft_ssl.h"

void	exitError(string str)
{
	putStr(STDERR_FILENO, RED);
	putStr(STDERR_FILENO, "Error: ft_ssl: ");
	putStr(STDERR_FILENO, str);
	putStr(STDERR_FILENO, "\n");
	putStr(STDERR_FILENO, RESET);
	exit(EXIT_FAILURE);
}

void	printUsage(void)
{
	putStr(STDOUT_FILENO, "\
Usage: \n\t ./ft_ssl <command> [flags] [file]\n\n \
Commands:\n \
	md5 ( \'Message Digest 5\' )\n \
	sha256 ( \'Secure Hash Algorithms\' ) \n\n \
Flags:\n \
	-h: Print usage\n \
	-p: Echo STDIN to STDOUT and append the checksum to STDOUT\n \
	-q: Quiet mode\n \
	-r: Reverse the format of the output\n \
	-s <string>: Print the sum of the given string\n");
}

void	*myAlloc(int size)
{
	void	*p = malloc(size);
	if (p == NULL)
	{
		exitError("Malloc failed, out of memory.");
		exit(EXIT_FAILURE);
	}
	return p;
}

int		strLen(string str)
{
	int i = 0;
	while (str && str[i]) i++;
	return (i);
}

void	putStr(int fd, string str)
{
	write(fd, str, strLen(str));
}

bool	strEqual(string s1, string s2)
{
	while (*s1 && *s2 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return (*(unsigned char *)s1 - *(unsigned char *)s2 == 0 ? true : false);
}

char	*strJoin(string s1, string s2)
{
	int i = 0;
	int j = 0;
	int len = strLen(s1) + strLen(s2);
	string str = myAlloc(sizeof(char) * (len + 1));
	while (s1 && s1[i])
	{
		str[i] = s1[i];
		i++;
	}
	while (s2 && s2[j])
	{
		str[i] = s2[j];
		i++;
		j++;
	}
	str[i] = '\0';
	return (str);
}

string	strDup(string str)
{
	if (!str)
		return (NULL);
	int i = 0;
	string newStr = myAlloc(sizeof(char) * (strLen(str) + 1));
	while (str[i])
	{
		newStr[i] = str[i];
		i++;
	}
	newStr[i] = '\0';
	return (newStr);
}

SSL		*parseParams(int ac, string *av)
{
	SSL		*ssl = myAlloc(sizeof(SSL));
	ssl->command = NULL;
	ssl->content = NULL;
	ssl->isThereFile = false;
	ssl->p = false;
	ssl->q = false;
	ssl->r = false;
	ssl->s = false;

	int i = 1;
	while (i < ac)
	{
		/* If a file has been inputed we stop looking after flags */
		if (ssl->isThereFile) {
			addBackContent(ssl, av[i], NULL, true);
			i++;
			continue;
		}

		/* Looking for flags */
		if (strEqual(av[i], "-h"))
			printUsage();
		else if (strEqual(av[i], "-p"))
			ssl->p = true;
		else if (strEqual(av[i], "-q"))
			ssl->q = true;
		else if (strEqual(av[i], "-r"))
			ssl->r = true;
		else if (strEqual(av[i], "-s"))
		{
			i++;
			addBackContent(ssl, strDup(av[i]), strDup(av[i]), false);
			ssl->s = true;
		}
		else if (ssl->command == NULL)
			ssl->command = strDup(av[i]);
		else {
			ssl->isThereFile = true;
			addBackContent(ssl, strDup(av[i]), NULL, true);
		}
		i++;
	}
	if (ssl->command == NULL) {
		printUsage();
		exitError("Missing command.");
	}
	return ssl;
}

void	addBackContent(SSL	*ssl, string key, string value, bool isFile)
{
	ContentList	*newContent = myAlloc(sizeof(ContentList));
	newContent->key = key;
	newContent->value = value;
	newContent->isFile = isFile;
	newContent->next = NULL;
	if (ssl->content == NULL)
		ssl->content = newContent;
	else
	{
		ContentList	*tmp = ssl->content;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = newContent;
	}
}

void addFrontContent(SSL *ssl, string key, string value, bool isFile)
{
	ContentList	*newContent = myAlloc(sizeof(ContentList));
	newContent->key = key;
	newContent->value = value;
	newContent->isFile = isFile;
	newContent->next = ssl->content;
	ssl->content = newContent;
}