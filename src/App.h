#ifndef APP_H
#define APP_H

#include <Application.h>
#include <Message.h>

#include "MainWindow.h"

#define M_QUIT_NOW 'qnow'

class App : public BApplication {
public:
	App(void);
	~App(void);

	virtual void AboutRequested();
	void MessageReceived(BMessage* msg);
	void ReadyToRun();

private:
	void ShowAlert(BString text);
	void ShowMainWindow(BPath path);

	MainWindow* fMainWindow;
};


#endif
