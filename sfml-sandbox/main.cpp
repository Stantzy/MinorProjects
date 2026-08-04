#include "sandbox.hpp"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

int main()
{
	Sandbox sandbox(WINDOW_WIDTH, WINDOW_HEIGHT);

	return sandbox.run();
}
