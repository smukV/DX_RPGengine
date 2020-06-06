#pragma once
#include <Windows.h>
#include <fstream>
class cDataPackage
{
protected:
	void *m_Buf;
	unsigned long m_Size;
public:
	cDataPackage();
	~cDataPackage();

	void *Create(unsigned long Size);
	void Free();
	BOOL Save(char *Filename);
	void *Load(char *Filename, unsigned long *Size);
};

