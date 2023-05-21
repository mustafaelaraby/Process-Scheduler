#include "Process.h"

Process::Process()
	: id(0), arrival_time(0),
	response_time(0), cpu_time(0),
	step(0), stepR(0), stepD(0),
	trm_time(0), trt_time(0),
	wait_time(0), num_io(0), forked(false),
	blocked_time(0), status(NEW), deadline(0),
	processor(""), fork_num(0) {}

Process::Process(const Process& other)
	: id(other.id), arrival_time(other.arrival_time),
	response_time(other.response_time), cpu_time(other.cpu_time),
	step(other.step), stepR(other.stepR), stepD(other.stepD),
	trm_time(other.trm_time), trt_time(other.trt_time),
	wait_time(other.wait_time), num_io(other.num_io), forked(other.forked),
	blocked_time(other.blocked_time), status(other.status), deadline(other.deadline),
	processor(other.processor), IOR(other.IOR), IOD(other.IOD), fork_num(other.fork_num)
{
	if (other.left)
		left = new Process(*other.left);
	if (other.right)
		right = new Process(*other.right);

	// Deep copy IOR and IOD queues
	IOR = other.IOR;
	IOD = other.IOD;
}

Process::Process(Process&& other)noexcept
	: id(other.id),
	arrival_time(other.arrival_time),
	cpu_time(other.cpu_time),
	response_time(other.response_time),
	trm_time(other.trm_time),
	trt_time(other.trt_time),
	wait_time(other.wait_time),
	num_io(other.num_io), forked(other.forked),
	blocked_time(other.blocked_time),
	deadline(other.deadline),
	processor(std::move(other.processor)),
	status(std::move(other.status)),
	step(other.step),
	stepR(other.stepR),
	stepD(other.stepD),
	fork_num(other.fork_num),
	IOR(std::move(other.IOR)),
	IOD(std::move(other.IOD)),
	left(other.left),
	right(other.right)
{
	other.left = nullptr;
	other.right = nullptr;
}

Process::~Process()
{
	delete left;
	delete right;
}

//============================= GETTERS ====================================\\

int Process::getID()				  const { return id; }
int Process::getArrival()			  const { return arrival_time; }
int Process::getResponse()			  const { return response_time; }
int Process::getCPU()				  const { return cpu_time; }
int Process::getTRM()				  const { return trm_time; }
int Process::getTRT()				  const { return trt_time; }
int Process::getWait()				  const { return wait_time; }
int Process::getNumIO()				  const { return num_io; }
bool Process::isForked()			  const { return forked; }
int Process::getBLK()				  const { return blocked_time; }
std::string Process::getProcessor()   const { return processor; }
STUTUS Process::getStatus()           const { return status; }

bool Process::canFork() const
{
	if (fork_num < 2)
		return true;
	else
		return false;
}

int Process::getStep(FLAG flag)       const
{
	switch (flag)
	{
	case C:return step; break;
	case R:return stepR; break;
	case D:return stepD; break;
	default:break;
	}
}

int Process::getIO(FLAG flag)
{
	int temp = -5000;
	if (flag == R)
	{
		if (!IOR.empty())
			temp = IOR.front();
	}
	else if (flag == D)
	{
		if (!IOD.empty())
			temp = IOD.front();
	}
	return temp;
}

int Process::getDeadline() const
{
	return deadline;
}


//============================= SETTERS ====================================\\

void Process::setID(int id) { this->id = id; }
void Process::setArrival(int arrival) { this->arrival_time = arrival; }
void Process::setResponse(int resp) { this->response_time = resp; }
void Process::setCPU(int cpu) { this->cpu_time = cpu; }
void Process::setTRM(int trm) { this->trm_time = trm; }
void Process::setTRT(int trt) { this->trt_time = trt; }
void Process::setWait(int w) { this->wait_time = w; }
void Process::setNumIO(int num) { this->num_io = num; }
void Process::setForked(bool f_cond) { forked = f_cond; }
void Process::setBLK(int blk) { this->blocked_time = blk; }
void Process::setDeadline(int d)
{
	deadline = d;
}

void Process::setProcessor(std::string p) { this->processor = p; }
void Process::setStutus(STUTUS newStatus)
{
	status = newStatus;
}

void Process::incrementStep(FLAG flag)
{
	switch (flag)
	{
	case C:
		if (step < cpu_time)
			++step;
		break;
	case R:
		if (!IOR.empty() && stepR < IOR.front())
			++stepR;
		break;
	case D:
		if (!IOD.empty() && stepD < IOD.front())
			++stepD;
		break;
	default:break;
	}
}

void Process::setStep(int num, FLAG flag)
{
	switch (flag)
	{
	case C:step = num; break;
	case R:stepR = num; break;
	case D:stepD = num; break;
	default:break;
	}
}

void Process::IO_Push(int num, FLAG flag)
{
	switch (flag)
	{
	case R:this->IOR.push(num); break;
	case D:this->IOD.push(num); break;
	default:break;
	}
}

void Process::IO_Pop(FLAG flag)
{
	switch (flag)
	{
	case R:this->IOR.pop(); break;
	case D:this->IOD.pop(); break;
	default:break;
	}
}

void Process::incrementBlk()
{
	++blocked_time;
}

void Process::incrementWait()
{
	++wait_time;
}



//============================= Binary Tree Functions ======================\\

void Process::insert(Process* child) {
	if (child->getID() < getID()) {
		if (left == nullptr)
			left = child;
		else
			left->insert(child);
	}
	else {
		if (right == nullptr)
			right = child;
		else
			right->insert(child);
	}
	++fork_num;
}

void Process::erase(int id)
{
	if (id < this->id)
	{
		if (left)
		{
			if (left->id == id)
			{
				delete left;
				left = nullptr;
			}
			else
			{
				left->erase(id);
			}
		}
	}
	else if (id > this->id)
	{
		if (right)
		{
			if (right->id == id)
			{
				delete right;
				right = nullptr;
			}
			else
			{
				right->erase(id);
			}
		}
	}
}

bool Process::contains(int id) const
{
	if (id < this->id)
	{
		if (left)
			return left->contains(id);
		else
			return false;
	}
	else if (id > this->id)
	{
		if (right)
			return right->contains(id);
		else
			return false;
	}
	else
	{
		return true;
	}
}

const Process* Process::find(int id) const
{
	if (id < this->id)
	{
		if (left)
			return left->find(id);
		else
			return nullptr;
	}
	else if (id > this->id)
	{
		if (right)
			return right->find(id);
		else
			return nullptr;
	}
	else
	{
		return this;
	}
}

std::size_t Process::size() const
{
	std::size_t count = 1;
	if (left)
		count += left->size();
	if (right)
		count += right->size();
	return count;
}

bool Process::empty() const
{
	return (left == nullptr && right == nullptr);
}

Queue<int> Process::getAllIDs() const {
	Queue<int> idQueue;
	getAllIDsHelper(idQueue);
	return idQueue;
}

void Process::getAllIDsHelper(Queue<int>& idQueue) const {
	idQueue.push(getID());

	if (left)
		left->getAllIDsHelper(idQueue);

	if (right)
		right->getAllIDsHelper(idQueue);
}

void Process::TerminateParent(STUTUS newStatus)
{
	status = newStatus;
	if (left != nullptr) {
		left->TerminateParent(ORPHAN);
	}
	if (right != nullptr) {
		right->TerminateParent(ORPHAN);
	}
}


//============================= Operators ==================================\\  // NOLINT(clang-diagnostic-comment)

Process& Process::operator=(const Process& other)
{
	if (this != &other)
	{
		id = other.id;
		arrival_time = other.arrival_time;
		response_time = other.response_time;
		cpu_time = other.cpu_time;
		step = other.step;
		stepR = other.stepR;
		stepD = other.stepD;
		trm_time = other.trm_time;
		trt_time = other.trt_time;
		wait_time = other.wait_time;
		num_io = other.num_io;
		forked = other.forked;
		blocked_time = other.blocked_time;
		deadline = other.deadline;
		status = other.status;
		processor = other.processor;

		// Deep copy IOR and IOD queues
		IOR = other.IOR;
		IOD = other.IOD;

		delete left;
		delete right;

		if (other.left)
			left = new Process(*other.left);
		else
			left = nullptr;

		if (other.right)
			right = new Process(*other.right);
		else
			right = nullptr;

	}
	return *this;
}

Process& Process::operator=(Process&& other)
{
	if (this != &other)
	{
		id = std::move(other.id);
		arrival_time = std::move(other.arrival_time);
		response_time = std::move(other.response_time);
		cpu_time = std::move(other.cpu_time);
		step = std::move(other.step);
		stepR = std::move(other.stepR);
		stepD = std::move(other.stepD);
		trm_time = std::move(other.trm_time);
		trt_time = std::move(other.trt_time);
		wait_time = std::move(other.wait_time);
		num_io = std::move(other.num_io);
		forked = std::move(other.forked);
		blocked_time = std::move(other.blocked_time);
		deadline = std::move(other.deadline);
		status = std::move(other.status);
		processor = std::move(other.processor);

		// Move IOR and IOD queues
		IOR = std::move(other.IOR);
		IOD = std::move(other.IOD);

		delete left;
		delete right;

		left = other.left;
		right = other.right;

		other.left = nullptr;
		other.right = nullptr;

	}
	return *this;
}

