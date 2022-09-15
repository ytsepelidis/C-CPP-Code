#include <iostream>
#include "Replica.h"
#include "DCS.h"

int main()
{
	srand((unsigned int)time(NULL));
	int instances;

	std::cout << "You are a client in a Distributed Computing System.\n";
	std::cout << "You have to enter the number of instances you want the system to accommodate.\n";
	std::cout << "\n";

	// Entering valid number of replicas for the DCS
	while (1)
	{
		std::cout << "Please enter a number of instances (minimum of 3): ";
		std::cin >> instances;

		if (instances < 3)
		{
			std::cout << "There cannot be a majority of processes with that number.\n";
		}
		else
		{
			break;
		}
	}
	std::cout << "\n";

	// Initializing the system
	DCS system(instances);

	// Starting the process
	system.StartSession();

	return 0;
}