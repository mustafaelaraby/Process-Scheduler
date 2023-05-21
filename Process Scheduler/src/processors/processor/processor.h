#pragma once
#include "../../Process/Process.h"


class Processor
{
protected:
	Process* __CURRENT__ = nullptr;
	int __CT__ = 0;
	int QT;
	std::string Type;
	inline static int count = 1;
	int ID;
	int busy_counter = 0;
	int idl_counter = 0;
	int overHeatLimit;
	bool overHeated = false;
public:
	//=========================== Operations ============================//

	virtual int getID() = 0;
	virtual void add(Process*) = 0;
	virtual int getCT() = 0;
	virtual void setCT() = 0;
	virtual int getQT() = 0;
	virtual void setQT() = 0;
	virtual bool emptyRQ() = 0;
	virtual Process* getCurrent() = 0;
	virtual void setCurrent(Process*) = 0;
	virtual void run(int) = 0;
	virtual Process* spell() = 0;
	virtual void print() = 0;
	virtual void showCurrent() = 0;
	virtual void updateWait() = 0;
	virtual int getBusy() = 0;
	virtual int getIdl() = 0;
	virtual void overHeating(int) = 0;
	virtual bool getOverHeating() = 0;
};