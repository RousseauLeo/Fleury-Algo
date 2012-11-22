#include <stdlib.h>
#include <stdio.h>

int main(int argc, char ** argv)
{
	int n = 0, i = 0, j = 0;
	
	if(argc < 2)
	{
		printf("usage: ./%s N\n", argv[0]);
		return 0;
	}
	
	n = atoi(argv[1]);
	
	printf("%d\n", n);
	
	for(i = 0; i < n; ++i)
	{
		for(j = 0; j < n; ++j)
		{
			if(j != i)
				printf("%d ", j);
		}
		printf("-1\n");
	}
	
	printf("\n");
	
	return 0;
}
