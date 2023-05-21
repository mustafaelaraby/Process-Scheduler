#include "SJF.h"


//================================ Constructors ============================//
SJF::SJF(int overheat)
{
	overHeatLimit = overheat;
	Type = "SJF";
	ID = count++;
	__CURRENT__ = nullptr;

}

SJF::~SJF()
{
	std::cout << "From SJF DESTRUCTOR" << std::endl;
}

//================================ Operations ==============================//

int SJF::getID()
{
	return ID;
}

void SJF::add(Process* process)
{
	__READY_QUEUE__.push(process);
	__CT__ += (process->getCPU() - process->getStep(C));
}

Process* SJF::spell()
{
	if (!__READY_QUEUE__.empty())
	{
		Process* process = __READY_QUEUE__.top();
		__READY_QUEUE__.pop();
		return process;
	}
	else
	{
		return nullptr;
	}
}

int SJF::getCT()
{
	return __CT__;
}

void SJF::setCT()
{
	if (__CURRENT__ != nullptr)
		__CT__ = __CURRENT__->getCPU() - __CURRENT__->getStep(C);
	else
		__CT__ = 0;

	Priority_Queue<Process*, comp_less> Ready_Queue = __READY_QUEUE__;
	while (!Ready_Queue.empty())
	{
		__CT__ += (Ready_Queue.top()->getCPU() - Ready_Queue.top()->getStep(C));
		Ready_Queue.pop();
	}
}

int SJF::getQT()
{
	return QT;
}

void SJF::setQT()
{
	QT = 0;
	Priority_Queue<Process*, comp_less> queue = __READY_QUEUE__;
	while (!queue.empty())
	{
		QT += (queue.top()->getCPU() - queue.top()->getStep(C));
		queue.pop();
	}
}

bool SJF::emptyRQ()
{
	return __READY_QUEUE__.empty();
}

void SJF::updateWait()
{
	Priority_Queue<Process*, comp_less> Ready_Queue = __READY_QUEUE__;
	while (!Ready_Queue.empty())
	{
		Ready_Queue.top()->incrementWait();
		Ready_Queue.pop();
	}
}

Process* SJF::getCurrent()
{
	return __CURRENT__;
}

void SJF::setCurrent(Process* current)
{
	this->__CURRENT__ = current;
}

void SJF::run(int step)
{
	if (!overHeated)
	{
		overHeating(step);
		updateWait();
		if (__CURRENT__ == nullptr)
		{
			if (!__READY_QUEUE__.empty())
			{
				++busy_counter;
				__CURRENT__ = __READY_QUEUE__.top();

				if (!__CURRENT__->getStep(C))
					__CURRENT__->setResponse((step - __CURRENT__->getArrival()));
				__READY_QUEUE__.pop();
				__CURRENT__->setStutus(RUN);
				__CURRENT__->setProcessor("SJF");
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
		else
		{
			++busy_counter;
			__CURRENT__->setStutus(RUN);
			__CURRENT__->setProcessor("SJF");
			__CURRENT__->incrementStep(C);
			__CURRENT__->incrementStep(R);
			--__CT__;
		}
	}
}

void SJF::print()
{
	int idWidth = 9;
	int typeWidth = 9;
	int sizeWidth = 9;
	int prSize = 101;

	printf("║%03d%*s║%-*s%*s║%03d%*s║", ID, idWidth - 1, " ", typeWidth, Type.c_str(), typeWidth - (int)Type.length() - 4, " ", __READY_QUEUE__.size(), sizeWidth - 1, " ");

	Priority_Queue<Process*, comp_less> readyQueue = __READY_QUEUE__;

	// Print processes in the ready queue
	while (!readyQueue.empty())
	{
		printf("%04d, ", readyQueue.top()->getID());
		readyQueue.pop();
	}

	printf("%*s║\n", prSize - (__READY_QUEUE__.size() * 6), " ");
	printf("╠═══════════╬═══════════╬═══════════╬═════════════════════════════════════════════════════════════════════════════════════════════════════╣\n");

}

void SJF::showCurrent()
{
	if (__CURRENT__ == nullptr)
		std::cout << "CURRENT SJF: " << "NONE" << std::endl;
	else
		std::cout << "CURRENT SJF: " << __CURRENT__->getCPU() << " " << __CURRENT__->getStep(C) << std::endl;
}

int SJF::getBusy()
{
	return busy_counter;
}

int SJF::getIdl()
{
	return idl_counter;
}

void SJF::overHeating(int step)
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

bool SJF::getOverHeating()
{
	return overHeated;
}


//================================ End ================================//