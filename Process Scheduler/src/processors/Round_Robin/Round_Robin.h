#pragma once
#include "stdafx.h"


class Round_Robin :public Processor
{
	Queue<Process*> __READY_QUEUE__;
	Queue<Process*> __MIGRATING__;
	int SLICE_TIME;
	int RTF;
public:

	//=========================== Constructors ============================//
	Round_Robin(int, int, int);
	~Round_Robin();

	//============================ Operations =============================//
	int getID() override;
	void add(Process*) override;
	Process* spell() override;
	int getCT() override;
	void setCT() override;
	int getQT() override;
	void setQT() override;
	bool emptyRQ() override;
	Process* getCurrent() override;
	void setCurrent(Process*) override;
	void run(int) override;
	Queue<Process*>& migrate();
	void emptyMigrating();
	void print() override;
	void showCurrent() override;
	void updateWait() override;
	int getBusy() override;
	int getIdl() override;
	void overHeating(int) override;
	bool getOverHeating() override;
};
//================================ End ================================//