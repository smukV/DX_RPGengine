#pragma once
#include <Unknwn.h>
class cStateManager
{
private:
	typedef struct sState {
		void((*Function))();
		sState *Next;
	} sState;
protected:
	sState *m_StateParent;
public:
	cStateManager() { m_StateParent = NULL; }
	~cStateManager();
	void Push(void(*Function)());
	
	BOOL Pop();
	
	BOOL Process();
	
};
