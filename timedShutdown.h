/**
*/

#ifndef TIMEDSHUTDOWN_H
#define TIMEDSHUTDOWN_H

//Use unicode
#define UNICODE

//Import headers
#include <wx/wx.h>
#include <wx/aboutdlg.h>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <iostream>

//Using the standard namespace
using namespace std;

//Define the main class
class timedShutdown : public wxApp 
{
	//Public method and variable declarations
    public:
        virtual bool OnInit();
		
	//Private methods and variable declarations
	private:
		int processArguments();
}; //End the class definition

//Define the frame class
class tsFrame : public wxFrame
{
	//Public methods and variable declarations
	public:
		tsFrame();
	
	//Private method and variable declarations
	private:
		int seconds(int seconds);
		int minutesToSeconds(int minutes);
		int daysToSeconds(int days);
		int hoursToSeconds(int hours);
		int weeksToSeconds(int weeks);
		void shutdown(int numSeconds);
		void about(wxCommandEvent& event);
		void exitApp(wxCommandEvent &event);
		void initiateShutdown(wxCommandEvent &event);
		void abortShutdown(wxCommandEvent &event);
		void unitsChange(wxCommandEvent &event);
		bool checkInput();
		void adjustTokenPrivs();
		DECLARE_EVENT_TABLE()
		
		//Enumeration for the event ids
		enum
		{
			//Exit, about, initiate shutdown
			idEx = 1000,
			idUC,
			idTF,
			idSh,
			idSb,
			idAb
		};
};

//Begin the event table declaration
BEGIN_EVENT_TABLE(tsFrame, wxFrame) 
	EVT_MENU(idEx, tsFrame::exitApp)
	EVT_MENU(idSh, tsFrame::initiateShutdown)
	EVT_MENU(idAb, tsFrame::abortShutdown)
    EVT_BUTTON(idSb, tsFrame::initiateShutdown) 
    EVT_COMBOBOX(idUC, tsFrame::unitsChange)
	EVT_MENU(wxID_ABOUT, tsFrame::about)
	EVT_TEXT_ENTER(idTF, tsFrame::initiateShutdown)
END_EVENT_TABLE() //End the event table declaration

//Function that adjusts the token privileges of the current process
void adjustTokenPrivsGlobal()
{
	//Handle for the current process
	HANDLE currToken;

	//Token privs
	TOKEN_PRIVILEGES pPrivs; 
 
   //Get the token for the current process
   if(!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &currToken)) 
   {
		//Failed
		wxMessageBox(wxT("Error: Cannot get the handle for the current process"), wxT("Error"), wxOK | wxICON_ERROR, NULL);
   }
   //Else, keep going
   else
   {
		//Get the LUID for the shutdown privilege. 
		LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &pPrivs.Privileges[0].Luid); 

		//One privilege to set
		pPrivs.PrivilegeCount = 1;

		//Set the shutdown privilege
		pPrivs.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 

		//Get the priv for the process
		AdjustTokenPrivileges(currToken, FALSE, &pPrivs, 0, (PTOKEN_PRIVILEGES)NULL, 0);
	}
}

#endif