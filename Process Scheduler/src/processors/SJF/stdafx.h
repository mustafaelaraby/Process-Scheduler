#pragma once

#include <iostream>
#include <queue>
#include "../processor/processor.h"
#include "../../../libs/Data Structures/Priority_Queue/Priority_Queue.h"
#include "../../../libs/helpers/helpers.h"


struct comp_less {
	bool operator()(const Process* process1, const Process* process2) const {
		return process1->getCPU() < process2->getCPU();
	}
};