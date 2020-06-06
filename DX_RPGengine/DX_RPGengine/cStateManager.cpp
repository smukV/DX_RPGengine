#include "cStateManager.h"

cStateManager::~cStateManager()
{
	sState *StatePtr;

	while ((StatePtr = m_StateParent) != NULL) {
		m_StateParent = StatePtr->Next;
		delete StatePtr;
	}
}

void cStateManager::Push(void(*Function)())
{
	if (Function != NULL) {
		sState *StatePtr = new sState;
		StatePtr->Next = m_StateParent;
		m_StateParent = StatePtr;
		StatePtr->Function = Function;
	}
}

BOOL cStateManager::Pop()
{
	sState *StatePtr = m_StateParent;
	if (StatePtr != NULL) {
		m_StateParent = StatePtr->Next;
		delete StatePtr;
	}
	if (m_StateParent == NULL)
		return FALSE;
	return TRUE;
}

BOOL cStateManager::Process()
{
	if (m_StateParent == NULL)
		return FALSE;
	m_StateParent->Function();
	return TRUE;
}
