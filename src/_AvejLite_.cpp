
#include "_AvejLite_.h"
#include <math.h>

////////////////////////////////////////////////////////////////////////////////
// Game callback

void* g_hNativeWindow      = 0;
int   g_nativeWindowWidth  = 0;
int   g_nativeWindowHeight = 0;

extern bool g_Game_Initialize(void);
extern void g_Game_Finalize(void);
extern bool g_Game_Process(void);

namespace avej_launcher
{
	bool Initialize(int app_id, void* handle, int screen_width, int screen_height)
	{
		g_hNativeWindow      = handle;
		g_nativeWindowWidth  = screen_width;
		g_nativeWindowHeight = screen_height;

		g_Game_Initialize();
	}

	void Finalize(void)
	{
		g_Game_Finalize();
	}

	bool Process(void)
	{
		return g_Game_Process();
	}
}

////////////////////////////////////////////////////////////////////////////////
//

using namespace Osp::Base;
using namespace Osp::Base::Runtime;
using namespace Osp::Graphics;
using namespace Osp::Locales;
using namespace Osp::System;
using namespace Osp::App;
using namespace Osp::System;
using namespace Osp::Ui;
using namespace Osp::Ui::Controls;

#define TIME_OUT  10
#define USE_LANDSCAPE_MODE

////////////////////////////////////////////////////////////////////////////////
// Form

class AvejLiteForm:
	public Osp::Ui::Controls::Form
{
public:
	AvejLiteForm(AvejLiteApp* pApp)
	: __pApp(pApp)
	{
	}
	virtual ~AvejLiteForm(void)
	{
	}

public:
	virtual result OnDraw(void)
	{
		if (__pApp)
		{
//			__pApp->Draw();
		}
		return E_SUCCESS;
	}

private:
	AvejLiteApp* __pApp;
};

////////////////////////////////////////////////////////////////////////////////
// AvejLiteApp

AvejLiteApp::AvejLiteApp()
: __pTimer(null), __pForm(null)
{
}

AvejLiteApp::~AvejLiteApp()
{
}


Application*
AvejLiteApp::CreateInstance(void)
{
	return new AvejLiteApp();
}

#define CHECK_COND(cond) if ((cond) == 0) throw 1;
#define CHECK_RESULT(r)  if (IsFailed(r)) throw 2;

bool
AvejLiteApp::OnAppInitializing(AppRegistry& appRegistry)
{
	Timer* tempTimer = null;
	result r = E_SUCCESS;

	try
	{
		__pForm = new AvejLiteForm(this);
		CHECK_COND(__pForm);

		r = __pForm->Construct(FORM_STYLE_NORMAL);
		CHECK_RESULT(r);

		r = GetAppFrame()->GetFrame()->AddControl(*__pForm);
		CHECK_RESULT(r);

#if defined(USE_LANDSCAPE_MODE)
		__pForm->SetOrientation(ORIENTATION_LANDSCAPE);
#endif

		tempTimer = new Timer;
		CHECK_COND(tempTimer);

		r = tempTimer->Construct(*this);
		CHECK_RESULT(r);

		__pTimer  = tempTimer;

		avej_launcher::Initialize(0, (void*)__pForm, 800, 480);

		return true;
	}
	catch (...)
	{
		delete __pForm;
		__pForm = null;

		delete tempTimer;
		avej_launcher::Finalize();

		AppLog("[SMGAL] Initialization fails");

		return false;
	}
}

bool
AvejLiteApp::OnAppTerminating(AppRegistry& appRegistry, bool forcedTermination)
{
	return true;
}


void
AvejLiteApp::OnForeground(void)
{
	if (__pTimer)
	{
		__pTimer->Start(TIME_OUT);
	}
}


void
AvejLiteApp::OnBackground(void)
{
	if (__pTimer)
	{
		__pTimer->Cancel();
	}
}


void
AvejLiteApp::OnLowMemory(void)
{
}


void
AvejLiteApp::OnBatteryLevelChanged(BatteryLevel batteryLevel)
{
}


void
AvejLiteApp::OnTimerExpired(Timer& timer)
{
	if (__pTimer)
	{
		__pTimer->Start(TIME_OUT);

		if (!avej_launcher::Process())
		{
			__pTimer->Cancel();
			avej_launcher::Finalize();

			this->Terminate();
		}
	}
}

void
AvejLiteApp::OnScreenOn(void)
{
}

void
AvejLiteApp::OnScreenOff(void)
{
}

void
AvejLiteApp::OnActionPerformed(const Osp::Ui::Control& source, int actionId)
{
}
