#pragma once

#include "MathLib.h"
class CDot
{
public:
	CDot();
	CDot(const CVector2& Pos, float fRadius);
	bool IsHovered(const CVector2& MousePos);
	void SetDepth(int nDepth);
	void SetColor(int nColor);
	void Draw(CImage& InImage, int nMaxWidth, int nMaxHeight, int nColor);
	void SetPos(const CVector2& Pos);
	const CVector2& GetPos() const { return m_Pos; }


private:
	CVector2 m_Pos;
	float m_fRadius;
	int m_nDepth;
	int m_nColor;
};

