#pragma once
#include "scheduler.h"

Queue<Process*>Scheduler::__TERMINATE__;
Queue<Process*>Scheduler::__BLOCK__;

Scheduler::Scheduler() {}

void Scheduler::fetch(std::string file)
{
	std::filesystem::path currentDir = std::filesystem::current_path();
	std::string currentDirStr = currentDir.string();
	std::string filename = currentDirStr + "\\docs\\input\\" + file + ".txt";
	input.open(filename.c_str());
}

void Scheduler::aggregate()
{
	if (input.fail())
	{
		std::cout << "File does not exist" << std::endl;
		std::cout << "Exit program" << std::endl;
	}

	{
		input >> nFCFS >> nSJF >> nRR >> nEDF;
		input >> slice_time >> over_heat_time;
		input >> RTF >> maxW >> STL >> forkP;
		input >> nprocess;
		FCFS::MAXW = maxW;
	}

	std::string sp;

	Queue<std::string> Data;
	std::getline(input, sp);
	for (int i = 0; i < nprocess; i++)
	{
		std::string line;
		std::getline(input, line);
		//	std::cout << line << std::endl;
		Data.push(std::move(line));
	}

	while (!Data.empty())
	{
		std::string input = Data.front();
		Data.pop();
		std::istringstream ss(input);
		std::string token;

		std::string temp[20];
		int j = 0;
		while (std::getline(ss, token, '-') && j < 20) {
			temp[j] = token;
			j++;
		}
		Process* tempProcess = new Process();

		tempProcess->setStutus(NEW);
		tempProcess->setArrival(std::stoi(temp[0]));
		tempProcess->setID(std::stoi(temp[1]));
		tempProcess->setCPU(std::stoi(temp[2]));
		tempProcess->setDeadline(std::stoi(temp[3]));
		tempProcess->setNumIO(std::stoi(temp[4]));
		for (int k = 5; k < tempProcess->getNumIO() + 5; k++)
		{
			int* pair;
			pair = extractIntegerWords(temp[k]);
			tempProcess->IO_Push(pair[0], R);
			tempProcess->IO_Push(pair[1], D);
		}
		last_arrival = tempProcess->getArrival();
		__NEW_PROCESSES__.push_back(tempProcess);
	}

	input >> nkill;
	Queue<std::string> Kill;
	std::getline(input, sp);
	for (int i = 0; i < nkill; i++)
	{
		std::string line;
		std::getline(input, line);
		//	std::cout << line << std::endl;
		Kill.push(std::move(line));
	}

	while (!Kill.empty())
	{
		int* pair;
		Kill_signal tempKill;
		pair = extractIntegerWords(Kill.front());
		tempKill.time = pair[0];
		tempKill.id = pair[1];
		addKill(tempKill);
		Kill.pop();
	}


	input.close();
}

void Scheduler::post()
{

	std::filesystem::path currentDir = std::filesystem::current_path();
	std::string currentDirStr = currentDir.string();

	std::cout << "Enter fileName: ";
	std::cin >> user_filename;
	std::cout << std::endl;
	std::string filename = currentDirStr + "\\docs\\output\\" + user_filename + ".txt";
	output.open(filename.c_str());

	if (output.is_open()) {
		sortTerminate();
		analyze();
		// Write the table headers
		output << "TT\tID\tAT\tCT\tIO_D\tWT\tRT\tTRT\n";

		Process* process = nullptr;
		while (!__SORTED_TERMINATE__.empty())
		{
			process = __SORTED_TERMINATE__.top();
			output
				<< std::setw(4) << std::setfill('0') << process->getTRM() << "\t"
				<< std::setw(4) << std::setfill('0') << process->getID() << "\t"
				<< std::setw(4) << std::setfill('0') << process->getArrival() << "\t"
				<< std::setw(4) << std::setfill('0') << process->getCPU() << "\t"
				<< std::setw(4) << std::setfill('0') << process->getBLK() << "\t"
				<< std::setw(4) << std::setfill('0') << process->getWait() << "\t"
				<< std::setw(4) << std::setfill('0') << process->getResponse() << "\t"
				<< std::setw(4) << std::setfill('0') << process->getTRT() << "\n";
			__SORTED_TERMINATE__.pop();
		}

		output << "\nProcesses: " << nOuts << "\n";
		output << "avg WT = " << waitAvg << "\t\t" << "avg RT = " << rtAvg << "\t\t" << "avg TRT = " << trtAvg << "\n";

		output
			<< "Migaration  %: " << "\t\t"
			<< "RTF = " << pRTF << "%" << "\t\t"
			<< "maxW = " << pMaxW << "%" << "\n";

		output << "Work Steal %: " << pSTL << "%" << "\n";
		output << "Forked Process %: " << pForked << "%" << "\n";
		output << "Killed Process %: " << pKLL << "%" << "\n";
		output << "Befor Deadline %: " << pfbd << "%" << "\n";

		output << "\nNo. OverHeated %: " << nOH << "\n";



		output
			<< "\nProcessors: " << (nFCFS + nSJF + nRR + nEDF) << "\t"
			<< "[" << nFCFS << " FCFS ," << nSJF << " SJF ," << nRR << " RR ," << nEDF << " EDF ]\n";

		output << "\nProcessor Load: \n";

		size_t i = 0;
		while (!processorsLoad.empty())
		{
			Pr_data pr = processorsLoad.front();
			if (i > 0 && !(i % 5))
				output << "\n";

			output << "P" << pr.id << " = " << pr.p << "%" << "\t";
			processorsLoad.pop();
			++i;
		}
		output << "\n";

		output << "\nProcessor Util: \n";
		size_t j = 0;
		while (!processorsUtil.empty())
		{
			Pr_data pr = processorsUtil.front();
			if (j > 0 && !(j % 5))
				output << "\n";

			output << "P" << pr.id << " = " << pr.p << "%" << "\t";
			processorsUtil.pop();
			++j;
		}
		output << "\n";

		output << "Average Utilization = " << avgUtil << "%" << "\n";

		output.close();
		std::cout << "Data saved to file." << std::endl;
	}
	else {
		std::cout << "Failed to open the file." << std::endl;
	}
}

void Scheduler::addKill(Kill_signal& kill_signal)
{
	FCFS::__KILL__.push(kill_signal);
}

void Scheduler::start(int timestep)
{

	std::list< Process* >::iterator iter = __NEW_PROCESSES__.begin();
	std::list< Process* >::iterator end = __NEW_PROCESSES__.end();

	while (iter != end)
	{

		if ((*iter)->getArrival() == timestep)
		{
			(__NEW__).push(*iter);
			iter = __NEW_PROCESSES__.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void Scheduler::build()
{
	size = nFCFS + nSJF + nRR + nEDF;
	for (int i = 0; i < size; i++)
	{
		if (i < nFCFS)
		{
			Processor* tempProcessor = new FCFS(over_heat_time);
			__Processors__.push_back(tempProcessor);
		}
		else if (i >= nFCFS && i < (nFCFS + nSJF))
		{
			Processor* tempProcessor = new SJF(over_heat_time);
			__Processors__.push_back(tempProcessor);
		}
		else if (i >= (nFCFS + nSJF) && i < (nFCFS + nSJF + nRR))
		{
			Processor* tempProcessor = new Round_Robin(slice_time, RTF, over_heat_time);
			__Processors__.push_back(tempProcessor);
		}
		else
		{
			Processor* tempProcessor = new EDF(over_heat_time);
			__Processors__.push_back(tempProcessor);
		}
	}
}

Processor* Scheduler::findMinimum()
{
	if (__Processors__.empty()) {
		throw std::runtime_error("Empty vector of processors");
	}

	Processor* minProcessor = nullptr;
	int minCT = std::numeric_limits<int>::max();

	for (Processor* processor : __Processors__) {
		int currentCT = processor->getCT();
		if (currentCT < minCT) {
			minProcessor = processor;
			minCT = currentCT;
		}
	}

	return minProcessor;
}

Processor* Scheduler::findMaximum()
{
	if (__Processors__.empty()) {
		throw std::runtime_error("Empty vector of processors");
	}

	Processor* maxProcessor = nullptr;
	int maxCT = std::numeric_limits<int>::min();

	for (Processor* processor : __Processors__) {
		int currentCT = processor->getCT();
		if (currentCT > maxCT) {
			maxProcessor = processor;
			maxCT = currentCT;
		}
	}

	return maxProcessor;
}

Processor* Scheduler::findProcessorWithMinCTExcludingID(int id)
{
	Processor* minCTProcessor = nullptr;
	int minCT = std::numeric_limits<int>::max();

	try
	{
		for (Processor* processor : __Processors__)
		{
			if (processor->getID() != id)
			{
				int currentCT = processor->getCT();
				if (currentCT < minCT)
				{
					minCT = currentCT;
					minCTProcessor = processor;
				}
			}
		}
	}
	catch (const std::exception& ex)
	{
		// Handle the exception
		std::cout << "Exception occurred: " << ex.what() << std::endl;
		// Optionally, you can re-throw the exception or return an appropriate value
		// throw; // Re-throw the exception
		// return nullptr; // Return a default value or nullptr
	}

	return minCTProcessor;
}

Processor* Scheduler::findSQF()
{
	if (__Processors__.empty()) {
		throw std::runtime_error("Empty vector of processors");
	}

	Processor* minProcessor = nullptr;
	int minQT = std::numeric_limits<int>::max();

	for (Processor* processor : __Processors__) {
		processor->setQT();
		int currentQT = processor->getQT();
		if (currentQT < minQT) {
			minProcessor = processor;
			minQT = currentQT;
		}
	}

	return minProcessor;
}

Processor* Scheduler::findLQF()
{
	if (__Processors__.empty()) {
		throw std::runtime_error("Empty vector of processors");
	}

	Processor* maxProcessor = nullptr;
	int maxQT = std::numeric_limits<int>::min();

	for (Processor* processor : __Processors__) {
		processor->setQT();
		int currentQT = processor->getQT();
		if (currentQT > maxQT) {
			maxProcessor = processor;
			maxQT = currentQT;
		}
	}

	return maxProcessor;
}

int Scheduler::generatrID()
{
	static int nextID = 1000;
	return nextID++;
}

void Scheduler::distripute()
{
	while (__NEW__.size())
	{
		Processor* min = findMinimum();
		__NEW__.front()->setStutus(READY);
		min->add(__NEW__.front());
		__NEW__.pop();
	}
}

void Scheduler::excute(int step)
{
	Vector<Processor*>::Iterator iter;
	for (iter = __Processors__.begin(); iter != __Processors__.end(); ++iter)
	{
		(*iter)->run(step);
		migrate();
	}
}

void Scheduler::setRUNNING()
{
	while (!__RUNNING__.empty())
	{
		__RUNNING__.pop();
	}

	Vector<Processor*>::Iterator iter;
	for (iter = __Processors__.begin(); iter != __Processors__.end(); ++iter)
	{
		__RUNNING__.push((*iter)->getCurrent());
	}
}

void Scheduler::trans(int step)
{
	Vector<Processor*>::Iterator iter = __Processors__.begin();
	Vector<Processor*>::Iterator end = __Processors__.end();

	for (iter; iter != end; ++iter)
	{
		if ((*iter)->getCurrent() != nullptr)
		{
			if ((*iter)->getCurrent()->getStep(C) == (*iter)->getCurrent()->getCPU())
			{
				(*iter)->getCurrent()->TerminateParent(TERMINATE);
				(*iter)->getCurrent()->setTRM(step);
				(*iter)->getCurrent()->setTRT(step - (*iter)->getCurrent()->getArrival());
				__TERMINATE__.push((*iter)->getCurrent());
				(*iter)->setCurrent(nullptr);
				(*iter)->setCT();
			}
			else if ((*iter)->getCurrent()->getStep(R) == (*iter)->getCurrent()->getIO(R))
			{
				(*iter)->getCurrent()->setStep(0, R);
				(*iter)->getCurrent()->IO_Pop(R);
				(*iter)->getCurrent()->setStutus(BLOCK);
				__BLOCK__.push((*iter)->getCurrent());
				(*iter)->setCurrent(nullptr);
				(*iter)->setCT();
			}

		}
	}
}

void Scheduler::updateWait()
{

	Queue<Process*> Ready_Queue = __BLOCK__;
	while (!Ready_Queue.empty())
	{
		__BLOCK__.front()->incrementWait();
		Ready_Queue.pop();
	}
}

void Scheduler::Block2Ready()
{
	updateWait();
	if (!__BLOCK__.empty())
	{
		if (__BLOCK__.front()->getStep(D) == __BLOCK__.front()->getIO(D))
		{
			Processor* min = findMinimum();
			__BLOCK__.front()->IO_Pop(D);
			__BLOCK__.front()->setStutus(READY);
			__BLOCK__.front()->setStep(0, D);
			min->add(__BLOCK__.front());
			__BLOCK__.pop();
			if (!__BLOCK__.empty())
			{
				__BLOCK__.front()->incrementStep(D);
				__BLOCK__.front()->incrementBlk();
			}
		}
		else
		{
			__BLOCK__.front()->incrementStep(D);
			__BLOCK__.front()->incrementBlk();
		}
	}

}

void Scheduler::migrate()
{
	for (auto processor : __Processors__)
	{
		if (typeid(*processor) == typeid(Round_Robin))
		{
			Round_Robin* round_robin = dynamic_cast<Round_Robin*>(processor);
			Queue<Process*> queue = round_robin->migrate();
			nMigratedRTF += queue.size();
			while (!queue.empty())
			{
				SJF* sjf = findSpecificMinimum<SJF>();
				sjf->add(queue.front());
				queue.pop();
				sjf->setQT();
				sjf->setCT();
			}
			round_robin->emptyMigrating();
		}
		else if (typeid(*processor) == typeid(FCFS))
		{
			FCFS* fcfs = dynamic_cast<FCFS*>(processor);
			Queue<Process*> queue = fcfs->migrate();
			nMigratedMAXW += queue.size();
			while (!queue.empty())
			{
				Round_Robin* round_robin = findSpecificMinimum<Round_Robin>();
				round_robin->add(queue.front());
				queue.pop();
				round_robin->setCT();
				round_robin->setQT();
			}
			fcfs->emptyMigrating();
		}
	}


}

void Scheduler::steal(int step)
{
	if (step > 0 && (step % 10 == 0))
	{
		Processor* min = findSQF();
		Processor* max = findLQF();
		int stealLimit = 0;
		if (max->getQT() > 0)
			stealLimit = static_cast<int>((static_cast<double>(max->getQT() - min->getQT()) / max->getQT()) * 100);

		if (typeid(*max) == typeid(FCFS) && typeid(*min) != typeid(FCFS))
		{
			FCFS* maxFCFS = dynamic_cast<FCFS*>(max);
			while (stealLimit > 40 && !maxFCFS->emptyRQ())
			{
				Process* process = maxFCFS->spellUnforked();
				if (process)
				{
					++nStolen;
					min->add(process);
					min->setQT(); min->setCT();
					maxFCFS->setQT(); maxFCFS->setCT();
					if (maxFCFS->getQT() > 0)
						stealLimit = static_cast<int>((static_cast<double>(maxFCFS->getQT() - min->getQT()) / maxFCFS->getQT()) * 100);
				}
				else
				{
					break;
				}
			}

		}
		else
		{
			while (stealLimit > 40 && !max->emptyRQ())
			{
				++nStolen;
				Process* process = max->spell();
				min->add(process);
				min->setQT(); min->setCT();
				max->setQT(); max->setCT();
				if (max->getQT() > 0)
					stealLimit = static_cast<int>((static_cast<double>(max->getQT() - min->getQT()) / max->getQT()) * 100);
			}
		}

	}
}

void Scheduler::fork(int step)
{
	for (auto processor : __Processors__)
	{
		if (typeid(*processor) == typeid(FCFS))
		{
			FCFS* fcfs = dynamic_cast<FCFS*>(processor);
			Process* current = fcfs->getCurrent();
			if (current != nullptr && current->canFork() && (current->getCPU() - current->getStep(C)))
			{
				bool f_cond = eventWithProbability(static_cast<double>(forkP) / 100);
				if (f_cond)
				{
					++nForked;
					Process* newProcess = new Process();
					newProcess->setID(generatrID());
					newProcess->setForked(true);
					newProcess->setArrival(step);
					newProcess->setCPU((current->getCPU() - current->getStep(C)));
					newProcess->setStutus(READY);
					newProcess->setProcessor("FCFS");
					current->insert(newProcess);
					FCFS* min = findSpecificMinimum<FCFS>();
					min->add(newProcess);
					min->setCT();
					min->setQT();
				}
			}

		}
	}
}

void Scheduler::sortTerminate()
{
	while (!__TERMINATE__.empty())
	{
		nOuts++;
		totalW += __TERMINATE__.front()->getWait();
		totalRT += __TERMINATE__.front()->getResponse();
		totalTRT += __TERMINATE__.front()->getTRT();

		if (__TERMINATE__.front()->getTRM() < __TERMINATE__.front()->getDeadline())
			++fb_dead;

		__SORTED_TERMINATE__.push(__TERMINATE__.front());
		__TERMINATE__.pop();
	}

}

void Scheduler::analyze()
{
	waitAvg = static_cast<int>(static_cast<double>(totalW) / nOuts);
	rtAvg = static_cast<int>(static_cast<double>(totalRT) / nOuts);
	trtAvg = static_cast<int>(static_cast<double>(totalTRT) / nOuts);

	pRTF = static_cast<int>((static_cast<double>(nMigratedRTF) / nOuts) * 100);
	pMaxW = static_cast<int>((static_cast<double>(nMigratedMAXW) / nOuts) * 100);
	pSTL = static_cast<int>((static_cast<double>(nStolen) / nOuts) * 100);
	pForked = static_cast<int>((static_cast<double>(nForked) / nOuts) * 100);
	pKLL = static_cast<int>((static_cast<double>(FCFS::nkilled) / nOuts) * 100);
	pfbd = static_cast<int>((static_cast<double>(fb_dead) / nOuts) * 100);


	for (auto processor : __Processors__)
	{
		Pr_data pr_data1, pr_data2;
		int pLoad = static_cast<int>((static_cast<double>(processor->getBusy()) / totalTRT) * 100);
		int pUtil = static_cast<int>((static_cast<double>(processor->getBusy()) / (processor->getBusy() + processor->getIdl())) * 100);
		pr_data1.p = pLoad;
		pr_data2.p = pUtil;
		pr_data1.id = processor->getID();
		pr_data2.id = processor->getID();
		avgUtil += pUtil;
		processorsLoad.push(pr_data1);
		processorsUtil.push(pr_data2);
	}
	avgUtil = static_cast<int>(static_cast<double>(avgUtil) / (__Processors__.size()));
}

bool Scheduler::turnON()
{
	bool active = false;
	bool NEW = false;
	bool BLK = false;
	bool oh = false;

	active = hasActiveProcessor();

	if (!__NEW__.empty())
		NEW = true;

	if (!__BLOCK__.empty())
		BLK = true;

	oh = areAllProcessorsOverheated();
	//if (!__NEW_PROCESSES__.empty())
	//	NEW = true;


	return ((NEW || active || BLK) && !oh);
}

bool Scheduler::areAllProcessorsOverheated()
{
	for (Processor* processor : __Processors__) {
		if (!processor->getOverHeating()) {
			// Found a processor that is not overheated
			return false;
		}
	}
	// All processors are overheated
	return true;
}


int Scheduler::getLastArrival()
{
	return last_arrival;
}

bool Scheduler::hasActiveProcessor()
{
	for (Processor* processor : __Processors__) {
		if (processor->getCurrent() != nullptr || !processor->emptyRQ()) {
			// Found a processor with either non-null Current or non-empty readyQueue
			return true;
		}
	}

	// No processor found with non-null Current or non-empty readyQueue
	return false;
}

Queue<Process*> Scheduler::getNEW()
{
	return __NEW__;
}

Vector<Processor*> Scheduler::getProcessors()
{
	return __Processors__;
}

Queue<Process*> Scheduler::getRunning()
{
	return __RUNNING__;
}

Queue<Process*> Scheduler::getBlock()
{
	return __BLOCK__;
}

Queue<Process*> Scheduler::getTerminate()
{
	return __TERMINATE__;
}

void Scheduler::uprootOverHeated()
{
	for (auto processor : __Processors__)
	{
		nOH = 0;
		if (processor->getOverHeating())
		{
			++nOH;
			if (processor->getCurrent() != nullptr)
			{
				if (processor->getCurrent()->isForked())
				{
					FCFS* min = findSpecificMinimumExcludingID<FCFS>(processor->getID());
					min->add(processor->getCurrent());
					min->setCT();
					min->setQT();
					processor->setCurrent(nullptr);
				}
				else
				{
					Processor* min = findProcessorWithMinCTExcludingID(processor->getID());
					min->add(processor->getCurrent());
					min->setCT();
					min->setQT();
					processor->setCurrent(nullptr);
				}
			}


			while (!processor->emptyRQ())
			{
				Process* process = processor->spell();
				if (process != nullptr)
				{
					if (process->isForked())
					{
						FCFS* min = findSpecificMinimumExcludingID<FCFS>(processor->getID());
						min->add(process);
						min->setCT();
						min->setQT();
					}
					else
					{
						Processor* min = findProcessorWithMinCTExcludingID(processor->getID());
						min->add(process);
						min->setCT();
						min->setQT();
					}

				}

			}
		}
	}
}

Scheduler::~Scheduler()
{
	//delete processors.
	for (Processor* ptr : __Processors__) {
		delete ptr;
	}
	__Processors__.clear();


	//delete __NEW_PROCESSES__.
	while (!__NEW_PROCESSES__.empty()) {
		delete __NEW_PROCESSES__.front();
		__NEW_PROCESSES__.pop_front();
	}

	//delete __NEW__.
	while (!__NEW__.empty()) {
		delete __NEW__.front();
		__NEW__.pop();
	}

	//delete __RUNNING__.
	while (!__RUNNING__.empty()) {
		delete __RUNNING__.front();
		__RUNNING__.pop();
	}

	//delete __RUNNING__.
	while (!__BLOCK__.empty()) {
		delete __BLOCK__.front();
		__BLOCK__.pop();
	}

	//delete __RUNNING__.
	while (!__TERMINATE__.empty()) {
		delete __TERMINATE__.front();
		__TERMINATE__.pop();
	}
}
