#pragma once
#include "stdafx.h"


class FCFS :public Processor
{
	Queue<Process*> __READY_QUEUE__;
	Queue<Process*> __MIGRATING__;
	static Queue<Kill_signal> __KILL__;
	static int MAXW;
	static int nkilled;
	//int nMigrated = 0;
public:
	//=========================== Constructors ============================//
	FCFS(int);
	~FCFS();
	//============================ Operations =============================//

	int getID() override;
	void add(Process*) override;
	Process* spell() override;
	Process* spellUnforked();
	int getCT() override;
	void setCT() override;
	int getQT() override;
	void setQT() override;
	bool emptyRQ()override;
	Process* getCurrent() override;
	void setCurrent(Process*) override;
	void run(int) override;
	Queue<Process*> migrate();
	void emptyMigrating();
	//Process* steal() override;
	void kill(int, int);
	static void enqueKill(Kill_signal&);
	static void dequeKill();
	static Kill_signal frontKill();
	void killOrphan(int);
	void print() override;
	void showCurrent() override;
	void updateWait() override;
	int getBusy() override;
	int getIdl() override;
	void overHeating(int) override;
	bool getOverHeating() override;
	friend class Scheduler;
};