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
		mGfx->setLinearFog(5.f,30.f,Colour(0.f,0.f,0.05f));
		mBullet->startSimulation();

		Console* mConsole = new Console();
		mCamera = new FPSCamera();
		mExplosions = new ExplosionManager;
		mChunks = new ChunkManager(Vector3(0,-10,0));

		mPlayer = mBullet->createBasicCharacterController(Vector3(0,10,0));
	}
	//-----------------------------------------------------------------------
	
	void PlayState::update(Real delta)
	{
		if(mInput->isKeyDown("KC_ESCAPE"))
			sendMessage(MessageAny<String>("kill"),"Engine");
		if(mInput->wasKeyPressed("KC_HOME"))
			mInput->toggleMouseGrab();
		mCamera->mPosNode->setPosition(mPlayer->getPosition()+Vector3(0,0.9f,0));
		mChunks->generate(mCamera->getPosition());

		if(mInput->wasButtonPressed("MB_Left"))
		{
			RaycastReport r = mBullet->raycast(mCamera->getPosition(),mCamera->getDirection(),10000.f,COLLISION_GROUP_3,COLLISION_GROUP_3);

			if(r.hit&&r.userData)
			{
				Chunk* xc = (Chunk*)r.userData;
				if(mInput->isKeyDown("KC_G"))
				{
					xc->killBlocks(r.position,4);
					mExplosions->createExplosion(r.position);
				}
				else
					xc->killBlock(r.position,r.normal);
			}
		}
	}
	//-----------------------------------------------------------------------
	
	void PlayState::deinit()
	{
		
	}
	//-----------------------------------------------------------------------	
}
