#include "ft_ssl.h"

void	printStrHexa(unsigned char *str, unsigned int len)
{
	char			hexa[16] = "0123456789abcdef";

	for (unsigned int i = 0 ; i < len ; i++)
	{
		write(1, &hexa[str[i] / 16], 1);
		write(1, &hexa[str[i] % 16], 1);
	}
}

void	printHash(SSL *ssl, unsigned char *hash, char *key)
{
	(void)ssl;
	(void)key;
	// if (ssl->q)
	printStrHexa(hash, 16);
	// else if (ssl->r)
	// 	printf("%s \"%s\"", hash, key);
	// else
	// 	printf("(\"%s\") = %s", key, hash);
	write(1, "\n", 1);
}

void	executeCommand(SSL *ssl, unsigned char *(*command)(SSL *ssl, unsigned char *str))
{
	ContentList	*tmp = ssl->content;
	while (tmp)
	{
		printf("key=\"%s\", value\"%s\"\n", tmp->key, tmp->value);
		if (tmp->isFile)
			readFromFile(ssl, tmp);

		if (!tmp->value) { tmp = tmp->next; continue; }

		unsigned char	*hash = command(ssl, (unsigned char*)tmp->value);
		printHash(ssl, hash, tmp->key);

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