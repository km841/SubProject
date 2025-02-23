﻿
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
	, m_nRandRadius(0)
	, m_bInitialized(false)
	, m_bRandomMovePlaying(false)
	, m_pClickedDot(nullptr)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CSubProjectDlg::~CSubProjectDlg()
{
	for (int i = 0; i < m_Dots.size(); ++i)
	{
		if (m_Dots[i] != nullptr)
			delete m_Dots[i];
	}

	m_Dots.clear();
}

void CSubProjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSubProjectDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CSubProjectDlg::OnBnClickedButtonClear)
	ON_BN_CLICKED(IDC_BUTTON_RANDOM, &CSubProjectDlg::OnBnClickedButtonRandom)
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
	m_BackBufBit.CreateCompatibleBitmap(m_MainDC, m_nWidth, m_nHeight + YOFFSET);
	m_BackBufDC.SelectObject(m_BackBufBit);

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
	if (!m_bInitialized)
	{
		RenderImage();
		RenderMainDC();
		m_bInitialized = true;
	}


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

void CSubProjectDlg::Initialize()
{
	int nBitsPerPixel = 8;

	if (!m_Image.IsNull())
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

CCircleInfo CSubProjectDlg::CalcCenterPointOfShape()
{
	CVector2 cp;

	int nSumX = 0;
	int nSumY = 0;
	int nCount = 0;

	unsigned char* p = (unsigned char*)m_Image.GetBits();
	int nPitch = m_Image.GetPitch();

	int nMaxX = -21e7;
	int nMinX = 21e7;

	for (int y = 0; y < m_Image.GetHeight(); ++y)
	{
		for (int x = 0; x < m_Image.GetWidth(); ++x)
		{
			if (p[y * nPitch + x] > 0x00)
			{
				nSumX += x;
				nSumY += y;
				nCount++;

				nMaxX = max(nMaxX, x);
				nMinX = min(nMinX, x);
			}
		}
	}

	cp.x = (double)nSumX / nCount;
	cp.y = (double)nSumY / nCount;

	CCircleInfo ci;
	ci.Center = cp;
	ci.fRadius = (nMaxX - nMinX) / 2;

	return ci;
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

int CSubProjectDlg::GetValueFromControl(int nID)
{
	CEdit* pEditBox = (CEdit*)GetDlgItem(nID);
	if (pEditBox == nullptr)
		return -1;

	CString strValue;
	pEditBox->GetWindowText(strValue);
	return _ttoi(strValue);
}

void CSubProjectDlg::SetValueToControl(int nID, int nValue)
{
	CEdit* pEditBox = (CEdit*)GetDlgItem(nID);
	if (pEditBox == nullptr)
		return;

	pEditBox->SetWindowText(std::to_wstring(nValue).c_str());
}

void CSubProjectDlg::SetValueToControl(int nID, CString strValue)
{
	CEdit* pEditBox = (CEdit*)GetDlgItem(nID);
	if (pEditBox == nullptr)
		return;

	pEditBox->SetWindowText(strValue);
}

CDot* CSubProjectDlg::AddDot(const CVector2& Pos, float fRadius)
{
	CDot* pDot = new CDot{ Pos, fRadius };
	m_Dots.push_back(pDot);

	pDot->SetColor(0x0000ff);
	for (int i = 0; i < m_Dots.size(); ++i)
	{
		m_Dots[i]->SetDepth(i);
	}

	return pDot;
}

void CSubProjectDlg::ClearDots()
{
	for (int i = 0; i < m_Dots.size(); ++i)
	{
		if (m_Dots[i])
		{
			m_Dots[i]->Draw(m_Image, m_nWidth, m_nHeight, 0x00);
			delete m_Dots[i];
		}
	}

	m_Dots.clear();
}

bool CSubProjectDlg::CalcCircle(const CVector2& P1, const CVector2& P2, const CVector2& P3, CVector2& Center, float& fRadius)
{
	float a1 = P2.x - P1.x;
	float b1 = P2.y - P1.y;
	float a2 = P3.x - P1.x;
	float b2 = P3.y - P1.y;

	float c1 = (a1 * (P2.x + P1.x) + b1 * (P2.y + P1.y)) / 2.0f;
	float c2 = (a2 * (P3.x + P1.x) + b2 * (P3.y + P1.y)) / 2.0f;

	float det = a1 * b2 - a2 * b1;

	if (std::fabs(det) < 1e-6) {
		return false;
	}

	Center.x = (c1 * b2 - c2 * b1) / det;
	Center.y = (a1 * c2 - a2 * c1) / det;

	fRadius = std::sqrt((P1.x - Center.x) * (P1.x - Center.x) + (P1.y - Center.y) * (P1.y - Center.y));
	return true;
}

CCircleInfo CSubProjectDlg::DrawCircleFromDots(int nThickness, int nColor)
{
	CCircleInfo Info;
	CVector2 Center;
	float fRadius = 0.0f;

	if (CalcCircle(m_Dots[0]->GetPos(), m_Dots[1]->GetPos(), m_Dots[2]->GetPos(), Center, fRadius))
	{
		unsigned char* p = (unsigned char*)m_Image.GetBits();
		int nPitch = m_Image.GetPitch();

		for (int y = Center.y - fRadius - nThickness; y < Center.y + fRadius + nThickness; ++y)
		{
			for (int x = Center.x - fRadius - nThickness; x < Center.x + fRadius + nThickness; ++x)
			{
				if (y < 0 || y >= m_nHeight || x < 0 || x >= m_nWidth)
					continue;

				int nDistX = x - Center.x;
				int nDistY = y - Center.y;
				int nDist = nDistX * nDistX + nDistY * nDistY;

				if (nDist >= (fRadius - nThickness) * (fRadius - nThickness) &&
					nDist <= (fRadius + nThickness) * (fRadius + nThickness))
				{
					p[y * nPitch + x] = nColor;
				}
			}
		}

		Info.Center = Center;
		Info.fRadius = fRadius;
		return Info;
	}

	return CCircleInfo{ CVector2{}, 0.0f };
}

void CSubProjectDlg::DrawDots(int nColor)
{
	for (int i = 0; i < m_Dots.size(); ++i)
	{
		m_Dots[i]->Draw(m_Image, m_nWidth, m_nHeight, nColor);
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

void CSubProjectDlg::RenderImage(int nOffsetX, int nOffsetY)
{
	m_Image.Draw(m_BackBufDC, nOffsetX, nOffsetY);
}

void CSubProjectDlg::UpdateDisplayWithDelay(int nTime, int nOffsetX, int nOffsetY)
{
	Sleep(nTime);
	RenderImage(nOffsetX, nOffsetY);
	RenderMainDC();
}

void CSubProjectDlg::RenderMainDC()
{
	m_MainDC->BitBlt(0, YOFFSET, m_nWidth, m_nHeight + YOFFSET, &m_BackBufDC, 0, 0, SRCCOPY);
}


void ActionThreadProcess(CWnd* pParent, const CVector2& StartP, const CVector2& EndP, int nRadius)
{
	CSubProjectDlg* pDialog = (CSubProjectDlg*)pParent;

	CVector2 DeltaV = EndP - StartP;
	CVector2 StartV = StartP;
	float Velocity = 50;

	DeltaV /= Velocity;
	for (int i = 0; i < Velocity; ++i)	{
		StartV += DeltaV;

		pDialog->DrawCircle(StartV.y, StartV.x, 0xff, pDialog->m_nRandRadius);
		pDialog->UpdateDisplayWithDelay(10);
		pDialog->Save(CString("Image"));
		pDialog->DrawCircle(StartV.y, StartV.x, 0x00, pDialog->m_nRandRadius + 50);
	}

	pDialog->DrawCircle(StartV.y, StartV.x, 0xff, pDialog->m_nRandRadius);
}


void CSubProjectDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	CDialogEx::OnMouseMove(nFlags, point);
	if (m_bRandomMovePlaying)
		return;


	m_MousePos = CVector2::CPointToVector2(point);
	m_MousePos.y -= YOFFSET;

	if (m_pClickedDot)
	{
		CVector2 PrevPos = m_pClickedDot->GetPos();
		if (PrevPos != m_MousePos)
		{
			float fThickness = 2.0f;
			DrawCircleFromDots(fThickness + 2.0f, 0x00);

			m_pClickedDot->Draw(m_Image, m_nWidth, m_nHeight, 0x00);
			m_pClickedDot->SetPos(m_MousePos);
			m_pClickedDot->Draw(m_Image, m_nWidth, m_nHeight, 0xff);

			DrawCircleFromDots(fThickness, 0xff);

			for (int i = 0; i < m_Dots.size(); ++i)
			{
				if (m_Dots[i] != nullptr)
					m_Dots[i]->Draw(m_Image, m_nWidth, m_nHeight, 0xff);
			}

			RenderImage();
			RenderMainDC();
		}

	}
}

void CSubProjectDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_bRandomMovePlaying)
		return;

	m_MousePos = CVector2::CPointToVector2(point);
	m_MousePos.y -= YOFFSET;

	if (m_Dots.size() == 3)
	{
		for (int i = 0; i < m_Dots.size(); ++i)
		{
			if (m_Dots[i]->IsHovered(m_MousePos))
			{
				m_pClickedDot = m_Dots[i];
				break;
			}
		}
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}

void CSubProjectDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_bRandomMovePlaying)
		return;
	m_pClickedDot = nullptr;

	if (m_Image.IsNull())
		Initialize();

	float fThickness = 2.0f;
	if (m_Dots.size() < 3)
	{
		AddDot(m_MousePos, 10.0f);
		for (int i = 0; i < m_Dots.size(); ++i)
		{
			if (m_Dots[i] != nullptr)
				m_Dots[i]->Draw(m_Image, m_nWidth, m_nHeight, 0xff);
		}
		RenderImage();
		RenderMainDC();

		if (m_Dots.size() == 3)
		{
			DrawCircleFromDots(fThickness, 0xff);
			RenderImage();
		}
	}

	CDialogEx::OnLButtonUp(nFlags, point);
	RenderMainDC();
}


void CSubProjectDlg::OnBnClickedButtonClear()
{
	Initialize();
	m_pClickedDot = nullptr;
	for (int i = 0; i < m_Dots.size(); ++i)
	{
		if (m_Dots[i] != nullptr)
			delete m_Dots[i];
	}

	m_Dots.clear();

	RenderImage();
	RenderMainDC();
}

void WorkThread(CWnd* pParent)
{
	CSubProjectDlg* pDialog = (CSubProjectDlg*)pParent;
	pDialog->ClearDots();

	float fThickness = 2.0f;
	for (int i = 0; i < 10; ++i)
	{
		pDialog->ClearDots();
		for (int j = 0; j < 3; ++j)
		{
			float fRandX = rand() % pDialog->m_nWidth;
			float fRandY = rand() % pDialog->m_nHeight;
			CVector2 Pos{ fRandX, fRandY };
			CDot* pDot = pDialog->AddDot(Pos, 10.0f);

			CImage* Image = pDialog->GetImage();
			pDot->Draw(*Image, Image->GetWidth(), Image->GetHeight(), 0x00);
		}

		pDialog->DrawCircleFromDots(fThickness, 0xff);
		pDialog->DrawDots(0xff);
		pDialog->UpdateDisplayWithDelay(500);
		pDialog->DrawCircleFromDots(fThickness, 0x00);
	}
	pDialog->m_bRandomMovePlaying = false;
}

void CSubProjectDlg::OnBnClickedButtonRandom()
{
	Initialize();
	m_bRandomMovePlaying = true;
	
	std::thread Work(WorkThread, this);
	Work.detach();

}
