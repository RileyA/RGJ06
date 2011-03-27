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

		mBest = 0.f;
		std::ifstream file;
		file.open("../media/gui/personalbest.score");
		if(file.is_open())
		{
			file>>mBest;
			file.close();
		}

		balance = 0;
		limit = 2;
		mGameOver = false;

		music = mAudio->play2D("../media/audio/Anamanaguchi_-_03_-_Airbase.ogg");//.getData().mLooping = true;

		mInput->initInput(mGfx->getWindowHandle(), true);
		mGfx->setBackgroundColor(Colour(0.f,0.f,0.05f));
		mGfx->setLinearFog(5.f,65.f,Colour(0.f,0.f,0.05f));
		//mGfx->setBackgroundColor(Colour(1.f,1.f,1.f));
		//mGfx->setLinearFog(5.f,65.f,Colour(1.f,1.f,1.f));
		//
		Colour HUD = Colour(1,1,1,0.7f);

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
		crosshair->setBackground(HUD);

		distance = new StaticText(scrn->getRootElement(0),
			"Title",0,20,Vector2(0.02f,0.02f),Vector2(0.4,1.f/20.f),"Distance: 0");
		distance->setColour(HUD);

		time = new StaticText(scrn->getRootElement(0),
			"Title",0,20,Vector2(0.02f,0.075f),Vector2(0.4,1.f/20.f),"Time: 00:00:00");
		time->setColour(HUD);

		frame = new GUIRectangle(scrn->getRootElement(0),"Coloredrect01",Colour(0.2,0.2,0.5,0.f),
			HUD,4.0,0,Vector2(0.2f,0.88f),Vector2(0.6f,0.05f));

		blueb = new GUIRectangle(scrn->getRootElement(0),"Coloredrect02",Colour(0.2,0.5,0.8,0.7f),
			Colour(0.3,0.8,0.4,0.f),4.0,0,Vector2(0.2f,0.88f),Vector2(0.3f,0.05f));

		orangeb = new GUIRectangle(scrn->getRootElement(0),"Coloredrect03",Colour(0.8,0.5,0.2,0.7f),
			Colour(0.3,0.8,0.4,0.f),4.0,0,Vector2(0.5f,0.88f),Vector2(0.3f,0.05f));

		gameover = new GUIRectangle(scrn->getRootElement(5),"gameover",Colour(0.f,0.f,0.f,0.8f),
			Colour(0.3,0.8,0.4,0.f),4.0,0,Vector2(2.f,2.f),Vector2(1.f,1.f));

		StaticText* tt = new StaticText(gameover,
			"Title",0,20,Vector2(0.2f,0.3f),Vector2(0.6,1.f/20.f),"Game Over!");
		tt->setAlign(GA_CENTER);


		score = new StaticText(gameover,
			"Title",0,20,Vector2(0.2f,0.4f),Vector2(0.6,1.f/20.f),"Distance: 500m");
		score->setAlign(GA_CENTER);

		best = new StaticText(gameover,
			"Title",0,20,Vector2(0.2f,0.45f),Vector2(0.6,1.f/20.f),"Personal Best: "
				+StringUtils::toString(mBest)+"m");
		best->setAlign(GA_CENTER);

		tt = new StaticText(gameover,
			"Title",0,20,Vector2(0.2f,0.55f),Vector2(0.6,1.f/20.f),"Press ENTER to play again.");
		tt->setAlign(GA_CENTER);

		tt = new StaticText(gameover,
			"Title",0,20,Vector2(0.2f,0.6f),Vector2(0.6,1.f/20.f),"Or ESC to quit.");
		tt->setAlign(GA_CENTER);

		mPlayerPos = Vector3(0,0,0);
		
		mTunnel = new SplineTunnel();
		Console* mConsole = new Console();
		mCamera = new FPSCamera();

		EventHandler::getDestination("OISSubsystem")->getSignal("mouseMoved")
			->addListener(createSlot("mouseMoved",this,&PlayState::mouseMove));
		mTunnel->getSignal("hitLaser")->addListener(createSlot("hitLaser",this,&PlayState::hitLaser));
		mTime = 0.f;
	}
	//-----------------------------------------------------------------------
	
	void PlayState::update(Real delta)
	{
		if(!music.isActive())
		{
			music = mAudio->play2D("../media/audio/Anamanaguchi_-_03_-_Airbase.ogg");//.getData().mLooping = true;
		}
		if(!mGameOver)
		{
			mTime+=delta;

			Vector3 d = mTunnel->getPlayerDirection();

			mCamera->mCamera->setDirection(d);

			Quaternion plane = Vector3::NEGATIVE_UNIT_Z.getRotationTo(d);

			Vector3 offset = plane * mPlayerPos;

			mCamera->mPosNode->setPosition(mTunnel->getPlayerPosition()+offset);

			mTunnel->collide(mTunnel->getPlayerPosition()+offset);

			crosshair->setPosition(Vector2(0.5f,0.5f)-Vector2(32.f/1024.f,23.f/768.f) +
				Vector2(mPlayerPos.x/27.f,mPlayerPos.y/27.f));

			Vector2 hudOffset = Vector2(mPlayerPos.x/140.f,mPlayerPos.y/140.f);

			frame->setPosition(Vector2(0.2,0.88)+hudOffset);
			blueb->setPosition(Vector2(0.2,0.88)+hudOffset);
			orangeb->setPosition(Vector2(0.5+balance*(0.3/limit),0.88)+hudOffset);
			distance->setPosition(Vector2(0.05,0.05)+hudOffset);
			time->setPosition(Vector2(0.05,0.105)+hudOffset);

			blueb->setScale(Vector2(0.3f+balance*(0.3/limit),0.05f));
			orangeb->setScale(Vector2(0.3f-balance*(0.3/limit),0.05f));

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


			int mins = floor(mTime / 60);
			Real tmpTime = mTime - 60*mins;
			int secs = floor(tmpTime);
			tmpTime -= floor(tmpTime);
			int mills = tmpTime*100;

			time->setCaption("Time: "+String(mins<10?"0":"")+StringUtils::toString(mins)+":"+String(secs<10?"0":"")
				+StringUtils::toString(secs)+":"+String(mills<10?"0":"")+StringUtils::toString(mills));
			distance->setCaption("Distance: "+StringUtils::toString(mTunnel->getDist())+"m");
		}
		else
		{
			if(mInput->isKeyDown("KC_RETURN"))
			{
				//mEngine->addState(new PlayState());
				//sendMessage(MessageAny<String>("end state"),"Engine");
				//
				// shiiiiiit, gorilla dies if you recreate texture atlases (like between states),
				// no time to fix it, so I guess I get to restart stuff here... :/
				mGameOver = false;
				mTime = 0.f;
				balance = 0;
				mTunnel->reset();
				gameover->setPosition(Vector2(2,2));
			}
		}

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
			if(blue)
			{
				++balance;
				mAudio->play2D("../media/audio/hit4.wav");
			}
			else
			{
				--balance;
				mAudio->play2D("../media/audio/hit3.wav");
			}
			if(balance==limit||-balance==limit)
			{
				mGameOver = true;
				mTunnel->gameover = true;
				gameover->setPosition(Vector2(0,0));
				mPlayerPos = Vector3(0,0,0);
				if(mTunnel->getDist() > mBest)
				{
					mBest = mTunnel->getDist();
					std::ofstream file;
					file.open("../media/gui/personalbest.score");
					file<<mBest;
					file.close();
				}
				score->setCaption("Distance: "+StringUtils::toString(mTunnel->getDist())+"m");
				best->setCaption("Personal Best: "+StringUtils::toString(mBest)+"m");
			}
			cooloff = 0.4f;
		}
	}
}
