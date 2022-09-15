#include "DCS.h"

std::array<std::string, 10> DCS::commands = {
	"cat", "dog", "catdog", "parrot", "hamster", "pizza", "burger", "fries", "pasta", "noodles"
};

DCS::DCS(int instances)
{
	replicas.reserve(instances);
	for (int i = 0; i < instances; i++)
	{
		replicas.emplace_back(Replica());
	}

	P = (int) replicas.size();
	Q = (P / 2) + 1;

	std::cout << "Consturcted a total of " << P << " replicas and there is a majority of " << Q << " replicas.\n";
	std::cout << "\n";
}

void DCS::StartSession()
{
	std::string command;
	bool hasCommand = false;
	int crashedRepsLim = P - Q;
	int crashedReps = 0;
	bool nextRound = true;
	int randRep;

	std::cout << "The JPaxos algorithm will be executed until the client enters 'quit'.\n";
	std::cout << "\n";
	
	// Main loop
	while (1)
	{
		// Simulating crashed replicas
		if (nextRound)
		{
			std::cout << "~~~~~~~~~~~~ A round of JPaxos begis now ~~~~~~~~~~~~\n";
			std::cout << "\n";

			std::cout << "You have to enter a number for the simulated crashed replicas.\n";

			while (1)
			{
				std::cout << "Please enter an integer (0 - " << crashedRepsLim << "): ";
				std::cin >> crashedReps;

				if (crashedReps >= 0 && crashedReps <= crashedRepsLim)
				{
					std::cout << crashedReps << " replica(s) will be offline for this round of commands.\n";
					CrashReplicas(crashedReps);
					nextRound = false;

					break;
				}
				else
				{
					std::cout << "Please enter a valid number. ";
				}
			}
			
			std::cout << "\n";
		}

		std::cout << "Please enter one of the following commands, 'stop' to issue the commands given so far or 'quit'.\n";
		PrintCommands();

		std::cout << "Please enter a command: ";
		std::cin >> command;
		std::cout << "\n";

		if (command == "quit")
		{
			std::cout << "You want to exit the program. Exiting program . . .\n";
			break;
		}
		else if (command == "stop")
		{
			if (hasCommand)
			{
				std::cout << "You entered 'stop', the ballot initialization phase will begin.\n";
				std::cout << "\n";

				// Beginning phase 1
				BallotInitialization();

				// Finding leader position in replicas
				leaderPos = FindLeader();

				if (leaderPos == -1)
				{
					std::cout << "Could not find a leader. Moving to next round...\n";
				}
				else
				{
					// Voting for a command
					Replica& leaderRep = replicas[leaderPos];

					// Beginning phase 2
					Voting(leaderRep);

					// Storing log information for Catch-Up Protocol
					prevBalLog = leaderRep.get_prevBal();
					currBalLog = leaderRep.get_currBal();
					prevDecLog = leaderRep.get_prevDec();
					
					for (int i = 0; i < leaderRep.get_issuedCnt(); i++)
					{
						commandLog.emplace_back(leaderRep.get_command(i));
					}

					ReplicasStatus();

					std::cout << "~~~~~ The CatchUpProtocol() will be called ~~~~~\n";
					std::cout << "The Catch-Up Protocol has been called in order to bring crashed replicas up to date.\n";
					std::cout << "\n";

					// Catch-Up Protocol
					CatchUpProtocol(crashedReps);

					ReplicasStatus();

					// Reseting leader
					leaderRep.ResetReplicaVotes();
					leaderRep.set_isLeader(false);
					leaderRep.SetLeaderProcId(-1);
					leaderRep.FireLeader();

					commandLog.clear();
				}

				// Clearing commands for next round of commands
				commandsToIssue.clear();
				hasCommand = false;
				nextRound = true;

				std::cout << "\n~~~~~~~~~~~~ This round of JPaxos has ended ~~~~~~~~~~~~\n";
			}
			else
			{
				std::cout << "No command has been proposed so the ballot initialization phase will not begin.\n";
				std::cout << "A valid command, or batch of commands, has to be proposed for the phase to begin.\n";
			}
		}
		else
		{
			if (IsCommand(command))
			{
				EnterCommand(command);
				hasCommand = true;

				std::cout << "Command '" << command << "' will be issued if consensus is reached.\n";
				std::cout << "You are free to enter another command for the batch of commands.\n";
			}
			else
			{
				std::cout << "You have to enter a valid command!\n";
			}
		}

		std::cout << "\n";
	}

	/*
	randRep = rand() % replicas.size();
	std::cout << "Client sending a command to a random replica (randRep: " << randRep << ")\n";

	if (!replicas[randRep].get_isLeader())
	{
		std::cout << "Random replica procId: " << replicas[randRep].get_procId() << "\n";
		std::cout << "Leader replica procId: " << replicas[leaderPos].get_procId() << "\n";
		std::cout << "\n";
	}
	else
	{

	}
	*/
}

void DCS::PrintCommands()
{
	for (int i = 0; i < commands.size(); i++)
	{
		if (i == commands.size() - 1)
		{
			std::cout << "'" << commands[i] << "'\n";
		}
		else
		{
			std::cout << "'" << commands[i] << "', ";
		}
	}
}

void DCS::EnterCommand(std::string& d)
{
	commandsToIssue.emplace_back(d);
}

bool DCS::IsCommand(std::string& d)
{
	for (auto& command : commands)
	{
		if (command == d)
		{
			return true;
		}
	}

	return false;
}

void DCS::BecomeLeader(Replica& rep)
{
	uint64_t ballot = rep.GenerateBalNum();
	rep.set_currBal(ballot);

	// send() to P
	for (auto& replica : replicas)
	{
		// Excluding crashed replicas
		if (!replica.get_CrashStatus())
		{
			// upon Prepare() from q
			if (rep.Prepare(ballot, replica))
			{
				rep.IncReplicaVotes();
			}

			// upon PrepareOK() from Q
			if (rep.get_replicaVotes() >= Q)
			{
				std::cout << "Replica with ID: " << rep.get_procId() << " was elected as a leader!\n";
				std::cout << "Leader has currBall: " << rep.get_currBal() << "\n";
				std::cout << "\n";

				rep.SetLeaderProcId(rep.get_procId());
				rep.set_isLeader(true);
				rep.ElectLeader();
				SetCurrBalAll(rep);

				break;
			}
		}
	}
}

void DCS::BallotInitialization()
{
	// Concurrently
	/*
	std::vector<std::thread> threadPool;

	for (int i = 0; i < replicas.size(); i++)
	{
		std::thread t;
		threadPool.emplace_back(std::move(t));
	}

	for (int i = 0; i < threadPool.size(); i++)
	{
		// threadPool[i] = std::thread(&DCS::BecomeLeader, this, std::ref(replicas[i]));

		// Excluding crashed replicas
		if (!replicas[i].get_CrashStatus())
		{
			threadPool[i] = std::thread(&DCS::BecomeLeader, this, std::ref(replicas[i]));
			replicas[i].ResetReplicaVotes();
		}

		if (replicas[i].IsLeaderElected())
		{
			break;
		}
	}

	for (int i = 0; i < threadPool.size(); i++)
	{
		threadPool[i].join();
	}
	*/

	std::cout << "~~~~~ The BallotInitialization() phase has begun ~~~~~\n";
	std::cout << "All replicas will try to be elected as leader.\n";
	std::cout << "\n";

	// Trying to elect a leader
	for (auto& replica : replicas)
	{
		// replica.set_isLeader(false);

		// Excluding crashed replicas
		if (!replica.get_CrashStatus())
		{
			BecomeLeader(replica);
			replica.ResetReplicaVotes();
		}

		if (replica.IsLeaderElected())
		{
			break;
		}
	}

	ReplicasStatus();
}

void DCS::Voting(Replica& leaderRep)
{
	bool commandIssued = false;

	std::cout << "~~~~~ The Voting() phase has begun ~~~~~\n";
	std::cout << "The leader will message all the other replicas in order to reach consensus for a command.\n";
	std::cout << "\n";

	// Voting() for command or batch of commands
	for (const auto& command : commandsToIssue)
	{
		std::cout << "Leader will try to reach consensus for command '" << command << "'.\n";
		std::cout << "\n";

		// Setting command for leader
		leaderRep.set_prevDec(command);

		// send() to P
		for (auto& replica : replicas)
		{
			// Excluding crashed replicas
			if (!replica.get_CrashStatus())
			{
				// upon Propose() from q
				if (leaderRep.Propose(replica, leaderRep.get_currBal(), command))
				{
					replica.set_prevDec(command);
					leaderRep.IncReplicaVotes();
				}

				// upon Accept() from Q
				if (leaderRep.get_replicaVotes() >= Q)
				{
					std::cout << "Leader with ID: " << leaderRep.get_procId() << " can execute command '" << command << "'.\n";
					std::cout << "\n";

					IssueCommandAll(leaderRep);
					SetPrevBalAll(leaderRep);
					commandIssued = true;

					break;
				}
			}
		}

		if (commandIssued)
		{
			std::cout << "Command '" << command << "' has been issued for all available replicas.\n";
			std::cout << "\n";

			commandIssued = false;
		}
		else
		{
			std::cout << "Voting phase was not passed for command '" << command << "'.\n";
			std::cout << "\n";
		}
	}
}

void DCS::ReplicasStatus()
{
	std::cout << "~~~~~~~~~ Printing replica status ~~~~~~~~~\n";

	if (Replica::IsLeaderElected())
	{
		std::cout << "A leader has been elected.\n";
		std::cout << "Leader procId: " << Replica::GetLeaderProcId() << '\n';
	}
	else
	{
		std::cout << "A leader has NOT been elected!!!\n";
	}

	for (auto& replica : replicas)
	{
		std::cout << "~~~~~~~~~~~~~~~~~~~~~\n";
		std::cout << "Replica procId: " << replica.get_procId() << '\n';
		std::cout << "Replica votes: " << replica.get_replicaVotes() << '\n';
		std::cout << "Replica isLeader: " << replica.get_isLeader() << '\n';
		std::cout << "Replica currBal: " << replica.get_currBal() << '\n';
		std::cout << "Replica prevBal: " << replica.get_prevBal() << '\n';
		std::cout << "Replica prevDec: " << replica.get_prevDec() << '\n';
		std::cout << "Replica crash status: " << replica.get_CrashStatus() << '\n';
		std::cout << "Replica no. of commands issued: " << replica.get_issuedCnt() << '\n';
		std::cout << "Replica issued commands:\n";
		replica.PrintIssuedCommands();

		std::cout << "\n";
	}

	std::cout << "\n";
}

int DCS::FindLeader()
{
	for (int i = 0; i < replicas.size(); i++)
	{
		if (replicas[i].get_procId() == replicas[i].GetLeaderProcId())
		{
			return i;
		}
	}

	return -1;
}

void DCS::SetCurrBalAll(Replica& rep)
{
	for (auto& replica : replicas)
	{
		// Excluding crashed replicas
		if (!replica.get_CrashStatus())
		{
			if (replica.get_currBal() != rep.get_currBal())
			{
				replica.set_currBal(rep.get_currBal());
			}
		}
	}
}

void DCS::SetPrevBalAll(Replica& rep)
{
	for (auto& replica : replicas)
	{
		// Excluding crashed replicas
		if (!replica.get_CrashStatus())
		{
			replica.set_prevBal(rep.get_currBal());
		}
	}
}

void DCS::IssueCommandAll(Replica& leaderRep)
{
	std::string d = leaderRep.get_prevDec();

	for (auto& replica : replicas)
	{
		// Excluding crashed replicas
		if (!replica.get_CrashStatus())
		{
			if (replica.get_prevDec() != leaderRep.get_prevDec())
			{
				replica.set_prevDec(leaderRep.get_prevDec());
			}

			replica.IssueCommand(d);
		}
	}
}

void DCS::CrashReplicas(const int crashedReps)
{
	for (int i = 0; i < replicas.size(); i++)
	{
		if (i < crashedReps)
		{
			replicas[i].set_CrashStatus(true);
		}
		else
		{
			replicas[i].set_CrashStatus(false);
		}
	}
}

void DCS::CatchUpProtocol(const int crashedReps)
{
	int pos;

	for (auto& replica : replicas)
	{
		if (!replica.get_isLeader())
		{
			if (replica.get_issuedCnt() < commandLog.size())
			{
				// Recovering replica will be updated
				replica.set_currBal(currBalLog);
				replica.set_prevBal(prevBalLog);
				replica.set_prevDec(prevDecLog);

				if (replica.get_issuedCnt() == 0)
				{
					pos = 0;
				}
				else
				{
					pos = (int)commandLog.size() - replica.get_issuedCnt();
				}

				for (int i = pos; i < commandLog.size(); i++)
				{
					replica.IssueCommand(commandLog[i]);
				}
			}
		}
	}
}