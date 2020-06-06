#pragma once
#include <Unknwn.h>
class cProcessManager
{
private:
	typedef struct sProcess {
		void((*Function))();
		sProcess *Next;
	} sProcess;
protected:
	sProcess *m_ProcessParent;
public:
	cProcessManager() { m_ProcessParent = NULL; }
	~cProcessManager();
	void Add(void(*Process)());
	void Process();
};

