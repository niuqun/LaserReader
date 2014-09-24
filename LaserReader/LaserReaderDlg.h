
// LaserReaderDlg.h : header file
//

#pragma once

#include "SerialPort.h"
#include "afxwin.h"
// CLaserReaderDlg dialog
class CLaserReaderDlg : public CDialogEx
{
// Construction
public:
	CLaserReaderDlg(CWnd* pParent = NULL);	// standard constructor
	~CLaserReaderDlg();

// Dialog Data
	enum { IDD = IDD_LASERREADER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


protected:
	bool m_bOpen;

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	void InitCosineValue(double dValues[], int nLength);
	afx_msg void OnBnClickedOpenclose();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
private:
	CSerialPort SerialPort;
	HANDLE hPort;
	int m_nTimer;
	CEdit edit;
	CFileException fException;
	
	FILE* fptr;

	BYTE bTotalPackage[2500];
	int nPackageIndex; // stores current index of total data package
	CComboBox m_cbPort;
	CComboBox m_cbBaudRate;
	CString szPort[10];
	int nBaudRateArray[10];

	// for the number of complete packages received
	int nCompletePackage;

	// cosine value for each point
	double dCosine[274];
};
