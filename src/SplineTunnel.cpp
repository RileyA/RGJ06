#include "SplineTunnel.h"

namespace RGJ
{
	SplineTunnel::SplineTunnel()
		:Object(),mPtsGenerated(0),mCurrentChunk(0),mRand(0)
	{
		mSpline = new Spline();
		mLastPoint = Vector3(0,0,0);
		mPlayerPos = 0.f;
		mPlayerSpeed = 20.f;
		mSpline->addPoint(mLastPoint);
		++mPtsGenerated;
		for(int i = 0; i < PTS_TO_GENERATE_AHEAD_OF_TIME; ++i)
		{
			Vector3 newPoint = Vector3(mRand.gen(-1,1),mRand.gen(-1,1),-2.5f);
			newPoint.normalize();
			newPoint*=POINT_SPACING;
			Vector3 tmp = mLastPoint;
			mLastPoint += newPoint;
			Vector3 dir = newPoint;//mLastPoint - tmp;
			dir.normalize();
			mSpline->addPoint(mLastPoint);
			Mesh* me = Engine::getPtr()->getSubsystem("OgreSubsystem")->castType<OgreSubsystem>()->createMesh("Ring.mesh");
			me->setPosition(mLastPoint);
			me->setOrientation(dir.getRotationTo(tmp));
			Engine::getPtr()->getSubsystem("OgreSubsystem")->castType<OgreSubsystem>()->getRootSceneNode()->addChild(me);
			++mPtsGenerated;
		}
	}

	SplineTunnel::~SplineTunnel()
	{
		delete mSpline;
	}

	void SplineTunnel::update(Real delta)
	{
		mPlayerPos += mPlayerSpeed * delta;
		generate();// try and generate a bit more if need be...
	}

	void SplineTunnel::generate()
	{
		while(floor(mPlayerPos/POINT_SPACING)+PTS_TO_GENERATE_AHEAD_OF_TIME > mPtsGenerated)
		{
			Vector3 newPoint = Vector3(mRand.gen(-1,1),mRand.gen(-1,1),-5.5f);
			newPoint.normalize();
			newPoint*=POINT_SPACING;
			mLastPoint += newPoint;
			mSpline->addPoint(mLastPoint);
			Mesh* me = Engine::getPtr()->getSubsystem("OgreSubsystem")->castType<OgreSubsystem>()->createMesh("Ring.mesh");
			me->setPosition(mLastPoint);
			Engine::getPtr()->getSubsystem("OgreSubsystem")->castType<OgreSubsystem>()->getRootSceneNode()->addChild(me);
			++mPtsGenerated;
		}
	}

	SplineTunnel::Chunk::Chunk()
	{

	}

	SplineTunnel::Chunk::~Chunk()
	{

	}

	Vector3 SplineTunnel::getPlayerPosition()
	{
		Real length = POINT_SPACING * mPtsGenerated;
		return mSpline->interpolate(std::min(0.99f,mPlayerPos / length));//std::min(0.99f,mPlayerPos));
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
