#pragma once
#include "stdafx.h"

class Scheduler
{
	std::ifstream input;
	std::ofstream output;
	std::string user_filename;
	int nFCFS, nSJF, nRR, nEDF, slice_time, over_heat_time, RTF, maxW, STL, forkP, nprocess, nkill, size;
	std::list<Process*> __NEW_PROCESSES__;
	Queue<Process*> __NEW__;
	Vector<Processor*> __Processors__;
	Queue<Process*> __RUNNING__;
	static Queue<Process*> __BLOCK__;
	static Queue<Process*> __TERMINATE__;
	Priority_Queue<Process*, TT_less> __SORTED_TERMINATE__;
	Queue<Pr_data> processorsLoad;
	Queue<Pr_data> processorsUtil;



	int totalW = 0, totalRT = 0, totalTRT = 0, last_arrival = 0;
	int nOuts = 0, waitAvg, trtAvg, rtAvg, avgUtil;
	int nForked = 0, nMigratedMAXW = 0, nMigratedRTF = 0, nStolen = 0, n_killed = 0, fb_dead = 0, nOH = 0;
	int pForked, pRTF, pMaxW, pSTL, pKLL, pfbd;

public:
	friend class FCFS;
	Scheduler();

	void p();
	void fetch(std::string);
	void aggregate();
	void post();
	static void addKill(Kill_signal&);
	void start(int);
	void build();
	void distripute();
	void excute(int);
	void setRUNNING();
	void trans(int);
	void Block2Ready();
	void print(int);
	void Show();
	void showRunning();
	void showBlock();
	void showTerminate();
	Processor* findMinimum();
	Processor* findMaximum();
	Processor* findSQF();
	Processor* findLQF();
	static int generatrID();
	void updateWait();
	void migrate();
	void steal(int);
	void fork(int);
	void sortTerminate();
	void analyze();
	bool turnON();
	bool hasActiveProcessor();
	int getLastArrival();
	bool areAllProcessorsOverheated();

	Processor* findProcessorWithMinCTExcludingID(int);

	void uprootOverHeated();

	Queue<Process*>getNEW();
	Vector<Processor*>getProcessors();
	Queue<Process*>getRunning();
	Queue<Process*> getBlock();
	Queue<Process*> getTerminate();

	~Scheduler();

	template<typename ProcessorType>
	typename std::enable_if<std::is_base_of<Processor, ProcessorType>::value, ProcessorType*>::type
		findSpecificMinimum();

	template <typename ProcessorType>
	typename std::enable_if<std::is_base_of<Processor, ProcessorType>::value, ProcessorType*>::type
		findSpecificMinimumExcludingID(int excludedID)
	{
		int minCT = std::numeric_limits<int>::max();
		ProcessorType* minCTProcessor = nullptr;

		try {
			for (Processor* processor : __Processors__) {
				if (typeid(*processor) == typeid(ProcessorType)) {
					ProcessorType* specificProcessor = dynamic_cast<ProcessorType*>(processor);
					if (specificProcessor && specificProcessor->getID() != excludedID) {
						if (specificProcessor->getCT() < minCT || minCTProcessor == nullptr) {
							minCT = specificProcessor->getCT();
							minCTProcessor = specificProcessor;
						}
					}
				}
			}
		}
		catch (const std::bad_cast& ex) {
			// Handle the bad_cast exception
			std::cerr << "Error: Bad cast occurred during dynamic_cast." << std::endl;
			throw ex;
		}
		catch (const std::exception& ex) {
			// Handle other exceptions
			std::cerr << "Error: " << ex.what() << std::endl;
			throw ex;
		}

		return minCTProcessor;
	}
};

template <typename ProcessorType>
typename std::enable_if<std::is_base_of<Processor, ProcessorType>::value, ProcessorType*>::type
Scheduler::findSpecificMinimum()
{
	int minCT = std::numeric_limits<int>::max();
	ProcessorType* minCTProcessor = nullptr;

	try {
		for (Processor* processor : __Processors__) {
			if (typeid(*processor) == typeid(ProcessorType)) {
				ProcessorType* specificProcessor = dynamic_cast<ProcessorType*>(processor);
				if (specificProcessor) {
					if (specificProcessor->getCT() < minCT || minCTProcessor == nullptr) {
						minCT = specificProcessor->getCT();
						minCTProcessor = specificProcessor;
					}
				}
			}
		}
	}
	catch (const std::bad_cast& ex) {
		// Handle the bad_cast exception
		std::cerr << "Error: Bad cast occurred during dynamic_cast." << std::endl;
		throw ex;
	}
	catch (const std::exception& ex) {
		// Handle other exceptions
		std::cerr << "Error: " << ex.what() << std::endl;
		throw ex;
	}

	return minCTProcessor;
}




