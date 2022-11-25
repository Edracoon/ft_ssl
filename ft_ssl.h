#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define RED "\e[0;31m"
#define RESET "\e[0m"

typedef	char*						string;
typedef unsigned long long			ull;
typedef enum e_bool { false, true } bool;

typedef struct s_givenString
{
	string					str;
	struct s_givenString	*next;

} givenString;

typedef struct	s_SSL
{
	string	command;
	string 	file;

	/* -- Flags -- */
	bool		p;			/* echo STDIN to STDOUT and append the checksum to STDOUT */
	bool		q;			/* quiet mode */
	bool		r;			/* reverse the format of the output */
	bool		s;			/* print the sum of the given string */
	givenString	*givenStr;	/* Only if -s */

} SSL;

string	md5(string	str);
string	sha256(string str);

/* --- Utils --- */
void	printUsage(void);
void	exitError(char *str);
void	*myAlloc(int taille);

/* --- Strings --- */
int		strLen(char *str);
void	putStr(int fd, char *str);
bool	strEqual(string s1, string s2);
char	*strJoin(char *s1, char *s2);
string	strDup(string str);
void	addGivenString(SSL *ssl, string str);