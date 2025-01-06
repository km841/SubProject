
// SubProjectDlg.h: 헤더 파일
//
#pragma once
#include "MathLib.h"
#include "CDot.h"
#include <vector>


struct CCircleInfo
{
	CVector2 Center;
	float fRadius;
};


// CSubProjectDlg 대화 상자
class CSubProjectDlg : public CDialogEx
{
private:
	CImage m_Image;

// 생성입니다.
public:
	CSubProjectDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.
	~CSubProjectDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SUBPROJECT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;
	
	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonLoadcircle();
	afx_msg void OnBnClickedButtonDraw();
	afx_msg void OnBnClickedButtonAction();
	afx_msg void OnBnClickedButtonThreedot();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

	void RenderImage(int nOffsetX = 0, int nOffsetY = 0);
	void UpdateDisplayWithDelay(int nTime, int nOffsetX = 0, int nOffsetY = 0);
	void RenderMainDC();
	void DrawCircle(int nY, int nX, unsigned char nColor, int nRadius);
	void Initialize();
	CCircleInfo CalcCenterPointOfShape();
	void Save(CString strFileName);
	void Load(CString strFileName);
	int GetValueFromControl(int nID);
	void SetValueToControl(int nID, int nValue);
	void SetValueToControl(int nID, CString strValue);
	CDot* AddDot(const CVector2& Pos, const CVector2& Size = CVector2(2.0f, 2.0f));
	void DeleteFrontDot();
	bool CalcCircle(const CVector2& P1, const CVector2& P2, const CVector2& P3, CVector2& Center, float& fRadius);
	CCircleInfo DrawCircleFromDots(int nThickness, int nColor);

	int m_nWidth;
	int m_nHeight;
	int m_nRandRadius;
	CDC* m_MainDC;
	CDC m_BackBufDC;
	CVector2 m_MousePos;
	CBitmap   m_BackBufBit;
	bool m_bDotMode;
	
	std::vector<CDot*> m_Dots;
};
