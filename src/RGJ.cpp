#include "RGJ.h"
#include "MenuState.h"

int main(int argc, char** argv)
{
	Logger::getPtr();
	TimeManager::getPtr();

	// create subsystems:
	ALSubsystem aSys = ALSubsystem();
	OgreSubsystem ogre = OgreSubsystem(1024,768,false);
	OISSubsystem ois = OISSubsystem();

	// allocate engine and add subsystems
	Engine* eng = new Engine();
	eng->addSubsystem(&aSys);
	eng->addSubsystem(&ogre);
	eng->addSubsystem(&ois);

	// initialize the engine
	eng->init();

	// add the menu state to the stack
	eng->addState(new RGJ::MenuState());

	// start up the engine
	eng->start();

	// delete the engine object
	delete eng;

	return 0;
}
