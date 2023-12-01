#ifndef BUFFER_SIZE
# define BUFFER_SIZE 42 
#endif

#include "unistd.h"
#include "stdlib.h"
#include "string.h"
#include "fcntl.h"
#include "stdio.h"

static char	*get_temp_buff(char *staticbuff, int fd);
static char	*get_return_buff(char *staticbuff);
static char	*get_static_buff(char *staticbuff);
static void	check_free(char *s1, char *s2, int flag);
static char	*join_str(char *s1, char *s2);
char	*ft_strjoin(char *s1, char *s2, int flag);
char	*ft_strdup(char *s1, int flag);
void	*ft_valloc(unsigned int size);
unsigned int	get_values_from_str(char *str);

char	*get_next_line(int fd)
{
	static char	*staticbuff;
	char		*returnbuff;

	if (fd < 0)
	{
		if (staticbuff)
			free(staticbuff);
		return (NULL);
	}
	staticbuff = get_temp_buff(staticbuff, fd);
	if (!staticbuff)
		return (NULL);
	returnbuff = get_return_buff(staticbuff);
	staticbuff = get_static_buff(staticbuff);
	return (returnbuff);
}

static char	*get_static_buff(char *staticbuff)
{
	char	*newstatbuff;
	size_t	newlen;
	size_t	statlen;

	if (staticbuff && strchr(staticbuff, '\n'))
	{
		newlen = 0;
		statlen = 0;
		while (staticbuff[statlen] != '\n')
			statlen++;
		statlen++;
		while (staticbuff[statlen + newlen])
			newlen++;
		newstatbuff = calloc(sizeof(char), newlen + 1);
		memmove(newstatbuff, &staticbuff[statlen], newlen);
		free(staticbuff);
		return (newstatbuff);
	}
	else
		free(staticbuff);
	return (NULL);
}

static char	*get_return_buff(char *staticbuff)
{
	char	*returnbuff;
	int		returnlen;

	returnlen = 0;
	while (staticbuff[returnlen] && staticbuff[returnlen] != '\n')
		returnlen++;
	if (staticbuff[returnlen] == '\n')
		returnlen++;
	if (returnlen == 0)
		return (NULL);
	returnbuff = calloc(sizeof(char), returnlen + 1);
	memmove(returnbuff, staticbuff, returnlen);
	return (returnbuff);
}

static char	*get_temp_buff(char *staticbuff, int fd)
{
	char		readbuff[BUFFER_SIZE + 1];
	int			readflag;

	if (staticbuff && strchr(staticbuff, '\n'))
		return (staticbuff);
	while (42)
	{
		readflag = read(fd, readbuff, BUFFER_SIZE);
		if (readflag < 0)
		{
			free(staticbuff);
			return (NULL);
		}
		if (readflag > 0)
		{
			readbuff[readflag] = '\0';
			staticbuff = ft_strjoin(staticbuff, readbuff, 1);
		}
		if (!staticbuff || strchr(staticbuff, '\n') || readflag == 0)
			break ;
	}
	return (staticbuff);
}


char	*ft_strjoin(char *s1, char *s2, int flag)
{
	char	*str;

	str = join_str(s1, s2);
	if (!str)
		return (NULL);
	if (flag)
		check_free(s1, s2, flag);
	return (str);
}

static char	*join_str(char *s1, char *s2)
{
	int		i;
	int		j;
	int		count;
	char	*str;

	if (!s1)
		return (ft_strdup(s2, 0));
	if (!s2)
		return (ft_strdup(s1, 0));
	i = strlen(s1);
	j = strlen(s2);
	count = 0;
	str = calloc((i + j + 1), 1);
	if (str == NULL)
		return (NULL);
	while (count < i)
	{
		str[count] = s1[count];
		count++;
	}
	while (--j >= 0)
		str[count + j] = s2[j];
	return (str);
}

static void	check_free(char *s1, char *s2, int flag)
{
	if (flag == 1)
	{
		if (s1)
			free(s1);
	}
	else if (flag == 2)
	{
		if (s2)
			free(s1);
	}
	else if (flag == 3)
	{
		if (s1)
			free(s1);
		if (s2)
			free(s1);
	}
	return ;
}

char	*ft_strdup(char *s1, int flag)
{
	int		len;
	char	*ptr;

	len = 0;
	while (s1[len])
		len++;
	ptr = ft_valloc(len + 1);
	if (ptr == NULL)
		return (NULL);
	while (len-- > 0)
		ptr[len] = s1[len];
	if (flag && s1)
		free(s1);
	return (ptr);
}

void	*ft_valloc(unsigned int size)
{
	void	*ptr;

	ptr = malloc(size);
	if (!ptr)
		return (NULL);
	((char *)ptr)[size - 1] = 0;
	return (ptr);
}

char	find_first_digit(char *str)
{
	while (str && *str) {
		if (strchr("0123456789", *str))
			return (*str);
		str++;
	}
	return ('0');
}

char	find_last_digit(char *str)
{
	unsigned int	len = strlen(str);

	len--;
	while (len >= 0) {
		if (strchr("0123456789", str[len]))
			return (str[len]);
		len--;
	}
	return ('0');
}


unsigned int	get_values_from_str(char *str)
{
	char	digit[3];

	memset(digit, 0, sizeof(digit));
	digit[0] = find_first_digit(str);
	digit[1] = find_last_digit(str);
	return atoi(digit);
}

int main(void) {

	char 				*line;
	int					fd;
	unsigned int		sum = 0;

	fd = open("AOC2023", O_RDONLY);
	if (!fd)
		return (0);

	line = get_next_line(fd);
	while (line) {
		sum += get_values_from_str(line);
		free(line);
		line = get_next_line(fd);
	}
	printf("The result is: %u\n", sum);
	return (0);
}
