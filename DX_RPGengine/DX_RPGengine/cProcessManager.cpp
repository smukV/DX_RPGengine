#include "cProcessManager.h"

cProcessManager::~cProcessManager()
{
	sProcess *ProcessPtr;

	while ((ProcessPtr = m_ProcessParent) != NULL) {
		m_ProcessParent = ProcessPtr->Next;
		delete ProcessPtr;
	}
}

void cProcessManager::Add(void(*Process)())
{
	if (Process != NULL) {
		sProcess *ProcessPtr = new sProcess;
		ProcessPtr->Next = m_ProcessParent;
		m_ProcessParent = ProcessPtr;
		ProcessPtr->Function = Process;
	}
}

void cProcessManager::Process()
{
	sProcess *ProcessPtr = m_ProcessParent;

	while (ProcessPtr != NULL) {
		ProcessPtr->Function();
		ProcessPtr = ProcessPtr->Next;
	}
}


