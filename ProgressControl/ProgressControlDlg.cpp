#include "ProgressControlDlg.h"

ProgressControlDlg* ProgressControlDlg::ptr = NULL;

ProgressControlDlg::ProgressControlDlg(void)
{
	ptr = this;
}

void ProgressControlDlg::Cls_OnClose(HWND hwnd)
{
	EndDialog(hwnd, 0);
}

BOOL ProgressControlDlg::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam) 
{
	srand(time(0));
	hDialog = hwnd;
	hProgress = GetDlgItem(hDialog, IDC_PROGRESS1);
	SendMessage(hProgress, PBM_SETRANGE, 0, MAKELPARAM(0, 60)); // Установка интервала для индикатора 
	SendMessage(hProgress, PBM_SETSTEP, 1, 0); // Установка шага приращения  индикатора 
	SendMessage(hProgress, PBM_SETPOS, 0, 0); // Установка текущей позиции индикатора
	SendMessage(hProgress, PBM_SETBKCOLOR, 0, LPARAM(RGB(0, 0, 255))); // Установка цвета фона индикатора
	SendMessage(hProgress, PBM_SETBARCOLOR, 0, LPARAM(RGB(255, 255, 0))); // Установка цвета заполняемых прямоугольников


	for (size_t i = 0; i < 16; i++)
	{
		hButtons[i] = GetDlgItem(hDialog, IDC_BUTTON2 + i);
	}
	hListBox = GetDlgItem(hDialog, IDC_LIST1);
	hNewGame = GetDlgItem(hDialog, IDC_BUTTON1);
	hEdit = GetDlgItem(hDialog, IDC_EDIT1);
	hSpin = GetDlgItem(hDialog, IDC_SPIN2);
	UDACCEL pAcceleration[3] = { { 1, 1 }, { 3, 100 }, { 5, 500 } };
	SendMessage(hSpin, UDM_SETACCEL, 3, LPARAM(pAcceleration));

	// Установим диапазон счётчика
	SendMessage(hSpin, UDM_SETRANGE32, 0, 1000);
	// Установим приятеля для счётчика
	SendMessage(hSpin, UDM_SETBUDDY, WPARAM(hEdit), 0);
	SetWindowText(hEdit, TEXT("0"));


	return TRUE;
}

void ProgressControlDlg::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	if (id == IDC_BUTTON1)
	{
		TCHAR str[100];
		int number;
		SetTimer(hwnd, 1, 100, NULL);
		int nCurrrentPosition = SendMessage(hSpin, UDM_GETPOS32, 0, 0);
		SendMessage(hProgress, PBM_SETRANGE, 0, MAKELPARAM(0, nCurrrentPosition));
		for (size_t i = 0; i < 16; i++)
		{
			number = rand() % 101;
			wsprintf(str, TEXT("%d"), number);
			SetWindowText(hButtons[i], str);
			EnableWindow(hButtons[i], TRUE);
		}
	}
	else{

	}
		
}

void ProgressControlDlg::Cls_OnTimer(HWND hwnd, UINT id)
{
	SendMessage(hProgress, PBM_STEPIT, 0, 0); // Изменение текущей позиции индикатора путём прибавления шага
}

BOOL CALLBACK ProgressControlDlg::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);
		HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInitDialog);
		HANDLE_MSG(hwnd, WM_COMMAND, ptr->Cls_OnCommand);
		HANDLE_MSG(hwnd, WM_TIMER, ptr->Cls_OnTimer);
	}
	return FALSE;
}