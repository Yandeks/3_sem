#include <shwlwrp.h>

int main()
{
	char command[MAXSIZE];
	char stopword = "exit program";
	int run = 1;

	while(run)
	{
		switch(Readline(command))
		{
			case 0:
			{
				printf("__INVALID_MESSAGE__\n");

				while(!Readline(command));
				break;
			}

			case 1:
			{
				if(!strcmp(command, stopword))
				{
					run = 0;
					break;
				}

				prog_run(command);
				break;
			}
		}

	}

return 0;

}