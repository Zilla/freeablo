#include "pathfinding.h"
#include <algorithm>
#include <cmath>

// Debug
#include <iostream>

namespace FAWorld
{
    std::pair<size_t, size_t> Path::nextStep()
    {
        std::cout << "nextStep(), this is " << std::hex << this << std::endl;
        std::cout << "mSteps.size() " << mSteps.size() << " front(): " << mSteps.front().first << "," << mSteps.front().second << std::endl;
        std::pair<size_t, size_t> step = mSteps.front();
        mSteps.pop_front();
        std::cout << "mSteps.size() " << mSteps.size() << " front(): " << mSteps.front().first << "," << mSteps.front().second << std::endl;
        std::cout << "nextStep(), this is " << std::hex << this << std::endl;
        return step;
    }
}

namespace FAWorld
{

    Path PathFinder::findPath(Level::Level *level, std::pair<size_t, size_t>& origin, std::pair<size_t, size_t>& destination)
    {
        Path path;

        mLevelHeight = level->height();

        mGCost = new uint32_t[level->width() * level->height()];
        mFCost = new uint32_t[level->width() * level->height()];

        mCameFromX = new uint32_t[level->width() * level->height()];
        mCameFromY = new uint32_t[level->width() * level->height()];

        memset(mGCost, 0xFF, level->width() * level->height() * sizeof(uint32_t));
        memset(mFCost, 0xFF, level->width() * level->height() * sizeof(uint32_t));

        mGCost[origin.first + (level->height() * origin.second)] = 0;
        mFCost[origin.first + (level->height() * origin.second)] = distance(origin, destination);

        mOpenSet.push_front(origin);

        while (!mOpenSet.empty())
        {
            std::pair<size_t, size_t> current;
            std::list< std::pair<size_t, size_t> > neighbors;

            mOpenSet.sort(fScoreCompare(*this));
            current = mOpenSet.front();

            if (current == destination)
            {
                constructPath(origin, destination, path);
                break;
            }

            mOpenSet.pop_front();
            mClosedSet.push_back(current);

            getNeighbors(current, neighbors);
            for (std::list<std::pair<size_t, size_t>>::iterator it = neighbors.begin(); it != neighbors.end(); it++)
            {
                std::list<std::pair<size_t, size_t>>::iterator fi = find(mClosedSet.begin(), mClosedSet.end(), *it);
                if (fi != mClosedSet.end() || tileBlocked(level, *it))
                    continue;

                uint32_t newGCost = mGCost[current.first + (level->height() * current.second)] + 1;
                fi = find(mOpenSet.begin(), mOpenSet.end(), *it);
                if (fi == mOpenSet.end() || newGCost < mGCost[(*it).first + (level->height() * (*it).second)])
                {
                    mCameFromX[(*it).first + (level->height() * (*it).second)] = current.first;
                    mCameFromY[(*it).first + (level->height() * (*it).second)] = current.second;

                    mGCost[(*it).first + (level->height() * (*it).second)] = newGCost;
                    mFCost[(*it).first + (level->height() * (*it).second)] =
                        mGCost[(*it).first + (level->height() * (*it).second)] +
                        distance(*it, destination);

                    if (fi == mOpenSet.end())
                        mOpenSet.push_back(*it);
                }
            }
        }

        cleanup();

        return path;
    }

    int32_t PathFinder::distance(std::pair<size_t, size_t>& origin, std::pair<size_t, size_t>& destination)
    {
        int32_t xlen = (origin.first > destination.first) ? (origin.first - destination.first) : (destination.first - origin.first);
        int32_t ylen = (origin.second > destination.second) ? (origin.second - destination.second) : (destination.second - origin.second);

        return int32_t(std::ceil(sqrt(xlen*xlen + ylen*ylen)));
    }

    bool PathFinder::tileBlocked(Level::Level *level, std::pair<size_t, size_t>& pos)
    {
        return (!(*level)[pos.first][pos.second].passable());
    }

    void PathFinder::constructPath(std::pair<size_t, size_t>& origin, std::pair<size_t, size_t>& destination, Path& path)
    {
        std::pair<size_t, size_t> curr = destination;

        while (curr != origin)
        {
            path.addStepRev(curr);

            uint32_t newX = mCameFromX[curr.first + (mLevelHeight * curr.second)];
            uint32_t newY = mCameFromY[curr.first + (mLevelHeight * curr.second)];

            curr.first = newX;
            curr.second = newY;
        }
    }

    void PathFinder::getNeighbors(std::pair<size_t, size_t>& current, std::list< std::pair<size_t, size_t> >& neighbors)
    {
        neighbors.clear();
        std::pair<size_t, size_t> n = current;

        n.first++;
        n.second++;
        neighbors.push_back(n);
        n.first--;
        neighbors.push_back(n);
        n.first--;
        neighbors.push_back(n);
        n.second--;
        neighbors.push_back(n);
        n.first += 2;
        neighbors.push_back(n);
        n.second--;
        neighbors.push_back(n);
        n.first--;
        neighbors.push_back(n);
        n.first--;
        neighbors.push_back(n);
    }

    void PathFinder::cleanup()
    {
        if (mGCost != NULL)
        {
            delete[] mGCost;
            mGCost = NULL;
        }
        if (mFCost != NULL)
        {
            delete[] mFCost;
            mFCost = NULL;
        }
        if (mCameFromX != NULL)
        {
            delete[] mCameFromX;
            mCameFromX = NULL;
        }
        if (mCameFromY != NULL)
        {
            delete[] mCameFromY;
            mCameFromY = NULL;
        }

        mClosedSet.clear();
        mOpenSet.clear();
    }
}