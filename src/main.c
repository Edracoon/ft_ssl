#include "ft_ssl.h"

void	executeCommand(SSL *ssl, string (*command)(string str))
{
	ContentList	*tmp = ssl->content;
	while (tmp)
	{
		printf("%s\n", tmp->key);
		if (tmp->isFile)
			readFromFile(ssl, tmp);

		if (!tmp->value) { tmp = tmp->next; continue; }

		command(tmp->value);

		tmp = tmp->next;
	}
}

void	chooseAlgo(SSL *ssl)
{
	if (strEqual(ssl->command, "md5"))
		executeCommand(ssl, md5);
	else if (strEqual(ssl->command, "sha256"))
		executeCommand(ssl, sha256);

	/* No command match */
	else
	{
		printUsage();
		exitError("Unkwown command.");
	}
}

int	main(int ac, char **av)
{
	SSL	*ssl = parseParams(ac, av);
	printf("command: %s, p: %d, q: %d, r: %d, s: %d\n",
		ssl->command, ssl->p, ssl->q, ssl->r, ssl->s);

	if (ssl->p || (!ssl->isThereFile && !ssl->s))
		readFromStdin(ssl);

	chooseAlgo(ssl);
	return (0);
}