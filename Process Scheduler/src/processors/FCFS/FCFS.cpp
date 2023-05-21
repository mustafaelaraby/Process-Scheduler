#include "FCFS.h"

#include "../../scheduler/scheduler.h"

Queue<Kill_signal> FCFS::__KILL__;
int FCFS::MAXW = 0;
int FCFS::nkilled = 0;
//=========================== Constructors ============================//
FCFS::FCFS(int overheat)
{
	overHeatLimit = overheat;
	ID = count++;
	Type = "FCFS";
	__CURRENT__ = nullptr;
}

FCFS::~FCFS()
{
	std::cout << "From FCFS DESTRUCTOR" << std::endl;
}

//============================= Operations ============================//
int FCFS::getID()
{
	return ID;
}

void FCFS::add(Process* process)
{
	__READY_QUEUE__.push(process);
	__CT__ += (process->getCPU() - process->getStep(C));
}

bool FCFS::emptyRQ()
{
	return __READY_QUEUE__.empty();
}

Process* FCFS::spell()
{
	if (!__READY_QUEUE__.empty())
	{
		Process* process = __READY_QUEUE__.front();
		__READY_QUEUE__.pop();
		return process;
	}
}

Process* FCFS::spellUnforked()
{
	try {
		Queue<Process*> tempQueue;

		while (!__READY_QUEUE__.empty()) {
			Process* process = __READY_QUEUE__.front();

			if (!process->isForked()) {
				__READY_QUEUE__.pop();
				return process;
			}

			tempQueue.push(process);
			__READY_QUEUE__.pop();
		}

		// No forked process found
		while (!tempQueue.empty()) {
			Process* process = tempQueue.front();
			tempQueue.pop();
			__READY_QUEUE__.push(process);
		}

		return nullptr;
	}
	catch (const std::exception& e) {
		// Handle and log the exception
		std::cout << "Exception occurred: " << e.what() << std::endl;
		return nullptr;
	}
	catch (...) {
		// Handle and log any other unexpected exceptions
		std::cout << "Unknown exception occurred." << std::endl;
		return nullptr;
	}
}

int FCFS::getCT()
{
	return __CT__;
}

void FCFS::setCT()
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

int FCFS::getQT()
{
	return QT;
}

void FCFS::setQT()
{
	QT = 0;
	Queue<Process*> queue = __READY_QUEUE__;
	while (!queue.empty())
	{
		QT += (queue.front()->getCPU() - queue.front()->getStep(C));
		queue.pop();
	}
}

void FCFS::updateWait()
{
	Queue<Process*> Ready_Queue = __READY_QUEUE__;
	while (!Ready_Queue.empty())
	{
		Ready_Queue.front()->incrementWait();
		Ready_Queue.pop();
	}
}

Queue<Process*> FCFS::migrate()
{
	return __MIGRATING__;
}

void FCFS::emptyMigrating()
{
	while (!__MIGRATING__.empty())
	{
		__MIGRATING__.pop();
	}
}

Process* FCFS::getCurrent()
{
	return __CURRENT__;
}

void FCFS::setCurrent(Process* CURRENT)
{
	this->__CURRENT__ = CURRENT;
}

void FCFS::run(int step)
{
	if (!overHeated)
	{
		overHeating(step);
		killOrphan(step);
		updateWait();
		if (!__READY_QUEUE__.empty())
		{
			size_t size = __READY_QUEUE__.size();

			for (size_t i = 0; i < size; ++i)
			{
				if (!__KILL__.empty() && __READY_QUEUE__.front()->getID() == __KILL__.front().id && __KILL__.front().time == step)
				{
					__KILL__.pop();
					__READY_QUEUE__.front()->setTRM(step);
					__READY_QUEUE__.front()->setTRT(step - __READY_QUEUE__.front()->getArrival());
					Scheduler::__TERMINATE__.push(__READY_QUEUE__.front());
					__READY_QUEUE__.pop();
					setCT();
					setQT();
					break;
				}
				__READY_QUEUE__.push(__READY_QUEUE__.front());
				__READY_QUEUE__.pop();
			}
		}

		if (__CURRENT__ == nullptr)
		{
			while ((__READY_QUEUE__.size() > 1)
				&& (__READY_QUEUE__.front()->getWait() > MAXW)
				&& (__READY_QUEUE__.front()->isForked() != true))
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
				__CURRENT__->setProcessor("FCFS");
				__CURRENT__->incrementStep(C);
				__CURRENT__->incrementStep(R);
				setCT();
				setQT();
				if (!__KILL__.empty() && __CURRENT__->getID() == __KILL__.front().id && __KILL__.front().time == step)
				{
					++nkilled;
					__KILL__.pop();
					__CURRENT__->setTRM(step);
					__CURRENT__->setTRT(step - __CURRENT__->getArrival());
					__CURRENT__->TerminateParent(TERMINATE);
					Scheduler::__TERMINATE__.push(__CURRENT__);
					__CURRENT__ = nullptr;
					setCT();
				}
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
			__CURRENT__->setProcessor("FCFS");
			__CURRENT__->incrementStep(C);
			__CURRENT__->incrementStep(R);
			--__CT__;
			if (!__KILL__.empty() && __CURRENT__->getID() == __KILL__.front().id && __KILL__.front().time == step)
			{
				++nkilled;
				__KILL__.pop();
				__CURRENT__->setTRM(step);
				__CURRENT__->setTRT(step - __CURRENT__->getArrival());
				__CURRENT__->TerminateParent(TERMINATE);
				Scheduler::__TERMINATE__.push(__CURRENT__);
				__CURRENT__ = nullptr;
				setCT();
			}
		}
	}
}

void FCFS::kill(int, int)
{

}

void FCFS::enqueKill(Kill_signal& kill_signal)
{
	__KILL__.push(kill_signal);
}

void FCFS::dequeKill()
{
	if (!__KILL__.empty())
		__KILL__.pop();
}

Kill_signal FCFS::frontKill()
{
	return __KILL__.front();
}

void FCFS::killOrphan(int step)
{
	if (__CURRENT__ && (__CURRENT__->getStatus() == ORPHAN
		|| (__CURRENT__->getStatus() == TERMINATE)
		&& __CURRENT__->isForked()))
	{
		nkilled++;
		__CURRENT__->TerminateParent(TERMINATE);
		__CURRENT__->setTRM(step);
		__CURRENT__->setTRT(step - __CURRENT__->getArrival());
		Scheduler::__TERMINATE__.push(__CURRENT__);
		__CURRENT__ = nullptr;
		setCT();
		setQT();
	}

	Queue<Process*> tempQueue;
	try {
		while (!__READY_QUEUE__.empty()) {
			Process* process = __READY_QUEUE__.front();

			if ((process->getStatus() == ORPHAN
				|| (process->getStatus() == TERMINATE)
				&& process->isForked()))
			{
				++nkilled;
				process->TerminateParent(TERMINATE);
				process->setTRM(step);
				process->setTRT(step - process->getArrival());
				Scheduler::__TERMINATE__.push(process);
				__READY_QUEUE__.pop();
			}
			else {
				tempQueue.push(process);
				__READY_QUEUE__.pop();
			}
		}

		// Move back the processes from tempQueue to processQueue
		while (!tempQueue.empty()) {
			Process* process = tempQueue.front();
			tempQueue.pop();
			__READY_QUEUE__.push(process);
		}

		setCT();
		setQT();
	}
	catch (const std::exception& e) {
		// Handle and log the exception
		std::cout << "Exception occurred: " << e.what() << std::endl;
	}
	catch (...) {
		// Handle and log any other unexpected exceptions
		std::cout << "Unknown exception occurred." << std::endl;
	}

}

void FCFS::print()
{
	int idWidth = 9;
	int typeWidth = 9;
	int sizeWidth = 9;
	int prSize = 101;

	printf("║%03d%*s║%-*s%*s║%03d%*s║", ID, idWidth - 1, " ", typeWidth, Type.c_str(), typeWidth - (int)Type.length() - 3, " ", __READY_QUEUE__.size(), sizeWidth - 1, " ");

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

void FCFS::showCurrent()
{
	if (__CURRENT__ == nullptr)
		std::cout << "CURRENT FCFS: " << "NONE" << std::endl;
	else
		std::cout << "CURRENT FCFS: " << __CURRENT__->getStep(C) << std::endl;
}

int FCFS::getBusy()
{
	return busy_counter;
}

int FCFS::getIdl()
{
	return idl_counter;
}

void FCFS::overHeating(int step)
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

bool FCFS::getOverHeating()
{
	return overHeated;
}



