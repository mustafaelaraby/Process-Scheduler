#pragma once
#include "stdafx.h"



class Wrapper {

	GUI* p_gui;
	Scheduler* p_scheduler;
	std::string file;
public:
	Wrapper();
	GUI* getGUI()const;
	Scheduler* getScheduler()const;

	void Run();
	void runInteractive();
	void runStep_by_step();
	void runSilent();

	~Wrapper();

};