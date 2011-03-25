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
		mGfx = dynamic_cast<OgreSubsystem*>(mEngine->getSubsystem("OgreSubsystem"));
		mAudio = dynamic_cast<ALSubsystem*>(mEngine->getSubsystem("ALSubsystem"));
		mScript = dynamic_cast<ChaiscriptSubsystem*>(mEngine->getSubsystem("ChaiscriptSubsystem"));
		mInput = dynamic_cast<OISSubsystem*>(mEngine->getSubsystem("OISSubsystem"));
		mInput->initInput(mGfx->getWindowHandle(), false);

		mGfx->setBackgroundColor(Colour(0.f,0.f,0.05f));

		// set up menu
		GUI* gui = mGfx->getGUI();
		GUIScreen* scrn = mGfx->getGUI()->createScreen(mGfx->getMainViewport(),"TechDemo","Test");
		scrn->getRootElement(0)->setAspectRatio(4,3);

		StaticText* txt = new StaticText(scrn->getRootElement(0),"Title",0,20,Vector2(0.3f,0.05f),Vector2(0.4,1.f/20.f),"Untitled");
		txt->setColour(Colour(1.f,1.f,1.f));
		txt->setAlign(GA_CENTER);

		txt = new StaticText(scrn->getRootElement(0),"Subtitle",0,20,Vector2(0.2f,0.11f),Vector2(0.6,1.f/20.f),"A 48hr Reddit Game Jam #6 Game");
		txt->setColour(Colour(1.f,1.f,1.f));
		txt->setAlign(GA_CENTER);

		txt = new StaticText(scrn->getRootElement(0),"Title",0,20,Vector2(0.01f,0.95f),Vector2(1.f,1.f/20.f),"(C) Riley Adams 2011");
		txt->setColour(Colour(1.f,1.f,1.f));
		
		txt = new StaticText(scrn->getRootElement(0),"Instr",0,20,Vector2(0.05f,0.2f),Vector2(1.f,1.f/20.f),"Instructions: ");
		txt = new StaticText(scrn->getRootElement(0),"Instr",0,20,Vector2(0.1f,0.25f),Vector2(1.f,1.f/20.f),"Do stuff. Win.");

		txt = new StaticText(scrn->getRootElement(0),"Controls",0,20,Vector2(0.05f,0.4f),Vector2(1.f,1.f/20.f),"Controls: ");
		txt = new StaticText(scrn->getRootElement(0),"Controls",0,20,Vector2(0.1f,0.45f),Vector2(1.f,1.f/20.f),"WASD - Move");
		txt = new StaticText(scrn->getRootElement(0),"Controls",0,20,Vector2(0.1f,0.5f),Vector2(1.f,1.f/20.f),"Mouse - Look");
		txt = new StaticText(scrn->getRootElement(0),"Controls",0,20,Vector2(0.1f,0.55f),Vector2(1.f,1.f/20.f),"Q - Something");
		txt = new StaticText(scrn->getRootElement(0),"Controls",0,20,Vector2(0.1f,0.6f),Vector2(1.f,1.f/20.f),"R - Something");
		txt = new StaticText(scrn->getRootElement(0),"Controls",0,20,Vector2(0.1f,0.65f),Vector2(1.f,1.f/20.f),"Z - Win");

		GUIRectangle* rect = new GUIRectangle(scrn->getRootElement(1),"cursor","Mouse");
		rect->setScale(Vector2(16.f/1024.f,23.f/768.f));
		scrn->setCursor(rect);

		rect = new GUIRectangle(scrn->getRootElement(0),"Coloredrect01",Colour(0.2,0.2,0.5,0.8),
			Colour(0.1,0.1,0.1,0.8),4.0,0,Vector2(0.1f,0.1f),Vector2(327.f/1024.f,30.f/768.f));
		rect->setPosition(Vector2(0.1405f,0.832f));

		rect = new GUIRectangle(scrn->getRootElement(0),"Coloredrect01",Colour(0.2,0.2,0.5,0.8),
			Colour(0.1,0.1,0.1,0.8),4.0,0,Vector2(0.1f,0.1f),Vector2(327.f/1024.f,30.f/768.f));
		rect->setPosition(Vector2(0.5405f,0.832f));

		txt = new StaticText(scrn->getRootElement(0),"PlayText",0,20,Vector2(0.4f,0.2f),Vector2(327.f/1024.f,30.f/768.f),"Play");
		txt->setColour(Colour(0.7f,0.7f,0.7f));
		txt->setAlign(GA_CENTER);
		txt->setPosition(Vector2(0.1405f,0.832f));
		txt->setHoverColour(Colour(0.9f,0.4f,0.2f));
		txt->getSignal("clicked")->addListener(createSlot("clickedPlay",this,&MenuState::clickedPlay));

		txt = new StaticText(scrn->getRootElement(0),"QuitText",0,20,Vector2(0.4f,0.2f),Vector2(327.f/1024.f,30.f/768.f),"Quit");
		txt->setColour(Colour(0.7f,0.7f,0.7f));
		txt->setAlign(GA_CENTER);
		txt->setPosition(Vector2(0.5405f,0.832f));
		txt->setHoverColour(Colour(0.9f,0.4f,0.2f));
		txt->getSignal("clicked")->addListener(createSlot("clickedQuit",this,&MenuState::clickedQuit));

		//rect = new GUIRectangle(scrn->getRootElement(0),"Coloredrect01",Colour(1,0,0,0.5),
		//Colour(0.3,0.6,0.9,0.8),4.0,0,Vector2(0.1f,0.1f),Vector2(0.1,0.1));
		//rect->setSprite("test");

		EventHandler::getDestination("OISSubsystem")->getSignal("mouseMovedAbs")
			->addListener(scrn->getSlot("moveCursor"));
		EventHandler::getDestination("OISSubsystem")->getSignal("pressed_MB_Left")
			->addListener(scrn->getSlot("click"));
		EventHandler::getDestination("OISSubsystem")->getSignal("released_MB_Left")
			->addListener(scrn->getSlot("release"));
		Console* mConsole = new Console();
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

	void MenuState::clickedPlay(const Message& m)
	{
		mEngine->addState(new PlayState());
		sendMessage(MessageAny<String>("end state"),"Engine");
	}
	//-----------------------------------------------------------------------
	
	void MenuState::clickedQuit(const Message& m)
	{
		sendMessage(MessageAny<String>("kill"),"Engine");
	}
	//-----------------------------------------------------------------------	
}
