#include <iostream>
#include <Unknwn.h>
#include <string>
#include <algorithm>
#include <iterator>
#include <cctype>
#include "cStateManager.h"
#include "cProcessManager.h"
using namespace std;


cStateManager SM;
cProcessManager PM;

#define MB(s) MessageBox(NULL, s, s, MB_OK);

void Func1() { MB("1"); SM.Pop(); }
void Func2() { MB("2"); SM.Pop(); }
void Func3() { MB("3"); SM.Pop(); }




int PASCAL WinMain(HINSTANCE hInst, HINSTANCE hPrev,
					LPSTR szCmdLine, int nCmdShow)

{
	BOOL Active;
	DWORD Tid;

	SM.Push(Func1);
	SM.Push(Func2);
	SM.Push(Func3);

	PM.Add(Func1);
	PM.Add(Func2);
	PM.Add(Func3);
	//PM.Process();
	//PM.Process();
	while (SM.Process() == TRUE);
	while (SM.Process() == TRUE);

	system("pause");
	return 0;
}