#include <BaseApplication.h>
#include <SolarSystemApplication.h>

int main() 
{	
	BaseApplication * app = new SolarSystemApplication();
	if (app->startup())
		app->run();
	app->shutdown();
	return 0;
}