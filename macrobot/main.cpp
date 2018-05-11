#include "stdafx.h"
#include "MacroBot.h"

int main(int argc, char * argv[])
{
	MacroBot * bot = NULL;

	if (argc < 2) {
		printf("Insufficient arguments\n");

		return -1;
	}

	bot = new MacroBot();

	bot->doFile(argv[1]);

	delete bot;

	return 0;
}
