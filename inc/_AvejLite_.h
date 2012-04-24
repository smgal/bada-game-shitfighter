#ifndef _AVEJLITE_H_
#define _AVEJLITE_H_


#include <FBase.h>
#include <FGraphics.h>
#include <FApp.h>
#include <FSystem.h>
#include <FUi.h>


class AvejLiteApp:
	public Osp::App::Application,
	public Osp::System::IScreenEventListener,
	public Osp::Base::Runtime::ITimerEventListener,
	public Osp::Ui::IActionEventListener
{
public:
	static Osp::App::Application* CreateInstance(void);


public:
	AvejLiteApp();
	~AvejLiteApp();


public:
	bool OnAppInitializing(Osp::App::AppRegistry& appRegistry);
	bool OnAppTerminating(Osp::App::AppRegistry& appRegistry, bool forcedTermination = false);
	void OnForeground(void);
	void OnBackground(void);
	void OnLowMemory(void);
	void OnBatteryLevelChanged(Osp::System::BatteryLevel batteryLevel);
	void OnScreenOn (void);
	void OnScreenOff (void);

public:
	void OnTimerExpired(Osp::Base::Runtime::Timer& timer);
	virtual void OnActionPerformed(const Osp::Ui::Control& source, int actionId);

private:
	Osp::Base::Runtime::Timer* __pTimer;
	Osp::Ui::Controls::Form*   __pForm;
};

#endif
