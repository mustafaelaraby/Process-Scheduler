#include "stdafx.h"


class EDF :
	public Processor
{
	Priority_Queue<Process*, deadline_less> __READY_QUEUE__;
public:
	EDF(int);
	~EDF();
public:
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
	void print() override;
	void showCurrent() override;
	void updateWait() override;
	int getBusy() override;
	int getIdl() override;
	void overHeating(int) override;
	bool getOverHeating() override;
};

