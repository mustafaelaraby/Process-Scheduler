#pragma once

#include "../processor/processor.h"
#include "../../libs/Data Structures/Priority_Queue/Priority_Queue.h"
#include "../../Process/Process.h"
#include "../../libs/helpers/helpers.h"

struct deadline_less {
	bool operator()(const Process* process1, const Process* process2) const {
		return process1->getDeadline() < process2->getDeadline();
	}
};