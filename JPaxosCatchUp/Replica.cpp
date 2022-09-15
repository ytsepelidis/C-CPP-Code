#include "Replica.h"

using namespace std::chrono;

int Replica::procIdInit = 0;
int Replica::LeaderProcId = -1;
bool Replica::leaderElected = false;

Replica::Replica()
{
	IncProcId();
	procId = procIdInit;

	replicaVotes = 0;
	isLeader = false;
	currBal = 0;
	prevBal = 0;
	issuedCnt = 0;
	hasCrashed = false;
}

void Replica::set_procId(int procId)
{
	this->procId = procId;
}

void Replica::set_isLeader(bool isLeader)
{
	this->isLeader = isLeader;
}

void Replica::set_currBal(uint64_t currBal)
{
	this->currBal = currBal;
}

void Replica::set_prevBal(uint64_t prevBal)
{
	this->prevBal = prevBal;
}

void Replica::set_prevDec(std::string prevDec)
{
	this->prevDec = prevDec;
}

void Replica::set_CrashStatus(bool hasCrashed)
{
	this->hasCrashed = hasCrashed;
}

int Replica::get_procId() const
{
	return procId;
}

int Replica::get_replicaVotes() const
{
	return replicaVotes;
}

bool Replica::get_isLeader() const
{
	return isLeader;
}

uint64_t Replica::get_currBal() const
{
	return currBal;
}

uint64_t Replica::get_prevBal() const
{
	return prevBal;
}

std::string Replica::get_prevDec() const
{
	return prevDec;
}

int Replica::get_issuedCnt() const
{
	return issuedCnt;
}

bool Replica::get_CrashStatus() const
{
	return hasCrashed;
}

std::string Replica::get_command(int pos) const
{
	return issued[pos];
}

void Replica::PrintIssuedCommands() const
{
	for (int i = 0; i < issued.size(); i++)
	{
		if (i == issued.size() - 1)
		{
			std::cout << "'" << issued[i] << "'";
		}
		else
		{
			std::cout << "'" << issued[i] << "', ";
		}
	}
}

void Replica::IncReplicaVotes()
{
	replicaVotes += 1;
}

void Replica::ResetReplicaVotes()
{
	replicaVotes = 0;
}

bool Replica::Prepare(const uint64_t& currBal, Replica& rep)
{
	if (currBal > rep.currBal)
	{
		return rep.PrepareOK(currBal, rep);
	}
	else
	{
		return false;
	}
}

bool Replica::PrepareOK(const uint64_t& currBal, Replica& rep)
{
	// PrepareOK() acceptance rate
	if (rand() % 100 > 30)
	{
		rep.set_currBal(currBal);
		return true;
	}
	else
	{
		return false;
	}
}

bool Replica::Propose(Replica& rep, const uint64_t& currBal, const std::string& d)
{
	if (currBal == rep.currBal)
	{
		return rep.Accept(rep, currBal);
	}
	else
	{
		return false;
	}
}

bool Replica::Accept(Replica& rep, const uint64_t& currBal)
{
	// Accept() acceptance rate
	if (rand() % 100 > 30)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Replica::IssueCommand(std::string& d)
{
	issued.emplace_back(d);
	issuedCnt += 1;
}

void Replica::IncProcId()
{
	procIdInit += 1;
}

uint64_t Replica::GenerateBalNum()
{
	uint64_t ballot;

	// std::this_thread::sleep_for(std::chrono::nanoseconds(1));
	ballot = duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();

	return ballot;
}

void Replica::SetLeaderProcId(int procId)
{
	LeaderProcId = procId;
}

int Replica::GetLeaderProcId()
{
	return LeaderProcId;
}

void Replica::ElectLeader()
{
	leaderElected = true;
}

void Replica::FireLeader()
{
	leaderElected = false;
}

bool Replica::IsLeaderElected()
{
	return leaderElected;
}