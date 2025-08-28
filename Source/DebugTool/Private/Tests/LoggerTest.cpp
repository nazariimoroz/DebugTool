#include "Misc/AutomationTest.h"
#include "DebugTool/DT_Logger.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FDT_LoggerTest, "DebugTool.Source.DebugTool.Logger",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FDT_LoggerTest::RunTest(const FString& Parameters)
{
	DT_DISPLAY("Test1 {}, Test2 {}", 10, 11);
	DT_DISPLAY() << "Test1 " << 10 << ", Test2 " << 11;
	/**
	* file.cpp(2): Test10, Test11
	*/

	#if 0
	DT_WARNING() << "Test1" << 10 << ", Test2" << 11;
	/**
	* file.cpp(3): Test10, Test11
	*/

	DT_ERROR("Error Message {}", 10);
	/**
	* file.cpp(4): Error Message 10
	*
	* Stack Trace:
	* ...
	*/

	DT_BREAKPOINT();
	/**
	* file.cpp(5): BREAKPOINT
	*
	* Stack Trace:
	* ...
	*/

	([]
	{
		DT_RETURN_IF(true && "Test");
		/**
		* file.cpp(6): true && "Test" (Test1: UObject1)
		*
		* Stack Trace:
		* ...
		*/
	})();

	([]()
	{
		DT_RETURN_A_IF(true && "Test", 10);
		/**
		* file.cpp(6): true && "Test" (Test1: UObject1)
		*
		* Stack Trace:
		* ...
		*/

		return 11;
	})();
#endif

	return true;
}
