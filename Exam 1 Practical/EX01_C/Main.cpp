// Include standard headers
#include "Main.h"

int main(void)
{
	Application* pApp = new Application();
	pApp->Init("", RES_S_1080, false, false);
	pApp->Run();
	SafeDelete(pApp);
	return 0;
}