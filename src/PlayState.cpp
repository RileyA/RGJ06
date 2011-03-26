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
		mGfx->setLinearFog(5.f,65.f,Colour(0.f,0.f,0.05f));
		mBullet->startSimulation();
		mBullet->setGravity(Vector3(0,-10,0));

		/*Spline* mSpline = new Spline();
		mSpline->addPoint(Vector3(0,0,0));
		mSpline->addPoint(Vector3(0,1,0));
		mSpline->addPoint(Vector3(0,2,0));
		mSpline->addPoint(Vector3(0,3,0));
		delete mSpline;*/
	
		mPlayerPos = Vector3(0,0,0);
		
		mTunnel = new SplineTunnel();
		//Console* mConsole = new Console();
		mCamera = new FPSCamera();
		mExplosions = new ExplosionManager;

		EventHandler::getDestination("OISSubsystem")->getSignal("mouseMoved")
			->addListener(createSlot("mouseMoved",this,&PlayState::mouseMove));
	}
	//-----------------------------------------------------------------------
	
	void PlayState::update(Real delta)
	{
		if(mInput->isKeyDown("KC_ESCAPE"))
			sendMessage(MessageAny<String>("kill"),"Engine");
		if(mInput->wasKeyPressed("KC_HOME"))
			mInput->toggleMouseGrab();	

		Vector3 d = mTunnel->getPlayerDirection();

		mCamera->mCamera->setDirection(d);

		Quaternion plane = Vector3::NEGATIVE_UNIT_Z.getRotationTo(d);

		Vector3 offset = plane * mPlayerPos;

		mCamera->mPosNode->setPosition(mTunnel->getPlayerPosition()+offset);

		//Vector3 move = 
		//	mCamera->getDirection()*((mInput->isKeyDown("KC_S")*-1+mInput->isKeyDown("KC_W"))*1) +
		//	mCamera->mCamera->getAbsoluteRight()*(mInput->isKeyDown("KC_D")-mInput->isKeyDown("KC_A"));
	}
	//-----------------------------------------------------------------------
	
	void PlayState::deinit()
	{
		
	}
	//-----------------------------------------------------------------------	

	void PlayState::mouseMove(const Message& message)
	{
		if(const Vector2* v = unpackMsg<Vector2>(message))
		{
			mPlayerPos += Vector3(v->x/100.f,v->y/100.f,0);
			Real len = mPlayerPos.normalize();
			len = std::min(4.1f,len);
			mPlayerPos *= len;
			//std::cout<<v->x<<" "<<v->y<<"\n";
		}
	}
}
