#ifndef RGJ_PLAY_STATE_H
#define RGJ_PLAY_STATE_H

#include "RGJ.h"
#include "OryxGameState.h"
#include "FPSCamera.h"
#include "SplineTunnel.h"

namespace RGJ
{
	class PlayState : public GameState
	{
	public:
		
		PlayState();

		virtual void init();
		virtual void update(Real delta);
		virtual void deinit();

		virtual void mouseMove(const Message& message);
		virtual void hitLaser(const Message& message);

	private:

		OgreSubsystem* mGfx;
		OISSubsystem* mInput;
		ALSubsystem* mAudio;

		FPSCamera* mCamera;

		SplineTunnel* mTunnel;

		SoundPtr music;
		Vector3 mPlayerPos;
		GUIRectangle* crosshair;
		GUIRectangle* frame;
		GUIRectangle* blueb;
		GUIRectangle* orangeb;
		
		GUIRectangle* gameover;
		StaticText* score;
		StaticText* best;

		StaticText* distance;
		StaticText* time;
		GUIRectangle* g;// I don't even know why this is called 'g'...
		Real cooloff;
		bool blue;
		bool done;
		int balance;
		int limit;
		Real mTime;
		bool mGameOver;

		Real mBest;
	};
}

#endif
