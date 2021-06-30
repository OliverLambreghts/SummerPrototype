#include "pch.h"
#include "Core.h"
#include <ctime>
#include "vld.h"

void StartHeapControl();
void DumpMemoryLeaks();

#pragma warning( push )
#pragma warning( disable : 4100 )
int main( int argc, char *argv[] )
{
	srand(static_cast<unsigned int>(time(nullptr)));
	{
		//StartHeapControl();

		Core core{ Window{ "Prototype Game", 640.0f, 640.0f } };
		core.Run();
	}
	//DumpMemoryLeaks();
	return 0;
}
#pragma warning( pop )

void StartHeapControl()
{
#if defined(DEBUG) | defined(_DEBUG)
	// Notify user if heap is corrupt
	HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

	// Report detected leaks when the program exits
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// Set a breakpoint on the specified object allocation order number
	//_CrtSetBreakAlloc( 194 );
#endif
}

void DumpMemoryLeaks()
{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtDumpMemoryLeaks();
#endif
}


