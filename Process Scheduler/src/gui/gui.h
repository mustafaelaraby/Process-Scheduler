#pragma once
#include "stdafx.h"

class GUI {
	std::string filename;
	int modeNum;
	Mode mode = INTERACTIVE;
public:
	void start();
	std::string getfile()const;
	Mode getMode()const;

	void showNew(Queue<Process*>);
	void showReady(Vector<Processor*>);
	void showRunning(Queue<Process*>);
	void showBlock(Queue<Process*>);
	void showTerminate(Queue<Process*>);

private:
	void setMode();
};