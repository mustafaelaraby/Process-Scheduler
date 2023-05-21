#include "EDF.h"

EDF::EDF(int overheat)
{
	overHeatLimit = overheat;
	ID = count++;
	Type = "EDF";
	__CURRENT__ = nullptr;
}

int EDF::getID()
{
	return ID;
}

void EDF::add(Process* process)
{
	__READY_QUEUE__.push(process);
	__CT__ += (process->getCPU() - process->getStep(C));
}

Process* EDF::spell()
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

int EDF::getCT()
{
	return __CT__;
}

void EDF::setCT()
{
	if (__CURRENT__ != nullptr)
		__CT__ = __CURRENT__->getCPU() - __CURRENT__->getStep(C);
	else
		__CT__ = 0;

	Priority_Queue<Process*, deadline_less> Ready_Queue = __READY_QUEUE__;
	while (!Ready_Queue.empty())
	{
		__CT__ += (Ready_Queue.top()->getCPU() - Ready_Queue.top()->getStep(C));
		Ready_Queue.pop();
	}
}

int EDF::getQT()
{
	return QT;
}

void EDF::setQT()
{
	QT = 0;
	Priority_Queue<Process*, deadline_less> queue = __READY_QUEUE__;
	while (!queue.empty())
	{
		QT += (queue.top()->getCPU() - queue.top()->getStep(C));
		queue.pop();
	}
}

bool EDF::emptyRQ()
{
	return __READY_QUEUE__.empty();
}

void EDF::updateWait()
{
	Priority_Queue<Process*, deadline_less> Ready_Queue = __READY_QUEUE__;
	while (!Ready_Queue.empty())
	{
		Ready_Queue.top()->incrementWait();
		Ready_Queue.pop();
	}
}

Process* EDF::getCurrent()
{
	return __CURRENT__;
}

void EDF::setCurrent(Process* current)
{
	this->__CURRENT__ = current;
}

void EDF::run(int step)
{
	if (!overHeated)
	{
		overHeating(step);
		if (__CURRENT__ == nullptr)
		{
			if (!__READY_QUEUE__.empty())
			{
				++busy_counter;
				__CURRENT__ = __READY_QUEUE__.top();
				__CURRENT__->setResponse((step - __CURRENT__->getArrival()));
				__READY_QUEUE__.pop();
				__CURRENT__->setStutus(RUN);
				__CURRENT__->setProcessor("EDF");
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
			if (!__READY_QUEUE__.empty())
			{
				if (__CURRENT__->getDeadline() > __READY_QUEUE__.top()->getDeadline())
				{
					__CURRENT__->setStutus(READY);
					__READY_QUEUE__.push(__CURRENT__);
					__CURRENT__ = __READY_QUEUE__.top();
					__READY_QUEUE__.pop();
					if (!__CURRENT__->getStep(C))
						__CURRENT__->setResponse((step - __CURRENT__->getArrival()));
					__CURRENT__->setStutus(RUN);
					__CURRENT__->setProcessor("EDF");
					__CURRENT__->incrementStep(C);
					__CURRENT__->incrementStep(R);
					setCT();
					setQT();
				}
				else
				{
					++busy_counter;
					__CURRENT__->setStutus(RUN);
					__CURRENT__->setProcessor("EDF");
					__CURRENT__->incrementStep(C);
					__CURRENT__->incrementStep(R);
					--__CT__;
				}
			}
			else
			{
				++busy_counter;
				__CURRENT__->setStutus(RUN);
				__CURRENT__->setProcessor("EDF");
				__CURRENT__->incrementStep(C);
				__CURRENT__->incrementStep(R);
				--__CT__;
			}
		}

	}
}

void EDF::print()
{
	int idWidth = 9;
	int typeWidth = 9;
	int sizeWidth = 9;
	int prSize = 101;

	printf("║%03d%*s║%-*s%*s║%03d%*s║", ID, idWidth - 1, " ", typeWidth, Type.c_str(), typeWidth - (int)Type.length() - 4, " ", __READY_QUEUE__.size(), sizeWidth - 1, " ");

	Priority_Queue<Process*, deadline_less> readyQueue = __READY_QUEUE__;

	// Print processes in the ready queue
	while (!readyQueue.empty())
	{
		printf("%04d, ", readyQueue.top()->getID());
		readyQueue.pop();
	}

	printf("%*s║\n", prSize - (__READY_QUEUE__.size() * 6), " ");
	printf("╠═══════════╬═══════════╬═══════════╬═════════════════════════════════════════════════════════════════════════════════════════════════════╣\n");

}

void EDF::showCurrent()
{
	if (__CURRENT__ == nullptr)
		std::cout << "CURRENT SJF: " << "NONE" << std::endl;
	else
		std::cout << "CURRENT SJF: " << __CURRENT__->getCPU() << " " << __CURRENT__->getStep(C) << std::endl;
}

int EDF::getBusy()
{
	return busy_counter;
}

int EDF::getIdl()
{
	return idl_counter;
}

void EDF::overHeating(int step)
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

bool EDF::getOverHeating()
{
	return overHeated;
}

