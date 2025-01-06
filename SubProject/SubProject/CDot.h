#pragma once

#include "MathLib.h"
class CDot
{
public:
	CDot();
	CDot(const CVector2& Pos, const CVector2& Size = CVector2(2, 2));
	bool IsHovered(const CVector2& MousePos);
	void SetDepth(int nDepth);
	void SetColor(int nColor);
	void Draw(CImage& InImage, int nMaxWidth, int nMaxHeight, int nColor);
	void SetPos(const CVector2& Pos);
	const CVector2& GetPos() { return m_Pos; }


private:
	CVector2 m_Pos;
	CVector2 m_Size;
	int m_nDepth;
	int m_nColor;
};

