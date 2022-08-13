// MainDlg.cpp : implementation of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "aboutdlg.h"
#include "MainFrame.h"
#include "DriverHelper.h"
#include "SecurityHelper.h"
#include "ClipboardHelper.h"
#include "SecurityInformation.h"
#include "NewKeyDlg.h"
#include "RenameKeyCommand.h"
#include "ColorsSelectionDlg.h"
#include "ThemeSystem.h"


#define IDC_VIEW_PROCESS 0xDAD

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg) {
	if (m_pQuickFindDlg && m_pQuickFindDlg->IsDialogMessage(pMsg))
		return TRUE;

	if (CFrameWindowImpl<CMainFrame>::PreTranslateMessage(pMsg))
		return TRUE;
	return FALSE;
}

LRESULT CMainFrame::OnForwardToActiveView(WORD id, WORD code, HWND h, BOOL& /*bHandled*/) {
	auto hWnd = m_hwndArray[_index];
	if (::IsWindow(hWnd)) {
		auto msg = GetCurrentMessage();
		::SendMessage(hWnd, msg->message, msg->wParam, msg->lParam);
	}
	return 0;
}


BOOL CMainFrame::OnIdle() {
	UpdateUI();
	UIUpdateToolBar();
	UIUpdateStatusBar();

	return FALSE;
}

void CMainFrame::UpdateUI() {

	return;
}

void CMainFrame::InitProcessTable() {
	BarDesc bars[] = {
		{19,"进程名",0},
		{19,"进程ID",0},
		{9,"会话",0},
		{20,"用户名",0},
		{12,"优先级",0},
		{9,"线程数",0},
		{9,"句柄数",0},
		{19,"属性",0},
		{20,"创建时间",0},
		{30,"描述",0},
		{20,"文件厂商",0},
		{20,"文件版本",0},
		{56,"映像路径",0},
		{856,"命令行参数",0}
	};
	TableInfo table = {
		1,1,TABLE_SORTMENU | TABLE_COPYMENU | TABLE_APPMENU,9,0,0,0
	};

	BarInfo info;
	info.nbar = _countof(bars);
	info.font = 9;
	for (int i = 0; i < info.nbar; i++) {
		info.bar[i].defdx = bars[i].defdx;
		info.bar[i].mode = bars[i].mode;
		info.bar[i].name = bars[i].name;
	}

	m_pProcTable = new CProcessTable(info, table);
	m_hWndClient = m_pProcTable->Create(m_hWnd, rcDefault, nullptr, WS_CHILD | WS_VSCROLL | WS_HSCROLL | WS_BORDER | WS_EX_LAYERED| WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	m_pProcTable->ShowWindow(SW_SHOW);
	m_pProcTable->m_Images.Create(16, 16, ILC_COLOR32, 64, 32);
	m_pProcTable->Refresh();
	m_hwndArray[static_cast<int>(TabColumn::Process)] = m_pProcTable->m_hWnd;
}

void CMainFrame::InitNetworkTable() {
	BarDesc bars[] = {
		{20,"进程名",0},
		{20,"进程ID",0},
		{10,"协议",0},
		{15,"状态",0},
		{20,"本地地址",0},
		{10,"本地端口",0},
		{20,"远程地址",0},
		{10,"远程端口",0},
		{20,"创建时间",0},
		{30,"模块名",0},
		{256,"模块路径",0},
	};

	TableInfo table = {
		1,1,TABLE_SORTMENU | TABLE_COPYMENU | TABLE_APPMENU,9,0,0,0
	};

	BarInfo info;
	info.nbar = _countof(bars);
	info.font = 9;
	for (int i = 0; i < info.nbar; i++) {
		info.bar[i].defdx = bars[i].defdx;
		info.bar[i].mode = bars[i].mode;
		info.bar[i].name = bars[i].name;
	}

	m_pNetTable = new CNetwrokTable(info, table);
	m_pNetTable->Create(m_hWnd, rcDefault, nullptr, WS_CHILD | WS_VSCROLL | WS_HSCROLL | WS_BORDER | WS_EX_LAYERED);
	m_hwndArray[static_cast<int>(TabColumn::Network)] = m_pNetTable->m_hWnd;
}

void CMainFrame::InitKernelModuleTable() {
	BarDesc bars[] = {
		{20,"驱动文件名",0},
		{20,"映像基址",0},
		{10,"映像大小",0},
		{16,"加载顺序",0},
		{22,"文件厂商",0},
		{260,"全路径",0},
	};

	TableInfo table = {
		1,1,TABLE_SORTMENU | TABLE_COPYMENU | TABLE_APPMENU,9,0,0,0
	};

	BarInfo info;
	info.nbar = _countof(bars);
	info.font = 9;
	for (int i = 0; i < info.nbar; i++) {
		info.bar[i].defdx = bars[i].defdx;
		info.bar[i].mode = bars[i].mode;
		info.bar[i].name = bars[i].name;
	}

	m_pKernelModuleTable = new CKernelModuleTable(info, table);
	m_pKernelModuleTable->Create(m_hWnd, rcDefault, nullptr, WS_CHILD | WS_VSCROLL | WS_HSCROLL | WS_BORDER | WS_EX_LAYERED);
	m_hwndArray[static_cast<int>(TabColumn::KernelModule)] = m_pKernelModuleTable->m_hWnd;
}

void CMainFrame::InitDriverTable() {
	BarDesc bars[] = {
		{20,"驱动服务名",0},
		{20,"显示名称",0},
		{10,"状态",0},
		{20,"驱动类型",0},
		{20,"启动类型",0},
		{20,"描述",0},
		{260,"二进制路径",0},
	};

	TableInfo table = {
		1,1,TABLE_SORTMENU | TABLE_COPYMENU | TABLE_APPMENU,9,0,0,0
	};

	BarInfo info;
	info.nbar = _countof(bars);
	info.font = 9;
	for (int i = 0; i < info.nbar; i++) {
		info.bar[i].defdx = bars[i].defdx;
		info.bar[i].mode = bars[i].mode;
		info.bar[i].name = bars[i].name;
	}

	m_pDriverTable = new CDriverTable(info, table);
	m_pDriverTable->Create(m_hWnd, rcDefault, nullptr, WS_CHILD | WS_VSCROLL | WS_HSCROLL | WS_BORDER | WS_EX_LAYERED);
	m_hwndArray[static_cast<int>(TabColumn::Driver)] = m_pDriverTable->m_hWnd;
}

void CMainFrame::InitServiceTable() {
	BarDesc bars[] = {
		{20,"服务名",0},
		{20,"显示名称",0},
		{10,"状态",0},
		{20,"服务类型",0},
		{20,"进程ID"},
		{30,"进程名"},
		{20,"启动类型",0},
		{260,"二进制路径",0},
		{30,"账户名",0},
		{20,"错误控制",0},
		{50,"描述",0},
		{50,"服务权限",0},
		{20,"触发器",0},
		{20,"依赖",0},
		{50,"接受的控制",0},
		{30,"安全标识符",0},
		{20,"Security ID类型"}
	};

	TableInfo table = {
		1,1,TABLE_SORTMENU | TABLE_COPYMENU | TABLE_APPMENU,9,0,0,0
	};

	BarInfo info;
	info.nbar = _countof(bars);
	info.font = 9;
	for (int i = 0; i < info.nbar; i++) {
		info.bar[i].defdx = bars[i].defdx;
		info.bar[i].mode = bars[i].mode;
		info.bar[i].name = bars[i].name;
	}

	m_pServiceTable = new CServiceTable(info, table);
	m_pServiceTable->Create(m_hWnd, rcDefault, nullptr, WS_CHILD | WS_VSCROLL | WS_HSCROLL | WS_BORDER | WS_EX_LAYERED);
	m_hwndArray[static_cast<int>(TabColumn::Service)] = m_pServiceTable->m_hWnd;
}

void CMainFrame::InitDriverInterface() {
	// 定位驱动二进制文件，提取到系统目录，然后安装
	auto hRes = ::FindResource(nullptr, MAKEINTRESOURCE(IDR_DRIVER), L"BIN");

	if (!hRes)
		return;

	auto hGlobal = ::LoadResource(nullptr, hRes);
	if (!hGlobal)
		return;

	auto size = ::SizeofResource(nullptr, hRes);
	void* pBuffer = ::LockResource(hGlobal);

	if (!DriverHelper::IsDriverLoaded()) {
		if (!SecurityHelper::IsRunningElevated()) {
			if (AtlMessageBox(nullptr, L"Kernel Driver not loaded. Some functionality will not be available. Install?", IDS_TITLE, MB_YESNO | MB_ICONQUESTION) == IDYES) {
				if (!SecurityHelper::RunElevated(L"install", false)) {
					AtlMessageBox(*this, L"Error running driver installer", IDS_TITLE, MB_ICONERROR);
				}
			}
		}
		else {
			if (!DriverHelper::InstallDriver(false,pBuffer,size) || !DriverHelper::LoadDriver()) {
				MessageBox(L"Failed to install driver. Some functionality will not be available.", L"WinArk", MB_ICONERROR);
			}
		}
	}
	if (DriverHelper::IsDriverLoaded()) {
		if (DriverHelper::GetVersion() < DriverHelper::GetCurrentVersion()) {
			auto response = AtlMessageBox(nullptr, L"驱动更新（A newer driver is available with new functionality. Update?）",
				IDS_TITLE, MB_ICONQUESTION | MB_YESNO | MB_DEFBUTTON1);
			if (response == IDYES) {
				if (SecurityHelper::IsRunningElevated()) {
					if (!DriverHelper::UpdateDriver(pBuffer,size))
						AtlMessageBox(nullptr, L"Failed to update driver", IDS_TITLE, MB_ICONERROR);
				}
				else {
					DriverHelper::CloseDevice();
					SecurityHelper::RunElevated(L"update", false);
				}
			}
		}
	}

	::SetPriorityClass(::GetCurrentProcess(), HIGH_PRIORITY_CLASS);
	::SetThreadPriority(::GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
}

void CMainFrame::InitRegistryView() {
	// 注册表
	m_RegView.Create(m_hWnd, rcDefault, nullptr, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

	m_RegView.m_MainSplitter.ShowWindow(SW_SHOW);
	m_hwndArray[static_cast<int>(TabColumn::Registry)] = m_RegView.m_hWnd;
}

// 设备列表
void CMainFrame::InitDeviceView() {
	auto hWnd = m_DevView.Create(m_hWnd, rcDefault, nullptr, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	m_hwndArray[static_cast<int>(TabColumn::Device)] = m_DevView.m_hWnd;
}

// 窗口视图
void CMainFrame::InitWindowsView() {
	// WS_CLIPCHILDREN 子窗口区域父窗口不负责绘制,子窗口自行绘制
	// 不设置的话，父窗口绘制会遮挡住子窗口
	HWND hWnd = m_WinView.Create(m_hWnd, rcDefault, nullptr, WS_CHILD|WS_VISIBLE | WS_CLIPSIBLINGS| WS_CLIPCHILDREN);
	m_hwndArray[static_cast<int>(TabColumn::Windows)] = m_WinView.m_hWnd;
}

void CMainFrame::InitKernelHookView() {
	HWND hWnd = m_KernelHookView.Create(m_hWnd, rcDefault, nullptr, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	m_hwndArray[static_cast<int>(TabColumn::KernelHook)] = m_KernelHookView.m_hWnd;
}

void CMainFrame::InitKernelView() {
	HWND hWnd = m_KernelView.Create(m_hWnd, rcDefault, nullptr, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	m_hwndArray[static_cast<int>(TabColumn::Kernel)] = m_KernelView.m_hWnd;
}

void CMainFrame::InitConfigView() {
	HWND hWnd = m_SysConfigView.Create(m_hWnd, rcDefault);
	m_hwndArray[static_cast<int>(TabColumn::Config)] = m_SysConfigView.m_hWnd;
}




LRESULT CMainFrame::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
	CreateSimpleReBar(ATL_SIMPLE_REBAR_NOBORDER_STYLE);

	HWND hWndCmdBar = m_CmdBar.Create(m_hWnd, rcDefault, nullptr, ATL_SIMPLE_CMDBAR_PANE_STYLE);
	CMenuHandle hMenu = GetMenu();
	UIAddMenu(hMenu);
	/*m_CmdBar.AttachMenu(hMenu);
	m_CmdBar.m_bAlphaImages = true;
	SetMenu(nullptr);*/

	InitCommandBar();

	AddSimpleReBarBand(hWndCmdBar);

	/*auto submenu = menu.GetSubMenu(1);
	WCHAR text[64];
	menu.GetMenuString(1, text, _countof(text), MF_BYPOSITION);
	menu.RemoveMenu(1, MF_BYPOSITION);
	menu.InsertMenu(1, MF_BYPOSITION, submenu.m_hMenu, text);
	DrawMenuBar();*/
	
	// center the dialog on the screen
	CenterWindow();

	// set icons
	HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
	SetIcon(hIconSmall, FALSE);

	CreateSimpleStatusBar();
	m_StatusBar.SubclassWindow(m_hWndStatusBar);
	int parts[] = { 100,200,300,430,560,700,830,960,1100 };
	m_StatusBar.SetParts(_countof(parts), parts);
	m_StatusBar.ShowWindow(SW_SHOW);
	UIAddStatusBar(m_StatusBar);

	CTabCtrl tabCtrl;
	CRect r;
	GetClientRect(&r);
	r.bottom = 25;
	auto hTabCtrl = tabCtrl.Create(m_hWnd, &r, nullptr, WS_CHILDWINDOW | WS_VISIBLE | WS_CLIPSIBLINGS
		| TCS_HOTTRACK | TCS_SINGLELINE | TCS_RIGHTJUSTIFY | TCS_TABS,
		WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR | WS_EX_NOPARENTNOTIFY, TabId);
	m_TabCtrl.SubclassWindow(hTabCtrl);
	// 初始化选择夹
	struct {
		PCWSTR Name;
	}columns[] = {
		L"进程",
		L"内核模块",
		L"内核",
		L"内核钩子",
		L"网络",
		L"驱动",
		L"注册表",
		L"设备",
		L"窗口",
		L"服务",
		L"配置",
		L"事件追踪"
	};

	int i = 0;
	for (auto& col : columns) {
		m_TabCtrl.InsertItem(i++, col.Name);
	}
	HFONT hFont = (HFONT)::GetStockObject(SYSTEM_FIXED_FONT);
	m_TabCtrl.SetFont(hFont, true);
	::DeleteObject(hFont);
	AddSimpleReBarBand(m_TabCtrl);

	auto hWndToolBar = m_tb.Create(m_hWnd, nullptr, nullptr, ATL_SIMPLE_TOOLBAR_PANE_STYLE | TBSTYLE_LIST, 0, ATL_IDW_TOOLBAR);
	m_tb.SetExtendedStyle(TBSTYLE_EX_MIXEDBUTTONS);
	InitProcessToolBar(m_tb);

	AddSimpleReBarBand(hWndToolBar, nullptr, TRUE);
	UIAddToolBar(hWndToolBar);

	UIEnable(ID_MONITOR_PAUSE, FALSE);
	UIEnable(ID_MONITOR_STOP, FALSE);

	CReBarCtrl rb(m_hWndToolBar);
	rb.LockBands(true);


	SetWindowLong(GWL_EXSTYLE, ::GetWindowLong(m_hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	SetLayeredWindowAttributes(m_hWnd, 0xffffff, 220, LWA_ALPHA);
	
	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

	InitDriverInterface();
	InitProcessTable();
	InitNetworkTable();
	InitKernelModuleTable();
	InitDriverTable();
	InitServiceTable();

	InitRegistryView();
	InitDeviceView();
	InitWindowsView();
	InitKernelHookView();
	InitKernelView();
	InitConfigView();
	InitEtwView();

	UpdateLayout();
	UpdateUI();

	return TRUE;
}

LRESULT CMainFrame::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled) {
	// unregister message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);

	bHandled = false;
	return 0;
}

LRESULT CMainFrame::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	CAboutDlg dlg;
	dlg.DoModal();
	return 0;
}

void CMainFrame::CloseDialog(int nVal) {
	DestroyWindow();
	::PostQuitMessage(nVal);
}

LRESULT CMainFrame::OnTcnSelChange(int, LPNMHDR hdr, BOOL&) {
	int index = 0;

	index = m_TabCtrl.GetCurSel();
	for (auto hwnd : m_hwndArray) {
		if (::IsWindow(hwnd)) {
			::ShowWindow(hwnd, SW_HIDE);
		}
	}

	ClearToolBarButtons(m_tb);
	
	m_hWndClient = m_hwndArray[index];
	switch (static_cast<TabColumn>(index)) {
		case TabColumn::Process:
			InitProcessToolBar(m_tb);
			UIAddToolBar(m_tb.m_hWnd);
			m_pProcTable->ShowWindow(SW_SHOW);
			m_pProcTable->SetFocus();
			break;
		case TabColumn::Network:
			m_pNetTable->ShowWindow(SW_SHOW);
			m_pNetTable->SetFocus();
			break;
		case TabColumn::KernelModule:
			m_pKernelModuleTable->ShowWindow(SW_SHOW);
			m_pKernelModuleTable->SetFocus();
			break;
		case TabColumn::Driver:
			m_pDriverTable->ShowWindow(SW_SHOW);
			m_pDriverTable->SetFocus();
			break;
		case TabColumn::Registry:
			InitRegToolBar(m_tb);
			UIAddToolBar(m_tb.m_hWnd);
			m_RegView.ShowWindow(SW_SHOW);
			break;
		case TabColumn::Device:
			m_DevView.ShowWindow(SW_SHOW);
			break;
		case TabColumn::Windows:
			m_WinView.ShowWindow(SW_SHOW);
			break;
		case TabColumn::KernelHook:
			m_KernelHookView.ShowWindow(SW_SHOW);
			break;
		case TabColumn::Service:
			m_pServiceTable->ShowWindow(SW_SHOW);
			m_pServiceTable->SetFocus();
			break;
		case TabColumn::Kernel:
			m_KernelView.ShowWindow(SW_SHOW);
			break;
		case TabColumn::Config:
			m_SysConfigView.ShowWindow(SW_SHOW);
			break;
		case TabColumn::Etw:
			InitEtwToolBar(m_tb);
			UIAddToolBar(m_tb.m_hWnd);
			m_pEtwView->ShowWindow(SW_SHOW);
			break;
		default:
			break;
	}
	::PostMessage(m_hWnd, WM_SIZE, 0, 0);
	_index = index;
	return 0;
}

void CMainFrame::OnGetMinMaxInfo(LPMINMAXINFO lpMMI) {
	lpMMI->ptMinTrackSize.x = 550;
	lpMMI->ptMinTrackSize.y = 450;
}

bool CMainFrame::CanPaste() const {
	return false;
}

UINT CMainFrame::TrackPopupMenu(CMenuHandle menu, int x, int y) {
	return (UINT)m_CmdBar.TrackPopupMenu(menu, TPM_LEFTALIGN | TPM_RIGHTBUTTON, x, y);
}

void CMainFrame::SetStartKey(const CString& key) {
	// m_RegView.SetStartKey(key);
}

void CMainFrame::SetStatusText(PCWSTR text) {
	m_StatusBar.SetText(1, m_StatusText = text, SBT_NOBORDERS);
}

LRESULT CMainFrame::OnMonitorStart(WORD, WORD, HWND, BOOL&) {
	m_pEtwView->StartMonitoring(m_tm, true);
	bool isWin8Plus = ::IsWindows8OrGreater();
	if (isWin8Plus) {
		m_tm.Start([&](auto data) {
			m_pEtwView->AddEvent(data);
			});
	}
	else {
		DWORD flags = 0;
		for (auto type : m_tm.GetKernelEventTypes()) {
			flags |= (DWORD)type;
		}
		if (flags == 0)
			flags = (DWORD)KernelEventTypes::Process;
		m_tm.Start([&](auto data) {
			m_pEtwView->AddEvent(data);
			}, flags);
	}

	UIEnable(ID_MONITOR_STOP, TRUE);
	UIEnable(ID_MONITOR_START, FALSE);
	UIEnable(ID_MONITOR_PAUSE, TRUE);

	SetTimer(1, 5000, nullptr);
	SetPaneIcon(1, m_RunIcon);
	return 0;
}

LRESULT CMainFrame::OnMonitorStop(WORD, WORD, HWND, BOOL&) {
	KillTimer(1);
	m_tm.Stop();
	m_tm.Pause(false);

	m_pEtwView->StartMonitoring(m_tm, false);
	SetPaneIcon(1, m_StopIcon);

	UIEnable(ID_MONITOR_STOP, FALSE);
	UIEnable(ID_MONITOR_START, TRUE);
	UIEnable(ID_MONITOR_PAUSE, FALSE);
	UISetCheck(ID_MONITOR_PAUSE, FALSE);

	return 0;
}

LRESULT CMainFrame::OnMonitorPause(WORD, WORD, HWND, BOOL&) {
	m_tm.Pause(!m_tm.IsPaused());
	UISetCheck(ID_MONITOR_PAUSE, m_tm.IsPaused());
	int image = 2;
	HICON hIcon = m_PauseIcon;
	if (!m_tm.IsPaused()) {
		hIcon = m_tm.IsRunning() ? m_RunIcon : m_StopIcon;
	}
	SetPaneIcon(1, hIcon);
	return 0;
}

BOOL CMainFrame::TrackPopupMenu(HMENU hMenu, HWND hWnd, POINT* pt, UINT flags) {
	POINT cursorPos;
	if (pt == nullptr) {
		::GetCursorPos(&cursorPos);
		pt = &cursorPos;
	}
	return m_CmdBar.TrackPopupMenu(hMenu, flags, pt->x, pt->y);
}

HFONT CMainFrame::GetMonoFont() {
	return m_MonoFont;
}

void CMainFrame::ViewDestroyed(void* view) {
}

TraceManager& CMainFrame::GetTraceManager() {
	return m_tm;
}

BOOL CMainFrame::SetPaneText(int index, PCWSTR text) {
	return m_StatusBar.SetText(index, text);
}

BOOL CMainFrame::SetPaneIcon(int index, HICON hIcon) {
	return m_StatusBar.SetIcon(index, hIcon);
}

CUpdateUIBase* CMainFrame::GetUpdateUI() {
	return this;
}

LRESULT CMainFrame::OnTimer(UINT, WPARAM id, LPARAM, BOOL&) {
	if (id == 1 && m_pEtwView) {
		KillTimer(1);
		MEMORYSTATUSEX ms = { sizeof(ms) };
		::GlobalMemoryStatusEx(&ms);
		if (ms.dwMemoryLoad > 94 && m_pEtwView) {
			if (AtlMessageBox(*this, L"Physical memory is low. Continue monitoring?",
				IDS_TITLE, MB_OKCANCEL | MB_DEFBUTTON2 | MB_ICONWARNING | MB_SETFOREGROUND | MB_SYSTEMMODAL) == IDCANCEL) {
				PostMessage(WM_COMMAND, ID_MONITOR_STOP);
			}
		}
	}

	return 0;
}

void CMainFrame::InitEtwToolBar(CToolBarCtrl& tb, int size) {
	CImageList tbImages;
	tbImages.Create(size, size, ILC_COLOR32, 8, 4);
	tb.SetImageList(tbImages);

	struct {
		UINT id;
		int image;
		int style = BTNS_BUTTON;
	}buttons[] = {
		{ ID_MONITOR_START,IDI_PLAY},
		{ ID_MONITOR_PAUSE, IDI_PAUSE },
		{ ID_MONITOR_STOP, IDI_STOP },
		{ 0 },
		{ ID_VIEW_AUTOSCROLL, IDI_SCROLL },
		{ 0 },
		{ ID_MONITOR_CLEARALL, IDI_CANCEL },
		{ 0 },
		{ ID_CONFIGURE_EVENTS, IDI_TOOLS },
		{ ID_CONFIGURE_FILTERS, IDI_FILTER },
		{ 0 },
		{ ID_EVENT_CALLSTACK, IDI_STACK },
		{ ID_EVENT_PROPERTIES, IDI_PROPERTIES },
	};

	for (auto& b : buttons) {
		if (b.id == 0)
			tb.AddSeparator(0);
		else {
			int image = tbImages.AddIcon(AtlLoadIconImage(b.image, 0, size, size));
			BYTE fsState = TBSTATE_ENABLED;
			
			if (b.id == ID_MONITOR_START) {
				if (m_tm.IsRunning()) {
					fsState = TBSTATE_INDETERMINATE;
				}
			}
			else if (b.id == ID_MONITOR_PAUSE) {
				if (m_tm.IsPaused()||!m_tm.IsRunning()) {
					fsState = TBSTATE_INDETERMINATE;
				}
			}
			else if(b.id == ID_MONITOR_STOP) {
				if (!m_tm.IsRunning()) {
					fsState = TBSTATE_INDETERMINATE;
				}
			}
			tb.AddButton(b.id, b.style, fsState, image, nullptr, 0);
		}
	}
}

void CMainFrame::ClearToolBarButtons(CToolBarCtrl& tb)
{
	while (tb.DeleteButton(0));
}

void CMainFrame::InitEtwView() {
	m_RunIcon.LoadIconWithScaleDown(IDI_PLAY, 20, 20);
	m_StopIcon.LoadIconWithScaleDown(IDI_STOP, 20, 20);
	m_PauseIcon.LoadIconWithScaleDown(IDI_PAUSE, 20, 20);
	SetPaneIcon(1, m_StopIcon);
	UIEnable(ID_MONITOR_STOP, FALSE);
	UIEnable(ID_MONITOR_PAUSE, FALSE);

	m_MonoFont.CreatePointFont(100, L"Consolas");

	m_pEtwView = new CEtwView(this);
	m_pEtwView->Create(m_hWnd, rcDefault, nullptr, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0);
	m_hwndArray[static_cast<int>(TabColumn::Etw)] = m_pEtwView->m_hWnd;
}

void CMainFrame::InitProcessToolBar(CToolBarCtrl& tb) {
	CImageList tbImages;
	tbImages.Create(24, 24, ILC_COLOR32, 8, 4);
	tb.SetImageList(tbImages);

	struct {
		UINT id;
		int image;
		int style = BTNS_BUTTON;
		PCWSTR text = nullptr;
	}buttons[] = {
		{ID_PROCESS_REFRESH,IDI_REFRESH},
	};

	for (auto& b : buttons) {
		if (b.id == 0)
			tb.AddSeparator(0);
		else {
			int image = tbImages.AddIcon(AtlLoadIconImage(b.image, 0, 24, 24));
			tb.AddButton(b.id, b.style, TBSTATE_ENABLED, image, b.text, 0);
		}
	}
}

LRESULT CMainFrame::OnClose(UINT, WPARAM, LPARAM, BOOL&) {
	if (m_tm.IsRunning())
		m_tm.Stop();

	return DefWindowProc();
}

void CMainFrame::InitCommandBar() {
	struct {
		UINT id, icon;
	} cmds[] = {
		{ ID_MONITOR_START, IDI_PLAY },
		{ ID_MONITOR_STOP, IDI_STOP },
		{ ID_MONITOR_CLEARALL, IDI_CANCEL },
		{ ID_MONITOR_CONFIGUREEVENTS, IDI_TOOLS },
		{ ID_CONFIGURE_FILTERS, IDI_FILTER },
		{ ID_EVENT_CALLSTACK, IDI_STACK },
		{ ID_EVENT_PROPERTIES, IDI_PROPERTIES },
		{ ID_MONITOR_PAUSE, IDI_PAUSE },
		{ ID_VIEW_AUTOSCROLL, IDI_SCROLL },
		{ ID_SEARCH_QUICKFIND,IDI_FIND},
		{ID_SEARCH_FINDALL,IDI_SEARCH},
	};
	for (auto& cmd : cmds)
		m_CmdBar.AddIcon(AtlLoadIcon(cmd.icon), cmd.id);
}

void CMainFrame::DoFind(PCWSTR text, const QuickFindOptions& options) {
	m_pEtwView->SendMessage(WM_COMMAND, ID_SEARCH_FINDNEXT);
}

void CMainFrame::WindowClosed() {
	m_pQuickFindDlg = nullptr;
}

LRESULT CMainFrame::OnQuickFind(WORD, WORD, HWND, BOOL&) {
	if (!m_pQuickFindDlg) {
		m_pQuickFindDlg = new CQuickFindDlg(this);
		m_pQuickFindDlg->Create(*this);
		m_pQuickFindDlg->ShowWindow(SW_SHOW);
	}
	m_pQuickFindDlg->BringWindowToTop();
	m_pQuickFindDlg->SetFocus();

	return 0;
}

void CMainFrame::InitRegToolBar(CToolBarCtrl& tb, int size) {
	CImageList tbImages;
	tbImages.Create(size, size, ILC_COLOR32, 8, 4);
	tb.SetImageList(tbImages);

	const struct {
		UINT id;
		int image;
		BYTE style = BTNS_BUTTON;
		PCWSTR text = nullptr;
	}buttons[] = {
		{ ID_KEY_GOTO, IDI_GOTO },
	};

	for (auto& b : buttons) {
		if (b.id == 0)
			tb.AddSeparator(0);
		else {
			auto hIcon = AtlLoadIconImage(b.image, 0, size, size);
			ATLASSERT(hIcon);
			int image = tbImages.AddIcon(hIcon);
			tb.AddButton(b.id, b.style, TBSTATE_ENABLED, image, b.text, 0);
		}
	}
}

LRESULT CMainFrame::OnColors(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	CColorsSelectionDlg dlg;
	if (dlg.DoModal() == IDOK) {

	}
	return TRUE;
}

LRESULT CMainFrame::OnOptionsFont(WORD, WORD, HWND, BOOL&) {
	LOGFONT lf;
	CFontHandle(g_hAppFont).GetLogFont(lf);
	CFontDialog dlg(&lf);
	if (dlg.DoModal() == IDOK) {
		dlg.GetCurrentFont(&lf);
		g_hAppFont = CreateFontIndirect(&lf);
	}
	return 0;
}