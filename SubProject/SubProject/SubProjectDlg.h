
// SubProjectDlg.h: 헤더 파일
//
#pragma once
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
	afx_msg void OnBnClickedButtonMakecircle();
	afx_msg void OnBnClickedButtonLoadcircle();

	void UpdateDisplay(int nOffsetX = 0, int nOffsetY = 50);
	void UpdateDisplayWithDelay(int nTime, int nOffsetX = 0, int nOffsetY = 50);
	void DrawCircle(int nY, int nX, unsigned char nColor, int nRadius);
	void Initialize();
	void Save(CString strFileName);
	void Load(CString strFileName);
	int m_nWidth;
	int m_nHeight;
	CDC* m_MainDC;
	CDC m_BackBufDC;
	CBitmap   m_BackBufBit;
};
