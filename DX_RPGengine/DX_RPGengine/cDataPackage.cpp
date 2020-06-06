#include "cDataPackage.h"

cDataPackage::cDataPackage()
{
	m_Buf = NULL;
	m_Size = 0;
}

cDataPackage::~cDataPackage()
{
	Free();
}

void * cDataPackage::Create(unsigned long Size)
{
	Free();
	return (m_Buf = (void*)new char[(m_Size = Size)]);
}

void cDataPackage::Free()
{
	delete m_Buf;
	m_Buf = NULL;
	m_Size = 0;
}

BOOL cDataPackage::Save(char * Filename)
{
	FILE *fp;

	if (m_Buf != NULL && m_Size) {
		if ((fp = fopen(Filename, "wb")) != NULL) {
			fwrite(&m_Size, 1, 4, fp);
			fwrite(m_Buf, 1, m_Size, fp);
			fclose(fp);
			return TRUE;
		}
	}
	return FALSE;
}

void * cDataPackage::Load(char * Filename, unsigned long * Size)
{
	FILE *fp;
	Free();
	if ((fp = fopen(Filename, "rb")) != NULL) {
		fread(&m_Size, 1, 4, fp);
		if ((m_Buf = (void*)new char[m_Size]) != NULL)
			fread(m_Buf, 1, m_Size, fp);
		fclose(fp);

		if (Size != NULL)
			*Size = m_Size;
		return m_Buf;
	}
	return NULL;
}
