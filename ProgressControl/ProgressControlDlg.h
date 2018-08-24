#pragma once
#include "header.h"

class ProgressControlDlg
{
public:
	ProgressControlDlg(void);
public:
	static BOOL CALLBACK DlgProc(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp);
	static ProgressControlDlg* ptr;
	BOOL Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
	void Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
	void Cls_OnTimer(HWND hwnd, UINT id);
	void Cls_OnClose(HWND hwnd);
	HWND hDialog, hProgress, hButtons[16], hListBox, hNewGame, hEdit, hSpin;
};
