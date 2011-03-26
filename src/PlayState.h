#ifndef RGJ_PLAY_STATE_H
#define RGJ_PLAY_STATE_H

#include "RGJ.h"
#include "OryxGameState.h"
#include "FPSCamera.h"
#include "ExplosionManager.h"
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

	private:

		OgreSubsystem* mGfx;
		OISSubsystem* mInput;
		ALSubsystem* mAudio;
		ChaiscriptSubsystem* mScript;
		BulletSubsystem* mBullet;

		FPSCamera* mCamera;
		ExplosionManager* mExplosions;

		BasicCharacterController* mPlayer;
		SplineTunnel* mTunnel;
	};
}

#endif
