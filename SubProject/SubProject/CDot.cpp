#include "pch.h"
#include "CDot.h"

CDot::CDot()
	: m_Pos{}
	, m_fRadius(2.0f)
	, m_nDepth(0)
	, m_nColor(0)
{
}

CDot::CDot(const CVector2& Pos, float fRadius)
	: m_Pos(Pos)
	, m_fRadius(fRadius)
	, m_nDepth(0)
	, m_nColor(0)
{
}

bool CDot::IsHovered(const CVector2& MousePos)
{
	CVector2 Diff = MousePos - m_Pos;
	float fLen = Diff.Length();

	if (fLen <= m_fRadius)
		return true;
	else
		return false;
}

void CDot::SetDepth(int nDepth)
{
	m_nDepth = nDepth;
}

void CDot::SetColor(int nColor)
{
	m_nColor = nColor;
}

void CDot::Draw(CImage& InImage, int nMaxWidth, int nMaxHeight, int nColor)
{
	unsigned char* p = (unsigned char*)InImage.GetBits();
	int nPitch = InImage.GetPitch();

	for (int y = m_Pos.y - m_fRadius; y < m_Pos.y + m_fRadius; ++y)
	{
		for (int x = m_Pos.x - m_fRadius; x < m_Pos.x + m_fRadius; ++x)
		{
			if (y < 0 || y >= nMaxHeight || x < 0 || x >= nMaxWidth)
				continue;

			int nDistX = x - m_Pos.x;
			int nDistY = y - m_Pos.y;
			int nDist = nDistX * nDistX + nDistY * nDistY;
			if (nDist < m_fRadius * m_fRadius)
			{
				p[y * nPitch + x] = nColor;
			}
		}
	}
}
void CDot::SetPos(const CVector2& Pos)
{
	m_Pos = Pos;
}
