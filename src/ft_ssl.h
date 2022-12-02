#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

/* Open includes */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define RED "\e[0;31m"
#define RESET "\e[0m"

typedef unsigned long long			ull;
typedef enum e_bool { false, true } bool;

typedef struct s_ContentList
{
	char					*key;
	char					*value;
	char					*crypted;

	bool					isFile;
	bool					isGivenStr;
	struct s_ContentList	*next;

} ContentList;

typedef struct	s_SSL
{
	char		*command;	/* md5, sha256, etc... */

	ContentList	*content;	/* Content list to encrypt */

	bool		isThereFile; /* Used only for parsing */

	/* -- Flags -- */
	bool		p;			/* echo STDIN to STDOUT and append the checksum to STDOUT */
	bool		q;			/* quiet mode */
	bool		r;			/* reverse the format of the output */
	bool		s;			/* print the sum of the given string */

} SSL;

unsigned char	*md5(SSL *ssl, unsigned char *str);
unsigned char	*sha256(SSL *ssl, unsigned char *str);

/* --- Utils --- */
SSL		*parseParams(int ac, char **av);
void	printUsage(void);
void	exitError(char *str);
void	*myAlloc(int taille);

/* --- Read and Files --- */
void	readFromFile(SSL *ssl, ContentList *content);
void	readFromStdin(SSL *ssl);
int		openFile(SSL *ssl, char *fileName);

/* --- Linked List --- */
void	addBackContent(SSL	*ssl, char *key, char * value, bool isFile);
void	addFrontContent(SSL *ssl, char * key, char * value, bool isFile);

/* --- Strings --- */
int		strLen(char *str);
void	putStr(int fd, char *str);
bool	strEqual(char *s1, char *s2);
char	*strJoin(char *s1, char *s2);
char	*strDup(char *str);
