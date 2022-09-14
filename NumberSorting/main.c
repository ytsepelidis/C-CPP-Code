#include <stdio.h>

int main()
{
	int i1, i2, i3, i4;

	// Reading the 4 integers
	printf("Please, type four integers: ");
	scanf_s("%d %d %d %d", &i1, &i2, &i3, &i4);
	printf("You typed the integers: %d, %d, %d and %d\n", i1, i2, i3, i4);

	// Sorting 4 integers without implementing loops
	// There are 4! = 4 * 3 * 2 * 1 = 24 different ways the 4 variables could be sorted
	// I have also implemented the possibility of the integers being equal
	if (i1 <= i2)
	{
		if (i2 <= i3 && i2 <= i4)
		{
			if (i3 <= i4)
			{
				printf("The sorted order is: %d, %d, %d, %d\n", i1, i2, i3, i4);
			}
			else
			{
				printf("The sorted order is: %d, %d, %d, %d\n", i1, i2, i4, i3);
			}
		}
		else if (i2 <= i3 || i2 <= i4)
		{
			if (i2 <= i3)
			{
				if (i1 <= i4)
				{
					printf("The sorted order is: %d, %d, %d, %d\n", i1, i4, i2, i3);
				}
				else
				{
					printf("The sorted order is: %d, %d, %d, %d\n", i4, i1, i2, i3);
				}
			}
			else if (i2 <= i4)
			{
				if (i1 <= i3)
				{
					printf("The sorted order is: %d, %d, %d, %d\n", i1, i3, i2, i4);
				}
				else
				{
					printf("The sorted order is: %d, %d, %d, %d\n", i3, i1, i2, i4);
				}
			}
		}
		else if (i3 <= i2 && i4 <= i2)
		{
			if (i1 <= i3 && i1 <= i4)
			{
				if (i3 <= i4)
				{
					printf("The sorted order is: %d, %d, %d, %d\n", i1, i3, i4, i2);
				}
				else
				{
					printf("The sorted order is: %d, %d, %d, %d\n", i1, i4, i3, i2);
				}
			}
			else if (i1 <= i3 || i1 <= i4)
			{
				if (i1 <= i3)
				{
					printf("The sorted order is: %d, %d, %d, %d\n", i4, i1, i3, i2);
				}
				else
				{
					printf("The sorted order is: %d, %d, %d, %d\n", i3, i1, i4, i2);
				}
			}
			else if (i3 <= i1 && i4 <= i1)
			{
				if (i3 <= i4)
				{
					printf("The sorted order is: %d, %d, %d, %d\n", i3, i4, i1, i2);
				}
				else
				{
					printf("The sorted order is: %d, %d, %d, %d\n", i4, i3, i1, i2);
				}
			}
		}
	}
	else
	{
		if (i1 <= i3 && i1 <= i4)
		{
			if (i3 <= i4)
			{
				printf("The sorted order is: %d, %d, %d, %d\n", i2, i1, i3, i4);
			}
			else
			{
				printf("The sorted order is: %d, %d, %d, %d\n", i2, i1, i4, i3);
			}
		}
		else if (i1 <= i3 || i1 <= i4)
		{
			if (i1 <= i3)
			{
				if (i2 <= i4)
				{
					printf("The sorted order is: %d, %d, %d, %d\n", i2, i4, i1, i3);
				}
				else
				{
					printf("The sorted order is: %d, %d, %d, %d\n", i4, i2, i1, i3);
				}
			}
			else if (i1 <= i4)
			{
				if (i2 <= i3)
				{
					printf("The sorted order is: %d, %d, %d, %d\n", i2, i3, i1, i4);
				}
				else
				{
					printf("The sorted order is: %d, %d, %d, %d\n", i3, i2, i1, i4);
				}
			}
		}
		else if (i3 <= i1 && i4 <= i1)
		{
			if (i2 <= i3 && i2 <= i4)
			{
				if (i3 <= i4)
				{
					printf("The sorted order is: %d, %d, %d, %d\n", i2, i3, i4, i1);
				}
				else
				{
					printf("The sorted order is: %d, %d, %d, %d\n", i2, i4, i3, i1);
				}
			}
			else if (i2 <= i3 || i2 <= i4)
			{
				if (i2 <= i3)
				{
					printf("The sorted order is: %d, %d, %d, %d\n", i4, i2, i3, i1);
				}
				else
				{
					printf("The sorted order is: %d, %d, %d, %d\n", i3, i2, i4, i1);
				}
			}
			else if (i3 <= i2 && i4 <= i2)
			{
				if (i3 <= i4)
				{
					printf("The sorted order is: %d, %d, %d, %d\n", i3, i4, i2, i1);
				}
				else
				{
					printf("The sorted order is: %d, %d, %d, %d\n", i4, i3, i2, i1);
				}
			}
		}
	}

	return 0;
}
