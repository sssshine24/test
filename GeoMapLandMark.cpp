#include "stdafx.h"
#include "GeoMapLandMark.h"

GeoMapLandMark::GeoMapLandMark(void)
{
	style = 0;
	deleteflag = 0;
	fixedflag = 0;
	isAuxillary = 0;
	isVisible = 1;
}

GeoMapLandMark::~GeoMapLandMark(void)
{
}
int GeoMapLandMark::IsPointOn(int vindex)
{
	for (int k = 0; k < landmark_points.size(); k++)
	{
		int vindex1 = landmark_points[k];
		if (vindex == vindex1)
		{
			return 1;
		}
	}
	return 0;
}
int GeoMapLandMark::IsEqual(GeoMapLandMark* pLandmark)
{
	if (landmark_points.size() != pLandmark->landmark_points.size())
		return 0;

	int equalflag = 1;
	for (int i = 0; i < landmark_points.size(); i++)
	{
		if (landmark_points[i] != pLandmark->landmark_points[i])
		{
			equalflag = 0;
			break;
		}
	}
	if (!equalflag)
	{
		for (int i = 0; i < landmark_points.size(); i++)
		{
			if (landmark_points[i] != pLandmark->landmark_points[landmark_points.size() - 1 - i])
			{
				equalflag = 0;
				break;
			}
		}
	}
	return equalflag;
}
