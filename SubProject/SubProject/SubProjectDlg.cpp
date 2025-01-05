
// SubProjectDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "SubProject.h"
#include "SubProjectDlg.h"
#include "afxdialogex.h"
#include <thread>
#include <filesystem>
#include <string>
#include <iostream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define YOFFSET 50

namespace fs = std::filesystem;

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSubProjectDlg 대화 상자



CSubProjectDlg::CSubProjectDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SUBPROJECT_DIALOG, pParent)
	, m_MainDC()
	, m_BackBufBit()
	, m_BackBufDC()
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSubProjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSubProjectDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_MAKECIRCLE, &CSubProjectDlg::OnBnClickedButtonMakecircle)
	ON_BN_CLICKED(IDC_BUTTON_LOADCIRCLE, &CSubProjectDlg::OnBnClickedButtonLoadcircle)
END_MESSAGE_MAP()


// CSubProjectDlg 메시지 처리기

BOOL CSubProjectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	srand((unsigned int)time(NULL));

	CRect ClientRect;
	GetClientRect(&ClientRect);
	m_nWidth = ClientRect.Width();
	m_nHeight = ClientRect.Height() - YOFFSET;

	m_MainDC = GetDC();
	m_BackBufDC.CreateCompatibleDC(m_MainDC);
	m_BackBufBit.CreateCompatibleBitmap(m_MainDC, m_nWidth, m_nHeight);
	m_BackBufDC.SelectObject(m_BackBufBit);

	CEdit* pEditBox = (CEdit*)GetDlgItem(IDC_EDIT_INPUTNUM);
	if (pEditBox == nullptr)
		return FALSE;

	pEditBox->SetWindowText(TEXT("0"));
	Initialize();

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CSubProjectDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CSubProjectDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CSubProjectDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void ThreadProcess(CWnd* pParent, int nTimes)
{
	CSubProjectDlg* pDialog = (CSubProjectDlg*)pParent;

	for (int i = 0; i < nTimes; ++i)
	{
		int nRandX = rand() % pDialog->m_nWidth;
		int nRandY = rand() % pDialog->m_nHeight + YOFFSET;
		int nRandRadius = rand() % 100;

		pDialog->DrawCircle(nRandY, nRandX, 0xff, nRandRadius);
		pDialog->UpdateDisplayWithDelay(10);
		pDialog->Save(CString("Image"));
		if (i+1 != nTimes)
			pDialog->DrawCircle(nRandY, nRandX, 0x00, nRandRadius);
	}
}

void CSubProjectDlg::OnBnClickedButtonMakecircle()
{
	Initialize();

	CEdit* pEditBox = (CEdit*)GetDlgItem(IDC_EDIT_INPUTNUM);
	if (pEditBox == nullptr)
		return;

	CString strValue;
	pEditBox->GetWindowText(strValue);
	int nTimes = _ttoi(strValue);

	std::thread Thread(ThreadProcess, this, nTimes);
	Thread.detach();

	UpdateDisplay();
}

void CSubProjectDlg::Initialize()
{
	int nBitsPerPixel = 8;

	if (m_Image != NULL)
	{
		m_Image.Destroy();
	}

	m_Image.Create(m_nWidth, m_nHeight, nBitsPerPixel);
	unsigned char* p = (unsigned char*)m_Image.GetBits();

	if (nBitsPerPixel == 8)
	{
		static RGBQUAD rgb[256];
		for (int i = 0; i < 256; ++i)
		{
			rgb[i].rgbRed = rgb[i].rgbGreen = rgb[i].rgbBlue = i;
		}
		m_Image.SetColorTable(0, 256, rgb);
	}
}

void CSubProjectDlg::Save(CString strFileName)
{
	if (m_Image.IsNull())
	{
		AfxMessageBox(_T("m_Image가 NULL 입니다."));
		return;
	}

	fs::path CurrentPath = fs::current_path();
	fs::path ImageFolderPath = CurrentPath / "Image";
	if (!fs::exists(ImageFolderPath))
		fs::create_directory(ImageFolderPath);
	
	fs::path CheckPath = ImageFolderPath / std::wstring(strFileName);
	fs::path ResultPath;
	int nSuffixNum = 0;
	
	do
	{
		ResultPath = CheckPath.wstring() + _T("_") + std::to_wstring(nSuffixNum) + _T(".bmp");
		nSuffixNum++;
	} while (fs::exists(ResultPath));

	m_Image.Save(ResultPath.c_str());
}

void CSubProjectDlg::Load(CString strFileName)
{
	if (!m_Image.IsNull())
		m_Image.Destroy();
	
	m_Image.Load(strFileName);

	if (m_Image.IsNull())
	{
		AfxMessageBox(_T("m_Image를 제대로 불러오지 못했습니다."));
		return;
	}
}

void CSubProjectDlg::DrawCircle(int nY, int nX, unsigned char nColor, int nRadius)
{
	unsigned char* p = (unsigned char*)m_Image.GetBits();
	int nPitch = m_Image.GetPitch();

	for (int y = nY - nRadius; y < nY + nRadius; ++y)
	{
		for (int x = nX - nRadius; x < nX + nRadius; ++x)
		{
			if (y < 0 || y >= m_nHeight || x < 0 || x >= m_nWidth)
				continue;

			int nDistX = x - nX;
			int nDistY = y - nY;
			int nDist = nDistX * nDistX + nDistY * nDistY;
			if (nDist < nRadius * nRadius)
			{
				p[y * nPitch + x] = nColor;
			}
		}
	}
}

void CSubProjectDlg::UpdateDisplay(int nOffsetX, int nOffsetY)
{
	m_Image.Draw(m_BackBufDC, nOffsetX, nOffsetY);
	m_MainDC->BitBlt(0, YOFFSET, m_nWidth, m_nHeight, &m_BackBufDC, 0, 0, SRCCOPY);
}

void CSubProjectDlg::UpdateDisplayWithDelay(int nTime, int nOffsetX, int nOffsetY)
{
	Sleep(nTime);
	UpdateDisplay(nOffsetX, nOffsetY);
}


void CSubProjectDlg::OnBnClickedButtonLoadcircle()
{
	CFileDialog FDialog(TRUE, _T("jpg"), nullptr,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("이미지 파일 (*.jpg;*.jpeg;*.bmp)|*.jpg;*.jpeg;*.bmp|모든 파일 (*.*)|*.*||"));

	if (FDialog.DoModal() != IDOK)
		return;

	Load(FDialog.GetPathName());
	int nSumX = 0;
	int nSumY = 0;
	int nCount = 0;

	unsigned char* p = (unsigned char*)m_Image.GetBits();
	int nPitch = m_Image.GetPitch();

	for (int y = 0; y < m_Image.GetHeight(); ++y)
	{
		for (int x = 0; x < m_Image.GetWidth(); ++x)
		{
			if (p[y * nPitch + x] > 0x00)
			{
				nSumX += x;
				nSumY += y;
				nCount++;
			}
		}
	}

	double dCenterX = (double)nSumX / nCount;
	double dCenterY = (double)nSumY / nCount;

	UpdateDisplay();

	CClientDC dc(this);

	LOGBRUSH lbr;
	lbr.lbStyle = BS_SOLID;
	lbr.lbColor = RGB(255, 0, 0);
	lbr.lbHatch = 0;

	CPen DotPen(PS_GEOMETRIC | PS_DOT, 3, &lbr, 0, 0);
	CPen* OldPen = dc.SelectObject(&DotPen);

	dc.MoveTo(dCenterX, YOFFSET);
	dc.LineTo(dCenterX, dCenterY + YOFFSET);

	dc.MoveTo(0, dCenterY + YOFFSET);
	dc.LineTo(dCenterX, dCenterY + YOFFSET);

	dc.SelectObject(OldPen);
	Invalidate(false);
}
