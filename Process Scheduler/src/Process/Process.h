#pragma once

#include "stdafx.h"

class Process
{
	int id, arrival_time, response_time, cpu_time,
		step, stepR, stepD, trm_time, trt_time, fork_num,
		wait_time, num_io, blocked_time, deadline;
	bool forked;
	STUTUS status;
	std::string processor;
	Queue<int> IOR;
	Queue<int> IOD;

	Process* left = nullptr;
	Process* right = nullptr;

public:
	//=========================== Constructors ============================//
	Process();
	Process(const Process&);
	Process(Process&&) noexcept;
	~Process();

	//============================== Getters ==============================//
	int getID()					   const;
	int getArrival()			   const;
	int getResponse()			   const;
	int getCPU()				   const;
	int getStep(FLAG)			   const;
	int getTRM()				   const;
	int getTRT()				   const;
	int getWait()				   const;
	int getNumIO()				   const;
	bool isForked()				   const;
	bool canFork()				   const;
	int getBLK()				   const;
	int getDeadline()			   const;
	int getIO(FLAG);
	std::string getProcessor()     const;
	STUTUS getStatus()             const;

	//============================== Setters ==============================//
	void setID(int);
	void setArrival(int);
	void setResponse(int);
	void setCPU(int);
	void incrementStep(FLAG);
	void setTRM(int);
	void setTRT(int);
	void setWait(int);
	void setNumIO(int);
	void setForked(bool);
	void setBLK(int);
	void setDeadline(int);
	void setProcessor(std::string);
	void setStutus(STUTUS);
	void IO_Push(int, FLAG);
	void IO_Pop(FLAG);
	void setStep(int, FLAG);
	void incrementBlk();
	void incrementWait();

	//============================= Binary Tree Functions =============================//
public:
	void insert(Process*);
	void erase(int);
	bool contains(int) const;
	const Process* find(int) const;
	std::size_t size() const;
	bool empty() const;
	Queue<int>getAllIDs() const;
	void TerminateParent(STUTUS);
private:
	void getAllIDsHelper(Queue<int>& idQueue) const;

	//============================= Operators =============================//
public:
	Process& operator=(const Process&);
	Process& operator=(Process&&);
};
