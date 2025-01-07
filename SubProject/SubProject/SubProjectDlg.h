
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
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButtonClear();
	afx_msg void OnBnClickedButtonRandom();

	// 이미지 초기화 함수
	void Initialize();

	// 이미지를 화면에 보여주는 함수 (Image -> BackBuffer)
	void RenderImage(int nOffsetX = 0, int nOffsetY = 0);

	// BackBuffer를 MainDC로 복사 (Double Buffering)
	void RenderMainDC();

	// RenderImage와 RenderMainDC를 엮고, 매 실행마다 딜레이를 주는 헬퍼 함수
	void UpdateDisplayWithDelay(int nTime, int nOffsetX = 0, int nOffsetY = 0);

	// 원 그리기 함수
	void DrawCircle(int nY, int nX, unsigned char nColor, int nRadius);

	// 이미지 프로세싱을 통한 도형 중심점 구하는 함수
	CCircleInfo CalcCenterPointOfShape();

	// 이미지 저장 및 불러오기
	void Save(CString strFileName);
	void Load(CString strFileName);

	// 컨트롤로부터 값을 가져오거나 집어넣는 입출력 함수
	int GetValueFromControl(int nID);
	void SetValueToControl(int nID, int nValue);
	void SetValueToControl(int nID, CString strValue);

	// 위치와 반지름을 받아 점을 생성하고 vector에 저장하는 함수
	CDot* AddDot(const CVector2& Pos, float fRadius=2.0f);

	// CDot의 vector를 초기화해주는 헬퍼 함수
	void ClearDots();

	// 세 점을 가지고 원을 만들 수 있는지 판별한 후, 만들 수 있다면 원의 중심점과 반지름을 반환하는 함수
	bool CalcCircle(const CVector2& P1, const CVector2& P2, const CVector2& P3, CVector2& Center, float& fRadius);

	// CalcCircle 함수를 이용하여 원을 그리는 함수
	CCircleInfo DrawCircleFromDots(int nThickness, int nColor);

	// Thread에서 Image를 쓸 수 있도록 Get 함수 작성
	CImage* GetImage() { return &m_Image; }

	// Thread에서 점들을 그릴 수 있도록 만든 헬퍼 함수
	void DrawDots(int nColor);

	int m_nWidth;
	int m_nHeight;
	int m_nRandRadius;
	CDC* m_MainDC;
	CDC m_BackBufDC;
	CVector2 m_MousePos;
	CBitmap   m_BackBufBit;
	bool m_bInitialized;
	CDot* m_pClickedDot;
	bool m_bRandomMovePlaying;
	
	std::vector<CDot*> m_Dots;
};
