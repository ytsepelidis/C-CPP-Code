#pragma once
#include <iostream>
#include <array>
#include <vector>
#include <thread>
#include "Replica.h"

class DCS
{
private:
	std::vector<Replica> replicas;
	static std::array<std::string, 10> commands;
	std::vector<std::string> commandsToIssue;		// the batch of commands

	int P;		// Set of all processes
	int Q;		// Majority set of processes
	int leaderPos;

	std::string prevDecLog;
	std::vector<std::string> commandLog;
	uint64_t currBalLog;
	uint64_t prevBalLog;
public:
	DCS(int instances);

	void StartSession();
	void PrintCommands();
	void EnterCommand(std::string& d);
	bool IsCommand(std::string& d);
	void BecomeLeader(Replica& rep);
	void BallotInitialization();
	void Voting(Replica& leaderRep);
	void ReplicasStatus();
	int FindLeader();
	void SetCurrBalAll(Replica& rep);
	void SetPrevBalAll(Replica& rep);
	void IssueCommandAll(Replica& leaderRep);
	void CrashReplicas(const int crashedReps);
	void CatchUpProtocol(const int crashedReps);
};