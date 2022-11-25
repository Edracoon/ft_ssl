#include "ft_ssl.h"

void	choseAlgorithm(string str, string algorithm)
{
	if (strEqual(algorithm, "md5"))
		md5(str);
	else if (strEqual(algorithm, "sha256"))
		sha256(str);
	else {
		printUsage();
		exitError("Unkwown command.");
	}
}

SSL		*parseParams(int ac, string *av)
{
	SSL		*ssl = myAlloc(sizeof(SSL));
	ssl->command = NULL;
	ssl->file = NULL;
	ssl->givenStr = NULL;
	ssl->p = false;
	ssl->q = false;
	ssl->r = false;
	ssl->s = false;

	int i = 1;
	while (i < ac)
	{
		printf("av[%d] = %s\n", i, av[i]);
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
			if (i < ac)
				addGivenString(ssl, strDup(av[i]));
			else {
				printUsage();
				exitError("Missing string after -s flag.");
			}
			ssl->s = true;
		}
		else if (ssl->command == NULL)
			ssl->command = strDup(av[i]);
		else
			ssl->file = strDup(av[i]);
		i++;
	}
	if (ssl->command == NULL) {
		printUsage();
		exitError("Missing command.");
	}
	return ssl;
}

int	main(int ac, char **av)
{
	SSL	*ssl = parseParams(ac, av);
	/* Debug Struct */
	printf("command: %s, file: %s, p: %d, q: %d, r: %d, s: %d\n", ssl->command, ssl->file, ssl->p, ssl->q, ssl->r, ssl->s);
	givenString *tmp = ssl->givenStr;
	while (tmp)
	{
		printf("givenStr: %s\n", tmp->str);
		tmp = tmp->next;
	}
	choseAlgorithm("test", ssl->command);
	return (0);
}