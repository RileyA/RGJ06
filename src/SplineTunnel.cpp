#include "SplineTunnel.h"

namespace RGJ
{
	SplineTunnel::SplineTunnel()
		:Object(),mPtsGenerated(0),mCurrentChunk(0),mRand(0)
	{
		mSpline = new Spline();
		mSpline->setAutoCalc(false);
		mLastPoint = Vector3(0,0,0);
		mPlayerPos = 0.f;
		mPlayerSpeed = 300.f;
		mSpline->addPoint(mLastPoint);
		mCurrentChunk = 0;
		++mPtsGenerated;
		for(int i = 0; i < PTS_TO_GENERATE_AHEAD_OF_TIME; ++i)
		{
			Vector3 newPoint = Vector3(mRand.gen(-1,1),mRand.gen(-1,1),-2.5f);
			newPoint.normalize();
			newPoint*=POINT_SPACING;
			mLastPoint += newPoint;
			mSpline->addPoint(mLastPoint);
			++mPtsGenerated;
		}
		mSpline->recalc();
	}

	SplineTunnel::~SplineTunnel()
	{
		delete mSpline;
	}

	void SplineTunnel::update(Real delta)
	{
		mPlayerPos += mPlayerSpeed * delta;
		//std::cout<<mPlayerPos<<"\n";
		generate();// try and generate a bit more if need be...
	}

	void SplineTunnel::generate()
	{
		/*while(floor(mPlayerPos/POINT_SPACING)+PTS_TO_GENERATE_AHEAD_OF_TIME > mPtsGenerated)
		{
			Vector3 newPoint = Vector3(mRand.gen(-1,1),mRand.gen(-1,1),-5.5f);
			newPoint.normalize();
			newPoint*=POINT_SPACING;
			mLastPoint += newPoint;
			mSpline->addPoint(mLastPoint);
			//Mesh* me = Engine::getPtr()->getSubsystem("OgreSubsystem")->castType<OgreSubsystem>()->createMesh("Ring.mesh");
			//me->setPosition(mLastPoint);
			//Engine::getPtr()->getSubsystem("OgreSubsystem")->castType<OgreSubsystem>()->getRootSceneNode()->addChild(me);
			++mPtsGenerated;
		}

		mSpline->recalc();*/

		Real length = POINT_SPACING * mPtsGenerated;
		while(mCurrentChunk < (mPlayerPos/RING_OFFSET)+7)
		{
			//if(mChunks.size()>0)
			//	return;
			if(mChunks.size()<NUM_CHUNKS)
			{
				mChunks.push_back(new Chunk());
				mChunks.back()->gen((mCurrentChunk)*RINGS_PER_CHUNK*RING_SPACING, *this, mPtsGenerated);
			}
			else
			{
				std::cout<<"reused mesh...\n";
				mChunks.push_back(mChunks.front());
				mChunks.pop_front();
				mChunks.back()->gen((mCurrentChunk)*RINGS_PER_CHUNK*RING_SPACING, *this, mPtsGenerated);
			}
			std::cout<<"Created Chunk: "<<(mCurrentChunk)*RINGS_PER_CHUNK*RING_SPACING<<"\n";
			++mCurrentChunk;
		}
	}

	SplineTunnel::Chunk::Chunk()
		:mMesh(0)
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

	Quaternion getDir(Vector3 pt1, Vector3 pt2)
	{
		Vector3 pt3 = pt2 - pt1;
		pt3.normalize();
		return Vector3::NEGATIVE_UNIT_Z.getRotationTo(pt3);
	}

	Vector3 SplineTunnel::Chunk::last = Vector3(0,0,0);

	void SplineTunnel::Chunk::gen(Real dist, SplineTunnel& spline, unsigned int ptgen)
	{
		MeshData d = MeshData();
		d.addTexcoordSet();

		Real length = POINT_SPACING * ptgen;

		Vector3 priorPos = dist - RING_SPACING > 0 ? spline.getSpline()->interpolate((dist-RING_SPACING)/length) 
			: Vector3(0,0,2);

		//Vector3 priorVerts[VERTS_PER_RING];
		Vector3 myPos = spline.getSpline()->interpolate(dist/length);
		Vector3 rootPos = myPos;
		myPos = Vector3(0,0,0);
		priorPos -= rootPos;

		Quaternion myRingDir = getDir(priorPos,myPos);

		Vector3 vertices[VERTS_PER_RING];

		Real pi = atan(1)*4;

		for(int i = 0; i < VERTS_PER_RING; ++i)
		{
			Real ratio = static_cast<Real>(i)/VERTS_PER_RING;
			Real angle = pi*2*ratio;
			vertices[i] = Vector3(cos(angle),sin(angle),0) * RING_RADIUS;
		}

		for(int i = 0; i<RINGS_PER_CHUNK;++i)
		{
			Vector3 nextPos = spline.getSpline()->interpolate((dist+(i+1)*RING_SPACING)/length) - rootPos;

			Quaternion nextRingDir = getDir(myPos,nextPos);

			// add some verts!
			int lastIndex = VERTS_PER_RING-1;
			for(int j = 0; j < VERTS_PER_RING; ++j)
			{
				d.vertex(nextPos+vertices[j],Vector2(0,1));//
				d.vertex(myPos+vertices[lastIndex],Vector2(1,0));//
				d.vertex(myPos+vertices[j],Vector2(0,0));//

				d.vertex(nextPos+vertices[j],Vector2(0,1));//
				d.vertex(nextPos+vertices[lastIndex],Vector2(1,1));//
				d.vertex(myPos+vertices[lastIndex],Vector2(1,0));//
				lastIndex = j;
			}

			myRingDir = nextRingDir;
			Vector3 temp = myPos;
			myPos = nextPos;
			priorPos = temp;
		}

		//std::cout<<dist+(RINGS_PER_CHUNK)*RING_SPACING<<"\n";
		std::cout<<"start: "<<dist<<"  "<<" length "<<(RINGS_PER_CHUNK)*RING_SPACING<<"\n";

		// build mesh data...
		
		// build the actual mesh:
		if(!mMesh)
		{
			mMesh = Engine::getPtr()->getSubsystem("OgreSubsystem")->castType<OgreSubsystem>()->createMesh(d);
			Engine::getPtr()->getSubsystem("OgreSubsystem")->castType<OgreSubsystem>()->getRootSceneNode()->addChild(mMesh);
			mMesh->setMaterialName("DebugGrid");
		}
		else
		{
			mMesh->update(d);
		}
		mMesh->setPosition(rootPos);
	}
}
