/*#include <windows.h>  
#include <CommCtrl.h> 
#include <ctime>  
#include <cstdlib>  
using namespace std;

#pragma comment(lib, "comctl32.lib")
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' " \
"version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")  

class frame_window {  
private:
	LPWSTR window_class_name;  
	HINSTANCE instance_handle;  
	HCURSOR cursor_arrow;  
	HWND window_handle;
	HWND progress_handle_1;
	HWND progress_handle_2;
	HWND progress_handle_3;
	RECT client_rectangle;   
public:
	frame_window(LPWSTR window_class_identity) : window_class_name(window_class_identity) {  
		INITCOMMONCONTROLSEX common_controls; 
		common_controls.dwSize = sizeof(INITCOMMONCONTROLSEX); 
		common_controls.dwICC = ICC_PROGRESS_CLASS; 
		InitCommonControlsEx(&common_controls); 
		int screen_width = GetSystemMetrics(SM_CXFULLSCREEN);  
		int screen_height = GetSystemMetrics(SM_CYFULLSCREEN);  
		instance_handle = GetModuleHandle(NULL);  

		WNDCLASS window_class = { CS_OWNDC, main_window_proc, 0, 0,    
			instance_handle, NULL,    
			NULL, NULL, NULL,    
			window_class_name };   

		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++   
		// Create a standard frame window   
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++   

		RegisterClass(&window_class);   
		window_handle = CreateWindowEx(WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE,    
			window_class_name,    
			L"Progress Bar Example",   
			WS_OVERLAPPEDWINDOW | 
			WS_CLIPCHILDREN | WS_CLIPSIBLINGS,   
			100, 100, screen_width-200,    
			screen_height-200, NULL, NULL,   
			instance_handle, NULL);
		RECT client_rectangle;
		GetClientRect(window_handle, &client_rectangle);
		int width = client_rectangle.right - client_rectangle.left;
		int height = client_rectangle.bottom - client_rectangle.top;
		progress_handle_1 = CreateWindowEx(WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE,    
			PROGRESS_CLASS, NULL,   
			WS_CHILD | WS_VISIBLE,   
			(width-200)/2, ((height-10)/2)-30, 200,
			20, window_handle, NULL,   
			instance_handle, NULL);
		progress_handle_2 = CreateWindowEx(WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE,    
			PROGRESS_CLASS, NULL,   
			WS_CHILD | WS_VISIBLE|PBS_MARQUEE,   
			(width-200)/2, ((height-10)/2)+10, 200,
			20, window_handle, NULL,   
			instance_handle, NULL);
		SendMessage(progress_handle_1, PBM_SETRANGE, 0, (LPARAM) MAKELPARAM(0, 59));
		SendMessage(progress_handle_1, PBM_SETPOS, 0, 0);
		SendMessage(progress_handle_2, PBM_SETRANGE, 0, (LPARAM) MAKELPARAM(0, 599));
		SendMessage(progress_handle_2, PBM_SETPOS, 0, 0);
		SetTimer(window_handle, 0, 1000, NULL);
		SetTimer(window_handle, 1, 100, NULL);
		SetCursor(LoadCursor(NULL, IDC_ARROW)); 
		SetWindowLongPtr(window_handle, GWL_USERDATA, (LONG)this);  
		ShowWindow(window_handle, SW_SHOW);   
		UpdateWindow(window_handle); 
	}  
	~frame_window() {  
		UnregisterClass(window_class_name, instance_handle);   
	}  

	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++   
	// Windows message processing function   
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++   
	static LRESULT WINAPI main_window_proc(HWND window_handle, UINT message,    
		WPARAM wparam, LPARAM lparam) {  
		frame_window *This = (frame_window *)GetWindowLongPtr(window_handle, 
			GWL_USERDATA);  
		switch ( message ) {  
			case WM_PAINT:   
			{   
				PAINTSTRUCT paint_structure;   
				RECT client_rect;   
				HDC paint_device_context, paint_dc;   
				HBITMAP bitmap;   

				paint_device_context = BeginPaint(window_handle, &paint_structure);   
				paint_dc = CreateCompatibleDC(paint_device_context);   
				GetClientRect(window_handle, &client_rect);   
				int window_width = client_rect.right - client_rect.left;  
				int window_height = client_rect.bottom - client_rect.top;  
				bitmap = CreateBitmap(window_width, window_height, 1, 32, NULL);  
				HGDIOBJ old_bitmap = SelectObject(paint_dc, bitmap);  

				// Fill the client aread with the user selected face colour  
				HBRUSH light_brush = CreateSolidBrush(RGB(255, 255, 238));  
				FillRect(paint_dc, &paint_structure.rcPaint, light_brush);  
				DeleteObject(light_brush);  

				BitBlt(paint_device_context, 0, 0,    
					client_rect.right-client_rect.left,   
					client_rect.bottom-client_rect.top,   
					paint_dc, 0, 0, SRCCOPY);   
				SelectObject(paint_dc, old_bitmap);   
				DeleteObject(bitmap);   
				DeleteDC(paint_dc);   
				EndPaint(window_handle, &paint_structure);   
				return 0;   
			}   
			case WM_ERASEBKGND:   
			{   
				return TRUE;   
			}  
			case WM_SIZE:   
			{
				InvalidateRect(window_handle, NULL, TRUE);   
				return 0;   
			}
			case WM_TIMER:
			{
				if ( wparam == 0 ) {
					int pos = SendMessage(This->progress_handle_1, PBM_GETPOS, 0, 0 ) + 1;
					SendMessage(This->progress_handle_1, PBM_SETPOS, pos, 0 );
					if ( pos == 60 ) {
						SendMessage(This->progress_handle_1, PBM_SETPOS, 0, 0 );
					}
				}
				if ( wparam == 1 ) {
					int pos = SendMessage(This->progress_handle_2, PBM_GETPOS, 0, 0 ) + 1;
					SendMessage(This->progress_handle_2, PBM_SETPOS, pos, 0 );
					if ( pos == 600 ) {
						SendMessage(This->progress_handle_2, PBM_SETPOS, 0, 0 );
					}
				}
				return 0;
			}
			case WM_CLOSE:   
			{   
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				// The user wants to close the window   
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

				PostQuitMessage(0);   
				return 0;   
			}   
		}   
		return DefWindowProc(window_handle, message, wparam, lparam);   
	}   

	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++   
	// Windows message loop   
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	void run() {  
		MSG window_message;   
		while ( GetMessage(&window_message, NULL, 0, 0) ) {   
			TranslateMessage(&window_message);   
			DispatchMessage(&window_message);   
		}   
	}  
};  

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++   
// Windows main entry point   
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++   

int WINAPI wWinMain(HINSTANCE instance_handle, HINSTANCE, LPWSTR, INT) {  
	frame_window main_window(L"my base window");  
	main_window.run();  
	return 0;   
}
*/