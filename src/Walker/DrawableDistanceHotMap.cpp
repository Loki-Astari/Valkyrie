#include "DrawableDistanceHotMap.h"
#include "Walker.h"

using namespace ThorsAnvil::ValkyrieWalker;

DrawableDistanceHotMap::DrawableDistanceHotMap(std::vector<Walker>& walkersR)
    : walkers(walkersR)
{
    (void)walkers;
}

void DrawableDistanceHotMap::draw(wxDC& dc) const
{
    wxSize size = getSize();
    dc.SetPen(wxNullPen);
    dc.DrawRectangle(wxPoint{0,0}, size);

    dc.SetPen(*wxBLACK_PEN);


    int     minBucketWidth  = 10;
    int     maxBucketCount  = (size.x - 20) / minBucketWidth;

    int     maxValue        = std::max(0, walkers[0].score() + 1);
    int     minValue        = std::min(0, walkers[99].score() - 1);
    float   bucketRange     = (maxValue - minValue) * 1.0 / maxBucketCount;
    int     posBucketCount  = (maxValue / bucketRange ) + 1;
    int     negBucketCount  = (-minValue / bucketRange) + 1;
    int     totalCount      = posBucketCount + negBucketCount;
    int     maxCount        = 0;

    std::vector<int>    count(totalCount, 0);
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
        maxCount = std::max(maxCount, count[bucketIndex]);
    }

    float   distYRel    = (size.y - 20) * 1.0 / maxCount;

    for (std::size_t loop = 0; loop < count.size(); ++loop)
    {
        int xCord   = 10 + loop * 10;
        int height  = count[loop] * distYRel * -1;
        dc.DrawRectangle(wxPoint{xCord, size.y -10}, wxSize{10, height});
    }
}

wxSize DrawableDistanceHotMap::getSize() const
{
    return wxSize(400, 400);
}

extern wxPanel* panelDistanceMap;

void DrawableDistanceHotMap::tick(bool update)
{
    if (panelDistanceMap && update)
    {
        panelDistanceMap->Refresh();
    }
}
