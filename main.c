#include "ft_ssl.h"

void	processAlgo(SSL *ssl)
{
	(void)ssl;
	// if (strEqual(command, "md5"))
	// 	md5(str);
	// else if (strEqual(command, "sha256"))
	// 	sha256(str);
	// else {
	// 	printUsage();
	// 	exitError("Unkwown command.");
	// }
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

int	main(int ac, char **av)
{
	SSL	*ssl = parseParams(ac, av);
	printf("command: %s, p: %d, q: %d, r: %d, s: %d\n",
		ssl->command, ssl->p, ssl->q, ssl->r, ssl->s);
	
	if (ssl->p || (!ssl->isThereFile && !ssl->s))
		readFromStdin(ssl);

	/* Debug content list */
	ContentList *tmp = ssl->content;
	printf("content list:\n");
	while (tmp)
	{
		printf("-> key: %s, value: %s, isFile: %d\n", tmp->key, tmp->value, tmp->isFile);
		tmp = tmp->next;
	}
	processAlgo(ssl);
	return (0);
}