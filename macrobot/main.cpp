#include "stdafx.h"
#include "MacroBot.h"

#include <iostream>

#define BUFF_SIZE 1024

int handleStdin(char ** buf)
{
	char * data;
	int count;

	data = (char *)malloc(sizeof(char) * BUFF_SIZE);

	*data = '\0';

	*buf = data;

	std::cin.getline(data, BUFF_SIZE);

	count = strlen(data);

	printf("[%d]\n", count);
	printf("[%s]\n", data);

	return count <= 0 ? -1 : count;
}

int main(int argc, char * argv[])
{
	MacroBot * bot = NULL;
	int ret;
	char * buff = NULL;

	if (argc < 2) {

		ret = handleStdin(&buff);

		if (ret < 0) {
			printf("Insufficient arguments\n");
			return ret;
		}
	}

	bot = new MacroBot();

	if (buff == NULL)
		bot->doFile(argv[1]);
	else
		bot->doString(buff);

	delete bot;

	if (buff != NULL)
		free(buff);

	return 0;
}
