#include "interfaces/IExecutable.h"

extern iExecutable* CreateExecutable();

int main()
{
	auto exe = CreateExecutable();
	
	uint64 error = 0;
	
	try
	{
		error = exe->Run();
		return (int)error;
	}
	catch (const std::exception& except)
	{
		std::cout << except.what() << "\n";
		return (int)error;
	}
}