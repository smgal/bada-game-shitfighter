
#include "../avej_config.h"

extern int AvejMain(void);

int main(int argc, char *argv[])
{
	ENABLE_MEMORY_MANAGER
//	_CrtSetBreakAlloc(??);

	return AvejMain();
}
