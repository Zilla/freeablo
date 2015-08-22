#include "position.h"
#include <misc/misc.h>

namespace FAWorld
{
    Position::Position(): mDist(0), mDirection(0),
        mMoving(false), mCurrent(std::make_pair(0,0)) {}

    Position::Position(size_t x, size_t y): mDist(0), mDirection(0), 
        mMoving(false), mCurrent(std::make_pair(x,y)) {}

    Position::Position(size_t x, size_t y, size_t direction): mDist(0), 
        mDirection(direction), mMoving(false),
        mCurrent(std::make_pair(x,y)) {}

    void Position::update()
    {
        if(mMoving)
        {
            mDist += 2;

            if(mDist >= 100)
            {
                nextStep();
                mDist = 0;
            }
        }
    }

    std::pair<size_t, size_t> Position::current() const
    {
        return mCurrent;
    }

    std::pair<size_t, size_t> Position::next() const
    {
        if(!mMoving || mPath.length() == 0)
            return mCurrent;

        return mPath.peekStep();
    }

	void Position::nextStep()
	{
		if (mPath.length() != 0)
		{
			mCurrent = mPath.nextStep();
			mDirection = Misc::getVecDir(Misc::getVec(mCurrent, mPath.peekStep()));
		}
	}
}
