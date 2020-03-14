#include "DrawableDistanceHotMap.h"
#include "Walker.h"

using namespace ThorsAnvil::ValkyrieWalker;

DrawableDistanceHotMap::DrawableDistanceHotMap(std::vector<Walker> const& walkersR)
    : walkers(walkersR)
    , minScore(0)
    , maxScore(0)
{
    (void)walkers;
}

void DrawableDistanceHotMap::draw(wxDC& dc) const
{
    wxSize size = getSize();
    dc.SetPen(wxNullPen);
    dc.DrawRectangle(wxPoint{0,0}, size);

    dc.SetPen(*wxBLACK_PEN);
    int     maxCount    = walkers.size() / 2;
    float   distYRel    = (size.y - 20) * 1.0 / maxCount;
    float   distXRel    = (size.y - 20) * 1.0 / count.size();
    int     width       = static_cast<int>(distXRel);

    for (std::size_t loop = 0; loop < count.size(); ++loop)
    {
        int xCord   = 10 + distXRel * loop;
        int height  = count[loop] * distYRel * -1;
        dc.DrawRectangle(wxPoint{xCord, size.y -10}, wxSize{width, height});
    }
}

void DrawableDistanceHotMap::tick(bool update)
{
    int     maxBucketCount  = 75;

    maxScore        = std::max(maxScore, walkers[0].score() + 1);
    minScore        = std::min(minScore, walkers[99].score() - 1);
    float   bucketRange     = (maxScore - minScore) * 1.0 / maxBucketCount;
    int     posBucketCount  = (maxScore / bucketRange ) + 1;
    int     negBucketCount  = (-minScore / bucketRange) + 1;
    int     totalCount      = posBucketCount + negBucketCount;

    count.clear();
    count.resize(totalCount, 0);

    for (auto const& walker: walkers)
    {
        int bucketIndex;
        int score = walker.score();

        if (score < 0)
        {
            bucketIndex = negBucketCount - 1 - (-score / bucketRange);
        }
        else
        {
            bucketIndex = negBucketCount + score / bucketRange;
        }
        ++count[bucketIndex];
    }

    if (update)
    {
        refresh();
    }
}

void DrawableDistanceHotMap::load(std::istream& stream)
{
    int                 tmpMinScore;
    int                 tmpMaxScore;
    std::vector<int>    tmpCount;
    int                 sizeCount;

    if (stream >> tmpMinScore >> tmpMaxScore >> sizeCount)
    {
        for (int loop = 0; loop < sizeCount; ++loop)
        {
            int val;
            if (stream >> val)
            {
                tmpCount.emplace_back(val);
            }
        }
    }
    if (stream)
    {
        minScore    = tmpMinScore;
        maxScore    = tmpMaxScore;
        count       = std::move(tmpCount);
    }
}

void DrawableDistanceHotMap::save(std::ostream& stream) const
{
    stream << minScore << " " << maxScore << "\n" << count.size();
    for (auto const& val: count)
    {
        stream << " " << val;
    }
    stream << "\n";
}
