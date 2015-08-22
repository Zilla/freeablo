#ifndef PATHFINDING_H
#define PATHFINDING_H

#include <level/level.h>
#include <list>

namespace FAWorld
{
    class Path
    {
    public:
        int32_t                   length() const                              { return mSteps.size(); }
        std::pair<size_t, size_t> destination() const                         { return mSteps.back(); }
        void                      addStep(std::pair<size_t, size_t>& step)    { mSteps.push_back(step); }
        void                      addStepRev(std::pair<size_t, size_t>& step) { mSteps.push_front(step); }
        std::pair<size_t, size_t> peekStep() const							  { return mSteps.front(); }

        std::pair<size_t, size_t> nextStep();

    private:
        std::list< std::pair<size_t, size_t> > mSteps;
    };

    class PathFinder
    {
    public:
        PathFinder() :
            mGCost(NULL),
            mFCost(NULL),
            mCameFromX(NULL),
            mCameFromY(NULL) {}

        ~PathFinder() { cleanup(); }
        Path findPath(Level::Level *level, std::pair<size_t, size_t>& origin, std::pair<size_t, size_t>& destination);

    private:
        int32_t distance(std::pair<size_t, size_t>& origin, std::pair<size_t, size_t>& destination);
        void cleanup();
        void constructPath(std::pair<size_t, size_t>& origin, std::pair<size_t, size_t>& destination, Path& path);
        void getNeighbors(std::pair<size_t, size_t>& current, std::list< std::pair<size_t, size_t> >& neighbors);
        bool tileBlocked(Level::Level *level, std::pair<size_t, size_t>& pos);

        int32_t travelCost(Path &path) { return path.length(); }

        struct fScoreCompare
        {
            fScoreCompare(const PathFinder& pf) : mPf(pf) {}
            const PathFinder& mPf;

            bool operator() (const std::pair<size_t, size_t>& first, const std::pair<size_t, size_t>& second)
            {
                return mPf.mFCost[first.first + (mPf.mLevelHeight * first.second)] < mPf.mFCost[second.first + (mPf.mLevelHeight * second.second)];
            }
        };

        std::list< std::pair<size_t, size_t> > mClosedSet;
        std::list< std::pair<size_t, size_t> > mOpenSet;
        uint32_t *mGCost;
        uint32_t *mFCost;

        size_t mLevelHeight;

        size_t *mCameFromX;
        size_t *mCameFromY;
    };
}

#endif