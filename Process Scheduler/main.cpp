#include "src/wrapper/wrapper.h"
#include "src/Process/Process.h"
#include <filesystem>
#include "libs/helpers/helpers.h"

int main()
{
	//std::filesystem::path currentDir = std::filesystem::current_path();
	//std::cout << "Current Directory: " << currentDir << std::endl;

	//Process* process = new Process();
	//process->setID(1); process->setCPU(16);

	//Process* process2 = new Process();
	//process2->setID(2); process2->setCPU(26);

	//Process* process3 = new Process();
	//process3->setID(3); process3->setCPU(18);

	//Process* process4 = new Process();
	//process4->setID(4); process4->setCPU(10);

	//Process* process5 = new Process();
	//process5->setID(5); process5->setCPU(10);

	//Process* process6 = new Process();
	//process6->setID(6); process6->setCPU(10);

	//Process* process7 = new Process();
	//process7->setID(7); process7->setCPU(10);

	//Process* process8 = new Process();
	//process8->setID(8); process8->setCPU(10);

	//Process* process9 = new Process();
	//process9->setID(9); process9->setCPU(10);

	//Process* process10 = new Process();
	//process10->setID(10); process10->setCPU(10);

	//process->insert(process2);
	//process->insert(process3);

	//process2->insert(process4);
	//process2->insert(process5);

	//process3->insert(process6);
	//process3->insert(process7);

	//process4->insert(process8);
	//process4->insert(process9);

	//process5->insert(process10);

	//process2->TerminateParent(TERMINATE);

	//std::cout << process->getStatus() << " ";
	//std::cout << process2->getStatus() << " ";
	//std::cout << process3->getStatus() << " ";
	//std::cout << process4->getStatus() << " ";
	//std::cout << process5->getStatus() << " ";
	//std::cout << process6->getStatus() << " ";
	//std::cout << process7->getStatus() << " ";
	//std::cout << process8->getStatus() << " ";
	//std::cout << process9->getStatus() << " ";
	//std::cout << process10->getStatus() << " ";

	Wrapper wrapper;
	wrapper.Run();
}
