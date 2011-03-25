#include "PlayState.h"
#include "OryxEventHandler.h"
#include "OryxMessageAny.h"
#include "Console.h"

namespace RGJ
{
	PlayState::PlayState(){}
	//-----------------------------------------------------------------------
	
	void PlayState::init()
	{
		mGfx = dynamic_cast<OgreSubsystem*>(mEngine->getSubsystem("OgreSubsystem"));
		mAudio = dynamic_cast<ALSubsystem*>(mEngine->getSubsystem("ALSubsystem"));
		mScript = dynamic_cast<ChaiscriptSubsystem*>(mEngine->getSubsystem("ChaiscriptSubsystem"));
		mInput = dynamic_cast<OISSubsystem*>(mEngine->getSubsystem("OISSubsystem"));
		mInput->initInput(mGfx->getWindowHandle(), false);
		mGfx->setBackgroundColor(Colour(0.f,0.f,0.05f));
		Console* mConsole = new Console();
	}
	//-----------------------------------------------------------------------
	
	void PlayState::update(Real delta)
	{
		if(mInput->isKeyDown("KC_ESCAPE"))
			sendMessage(MessageAny<String>("kill"),"Engine");
		if(mInput->wasKeyPressed("KC_HOME"))
			mInput->toggleMouseGrab();
	}
	//-----------------------------------------------------------------------
	
	void PlayState::deinit()
	{

	}
	//-----------------------------------------------------------------------	
}
