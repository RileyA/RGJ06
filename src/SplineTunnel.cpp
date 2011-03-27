#include "SplineTunnel.h"

namespace RGJ
{
	SplineTunnel::SplineTunnel()
		:Object(),mPtsGenerated(0),mCurrentChunk(0),mRand(0)
	{
		gameover = false;
		mSpline = new Spline();
		mSpline->setAutoCalc(false);
		mLastPoint = Vector3(0,0,0);
		mPlayerPos = 0.f;
		mPlayerSpeed = 10.f;
		mSpline->addPoint(mLastPoint);
		mCurrentChunk = 0;
		++mPtsGenerated;
		for(int i = 0; i < PTS_TO_GENERATE_AHEAD_OF_TIME; ++i)
		{
			Vector3 newPoint = Vector3(mRand.gen(-1,1),mRand.gen(-1,1),-5.5f);
			newPoint.normalize();
			newPoint*=POINT_SPACING;
			mLastPoint += newPoint;
			mSpline->addPoint(mLastPoint);
			++mPtsGenerated;
		}
		mSpline->recalc();
		createSignal("hitLaser");
	}

	SplineTunnel::~SplineTunnel()
	{
		delete mSpline;
		for(int i = 0;i<mChunks.size();++i)
			delete mChunks[i];
	}

	void SplineTunnel::update(Real delta)
	{
		if(gameover)return;
		mPlayerPos += mPlayerSpeed * delta;
		mPlayerSpeed += delta * (50.f/300.f);
		generate();// try and generate a bit more if need be...
	}

	void SplineTunnel::generate()
	{
		Real length = POINT_SPACING * mPtsGenerated;
		while(mCurrentChunk < (mPlayerPos/RING_OFFSET)+7)
		{
			if(mChunks.size()<NUM_CHUNKS)
			{
				mChunks.push_back(new Chunk());
				mChunks.back()->gen((mCurrentChunk)*RINGS_PER_CHUNK*RING_SPACING, *this, mPtsGenerated,&mRand);
			}
			else
			{
				mChunks.push_back(mChunks.front());
				mChunks.pop_front();
				mChunks.back()->gen((mCurrentChunk)*RINGS_PER_CHUNK*RING_SPACING, *this, mPtsGenerated,&mRand);
			}
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

	void SplineTunnel::reset()
	{
		gameover = false;
		mPlayerPos = 0.f;
		mPlayerSpeed = 10.f;
		mCurrentChunk = 0;
		delete mSpline;
		mSpline = new Spline();
		mSpline->setAutoCalc(false);
		mLastPoint = Vector3(0,0,0);
		mPlayerPos = 0.f;
		mPlayerSpeed = 10.f;
		mSpline->addPoint(mLastPoint);
		mCurrentChunk = 0;
		mPtsGenerated = 0;
		++mPtsGenerated;
		for(int i = 0; i < PTS_TO_GENERATE_AHEAD_OF_TIME; ++i)
		{
			Vector3 newPoint = Vector3(mRand.gen(-1,1),mRand.gen(-1,1),-5.5f);
			newPoint.normalize();
			newPoint*=POINT_SPACING;
			mLastPoint += newPoint;
			mSpline->addPoint(mLastPoint);
			++mPtsGenerated;
		}
		mSpline->recalc();
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

	void SplineTunnel::Chunk::gen(Real dist, SplineTunnel& spline, unsigned int ptgen,Rand* mRand)
	{
		for(int i=0;i<mLasers.size();++i)
		{
			mLasers[i].active = false;
			mLasers[i].mMesh->setVisible(false);
		}
		unsigned int lase = 0;
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


			if(mRand->gen(0,2)==0&&dist > 20.f)
			{
				if(mLasers.size()<=lase)
				{
					Mesh* m = Engine::getPtr()->getSubsystem("OgreSubsystem")->
						castType<OgreSubsystem>()->createMesh("Laser.mesh");
					Engine::getPtr()->getSubsystem("OgreSubsystem")
						->castType<OgreSubsystem>()->getRootSceneNode()->addChild(m);
					mLasers.push_back(Laser(m));
					mLasers.back().reactivate(rootPos+myPos+Vector3(mRand->gen(-3,3),mRand->gen(-3,3),0),
						mRand->gen(0,VERTS_PER_RING-1)*(360/VERTS_PER_RING)
						,mRand->gen(0,1)==0);
					++lase;
				}
				else
				{
					mLasers[lase].reactivate(rootPos+myPos+Vector3(mRand->gen(-3,3),mRand->gen(-3,3),0),
						mRand->gen(0,VERTS_PER_RING-1)*(360/VERTS_PER_RING),mRand->gen(0,1)==0);
					++lase;
				}
				/*Mesh* m = Engine::getPtr()->getSubsystem("OgreSubsystem")->castType<OgreSubsystem>()->createMesh("Laser.mesh");
				Engine::getPtr()->getSubsystem("OgreSubsystem")->castType<OgreSubsystem>()->getRootSceneNode()->addChild(m);
				m->roll(rand()%360);
				m->setPosition(rootPos + myPos + Vector3(mRand->gen(-3,3),mRand->gen(-3,3),0));
				m->setScale(Vector3(RING_RADIUS*2,RING_RADIUS*2,RING_RADIUS*2));

				if(mRand->gen(0,1)==0)
					m->setMaterialName("Laser_Orange");*/
			}

			myRingDir = nextRingDir;
			Vector3 temp = myPos;
			myPos = nextPos;
			priorPos = temp;
		}

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

	Vector3 SplineTunnel::getPlayerDirection()
	{
		/*Real length = POINT_SPACING * mPtsGenerated;

		Real factor = 10.f;

		Real lastLastRing = floor((mPlayerPos-RING_SPACING*factor)/(RING_SPACING*factor));
		Real lastRing = floor(mPlayerPos/(RING_SPACING*factor));
		Real nextRing = ceil(mPlayerPos/(RING_SPACING*factor));

		Real interpolation = (nextRing - lastRing)/(RING_SPACING*factor);

		Vector3 lastLastPos = lastLastRing < 0 ? Vector3(0,0,RING_SPACING*factor) : mSpline->interpolate(lastLastRing/length);
		Vector3 lastPos = mSpline->interpolate(lastRing/length);
		Vector3 nextPos = mSpline->interpolate(nextRing/length);

		Vector3 lastDir = lastPos - lastLastPos;
		Vector3 nextDir = nextPos = lastPos;*/
		Real length = POINT_SPACING * mPtsGenerated;
		Vector3 nextDir = mSpline->interpolate((mPlayerPos+10.f)/length);
		Vector3 out = nextDir - getPlayerPosition();
		out.normalize();
		return out;
	}

	bool SplineTunnel::Chunk::Laser::collide(Vector3 player)
	{
		if(!active||(player-mMesh->getPosition()).squaredLength()>49)
			return false;

		Sphere sph(0.8f,player);
		Ray r;
		r.origin = mMesh->getPosition() + mMesh->getOrientation() * Vector3::UNIT_Y * 15;
		r.direction = mMesh->getOrientation() * Vector3::NEGATIVE_UNIT_Y;
		if(sph.intersects(r))
		{
			active = false;
			mMesh->setVisible(false);
			return true;
		}
		return false;
	}
}
