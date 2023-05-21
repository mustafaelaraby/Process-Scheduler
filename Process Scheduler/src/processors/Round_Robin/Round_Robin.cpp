#include "Round_Robin.h"

//=========================== Constructors ============================//
Round_Robin::Round_Robin(int slice_time, int rtf, int overheat) :SLICE_TIME(slice_time), RTF(rtf)
{
	overHeatLimit = overheat;
	Type = "RR";
	ID = count++;
	__CURRENT__ = nullptr;
}

Round_Robin::~Round_Robin()
{
	std::cout << "From Round_Robin DESTRUCTOR" << std::endl;
}

//============================ Operations =============================//
int Round_Robin::getID()
{
	return ID;
}

void Round_Robin::add(Process* process)
{
	__READY_QUEUE__.push(process);
	(__CT__) += (process->getCPU() - process->getStep(C));
}

Process* Round_Robin::spell()
{
	if (!__READY_QUEUE__.empty())
	{
		Process* process = __READY_QUEUE__.front();
		__READY_QUEUE__.pop();
		return process;
	}
	else
	{
		return nullptr;
	}
}

int Round_Robin::getCT()
{
	return __CT__;
}

void Round_Robin::setCT()
{
	if (__CURRENT__ != nullptr)
		__CT__ = __CURRENT__->getCPU() - __CURRENT__->getStep(C);
	else
		__CT__ = 0;

	Queue<Process*> Ready_Queue = __READY_QUEUE__;
	while (!Ready_Queue.empty())
	{
		__CT__ += (Ready_Queue.front()->getCPU() - Ready_Queue.front()->getStep(C));
		Ready_Queue.pop();
	}
}

int Round_Robin::getQT()
{
	return QT;
}

void Round_Robin::setQT()
{
	QT = 0;
	Queue<Process*> queue = __READY_QUEUE__;
	while (!queue.empty())
	{
		QT += (queue.front()->getCPU() - queue.front()->getStep(C));
		queue.pop();
	}
}

bool Round_Robin::emptyRQ()
{
	return __READY_QUEUE__.empty();
}

void Round_Robin::updateWait()
{
	Queue<Process*> Ready_Queue = __READY_QUEUE__;
	while (!Ready_Queue.empty())
	{
		Ready_Queue.front()->incrementWait();
		Ready_Queue.pop();
	}
}

Process* Round_Robin::getCurrent()
{
	return __CURRENT__;
}

void Round_Robin::setCurrent(Process* current)
{
	this->__CURRENT__ = current;
}

Queue<Process*>& Round_Robin::migrate()
{
	return __MIGRATING__;
}

void Round_Robin::emptyMigrating()
{
	while (!__MIGRATING__.empty())
	{
		__MIGRATING__.pop();
	}
}

void Round_Robin::run(int step)
{
	if (!overHeated)
	{
		overHeating(step);
		updateWait();
		if (__CURRENT__ == nullptr)
		{

			while (!__READY_QUEUE__.empty() && ((__READY_QUEUE__.front()->getCPU() - __READY_QUEUE__.front()->getStep(C)) < RTF))
			{
				__MIGRATING__.push(__READY_QUEUE__.front());
				__READY_QUEUE__.pop();
				setCT();
				setQT();
			}

			if (!__READY_QUEUE__.empty())
			{
				++busy_counter;
				__CURRENT__ = __READY_QUEUE__.front();
				__CURRENT__->setResponse((step - __CURRENT__->getArrival()));
				__READY_QUEUE__.pop();
				__CURRENT__->setStutus(RUN);
				__CURRENT__->setProcessor("RR");
				__CURRENT__->incrementStep(C);
				__CURRENT__->incrementStep(R);
				setCT();
				setQT();
			}
			else
			{
				++idl_counter;
			}
		}
		else if (!(__CURRENT__->getStep(C) % 3))
		{
			++busy_counter;
			__CURRENT__->setStutus(READY);
			__READY_QUEUE__.push(__CURRENT__);
			__CURRENT__ = __READY_QUEUE__.front();
			__CURRENT__->setStutus(RUN);
			__CURRENT__->setProcessor("RR");
			__CURRENT__->incrementStep(C);
			__CURRENT__->incrementStep(R);
			__READY_QUEUE__.pop();
			setCT();
			setQT();
		}
		else
		{
			++busy_counter;
			__CURRENT__->setStutus(RUN);
			__CURRENT__->setProcessor("RR");
			__CURRENT__->incrementStep(C);
			__CURRENT__->incrementStep(R);
			--__CT__;
		}
	}
}

void Round_Robin::print()
{
	int idWidth = 9;
	int typeWidth = 9;
	int sizeWidth = 9;
	int prSize = 101;

	printf("║%03d%*s║%-*s%*s║%03d%*s║", ID, idWidth - 1, " ", typeWidth, Type.c_str(), typeWidth - (int)Type.length() - 5, " ", __READY_QUEUE__.size(), sizeWidth - 1, " ");

	Queue<Process*> readyQueue = __READY_QUEUE__;

	// Print processes in the ready queue
	while (!readyQueue.empty())
	{
		printf("%04d, ", readyQueue.front()->getID());
		readyQueue.pop();
	}

	printf("%*s║\n", prSize - (__READY_QUEUE__.size() * 6), " ");
	printf("╠═══════════╬═══════════╬═══════════╬═════════════════════════════════════════════════════════════════════════════════════════════════════╣\n");

}

void Round_Robin::showCurrent()
{
	if (__CURRENT__ == nullptr)
		std::cout << "CURRENT RR: " << "NONE" << std::endl;
	else
		std::cout << "CURRENT RR: " << __CURRENT__->getStep(C) << std::endl;
}

int Round_Robin::getBusy()
{
	return busy_counter;
}

int Round_Robin::getIdl()
{
	return idl_counter;
}

void Round_Robin::overHeating(int step)
{
	if (step > overHeatLimit)
	{
		bool f_cond = eventWithProbability(0.01);
		switch (f_cond)
		{
		case true:overHeated = true; break;
		case false:overHeated = false; break;
		default:overHeated = false; break;
		}
	}
}

bool Round_Robin::getOverHeating()
{
	return overHeated;
}


//================================ End ================================//
