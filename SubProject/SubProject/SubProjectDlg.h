
// SubProjectDlg.h: 헤더 파일
//

#pragma once


// CSubProjectDlg 대화 상자
class CSubProjectDlg : public CDialogEx
{
private:
	CImage m_Image;
	int m_nWidth;
	int m_nHeight;
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
	void Initialize();
	void DrawCircle(int nY, int nX);

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonMakecircle();
	void UpdateDisplay(int OffsetX = 0, int OffsetY = 50);
};
