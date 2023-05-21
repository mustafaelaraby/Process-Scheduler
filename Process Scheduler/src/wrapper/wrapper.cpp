#include "wrapper.h"

Wrapper::Wrapper()
{
	p_scheduler = new Scheduler;
	p_gui = new GUI;
}

GUI* Wrapper::getGUI() const
{
	return p_gui;
}

void Wrapper::Run()
{
	p_gui->start();

	char ch;
	bool go = true;

	while (go)
	{
		file = p_gui->getfile();
		Mode mode = p_gui->getMode();
		switch (mode)
		{
		case INTERACTIVE:runInteractive(); break;
		case STEP_BY_STEP:runStep_by_step(); break;
		case SILENT:runSilent(); break;
		default:runStep_by_step();
		}
		animateProgressBar(30, 100, "Well dude!!  do you want to simulate again (y or n):");
		std::cin >> ch;
		switch (ch)
		{
		case ('y' | 'Y'):go = true; break;
		case ('n' | 'N'):go = false; break;
		default:break;
		}
	}

}

void Wrapper::runInteractive()
{
	p_scheduler->fetch(file);
	p_scheduler->aggregate();
	p_scheduler->build();

	char ch;

	int i = 0;
	bool go = true;

	while (go)
	{
		if (_kbhit()) {
			ch = _getch();
			if (ch == 27) { // ESC key
				break;
			}
		}

		std::cout << std::endl;
		printf("                                             ╔═══════════════════════════════════════════╗\n");
		printf("                                             ║                    %04d                   ║\n", i);
		printf("                                             ╚═══════════════════════════════════════════╝\n");


		p_scheduler->start(i);
		p_gui->showNew(p_scheduler->getNEW());
		p_scheduler->distripute();
		p_scheduler->excute(i);
		p_scheduler->steal(i);
		p_scheduler->setRUNNING();
		p_scheduler->fork(i);
		p_gui->showReady(p_scheduler->getProcessors());
		p_gui->showRunning(p_scheduler->getRunning());
		p_scheduler->trans(i);
		p_gui->showBlock(p_scheduler->getBlock());
		p_gui->showTerminate(p_scheduler->getTerminate());
		p_scheduler->Block2Ready();
		p_scheduler->uprootOverHeated();

		waitForEnter();
		++i;

		if (i <= p_scheduler->getLastArrival())
			go = true;
		else
			go = p_scheduler->turnON();
	}
	p_scheduler->post();
	waitForEnter();
}

void Wrapper::runStep_by_step()
{
	p_scheduler->fetch(file);
	p_scheduler->aggregate();
	p_scheduler->build();


	char ch;
	int i = 0;
	bool go = true;

	while (go) {
		if (_kbhit()) {
			ch = _getch();
			if (ch == 27) { // ESC key
				break;
			}
		}

		std::cout << std::endl;
		printf("                                             ╔═══════════════════════════════════════════╗\n");
		printf("                                             ║                    %04d                   ║\n", i);
		printf("                                             ╚═══════════════════════════════════════════╝\n");

		p_scheduler->start(i);
		p_gui->showNew(p_scheduler->getNEW());
		p_scheduler->distripute();
		p_scheduler->excute(i);
		p_scheduler->steal(i);
		p_scheduler->setRUNNING();
		p_scheduler->fork(i);
		p_gui->showReady(p_scheduler->getProcessors());
		p_gui->showRunning(p_scheduler->getRunning());
		p_scheduler->trans(i);
		p_gui->showBlock(p_scheduler->getBlock());
		p_gui->showTerminate(p_scheduler->getTerminate());
		p_scheduler->Block2Ready();
		p_scheduler->uprootOverHeated();

		delay(1000);
		++i;

		if (i <= p_scheduler->getLastArrival())
			go = true;
		else
			go = p_scheduler->turnON();
	}

	p_scheduler->post();
	waitForEnter();
}

void Wrapper::runSilent()
{
	p_scheduler->fetch(file);
	p_scheduler->aggregate();
	p_scheduler->build();

	char ch;
	int i = 0;
	bool go = true;
	while (go)
	{
		if (_kbhit()) {
			ch = _getch();
			if (ch == 27) { // ESC key
				break;
			}
		}
		p_scheduler->start(i);
		p_scheduler->distripute();
		p_scheduler->excute(i);
		p_scheduler->steal(i);
		p_scheduler->setRUNNING();
		p_scheduler->fork(i);
		p_scheduler->trans(i);
		p_scheduler->Block2Ready();
		p_scheduler->uprootOverHeated();

		++i;
		if (i <= p_scheduler->getLastArrival())
			go = true;
		else
			go = p_scheduler->turnON();
	}
	p_scheduler->post();
	waitForEnter();

}

Wrapper::~Wrapper()
{
	delete p_gui;
	delete p_scheduler;
}

