#ifndef SPLINES_ARE_FUN
#define SPLINES_ARE_FUN

#include "RGJ.h"
#include "OryxObject.h"
#include "OgreSubsystem/Spline.h"

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

	private:

		Spline* mSpline;

		int mPtsGenerated;
		int mCurrentChunk;

		Vector3 mLastPoint;
		Rand mRand;

		Real mPlayerPos;
		Real mPlayerSpeed;

		static const Real POINT_SPACING = 30.f;// 30m between catmull-rom spline points
		static const Real RING_SPACING = 2.f;// 2m between each ring
		static const Real RING_RADIUS = 5.f; // 5m radius for each ring
		static const int PTS_TO_GENERATE_AHEAD_OF_TIME = 50;
		static const int RINGS_PER_CHUNK = 30;

		// a mesh representing a length of the tunnel
		class Chunk
		{
		public:

			Chunk();
			~Chunk();

			// builds the mesh from a given distance on
			void gen(Real dist, Spline& spline);

		private:

			Mesh* mMesh;

		};

		// there should ideally only be a couple of these
		// that get resued (fog and no backtracking should ensure no pop-in)
		std::vector<Chunk*> mChunks;

	};
}

#endif
