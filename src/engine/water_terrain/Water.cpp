#include "Water.h"

float frand()
{
	int min = -1;
	int max = 1;

	float random = ((float) rand()) / (float) RAND_MAX;
    float range = max - min;  
    return (random*range) + min;
}

Water::Water(void)
{
}

Water::Water(int xDimensions, int zDimensions, float yBase)
{
	//m_plane = new float[xDimensions, zDimensions];

	for(int i = 0; i < xDimensions; i++)
	{
		for(int j = 0; j < zDimensions; j++)
			m_plane[i][j] = yBase;
	}

	m_iSizeX = xDimensions;
	m_iSizeZ = zDimensions;
}

Water::~Water(void)
{
}

void Water::DiamondSquare(unsigned x1, unsigned y1, unsigned x2, unsigned y2, float range, unsigned level) {

	float a, b, c, d, e;

	if(level >= 1)
	{
	// diamonds
    for (unsigned i = x1 + level; i < x2; i += level)
        for (unsigned j = y1 + level; j < y2; j += level) {
            a = m_plane[i - level][j - level];
            b = m_plane[i][j - level];
            c = m_plane[i - level][j];
            d = m_plane[i][j];
            m_plane[i - level / 2][j - level / 2] = (a + b + c + d) / 4 + frand() * range;
        }

    // squares
    for (unsigned i = x1 + 2 * level; i < x2; i += level)
        for (unsigned j = y1 + 2 * level; j < y2; j += level) {
            a = m_plane[i - level][j - level];
            b = m_plane[i][j - level];
            c = m_plane[i - level][j];
            d = m_plane[i][j];
            e = m_plane[i - level / 2][j - level / 2];

            m_plane[i - level][j - level / 2] = (a + c + e + m_plane[i - 3 * level / 2][j - level / 2]) / 4 + frand() * range;
            m_plane[i - level / 2][j - level] = (a + b + e + m_plane[i - level / 2][j - 3 * level / 2]) / 4 + frand() * range;
        }

		DiamondSquare(x1, y1, x2, y2, range / 2, level / 2);
	}
}

int Water::GetSizeX()
{
	return m_iSizeX;
}

int Water::GetSizeZ()
{
	return m_iSizeZ;
}

float Water::GetValAt(int x, int z)
{
	return m_plane[x][z];
}