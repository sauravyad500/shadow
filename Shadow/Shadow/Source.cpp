#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <windowsx.h>
#include <Richedit.h>
#include <CommCtrl.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
wchar_t temp[15] = L"0";
HINSTANCE hInst;
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
	// Register the window class.
	const wchar_t CLASS_NAME[] = L"Sample Window Class";
	hInst = hInstance;
	WNDCLASS wc = {};

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	RegisterClass(&wc);

	// Create the window.

	HWND hwnd = CreateWindowEx(
		0,                              // Optional window styles.
		CLASS_NAME,                     // Window class
		L"Learn to Program Windows",    // Window text
		WS_OVERLAPPEDWINDOW,            // Window style
										// Size and position
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

		NULL,       // Parent window    
		NULL,       // Menu
		hInstance,  // Instance handle
		NULL        // Additional application data
	);

	if (hwnd == NULL)
	{
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);

	// Run the message loop.

	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

BOOL clicked = FALSE, in_sun = FALSE;
int gx, gy, sun_radius, sun_height, x, y = 256, block_height, sun_x = 250, sun_y = 150, w, fx, fy, mx, my;
static HWND SUN_HEIGHTEDIT, BLOCK_HEIGHTEDIT, CONTINUE,progress;
static LPWSTR p=L"";
HFONT h , hf , hfont;
int focusn=0;
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;

		case WM_CREATE:
			SUN_HEIGHTEDIT = CreateWindowEx(NULL, L"edit", L"Sun's Height", WS_VISIBLE | WS_CHILD | ES_NUMBER | WS_BORDER, 0, 0, 150, 30, hwnd, (HMENU)100, NULL, NULL);
			BLOCK_HEIGHTEDIT = CreateWindowEx(NULL, L"edit", L"Block's Height", WS_VISIBLE | WS_CHILD | ES_NUMBER | WS_BORDER, 0, 0, 150, 30, hwnd, (HMENU)101, NULL, NULL);
			CONTINUE = CreateWindowEx(NULL, L"button", L"Animate", WS_CHILD | WS_VISIBLE, 0, 0, 100, 35, hwnd, (HMENU)110, NULL, NULL);
			progress = CreateWindowEx(0 , PROGRESS_CLASS , L"progress" , WS_VISIBLE | WS_CHILD | PBS_MARQUEE | PBS_SMOOTH , 0 , 0 , 600 , 30 , hwnd , (HMENU)200 , hInst , NULL);
			SendMessage(progress, PBM_SETRANGE, 0, MAKELPARAM(0, 599));
			SendMessage(progress , PBM_SETPOS , 0 , 40);
			SendMessage(progress , PBM_STEPIT , 99 , 99);
			SendMessage(progress, PBM_SETSTEP, 10, CLR_DEFAULT);
			//SendMessage(SUN_HEIGHTEDIT , EM_SETBKGNDCOLOR , (WPARAM)0 , (LPARAM)RGB(255,0,0));
			/*SendMessage(SUN_HEIGHTEDIT,
			EM_SETPASSWORDCHAR, 
			(WPARAM) '*', 
			(LPARAM) 0);*/

			return 0;

		case WM_SIZE:
		{
			gx = LOWORD(lParam);
			gy = HIWORD(lParam);
			SetWindowPos(SUN_HEIGHTEDIT, NULL, gx / 4 - 75, 4 * gy / 7, NULL, NULL, SWP_NOSIZE);
			SetWindowPos(BLOCK_HEIGHTEDIT, NULL, 3 * gx / 4 - 75, 4 * gy / 7, NULL, NULL, SWP_NOSIZE);
			SetWindowPos(CONTINUE, NULL, gx / 2 - 50, 4 * gy / 7 + 50, NULL, NULL, SWP_NOSIZE);
			if (gx<1200 || gy<600)    sun_radius = (80 * (gx<gy ? gx : gy)) / 1000;
			sun_x = 2 * gx / 7;
			if (sun_height*gy / 500)  sun_y = sun_height*gy / 500;
			y = y*gy / 500;
			x = x*gx / 1200;
			w = (12 * gx) / 1000;
			InvalidateRect(hwnd, NULL, false);
			break;
		}

		case WM_COMMAND:
		{
			switch (LOWORD(wParam))
			{
				case 100:
					if (HIWORD(wParam) == EN_SETFOCUS){
						GetWindowText(SUN_HEIGHTEDIT, temp,13);
						if (lstrcmp(temp, L"Sun's Height")==0)  SetWindowText(SUN_HEIGHTEDIT, L"");
						GetWindowText(BLOCK_HEIGHTEDIT, temp,15);
						if (lstrcmp(temp, L"")==0)  SetWindowText(BLOCK_HEIGHTEDIT, L"Block's Height");
					}break;

				case 101:
					if (HIWORD(wParam) == EN_SETFOCUS){
						GetWindowText(BLOCK_HEIGHTEDIT, temp,15);
						if (lstrcmp(temp, L"Block's Height")==0)  SetWindowText(BLOCK_HEIGHTEDIT, L"");
						GetWindowText(SUN_HEIGHTEDIT, temp,13);
						if (lstrcmp(temp, L"")==0)  SetWindowText(SUN_HEIGHTEDIT, L"Sun's Height");
					}
					break;

				case 110:
					GetWindowText(SUN_HEIGHTEDIT, temp, 4);
					sun_y = sun_height = gy - _wtoi(temp);
					GetWindowText(BLOCK_HEIGHTEDIT, temp, 4);
					y = block_height = gy - _wtoi(temp);
					clicked = TRUE;
					SetWindowPos(SUN_HEIGHTEDIT, NULL, 0, 0, NULL, NULL, SWP_HIDEWINDOW);
					SetWindowPos(BLOCK_HEIGHTEDIT, NULL, 0, 0, NULL, NULL, SWP_HIDEWINDOW);
					SetWindowPos(CONTINUE, NULL, 0, 0, NULL, NULL, SWP_HIDEWINDOW);
					SetFocus(hwnd);
					InvalidateRect(hwnd, NULL, FALSE);
					break;
			}
			break;
		}

		case WM_KEYDOWN:
		{
			switch (wParam)
			{
				case VK_LEFT:  x -= 5;  InvalidateRect(hwnd, NULL, false);  break;

				case VK_DOWN: y += 2;  InvalidateRect(hwnd, NULL, false);  break;

				case VK_RIGHT: x += 5;  InvalidateRect(hwnd, NULL, false);  break;

				case VK_UP: y -= 2;  InvalidateRect(hwnd, NULL, false);  break;
			} break;
		}

		case WM_GETMINMAXINFO:
		{
			LPMINMAXINFO lp = (LPMINMAXINFO)lParam;
			lp->ptMinTrackSize.x = 300;
			lp->ptMinTrackSize.y = 300;
			break;
		}

		case WM_LBUTTONDOWN:
		{
			SendMessage(progress , PBM_STEPIT , 0,0);
			mx = GET_X_LPARAM(lParam);
			my = GET_Y_LPARAM(lParam);
			if ( !clicked ){
				if ( !(mx > gx / 4 - 75 && mx < gx / 4 - 75 + 150 && my>4 * gy / 7 && my < 4 * gy / 7 + 30)
					&& !(mx > 3 * gx / 4 - 75 && mx < 3 * gx / 4 - 75 + 150 && my>4 * gy / 7 && my < 4 * gy / 7 + 30)
					&& !(mx > gx / 2 - 50 && mx < gx / 2 - 50 + 100 && my>4 * gy / 7 + 50 && my < 4 * gy / 7 + 50 + 35) )
				{
					GetWindowText(SUN_HEIGHTEDIT, temp,13);
					if (lstrcmp(temp, L"")==0)  SetWindowText(SUN_HEIGHTEDIT, L"Sun's Height");
					GetWindowText(BLOCK_HEIGHTEDIT, temp,13);
					if (lstrcmp(temp, L"")==0)  SetWindowText(BLOCK_HEIGHTEDIT, L"Block's Height");
					SetFocus(hwnd);
				}
			}
			else{
				if ( mx > sun_x - sun_radius && mx<sun_x + sun_radius && my>sun_y - sun_radius && my < sun_y + sun_radius )
				{
					in_sun = TRUE;
					fx = mx , fy = my;
				}
			}
			break;
		}

		case WM_MOUSEMOVE:
		{
			mx = GET_X_LPARAM(lParam);
			my = GET_Y_LPARAM(lParam);
			if (in_sun)
			{
				sun_x += mx - fx;
				sun_y += my - fy;
				fx = mx, fy = my;
				InvalidateRect(hwnd, NULL, FALSE);
			}
			break;
		}

		case WM_LBUTTONUP:
		{
			mx = GET_X_LPARAM(lParam);
			my = GET_Y_LPARAM(lParam);
			in_sun = FALSE;
			break;
		}

		case WM_CTLCOLOREDIT:
		{
			if ((HWND)lParam == GetDlgItem(hwnd, 100))
			{			
				SetBkColor((HDC)wParam, RGB(200,200,2));
				SetTextColor((HDC)wParam, RGB(255, 0, 0));
				return (LRESULT)GetSysColorBrush(COLOR_WINDOW);
			}
			if ((HWND)lParam == GetDlgItem(hwnd, 101))
			{			
				SetBkColor((HDC)wParam, GetSysColor(COLOR_WINDOW));
				SetTextColor((HDC)wParam, RGB(255, 0, 0));
				return (LRESULT)GetSysColorBrush(COLOR_WINDOW);
			}
		}break;

		case WM_CTLCOLORBTN:
		{
			if ((HWND)lParam == GetDlgItem(hwnd, 110))
			{			
				SetBkColor((HDC)wParam, GetSysColor(COLOR_WINDOW));
				SetTextColor((HDC)wParam, RGB(255, 0, 0));
				return (LRESULT)GetSysColorBrush(COLOR_WINDOW);
			}
		}
		break;

		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);
			POINT p[2][4], t[4];
			if (clicked)
			{
				HDC hdcMem = CreateCompatibleDC(hdc);
				HBITMAP h = CreateCompatibleBitmap(hdc, gx, gy);
				HBITMAP bold=(HBITMAP)SelectObject(hdcMem, h);
				FillRect(hdcMem, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
				HPEN pen = CreatePen(PS_NULL, 0, RGB(0, 0, 0));
				SelectObject(hdcMem, pen);

				if (y>sun_y)
				{
					p[0][0].x = 250 + x, p[0][0].y = 250 + x<sun_x ? y : 20 + y - 1;
					p[0][1].x = 250 + x + ((250 + x - sun_x)*(gy - y)) / (y - sun_y), p[0][1].y = gy;
					p[0][2].x = 250 + x + (300 * gx) / 2000 - w + ((250 + x + (300 * gx) / 2000 - w - sun_x)*(gy - y)) / (y - sun_y), p[0][2].y = gy;
					p[0][3].x = 250 + x + (300 * gx) / 2000 - w - 1, p[0][3].y = 250 + x + (300 * gx) / 2000 - w<sun_x ? 20 + y - 1 : y;
					p[1][0].x = 250 + x + (300 * gx) / 2000 + w, p[1][0].y = 250 + x + (300 * gx) / 2000 + w>sun_x ? y + 20 - 1 : y;
					p[1][1].x = 250 + x + (300 * gx) / 2000 + w + ((250 + x + (300 * gx) / 2000 + w - sun_x)*(gy - y)) / (y - sun_y), p[1][1].y = gy;
					p[1][2].x = 250 + x + (300 * gx) / 1000 + ((250 + x + (300 * gx) / 1000 - sun_x)*(gy - y)) / (y - sun_y), p[1][2].y = gy;
					p[1][3].x = 250 + x + (300 * gx) / 1000 - 1, p[1][3].y = 250 + x + (300 * gx) / 1000>sun_x ? y : 20 + y - 1;
				}
				else if (y<sun_y)
				{
					p[0][0].x = 250 + x, p[0][0].y = 250 + x>sun_x ? y : y + 20 - 1;
					p[0][1].x = 250 + x + ((250 + x - sun_x)*y) / (sun_y - y), p[0][1].y = 0;
					p[0][2].x = 250 + x + (300 * gx) / 2000 - w + ((250 + x + (300 * gx) / 2000 - w - sun_x)*y) / (sun_y - y), p[0][2].y = 0;
					p[0][3].x = 250 + x + (300 * gx) / 2000 - w - 1, p[0][3].y = 250 + x + (300 * gx) / 2000 - w>sun_x ? y + 20 - 1 : y;
					p[1][0].x = 250 + x + (300 * gx) / 2000 + w, p[1][0].y = 250 + x + (300 * gx) / 2000 + w>sun_x ? y : 20 + y - 1;
					p[1][1].x = 250 + x + (300 * gx) / 2000 + w + ((250 + x + (300 * gx) / 2000 + w - sun_x)*y) / (sun_y - y), p[1][1].y = 0;
					p[1][2].x = 250 + x + (300 * gx) / 1000 + ((250 + x + (300 * gx) / 1000 - sun_x)*y) / (sun_y - y), p[1][2].y = 0;
					p[1][3].x = 250 + x + (300 * gx) / 1000 - 1, p[1][3].y = 250 + x + (300 * gx) / 1000>sun_x ? 20 + y - 1 : y;
				}
				else if (y == sun_y)
				{
					p[0][0].x = 250 + x + (300 * gx) / 2000 - w - 1, p[0][0].y = y;
					p[0][1].x = 250 + x + (300 * gx) / 2000 - w - 1, p[0][1].y = 20 + y - 1;
					p[0][2].x = 250 + x + (300 * gx) / 2000 + w + 1, p[0][2].y = 20 + y - 1;
					p[0][3].x = 250 + x + (300 * gx) / 2000 + w + 1, p[0][3].y = y;
					p[1][0].x = 250 + x + (300 * gx) / 1000 - 1, p[1][0].y = y;
					p[1][1].x = 250 + x + (300 * gx) / 1000 - 1, p[1][1].y = 20 + y - 1;
					p[1][2].x = gx, p[1][2].y = 20 + y - 1;
					p[1][3].x = gx, p[1][3].y = y;
					t[0].x = 250 + x, t[0].y = y;
					t[1].x = 250 + x, t[1].y = 20 + y - 1;
					t[2].x = 0, t[2].y = 20 + y - 1;
					t[3].x = 0, t[3].y = y;
				}

				HBRUSH b = CreateSolidBrush(RGB(100, 100, 100));
				SelectObject(hdcMem, b);
				Polygon(hdcMem, p[0], 4);
				Polygon(hdcMem, p[1], 4);
				if (y == sun_y)
				{
					Polygon(hdcMem, p[0], 4);
					if (250 + x<sun_x) Polygon(hdcMem, t, 4);
					if (250 + x + (300 * gx) / 1000>sun_x) Polygon(hdcMem, p[1], 4);
				}

				HBRUSH block = CreateSolidBrush(RGB(255, 100, 100));
				HBRUSH hold = (HBRUSH)SelectObject(hdcMem, block);
				Rectangle(hdcMem, 250 + x, y, 250 + x + (300 * gx) / 2000 - w, 20 + y);
				Rectangle(hdcMem, 250 + x + (300 * gx) / 2000 + w, y, 250 + x + (300 * gx) / 1000, 20 + y);

				HBRUSH sun_color = CreateSolidBrush(RGB(255, 255, 0));
				SelectObject(hdcMem, sun_color);
				Ellipse(hdcMem, sun_x - sun_radius, sun_y - sun_radius, sun_x + sun_radius, sun_y + sun_radius);

				BitBlt(hdc, 0, 0, gx, gy, hdcMem, 0, 0, SRCCOPY);
				SelectObject(hdc,bold);
				DeleteObject(bold);
				DeleteBitmap(h);
				DeleteDC(hdcMem);
			}
			else FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
			EndPaint(hwnd, &ps);
		}

		return 0;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}