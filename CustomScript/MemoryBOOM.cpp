#include "MemoryBOOM.h"

MemoryBOOM::MemoryBOOM()
{
}

MemoryBOOM::~MemoryBOOM()
{
	delete boomFLOAT;
}

void MemoryBOOM::BOOM()
{
	boomFLOAT = new float[boomNum];
}

void MemoryBOOM::Delete()
{
	delete boomFLOAT;
}
