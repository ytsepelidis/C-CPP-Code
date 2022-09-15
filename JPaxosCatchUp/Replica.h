#pragma once
#include <iostream>
#include <vector>
#include <array>
#include <thread>
#include <chrono>
#include <cstdlib>

class Replica
{
private:
	static int procIdInit;
	static int LeaderProcId;
	static bool leaderElected;

	int replicaVotes;
	int procId;
	bool isLeader;
	uint64_t currBal;
	uint64_t prevBal;
	std::string prevDec;
	std::vector<std::string> issued;
	int issuedCnt;
	bool hasCrashed;
public:
	Replica();

	void set_procId(int procId);
	void set_isLeader(bool isLeader);
	void set_currBal(uint64_t currBal);
	void set_prevBal(uint64_t prevBal);
	void set_prevDec(std::string prevDec);
	void set_CrashStatus(bool hasCrashed);

	int get_procId() const;
	int get_replicaVotes() const;
	bool get_isLeader() const;
	uint64_t get_currBal() const;
	uint64_t get_prevBal() const;
	std::string get_prevDec() const;
	int get_issuedCnt() const;
	bool get_CrashStatus() const;
	std::string get_command(int pos) const;

	void PrintIssuedCommands() const;
	void IncReplicaVotes();
	void ResetReplicaVotes();
	bool Prepare(const uint64_t& currBal, Replica& rep);
	bool PrepareOK(const uint64_t& currBal, Replica& rep);
	bool Propose(Replica& rep, const uint64_t& currBal, const std::string& d);
	bool Accept(Replica& rep, const uint64_t& currBal);
	void IssueCommand(std::string& d);

	static void IncProcId();
	static uint64_t GenerateBalNum();
	static void SetLeaderProcId(int procId);
	static int GetLeaderProcId();
	static void ElectLeader();
	static void FireLeader();
	static bool IsLeaderElected();
};