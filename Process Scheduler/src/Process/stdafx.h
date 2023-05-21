#pragma once

#include <string>
#include <queue>
#include "../../libs/Data Structures/Linked Queue/Linked_Queue.h"
#include "../../libs/Data Structures/Queue/Queue.h"
#include "../../libs/Data Structures/Stack/Stack.h"

enum FLAG { C, R, D, IO_R, IO_D };
enum STUTUS { NEW, READY, RUN, BLOCK, ORPHAN, FORKED, TERMINATE };
