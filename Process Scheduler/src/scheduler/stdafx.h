#pragma once

#include <fstream>
#include <iostream>
#include <stdio.h>
#include <list>
#include <queue>
#include <typeinfo>
#include <limits>
#include <stdexcept>
#include <type_traits>
#include <filesystem>
#include "../../libs/helpers/helpers.h"
#include "../processors/processor/processor.h"
#include "../processors/FCFS/FCFS.h"
#include "../processors/SJF/SJF.h"
#include "../processors/Round_Robin/Round_Robin.h"
#include "../processors/EDF/EDF.h"
#include "../Process/Process.h"
#include <sstream>
#include <string>
#include "../../libs/Data Structures/Priority_Queue/Priority_Queue.h"

struct TT_less {
	bool operator()(const Process* process1, const Process* process2) const {
		return process1->getTRM() < process2->getTRM();
	}
};

struct Pr_data
{
	int p;
	int id;
};
