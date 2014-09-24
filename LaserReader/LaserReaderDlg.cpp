
// LaserReaderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LaserReader.h"
#include "LaserReaderDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <cmath>

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CLaserReaderDlg dialog




CLaserReaderDlg::CLaserReaderDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLaserReaderDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_bOpen = false;
	nPackageIndex = 0;

	fptr = NULL;
	fptr = fopen("DistanceValue.txt", "w");

	szPort[0] = L"COM1";
	szPort[1] = L"COM2";
	szPort[2] = L"COM3";
	szPort[3] = L"COM4";
	szPort[4] = L"COM5";
	szPort[5] = L"COM6";
	szPort[6] = L"COM7";
	szPort[7] = L"COM8";
	
	//4800;9600;19200;38400;57600;115200;460800;
	nBaudRateArray[0] = 4800;
	nBaudRateArray[1] = 9600;
	nBaudRateArray[2] = 19200;
	nBaudRateArray[3] = 38400;
	nBaudRateArray[4] = 57600;
	nBaudRateArray[5] = 115200;
	nBaudRateArray[6] = 460800;

	nCompletePackage = 0;

	InitCosineValue(dCosine, 274);
}


CLaserReaderDlg::~CLaserReaderDlg()
{
	edit.Detach();
	if (fptr)
	{
		fclose(fptr);
	}
}


void CLaserReaderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PORT, m_cbPort);
	DDX_Control(pDX, IDC_BAUDRATE, m_cbBaudRate);
}

BEGIN_MESSAGE_MAP(CLaserReaderDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_OPENCLOSE, &CLaserReaderDlg::OnBnClickedOpenclose)
END_MESSAGE_MAP()


// CLaserReaderDlg message handlers

BOOL CLaserReaderDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	edit.Attach(GetDlgItem(IDC_COMDATA)->m_hWnd);

	GetDlgItem(IDC_STATIC_PORT)->MoveWindow(5, 12, 42, 20);
	GetDlgItem(IDC_PORT)->MoveWindow(40, 10, 85, 20);
	GetDlgItem(IDC_STATIC_BAUDRATE)->MoveWindow(160, 12, 57, 20);
	GetDlgItem(IDC_BAUDRATE)->MoveWindow(210, 10, 85, 20);
	GetDlgItem(IDC_OPENCLOSE)->MoveWindow(450, 8, 75, 28);
	GetDlgItem(IDC_COMDATA)->MoveWindow(5, 50, 550, 350);

	GetDlgItem(IDC_STATIC_NUMBER)->MoveWindow(300, 12, 40, 20);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CLaserReaderDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CLaserReaderDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CLaserReaderDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CLaserReaderDlg::OnBnClickedOpenclose()
{
	// TODO: Add your control notification handler code here
	if (m_bOpen)
	{
		// comm already open
		SerialPort.Flush();
		SerialPort.Close();
		KillTimer(m_nTimer);
		m_bOpen = false;
		GetDlgItem(IDC_OPENCLOSE)->SetWindowText(L"´ò¿ª");
	}
	else
	{
		UpdateData(TRUE);

		//COMMCONFIG config;
		//CSerialPort::GetDefaultConfig(6, config);

		SerialPort.Open(szPort[m_cbPort.GetCurSel()],
			nBaudRateArray[m_cbBaudRate.GetCurSel()],
			CSerialPort::NoParity,
			8, CSerialPort::OneStopBit);
		hPort = SerialPort.Detach();
		SerialPort.Attach(hPort);

		m_nTimer = 1000;
		if (SetTimer(m_nTimer, 10, NULL) == 0)
		{
			MessageBox(L"Start timer failed", L"ERROR", MB_OKCANCEL | MB_ICONERROR);
			SerialPort.Flush();
			SerialPort.Close();
		}
		else
		{
			m_bOpen = true;
			GetDlgItem(IDC_OPENCLOSE)->SetWindowText(L"¹Ø±Õ");
		}
	}
}


void CLaserReaderDlg::OnTimer(UINT_PTR nIDEvent)
{	
	DWORD dwReadLength = 0;
	BYTE bTmpBuffer[2048];
	//int i = 0;
	int nRemains = 0;

	dwReadLength = SerialPort.Read(bTmpBuffer, 1024);

	//for (i = 0; i < dwReadLength; ++i)
	//{
	//	fprintf(fptr, "%d ", (int)(bTmpBuffer[i]));
	//}
			
	for (unsigned int i = 0; dwReadLength > 6 && i < dwReadLength - 6; i++)
	{
		if (bTmpBuffer[i] == 0xFC && bTmpBuffer[i + 1] == 0xFD
		&& bTmpBuffer[i + 2] == 0xFE && bTmpBuffer[i + 3] == 0xFF)
		{
			nCompletePackage++;
			CString csNumber;
			csNumber.Format(L"%d", nCompletePackage);
			this->GetDlgItem(IDC_STATIC_NUMBER)->SetWindowText(csNumber);

			char* ptr = (char*)bTotalPackage;
			nRemains = (int)(bTmpBuffer[i + 4]) + 256 * (int)(bTmpBuffer[i + 5]);

			for (int j = i + 6; (j < dwReadLength) && (nRemains > 0); j++)
			{
				*ptr = bTmpBuffer[j];
				ptr++;
				nRemains--;
			}

			while (nRemains > 0)
			{
				dwReadLength = SerialPort.Read(ptr, nRemains);
				nRemains -= dwReadLength;
				ptr += dwReadLength;
			}

			int nPlaneTmp;
			int nTmp;

			nPlaneTmp = (int)(bTotalPackage[8]);
			fprintf(fptr, "Plane %d:\n", nPlaneTmp);
			for (int j = 9; j < 556; j += 2)
			{
				nTmp = (int)(bTotalPackage[j + 1]) * 256 + (int)(bTotalPackage[j]);
				
				// calculate height
				nTmp *= dCosine[(j - 9) / 2];
				
				fprintf(fptr, "%d ", nTmp);
			}
			fprintf(fptr, "\n");

			nPlaneTmp = (int)(bTotalPackage[1655]);
			fprintf(fptr, "Plane %d:\n", nPlaneTmp);
			for (int j = 1656; j < 2203; j += 2)
			{
				nTmp = (int)(bTotalPackage[j + 1]) * 256 + (int)(bTotalPackage[j]);

				// calculate height
				nTmp *= dCosine[(j - 1656) / 2];

				fprintf(fptr, "%d ", nTmp);
			}
			fprintf(fptr, "\n");
		}
	}
	
}

void CLaserReaderDlg::InitCosineValue(double dValues[], int nLength)
{
	double dAnglePerPoint = 96.0 / 273; // 96 and 96.0, harsh reality!
	double PI = 3.14159265;

	for (int i = 0; i < 137; i++)
	{
		dValues[i] = cos((-48 + dAnglePerPoint * i) * PI / 180.0);
		dValues[273 - i] = dValues[i];
	}
}