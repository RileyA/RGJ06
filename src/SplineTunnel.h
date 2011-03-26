#ifndef SPLINES_ARE_FUN
#define SPLINES_ARE_FUN

#include "RGJ.h"
#include "OryxObject.h"
#include "OgreSubsystem/Spline.h"
#include "OgreSubsystem/Mesh.h"

namespace RGJ
{
	class SplineTunnel : public Object
	{
	public:

		SplineTunnel();
		virtual ~SplineTunnel();

		virtual void update(Real delta);

		void generate();

		Vector3 getPlayerPosition();
		Vector3 getPlayerDirection();

		Spline* getSpline(){return mSpline;};

		void collide(Vector3 player)
		{
			for(int i=0;i<mChunks.size();++i)
				mChunks[i]->collide(player,getSignal("hitLaser"));
		}

	private:

		Spline* mSpline;

		int mPtsGenerated;
		int mCurrentChunk;

		Vector3 mLastPoint;
		Rand mRand;

		Real mPlayerPos;
		Real mPlayerSpeed;

		static const int NUM_CHUNKS = 10;

		static const Real RING_OFFSET = 60.f;
		static const Real POINT_SPACING = 60.f;// 60m between catmull-rom spline points
		static const Real RING_SPACING = 2.f;// 2m between each ring
		static const int RINGS_PER_CHUNK = 30;

		static const int PTS_TO_GENERATE_AHEAD_OF_TIME = 25000;
		static const Real RING_RADIUS = 5.f; // 5m radius for each ring
		static const int VERTS_PER_RING = 32;
		// a mesh representing a length of the tunnel
		class Chunk
		{
		public:

			Chunk();
			~Chunk();

			// builds the mesh from a given distance on
			void gen(Real dist, SplineTunnel& spline, unsigned int ptgen,Rand* mRand);

			void collide(Vector3 pos,Signal* s)
			{
				for(int i=0;i<mLasers.size();++i)
				{
					if(mLasers[i].collide(pos))
						s->send(mLasers[i].isblue);
						//std::cout<<"OWWW!\n";
				}
			}

		private:

			Mesh* mMesh;
			static Vector3 last;


			// must... gooo... deeepeeerrr...
			class Laser
			{
			public:

				Laser(Mesh* m)
				{
					active = false;
					mMesh = m;
					isblue = false;
				}

				void reactivate(Vector3 pos,Real roll, bool blue)
				{
					isblue = blue;
					mMesh->setVisible(true);
					mMesh->setPosition(pos);
					mMesh->roll(roll);
					mMesh->setMaterialName(blue ? "Laser" : "Laser_Orange");
					mMesh->setScale(Vector3(RING_RADIUS*2,RING_RADIUS*2,RING_RADIUS*2));
					active = true;
				}

				bool collide(Vector3 player);
				
				bool active;
				Mesh* mMesh;
				bool isblue;

			};
			std::vector<Laser> mLasers;
		};

		// there should ideally only be a couple of these
		// that get resued (fog and no backtracking should ensure no pop-in)
		std::deque<Chunk*> mChunks;

	};
}

#endif
