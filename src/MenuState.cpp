#include "MenuState.h"
#include "PlayState.h"
#include "OryxEventHandler.h"
#include "OryxMessageAny.h"
#include "Console.h"

namespace RGJ
{
	MenuState::MenuState(){}
	//-----------------------------------------------------------------------
	
	void MenuState::init()
	{
		Console* mConsole = new Console();
		mGfx = dynamic_cast<OgreSubsystem*>(mEngine->getSubsystem("OgreSubsystem"));
		mAudio = dynamic_cast<ALSubsystem*>(mEngine->getSubsystem("ALSubsystem"));
		mScript = dynamic_cast<ChaiscriptSubsystem*>(mEngine->getSubsystem("ChaiscriptSubsystem"));
		mInput = dynamic_cast<OISSubsystem*>(mEngine->getSubsystem("OISSubsystem"));
		mInput->initInput(mGfx->getWindowHandle(), false);

		// set up menu
		GUI* gui = mGfx->getGUI();
		GUIScreen* scrn = mGfx->getGUI()->createScreen(mGfx->getMainViewport(),"TechDemo","Test");
		scrn->getRootElement(0)->setAspectRatio(4,3);

		StaticText* txt = new StaticText(scrn->getRootElement(0),"Title",0,20,Vector2(0.3f,0.2f),Vector2(0.4,1.f/20.f),"RGJ #6!");
		txt->setColour(Colour(1.f,1.f,1.f));
		txt->setAlign(GA_CENTER);

		GUIRectangle* rect = new GUIRectangle(scrn->getRootElement(1),"cursor","Mouse");
		rect->setScale(Vector2(16.f/1024.f,23.f/768.f));
		scrn->setCursor(rect);

		rect = new GUIRectangle(scrn->getRootElement(0),"Coloredrect01",Colour(1,0,0,0.1),
			Colour(0.3,0.6,0.9,0.1),4.0,0,Vector2(0.1f,0.1f),Vector2(327.f/1024.f,30.f/768.f));
		//rect = new GUIRectangle(scrn->getRootElement(0),"test","Play");
		//rect->setScale(Vector2(327.f/1024.f,80.f/768.f));
		rect->setPosition(Vector2(0.3405f,0.532f));

		txt = new StaticText(scrn->getRootElement(0),"PlayText",0,20,Vector2(0.4f,0.2f),Vector2(327.f/1024.f,30.f/768.f),"Play");
		txt->setColour(Colour(1.f,1.f,1.f));
		txt->setAlign(GA_CENTER);
		txt->setPosition(Vector2(0.3405f,0.532f));
		txt->setHoverColour(Colour(0.8f,0.8f,0.8f));

		//rect = new GUIRectangle(scrn->getRootElement(0),"Coloredrect01",Colour(1,0,0,0.5),
	//		Colour(0.3,0.6,0.9,0.8),4.0,0,Vector2(0.1f,0.1f),Vector2(0.1,0.1));
		//rect->setSprite("test");

		EventHandler::getDestination("OISSubsystem")->getSignal("mouseMovedAbs")
			->addListener(scrn->getSlot("moveCursor"));
		EventHandler::getDestination("OISSubsystem")->getSignal("pressed_MB_Left")
			->addListener(scrn->getSlot("click"));
		EventHandler::getDestination("OISSubsystem")->getSignal("released_MB_Left")
			->addListener(scrn->getSlot("release"));
	}
	//-----------------------------------------------------------------------
	
	void MenuState::update(Real delta)
	{
		if(mInput->isKeyDown("KC_ESCAPE"))
			sendMessage(MessageAny<String>("kill"),"Engine");
		if(mInput->wasKeyPressed("KC_HOME"))
			mInput->toggleMouseGrab();
	}
	//-----------------------------------------------------------------------
	
	void MenuState::deinit()
	{

	}
}
