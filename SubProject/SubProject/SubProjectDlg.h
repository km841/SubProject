
// SubProjectDlg.h: 헤더 파일
//
#pragma once

struct CVector2
{
	float x = 0.0f;
	float y = 0.0f;

	CVector2(float x, float y)
		: x(x), y(y)
	{}

	CVector2()
		: x(0.0f), y(0.0f)
	{}

	CVector2 operator/(float value)
	{
		return CVector2(x / value, y / value);
	}

	CVector2& operator+=(const CVector2& other)
	{
		x += other.x;
		y += other.y;
		return *this;
	}

	CVector2& operator/=(float value)
	{
		x /= value;
		y /= value;
		return *this;
	}

	CVector2 operator-(const CVector2& other)
	{
		return CVector2(x - other.x, y - other.y);
	}

	CVector2 operator-(const CVector2& other) const
	{
		return CVector2(x - other.x, y - other.y);
	}

	static CVector2 CPointToVector2(const CPoint& cp)
	{
		return CVector2((float)cp.x, (float)cp.y);
	}
};

struct CCircleInfo
{
	CVector2 cp;
	int nRadius;
};


// CSubProjectDlg 대화 상자
class CSubProjectDlg : public CDialogEx
{
private:
	CImage m_Image;

// 생성입니다.
public:
	CSubProjectDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

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

	void UpdateDisplay(int nOffsetX = 0, int nOffsetY = 0);
	void UpdateDisplayWithDelay(int nTime, int nOffsetX = 0, int nOffsetY = 0);
	void DrawCircle(int nY, int nX, unsigned char nColor, int nRadius);
	void Initialize();
	CCircleInfo CalcCenterPointOfShape();
	void Save(CString strFileName);
	void Load(CString strFileName);
	int GetValueFromEditBox(int nID);
	void SetValueToEditBox(int nID, int nValue);
	void SetValueToEditBox(int nID, CString strValue);

	int m_nWidth;
	int m_nHeight;
	int m_nRandRadius;
	CDC* m_MainDC;
	CDC m_BackBufDC;
	CBitmap   m_BackBufBit;
};
