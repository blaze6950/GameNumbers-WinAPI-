#include "TheGame.h"

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
	hSpin = GetDlgItem(hDialog, IDC_SPIN1);
	UDACCEL pAcceleration[3] = { { 1, 1 }, { 3, 10 }, { 5, 50 } };
	SendMessage(hSpin, UDM_SETACCEL, 3, LPARAM(pAcceleration));

	// Установим диапазон счётчика
	SendMessage(hSpin, UDM_SETRANGE32, 0, 300);
	// Установим приятеля для счётчика
	SendMessage(hSpin, UDM_SETBUDDY, WPARAM(hEdit), 0);
	SetWindowText(hEdit, TEXT("0"));


	return TRUE;
}

int compare(const void * x1, const void * x2)   // функция сравнения элементов массива
{
	return (*(int*)x1 - *(int*)x2);              // если результат вычитания равен 0, то числа равны, < 0: x1 < x2; > 0: x1 > x2
}

void ProgressControlDlg::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	static int numbers[16] = { 0 }, count = 0;
	static int nCurrrentPosition;
	int number = 0, buf = 0;
	TCHAR str[100];
	if (id == IDC_BUTTON1)
	{
		nCurrrentPosition = SendMessage(hSpin, UDM_GETPOS32, 0, 0);
		if (nCurrrentPosition == 0)
		{
			MessageBox(hwnd, TEXT("Вы не поставили время!"), TEXT("Ошибка!"), MB_OK);
		}
		else{
			SendMessage(hListBox, LB_RESETCONTENT, 0, 0);
			SendMessage(hProgress, PBM_SETPOS, 0, 0);
			SetTimer(hwnd, 1, 1000, NULL);
			SendMessage(hProgress, PBM_SETRANGE, 0, MAKELPARAM(0, nCurrrentPosition));
			for (size_t i = 0; i < 16; i++)
			{
				do
				{
					buf = number;
					number = rand() % 101;
				} while (buf == number);
				numbers[i] = number;
				wsprintf(str, TEXT("%d"), number);
				SetWindowText(hButtons[i], str);
				EnableWindow(hButtons[i], TRUE);
			}
			qsort(numbers, 16, sizeof(int), compare);      // сортируем массив чисел
		}
	}
	else if (id >= IDC_BUTTON2 && id <= IDC_BUTTON17){
		SendMessage(hwndCtl, WM_GETTEXT, 100, LPARAM(str));
		number = _wtoi(str);
		if (number == numbers[count])
		{
			SendMessage(hListBox, LB_ADDSTRING, 0, LPARAM(str));
			EnableWindow(hwndCtl, FALSE);
			count++;
			if (count == 16)
			{
				SendMessage(hListBox, LB_RESETCONTENT, 0, 0);
				SendMessage(hProgress, PBM_SETPOS, 0, 0);
				SendMessage(hProgress, PBM_SETRANGE, 0, MAKELPARAM(0, 0));
				KillTimer(hwnd, NULL);
				MessageBox(hwnd, TEXT("Вы выиграли!"), TEXT("Победа!"), MB_OK);
				for (size_t i = 0; i < 16; i++)
				{
					SetWindowText(hButtons[i], TEXT(""));
					EnableWindow(hButtons[i], FALSE);
				}
				return;
			}
		}
	}

}

void ProgressControlDlg::Cls_OnTimer(HWND hwnd, UINT id)
{
	SendMessage(hProgress, PBM_STEPIT, 0, 0); // Изменение текущей позиции индикатора путём прибавления шага
	if (SendMessage(hSpin, UDM_GETPOS32, 0, 0) == SendMessage(hProgress, PBM_GETPOS, 0, 0))
	{
		SendMessage(hListBox, LB_RESETCONTENT, 0, 0);
		SendMessage(hProgress, PBM_SETPOS, 0, 0);
		SendMessage(hProgress, PBM_SETRANGE, 0, MAKELPARAM(0, 0));
		KillTimer(hwnd, NULL);
		MessageBox(hwnd, TEXT("Вы проиграли!"), TEXT("Поражение!"), MB_OK);
		for (size_t i = 0; i < 16; i++)
		{
			SetWindowText(hButtons[i], TEXT(""));
			EnableWindow(hButtons[i], FALSE);
		}
		return;
	}
}

BOOL CALLBACK ProgressControlDlg::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);
		HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInitDialog);
		HANDLE_MSG(hwnd, WM_COMMAND, ptr->Cls_OnCommand);
		HANDLE_MSG(hwnd, WM_TIMER, ptr->Cls_OnTimer);
	}
	return FALSE;
}