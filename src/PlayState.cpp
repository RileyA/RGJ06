#include "PlayState.h"
#include "OryxEventHandler.h"
#include "OryxMessageAny.h"
#include "Console.h"
#include "BulletSubsystem/BasicCharacterController.h"

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
		mBullet = dynamic_cast<BulletSubsystem*>(mEngine->getSubsystem("BulletSubsystem"));

		mInput->initInput(mGfx->getWindowHandle(), false);
		mGfx->setBackgroundColor(Colour(0.f,0.f,0.05f));
		//mGfx->setLinearFog(5.f,30.f,Colour(0.f,0.f,0.05f));
		mBullet->startSimulation();
		mBullet->setGravity(Vector3(0,-10,0));

		Console* mConsole = new Console();
		mCamera = new FPSCamera();
		mExplosions = new ExplosionManager;
	}
	//-----------------------------------------------------------------------
	
	void PlayState::update(Real delta)
	{
		if(mInput->isKeyDown("KC_ESCAPE"))
			sendMessage(MessageAny<String>("kill"),"Engine");
		if(mInput->wasKeyPressed("KC_HOME"))
			mInput->toggleMouseGrab();
		
		//Vector3 move = 
		//	mCamera->getDirection()*((mInput->isKeyDown("KC_S")*-1+mInput->isKeyDown("KC_W"))*1) +
		//	mCamera->mCamera->getAbsoluteRight()*(mInput->isKeyDown("KC_D")-mInput->isKeyDown("KC_A"));
	}
	//-----------------------------------------------------------------------
	
	void PlayState::deinit()
	{
		
	}
	//-----------------------------------------------------------------------	
}
