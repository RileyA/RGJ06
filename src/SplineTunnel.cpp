#include "SSplineTunnel.h"

namespace RGJ
{
	SplineTunnel::SplineTunnel()
		:Object(),mSpline(),mPtsGenerated(0),mCurrentChunk(0),mRand(0)
	{
		mLastPoint = Vector3(0,0,0);
		mSpline.addPoint(mLastPoint);
		for(int i = 0; i < PTS_TO_GENERATE_AHEAD_OF_TIME; ++i)
		{
			Vector3 newPoint = Vector3(mRand.gen(-1,1),mRand.gen(-1,1),1);
			newPoint.normalize();
			newPoint*POINT_SPACING;
			mLastPoint += newPoint;
			mSpline.addPoint(mLastPoint);
		}
	}

	SplineTunnel::~SplineTunnel()
	{

	}

	void SplineTunnel::update(Real delta)
	{
		
	}

	void SplineTunnel::generate(Vector3 pos)
	{

	}

	SplineTunnel::Chunk::Chunk()
	{

	}

	SplineTunnel::Chunk::~Chunk()
	{

	}

	void SplineTunnel::Chunk::gen(Real dist, Spline& spline)
	{
		//MeshData d;

		// build mesh data...
		
		// build the actual mesh:
		//if(!mMesh)
		//	mMesh = Engine::getPtr()->getSubsystem("OgreSubsystem")->castType<OgreSubsystem>()->creareMesh(d);
		//else
		//	mMesh->update(d);
	}
}
