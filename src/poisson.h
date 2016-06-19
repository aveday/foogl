#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "util.h"

bool IsInRectangle(const glm::vec2 &point)
{
    return point.x >= 0 && point.y >= 0
        && point.x <= 1 && point.y <= 1;
}

bool IsInCircle(const glm::vec2 &point)
{
    float fx = point.x - 0.5f;
    float fy = point.y - 0.5f;
    return ( fx*fx + fy*fy ) <= 0.25f;
}

glm::ivec2 ImageToGrid( const glm::vec2& P, float CellSize )
{
	return glm::ivec2( (int)(P.x / CellSize), (int)(P.y / CellSize) );
}

struct sGrid
{
	sGrid( int W, int H, float CellSize )
		: m_W( W ) , m_H( H ) , m_CellSize( CellSize )
	{
		m_Grid.resize( m_H );

		for ( auto i = m_Grid.begin(); i != m_Grid.end(); i++ )
            i->resize( m_W );
	}

	void Insert( const glm::vec2& P )
	{
		glm::ivec2 G = ImageToGrid( P, m_CellSize );
		m_Grid[ G.x ][ G.y ] = P;
	}

	bool IsInNeighbourhood( glm::vec2 Point, float MinDist, float CellSize )
	{
		glm::ivec2 G = ImageToGrid( Point, CellSize );

		// number of adjacent cells to look for neighbour points
		const int D = 5;

		// scan the neighbourhood of the point in the grid
		for ( int i = G.x - D; i < G.x + D; i++ )
			for ( int j = G.y - D; j < G.y + D; j++ )
				if ( i >= 0 && i < m_W && j >= 0 && j < m_H )
                    if (glm::distance2(m_Grid[i][j], Point) < MinDist*MinDist)
                        return true;

		return false;
	}

private:
	int m_W;
	int m_H;
	float m_CellSize;

	std::vector< std::vector<glm::vec2> > m_Grid;
};

glm::vec2 PopRandom( std::vector<glm::vec2>& Points )
{
	const int Idx = random_i(Points.size());
	const glm::vec2 P = Points[ Idx ];
	Points.erase( Points.begin() + Idx );
	return P;
}

glm::vec2 GenerateRandomPointAround( const glm::vec2& P, float MinDist )
{
    float Radius = random_f(MinDist, 2*MinDist);
	float Angle = random_f(2*M_PI);
	return glm::vec2(P.x + Radius * cos(Angle),
                     P.y + Radius * sin(Angle));
}

std::vector<glm::vec2> GeneratePoissonPoints(
        int NumPoints,
        int NewPointsCount = 30,
        bool Circle = true,
        float MinDist = -1.0f)
{
	if ( MinDist < 0.0f )
		MinDist = sqrt( float(NumPoints) ) / float(NumPoints);

	std::vector<glm::vec2> SamplePoints;
	std::vector<glm::vec2> ProcessList;

	// create the grid
	float CellSize = MinDist / M_SQRT2;

	int GridW = (int)ceil( 1.0f / CellSize );
	int GridH = (int)ceil( 1.0f / CellSize );

	sGrid Grid( GridW, GridH, CellSize );

	glm::vec2 FirstPoint;
 	do {
		FirstPoint = glm::vec2( random_f(), random_f() );	     
	} while (!(Circle ? IsInCircle(FirstPoint) : IsInRectangle(FirstPoint)));

	// update containers
	ProcessList.push_back( FirstPoint );
	SamplePoints.push_back( FirstPoint );
	Grid.Insert( FirstPoint );

	// generate new points for each point in the queue
	while ( !ProcessList.empty() && (int)SamplePoints.size() < NumPoints ) {
		glm::vec2 Point = PopRandom( ProcessList );

		for ( int i = 0; i < NewPointsCount; i++ ) {
			glm::vec2 NewPoint = GenerateRandomPointAround( Point, MinDist );

			bool Fits = Circle ? IsInCircle(NewPoint) : IsInRectangle(NewPoint);

			if ( Fits && !Grid.IsInNeighbourhood( NewPoint, MinDist, CellSize ) ) {
				ProcessList.push_back( NewPoint );
				SamplePoints.push_back( NewPoint );
				Grid.Insert( NewPoint );
				continue;
			}
		}
	}

	return SamplePoints;
}

