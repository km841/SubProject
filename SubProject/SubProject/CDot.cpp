#include "pch.h"
#include "CDot.h"

CDot::CDot()
	: m_Pos{}
	, m_Size{}
	, m_nDepth(0)
	, m_nColor(0)
{
}

CDot::CDot(const CVector2& Pos, const CVector2& Size)
	: m_Pos(Pos)
	, m_Size(Size)
	, m_nDepth(0)
	, m_nColor(0)
{
}

bool CDot::IsHovered(const CVector2& MousePos)
{
	if (MousePos.x > m_Pos.x - m_Size.x / 2 &&
		MousePos.x < m_Pos.x + m_Size.x / 2 &&
		MousePos.y > m_Pos.y - m_Size.y / 2 &&
		MousePos.y < m_Pos.y + m_Size.y / 2)
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

	for (int y = m_Pos.y - m_Size.y / 2; y < m_Pos.y + m_Size.y / 2; ++y)
	{
		for (int x = m_Pos.x - m_Size.x / 2; x < m_Pos.x + m_Size.x / 2; ++x)
		{
			if (y < 0 || y >= nMaxHeight || x < 0 || x >= nMaxWidth)
				continue;

			p[y * nPitch + x] = nColor;
			
		}
	}
}
void CDot::SetPos(const CVector2& Pos)
{
	m_Pos = Pos;
}
