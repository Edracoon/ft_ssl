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
	while (str[i]) i++;
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
	while (s1[i])
	{
		str[i] = s1[i];
		i++;
	}
	while (s2[j])
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

void	addGivenString(SSL *ssl, string str)
{
	givenString *new = myAlloc(sizeof(givenString));
	new->str = str;
	new->next = NULL;
	if (ssl->givenStr == NULL)
		ssl->givenStr = new;
	else
	{
		givenString *tmp = ssl->givenStr;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}