#include <BaseApplication.h>
#include <Texturing.h>

int main() 
{	
	BaseApplication * app = new Texturing();
	if (app->startup())
		app->run();
	app->shutdown();
	return 0;
}