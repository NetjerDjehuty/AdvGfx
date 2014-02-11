#include "MainViewer.h"

using namespace GUI;

[STAThreadAttribute]
int main(array<System::String^>^ args)
{
	Application::Run(gcnew MainViewer());

	return 0;
}