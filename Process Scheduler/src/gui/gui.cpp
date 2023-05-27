#include "gui.h"


void GUI::start()
{
	int progressBarWidth = 20;  // Width of the progress bar
	int delay = 100;            // Delay between each progress update in milliseconds
	std::string welcomeMessage = "Welcome to Process Scheduler";

	animateProgressBar(progressBarWidth, delay, welcomeMessage);

	std::cout << std::endl;
	std::cout << "Enter Filename: ";
	std::cin >> filename;
	std::cout << std::endl;
	std::cout << "Please Choose Simulation Mode [Interactive: 0, Step-by-Step: 1, Silent: 2] :";
	std::cin >> modeNum;
	setMode();
}

void GUI::setMode()
{
	switch (modeNum)
	{
	case 0:mode = INTERACTIVE; break;
	case 1:mode = STEP_BY_STEP; break;
	case 2:mode = SILENT; break;
	default:break;
	}
}

Mode GUI::getMode() const
{
	return mode;
}

std::string GUI::getfile() const
{
	return filename;
}


void GUI::showNew(Queue<Process*> new_list)
{
	printf("===============((");
	printf(" NEW ");
	printf("))===============\n");

	// Define the column widths
	int idWidth = 10;
	int arrivalWidth = 10;
	int cpuWidth = 10;
	
	// Print the table header
	printf("╔══════════╦══════════╦══════════╗\n");
	printf("║ %-*s ║ %-*s ║ %-*s ║\n", idWidth - 2, "ID", arrivalWidth - 2, "Arrival", cpuWidth - 2, "CPU");
	printf("╠══════════╬══════════╬══════════╣\n");

	while (!new_list.empty())
	{
		if (new_list.front() != nullptr)
		{
			printf("║%03d%*s║%03d%*s║%03d%*s║\n", new_list.front()->getID(), idWidth - 3, " ",
				new_list.front()->getArrival(), arrivalWidth - 3, " ",
				new_list.front()->getCPU(), cpuWidth - 3, " ");
		}
		new_list.pop();
	}

	printf("╚══════════╩══════════╩══════════╝\n");
}

void GUI::showReady(Vector<Processor*> processors)
{
	printf("===============((");
	printf(" Ready ");
	printf("))===============\n");

	int idWidth = 9;
	int typeWidth = 9;
	int SizeWidth = 9;
	int prsize = 99;

	printf("╔═══════════╦═══════════╦═══════════╦═════════════════════════════════════════════════════════════════════════════════════════════════════╗\n");
	printf("║ %-*s ║ %-*s ║ %-*s ║ %-*s ║\n", idWidth, "ID", typeWidth, "Type", SizeWidth, "NO.P", prsize, "Processes");
	printf("╠═══════════╬═══════════╬═══════════╬═════════════════════════════════════════════════════════════════════════════════════════════════════╣\n");

	Vector<Processor*>::Iterator iter;
	for (iter = processors.begin(); iter != processors.end(); ++iter)
	{
		(*iter)->print();
	}
	std::cout << std::endl;
}

void GUI::showRunning(Queue<Process*> Running)
{
	printf("===============((");
	printf(" Running ");
	printf("))===============\n");


	// Define the column widths
	int idWidth = 10;
	int pWidth = 14;
	int cpuWidth = 10;
	int stepWidth = 10;

	// Print the table header
	printf("╔══════════╦═════════════╦══════════╦══════════╗\n");
	printf("║ %-*s ║ %-*s ║ %-*s ║ %-*s ║\n", idWidth - 2, "ID", pWidth - 3, "Processor", cpuWidth - 2, "CPU", stepWidth - 2, "Step");
	printf("╠══════════╬═════════════╬══════════╬══════════╣\n");

	while (!Running.empty())
	{
		if (Running.front() != nullptr)
		{
			printf("║ %04d %*s ║ %-*s ║ %03d %*s ║ %03d %*s ║\n", Running.front()->getID(), idWidth - 7, " ",
				pWidth - 3, Running.front()->getProcessor().c_str(),
				Running.front()->getCPU(), cpuWidth - 6, " ",
				Running.front()->getStep(C), stepWidth - 6, " ");
		}
		Running.pop();
	}

	printf("╚══════════╩═════════════╩══════════╩══════════╝\n");
}

void GUI::showBlock(Queue<Process*> BLOCK)
{
	printf("===============((");
	printf(" BLOCK ");
	printf("))===============\n");


	// Define the column widths
	int idWidth = 10;
	int IODWidth = 10;
	int stepDWidth = 10;

	// Print the table header
	printf("╔══════════╦══════════╦══════════╗\n");
	printf("║ %-*s ║ %-*s ║ %-*s ║\n", idWidth - 2, "ID", IODWidth - 2, "CPU", stepDWidth - 2, "Step");
	printf("╠══════════╬══════════╬══════════╣\n");

	while (!BLOCK.empty())
	{
		if (BLOCK.front() != nullptr)
		{
			printf("║ %04d %*s ║ %03d %*s ║ %03d %*s ║\n", BLOCK.front()->getID(), idWidth - 7, " ",
				BLOCK.front()->getIO(D), IODWidth - 6, " ",
				BLOCK.front()->getStep(D), stepDWidth - 6, " ");
		}
		BLOCK.pop();
	}

	printf("╚══════════╩══════════╩══════════╝\n");
}

void GUI::showTerminate(Queue<Process*> Terminate)
{
	printf("===============((");
	printf(" Terminate ");
	printf("))===============\n");


	// Define the column widths
	int idWidth = 10;
	int pWidth = 14;
	int cpuWidth = 10;
	int stepWidth = 10;

	// Print the table header
	printf("╔══════════╦═════════════╦══════════╦══════════╗\n");
	printf("║ %-*s ║ %-*s ║ %-*s ║ %-*s ║\n", idWidth - 2, "ID", pWidth - 3, "Processor", cpuWidth - 2, "TRM", stepWidth - 2, "Deadl.");
	printf("╠══════════╬═════════════╬══════════╬══════════╣\n");

	while (!Terminate.empty())
	{
		if (Terminate.front() != nullptr)
		{
			printf("║ %04d %*s ║ %-*s ║ %03d %*s ║ %03d %*s ║\n", Terminate.front()->getID(), idWidth - 7, " ",
				pWidth - 3, Terminate.front()->getProcessor().c_str(),
				Terminate.front()->getTRM(), cpuWidth - 6, " ",
				Terminate.front()->getDeadline(), stepWidth - 6, " ");
		}
		Terminate.pop();
	}

	printf("╚══════════╩═════════════╩══════════╩══════════╝\n");
}


