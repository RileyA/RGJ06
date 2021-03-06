#ifndef RGJ_MENU_STATE_H
#define RGJ_MENU_STATE_H

#include "RGJ.h"
#include "OryxGameState.h"

namespace RGJ
{
	class MenuState : public GameState
	{
	public:
		
		MenuState();

		virtual void init();
		virtual void update(Real delta);
		virtual void deinit();

		void clickedPlay(const Message& m);
		void clickedQuit(const Message& m);

	private:

		OgreSubsystem* mGfx;
		OISSubsystem* mInput;
		ALSubsystem* mAudio;

	};
}

#endif
