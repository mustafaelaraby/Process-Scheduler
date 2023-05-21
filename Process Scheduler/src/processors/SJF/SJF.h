#pragma once

#include "stdafx.h"


class SJF :public Processor
{
	Priority_Queue<Process*, comp_less > __READY_QUEUE__;
public:
	//=========================== Constructors ============================//
	SJF(int);
	~SJF();
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
	//Process* steal() override;
	void print() override;
	void showCurrent() override;
	void updateWait() override;
	int getBusy() override;
	int getIdl() override;
	void overHeating(int) override;
	bool getOverHeating() override;
};
//================================ End ================================//