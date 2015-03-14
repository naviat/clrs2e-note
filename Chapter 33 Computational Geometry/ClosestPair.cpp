#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <limits>
#include <vector>

struct Point
{
    float x, y;
};

using namespace std;

typedef pair<Point, Point> Pair;
typedef vector<Point>::const_iterator PointConstIterator;
typedef vector<size_t>::const_iterator IndexConstIterator;

vector<Point> points;  // input points
vector<bool> leftFlags;  // auxiliary

void readPoints(char *inFileName)
{
    ifstream inFile(inFileName);
    if (!inFile)
    {
        cerr << "Cannot open '" << inFileName << "'." << endl;
        exit(1);
    }

    int numPoints = 0;
    inFile >> numPoints;

    points.reserve(numPoints);

    for (int ii = 0; ii < numPoints; ++ii)
    {
        Point p;
        inFile >> p.x >> p.y;
        points.push_back(p);
    }

    leftFlags.resize(numPoints);
}

ostream & operator<<(ostream &ostrm, const Point &p)
{
    return ostrm << "(" << p.x << ", " << p.y << ")";
}

float distanceSquare(const Point &p1, const Point &p2)
{
    return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
}

Pair findClosestPair_ExhaustiveSearch(const vector<size_t> &pointIndexes)
{
    if (pointIndexes.size() < 2)
    {
        cerr << "Error: findClosestPair_ExhaustiveSearch: less than two points" << endl;
        exit(1);
    }

    float minDistanceSquare = numeric_limits<float>::max();
    Pair closestPair;

    for (size_t idx1 = 0; idx1 + 1 < pointIndexes.size(); ++idx1)
    {
        for (size_t idx2 = idx1 + 1; idx2 < pointIndexes.size(); ++idx2)
        {
            const float dSqr = distanceSquare(points[pointIndexes[idx1]], points[pointIndexes[idx2]]);
            if (dSqr < minDistanceSquare)
            {
                minDistanceSquare = dSqr;
                closestPair.first = points[pointIndexes[idx1]];
                closestPair.second = points[pointIndexes[idx2]];
            }
        }
    }

    return closestPair;
}

bool findClosestPair_CrossVerticalLine(const vector<size_t> &sortedY, float verticalLine, /*in, out*/ float *pClosestDistSqr, /*out*/ Pair *pClosestPair)
{
    vector<size_t> sortedYNearVerticalLine;

    for (IndexConstIterator iter = sortedY.begin(); sortedY.end() != iter; ++iter)
    {
        const float delta = points[*iter].x - verticalLine;
        if (delta * delta <= *pClosestDistSqr)
        {
            sortedYNearVerticalLine.push_back(*iter);
        }
    }

    bool bFoundCloserPair = false;

    if (sortedYNearVerticalLine.size() > 1)
    {
        for (size_t ii = 0; ii < sortedYNearVerticalLine.size() - 1; ++ii)
        {
            for (size_t jj = ii + 1; ii - jj < 8 && jj < sortedYNearVerticalLine.size(); ++ jj)
            {
                const size_t idx1 = sortedYNearVerticalLine[ii], idx2 = sortedYNearVerticalLine[jj];
                const float distSqr = distanceSquare(points[idx1], points[idx2]);
                if (distSqr < *pClosestDistSqr)
                {
                    *pClosestDistSqr = distSqr;
                    pClosestPair->first = points[idx1];
                    pClosestPair->second = points[idx2];
                    bFoundCloserPair = true;
                }
            }
        }
    }

    return bFoundCloserPair;
}

Pair findClosestPair_FastRecursive(const vector<size_t> &sortedX, const vector<size_t> &sortedY)
{
    if (sortedX.size() != sortedY.size())
    {
        cerr << "Error: findClosestPair_FastRecursive: sortedX.size() != sortedY.size()" << endl;
        exit(1);
    }

    if (sortedX.size() <= 3)
    {
        return findClosestPair_ExhaustiveSearch(sortedX);
    }

    // Flag left points
    const size_t mid = sortedX.size() / 2;
    for (size_t ii = 0; ii < mid; ++ii)
    {
        leftFlags[sortedX[ii]] = true;
    }
    for (size_t ii = mid; ii < sortedX.size(); ++ii)
    {
        leftFlags[sortedX[ii]] = false;
    }

    vector<size_t> leftX(sortedX.begin(), sortedX.begin() + mid),
                   rightX(sortedX.begin() + mid, sortedX.end());

    // Split sortedY into two sorted array
    vector<size_t> leftY, rightY;
    leftY.reserve(leftX.size());
    rightY.reserve(rightX.size());
    for (IndexConstIterator iter = sortedY.begin(); sortedY.end() != iter; ++iter)
    {
        if (leftFlags[*iter])
        {
            leftY.push_back(*iter);
        }
        else
        {
            rightY.push_back(*iter);
        }
    }

    Pair closestPair = findClosestPair_FastRecursive(leftX, leftY);
    float closestDistSqr = distanceSquare(closestPair.first, closestPair.second);

    const Pair closestPairRight = findClosestPair_FastRecursive(rightX, rightY);
    const float closestDistSqrRight = distanceSquare(closestPairRight.first, closestPairRight.second);

    if (closestDistSqrRight < closestDistSqr)
    {
        closestPair = closestPairRight;
        closestDistSqr = closestDistSqrRight;
    }

    findClosestPair_CrossVerticalLine(sortedY, points[sortedX[mid]].x, &closestDistSqr, &closestPair);

    return closestPair;
}

struct PointComparatorX
{
    bool operator()(size_t idx1, size_t idx2)
    {
        return points[idx1].x < points[idx2].x;
    }
};

struct PointComparatorY
{
    bool operator()(size_t idx1, size_t idx2)
    {
        return points[idx1].y < points[idx2].y;
    }
};

Pair findClosestPair_FastSearch()
{
    vector<size_t> sortedX(points.size()), sortedY(points.size());

    for (size_t idx = 0; idx < points.size(); ++idx)
    {
        sortedX[idx] = idx;
        sortedY[idx] = idx;
    }

    sort(sortedX.begin(), sortedX.end(), PointComparatorX());
    sort(sortedY.begin(), sortedY.end(), PointComparatorY());

    return findClosestPair_FastRecursive(sortedX, sortedY);
}

int main(int argc, char *argv[])
{
    if (2 != argc)
    {
        cerr << "Usage: ClosestPair in-file" << endl;
        exit(1);
    }

    readPoints(argv[1]);

    cout << "All points:" << endl;
    for (PointConstIterator iter = points.begin(); points.end() != iter; ++iter)
    {
        cout << *iter << endl;
    }
    cout << endl;

    vector<size_t> pointIndexes(points.size());
    for (size_t ii = 0; ii < points.size(); ++ii)
    {
        pointIndexes[ii] = ii;
    }

    Pair closestPair = findClosestPair_ExhaustiveSearch(pointIndexes);
    cout << "Closest pair by exhaustive search: " << closestPair.first << ", " << closestPair.second << endl;

    closestPair = findClosestPair_FastSearch();
    cout << "Closest pair by fast search: " << closestPair.first << ", " << closestPair.second << endl;

    return 0;
}

