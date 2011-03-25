#ifndef CONSOLE_H
#define CONSOLE_H

#include "RGJ.h"
#include "OryxObject.h"

namespace RGJ
{
	/** A simple GUI chaiscript console */
	class Console : public Oryx::Object
	{
	public:

		Console();
		virtual ~Console();

		void update(Real delta);
		void charPressed(const Message& message);
		void catchLog(const Message& message);

	private:

		void addOutput(String out);

		bool mActive;
		size_t mOutputSize;
	
		std::deque<String> mHistory;

		// plaintext command (the actual caption might 
		// contain markup data, so this saves some trouble)
		String mCommand;

		StaticText* mInputText;
		std::vector<StaticText*> mOutputText;
		GUIEmpty* mParent;
		GUIScreen* mScreen;

		OgreSubsystem* mGfx;
		OISSubsystem* mInput;
		
	};
}

#endif
