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

		mInput->initInput(mGfx->getWindowHandle(), true);
		mGfx->setBackgroundColor(Colour(0.f,0.f,0.05f));
		mGfx->setLinearFog(5.f,65.f,Colour(0.f,0.f,0.05f));
		//mGfx->setBackgroundColor(Colour(1.f,1.f,1.f));
		//mGfx->setLinearFog(5.f,65.f,Colour(1.f,1.f,1.f));

		GUI* gui = mGfx->getGUI();
		GUIScreen* scrn = mGfx->getGUI()->createScreen(mGfx->getMainViewport(),"hud","Test");
		scrn->getRootElement(0)->setAspectRatio(4,3);

		
		crosshair = new GUIRectangle(scrn->getRootElement(1),"cursor","Mouse");
		crosshair->setScale(Vector2(32.f/1024.f,23.f/768.f));

		g = new GUIRectangle(scrn->getRootElement(5),"hit","Mouse");
		Real cooloff = 0.4f;
		g->setScale(Vector2(1,1));
		g->setPosition(Vector2(0,0));
		//g->setBackground(Colour(0.2,0.5,0.6,0.4));
		blue = false;
		done = false;
		g->setBackground(Colour(0.8,0.5,0.2,0.8));

		crosshair->setPosition(Vector2(0.5f,0.5f)-Vector2(32.f/1024.f,23.f/768.f));

		StaticText* txt = new StaticText(scrn->getRootElement(0),
			"Title",0,20,Vector2(0.02f,0.02f),Vector2(0.4,1.f/20.f),"Distance: 0");

		mPlayerPos = Vector3(0,0,0);
		
		mTunnel = new SplineTunnel();
		Console* mConsole = new Console();
		mCamera = new FPSCamera();

		EventHandler::getDestination("OISSubsystem")->getSignal("mouseMoved")
			->addListener(createSlot("mouseMoved",this,&PlayState::mouseMove));
		mTunnel->getSignal("hitLaser")->addListener(createSlot("hitLaser",this,&PlayState::hitLaser));
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

		mTunnel->collide(mTunnel->getPlayerPosition()+offset);

		crosshair->setPosition(Vector2(0.5f,0.5f)-Vector2(32.f/1024.f,23.f/768.f) +
			Vector2(mPlayerPos.x/27.f,mPlayerPos.y/27.f));

		cooloff-=delta;
		if(cooloff<=0.01f&&!done)
		{
			cooloff = 0.01f;
			g->setBackground(blue ? Colour(0.2f,0.5f,0.8f,0.f) :
				Colour(0.9f,0.5f,0.2f,0.f));
			done = true;
		}
		else
		{
			g->setBackground(blue ? Colour(0.2f,0.5f,0.8f,cooloff*2) :
				Colour(0.9f,0.5f,0.2f,cooloff*2));
		}

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
	
	void PlayState::hitLaser(const Message& m)
	{
		if(const bool* b = unpackMsg<bool>(m))
		{
			done = false;
			blue = *b;
			cooloff = 0.4f;
		}
	}
}
