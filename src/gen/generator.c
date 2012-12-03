/*
   -----------------------------------------------------------------------------
   Copyright (c) 2012 Léo Rousseau and Aurélien Cavelan
   
   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:
   
   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.
   
   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.
   -----------------------------------------------------------------------------
*/

#include <stdlib.h>
#include <stdio.h>

/* Non-Random generator for Kn graph on the standard output */
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
				printf("%d ", j + 1);
		}
		printf("-1\n");
	}
	
	printf("\n");
	
	return 0;
}
