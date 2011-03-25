#include "Console.h"

namespace RGJ
{
	Console::Console()
		:Object(),mOutputSize(10),mCommand(""),mInputText(0),mActive(0)
	{
		mGfx = Engine::getPtr()->getSubsystem("OgreSubsystem")->castType<OgreSubsystem>();
		mInput = Engine::getPtr()->getSubsystem("OISSubsystem")->castType<OISSubsystem>();
		GUI* gui = mGfx->getGUI();
		mScreen = mGfx->getGUI()->createScreen(mGfx->getMainViewport(),"Console","Console");
		mScreen->getRootElement(0)->setAspectRatio(4,3);
		mParent = new GUIEmpty(mScreen->getRootElement(0),"ConsoleRoot");

		mInputText = new StaticText(mParent,
			"Console",0,20,Vector2(0.01f,0.95f),Vector2(1.0,1.f/20.f),">");
		mInputText->setColour(Colour(0.3f,0.9f,0.5f));

		for(int i = 0;i<mOutputSize;++i)
		{
			mOutputText.push_back(new StaticText(mParent,"Output"+StringUtils::toString(i),
				0,20,Vector2(0.01f,0.91f-0.028f*i),Vector2(1.0,1.f/20.f),"~"));
			mOutputText.back()->setTextScale(0.65f);
			mOutputText.back()->setColour(Colour(0.5,0.8,0.55,0.8f));
		}

		GUIRectangle* rect = new GUIRectangle(mParent,"TerminalBG",Colour(0.2f,0.15f,0.1f,0.8f));
		rect->setPosition(Vector2(0.f,0.945f));
		rect->setScale(Vector2(1.f,0.2f));

		rect = new GUIRectangle(mParent,"TerminalBG2",Colour(0.1f,0.2f,0.1f,0.7f));
		rect->setPosition(Vector2(0.f,0.645f));
		rect->setScale(Vector2(1.f,0.3f));

		mParent->setPosition(Vector2(2.f,2.f));

		// track keyboard input
		EventHandler::getDestination("OISSubsystem")->getSignal("charPressed")
			->addListener(createSlot("recieveChar",this,&Console::charPressed));
		// track the logger
		Logger::getPtr()->getSignal("Log")->addListener(createSlot("catchLog",this,&Console::catchLog));
	}

	Console::~Console()
	{
	}

	void Console::update(Real delta)
	{
		if(mActive)
		{
			if(mInput->wasKeyPressed("KC_BACK"))
			{
				if(mCommand.size()>0)
				{
					mCommand.resize(mCommand.size()-1);
					mInputText->setCaption("> "+mCommand+"_");
				}
			}
			if(mInput->wasKeyPressed("KC_RETURN"))
			{
				addOutput("> "+mCommand);
				Engine::getPtr()->getSubsystem("ChaiscriptSubsystem")->
					castType<ChaiscriptSubsystem>()->runString(mCommand);
				mCommand.clear();
				mInputText->setCaption("> "+mCommand+"_");
			}
		}
		if(mInput->wasKeyPressed("KC_GRAVE"))
		{
			mActive = !mActive;
			if(mActive)
				mParent->setPosition(Vector2(0.f,0.f));
			else
				mParent->setPosition(Vector2(2.f,2.f));
		}
	}

	void Console::charPressed(const Message& message)
	{
		if(!mActive) return;
		if(const unsigned int* c = unpackMsg<unsigned int>(message))
		{
			String out = "";
			out.append(1,static_cast<char>(*c));
			// exclude non-characters
			if(!(*c<127&&*c>31)||*c=='`')
				return;
			mCommand += out;
			mInputText->setCaption("> "+mCommand+"_");
		}
	}

	void Console::addOutput(String out)
	{
		String next = out;
		for(int i=0;i<mOutputText.size();++i)
		{
			String temp = mOutputText[i]->getCaption();
			mOutputText[i]->setCaption(next);
			next = temp;
		}
	}

	void Console::catchLog(const Message& message)
	{
		if(const String* c = unpackMsg<String>(message))
			addOutput(*c);
	}
}
