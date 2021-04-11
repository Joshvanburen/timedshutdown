/*
Josh Van Buren
Main file for timedshutdown
*/

//Include the header with function and class definitions
#include "timedShutdown.h"

//Macro to implement the app
IMPLEMENT_APP(timedShutdown)

//Using the standard namespace
using namespace std;

//Define the initialization method
bool timedShutdown::OnInit() 
{
	//Create the wxFrame
	tsFrame * content = new tsFrame();
	
	//Center the frame
	content->Centre();
		
	//Process command line arguments
	int mode = processArguments();
	
	//If gui mode, show
	if(mode == 0)
	{		
		//Show the frame
		content->Show(true);
	}
	//Else non-gui mode
	else
	{
		//Destroy the frame
		content->Destroy();
	}

	//Return
    return true;
}

//Process command line arguments
int timedShutdown::processArguments()
{
	//Mode, 0 = show GUI, 1 = no GUI
	int mode = 0;
	
	//Number of secs to wait unti shutdown
	int secs = 0;
	
	//Is there an error
	bool isError = false;
	
	//If no arguments, return
	if(argc <= 1)
		//Return 0, show GUI
		return 0;
	//Else, process arguments
	else
	{
		//For loop to loop through the arguments
		for(int i = 1; i< wxApp::argc; i++)
		{
			//Long for conversion
			long conversion;
			
			//Convert the arguments to a wxString
			wxString arg(wxApp::argv[i]);
			
			//Check for help command
			if((arg.Cmp(L"-?") == 0) || (arg.Cmp(wxT("-h")) == 0) || (arg.Cmp(L"-help") == 0))
			{
				//Create the usage statement
				wxString usageStatement(L"Usage: ");
				usageStatement.Append(wxApp::argv[0]);
				usageStatement.Append(L" [-?|-h|-help] [-s] [-a] [-sec seconds] [-hr] [-min minutes] [-day days] [-wk weeks]\n\n[-?|-h|-help]\t Displays help\n[-s]\tSilent mode\n[-a]\tAborts planned shutdown\n[-sec seconds]\tTime in seconds\n[-min minutes]\tTime in minutes\n[-hr hours]\tTime in hours\n[-day days]\tTime in days\n[-wk weeks]\tTime in weeks\n\nExample: ");
				usageStatement.Append(wxApp::argv[0]);
				usageStatement.Append(L" -s -hr 1 -min 5\n\nShuts down the computer in one hour and five minutes.\n\nNote: Specify silent mode with no time parameters will immediatly shut down the machine.");
				
				//Display usage statement
				wxMessageBox(usageStatement, L"Usage Information", wxOK | wxICON_INFORMATION, NULL);
				
				//Return
				return 1;
			}
			//Check for silent mode
			else if(arg.Cmp(L"-s") == 0)
			{
				//Set the mode to no gui
				mode = 1;
			}
			//Check for abort mode
			else if(arg.Cmp(L"-a") == 0)
			{
				//Adjust the token privileges for the process
				adjustTokenPrivsGlobal();
				
				//Attempt to shutdown
				unsigned long res = AbortSystemShutdown(NULL);
					
				//If not successful, process
				if(res == 0)
				{
					//Character pointer
					wchar_t * buf;
				
					//Formats the error message
					FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(), 0 ,
						(wchar_t *)&buf, 0, NULL);
					
					//Error string
					wxString errorString(buf);
					
					//Alert the user to the error
					wxMessageBox(buf, L"Error", wxOK | wxICON_ERROR, NULL);
				}
			}
			//Else if
			else if(arg.Cmp(L"-sec") == 0)
			{
				//If the argument is there to be processed
				if((i + 1) < wxApp::argc)
				{
					//Get the specified argument
					wxString arg2(wxApp::argv[i + 1]);
					
					//If the argument can be converted to a long
					if(arg2.ToLong(&conversion, 10))
					{
						//Add the converted number to the time
						secs = secs + conversion;
					}
					//Else, error
					else
					{
						//Error
						isError = true;
					}
				}
			}
			//Else if
			else if(arg.Cmp(L"-hr") == 0)
			{
				//If the argument is there to be processed
				if((i + 1) < wxApp::argc)
				{
					//Get the specified argument
					wxString arg2(wxApp::argv[i + 1]);

					//If the argument can be converted to a long
					if(arg2.ToLong(&conversion, 10))
					{
						//Add the converted number to the time
						secs = secs + (3600 * conversion);
					}
					//Else, error
					else
					{
						//Error
						isError = true;
					}
				}
			}
			//Else if
			else if(arg.Cmp(L"-min") == 0)
			{
				//If the argument is there to be processed
				if((i + 1) < wxApp::argc)
				{
					//Get the specified argument
					wxString arg2(wxApp::argv[i + 1]);
					
					//If the argument can be converted to a long
					if(arg2.ToLong(&conversion, 10))
					{
						//Add the converted number to the time
						secs = secs + (60 * conversion);
					}
					//Else, error
					else
					{
						//Error
						isError = true;
					}
				}
			}
			//Else if
			else if(arg.Cmp(L"-day") == 0)
			{
				//If the argument is there to be processed
				if((i + 1) < wxApp::argc)
				{
					//Get the specified argument
					wxString arg2(wxApp::argv[i + 1]);
					
					//If the argument can be converted to a long
					if(arg2.ToLong(&conversion, 10))
					{
						//Add the converted number to the time
						secs = secs + (86400 * conversion);
					}
					//Else, error
					else
					{
						//Error
						isError = true;
					}
				}
			}
			//Else if
			else if(arg.Cmp(L"-wk") == 0)
			{
				//If the argument is there to be processed
				if((i + 1) < wxApp::argc)
				{
					//Get the specified argument
					wxString arg2(wxApp::argv[i + 1]);
					
					//If the argument can be converted to a long
					if(arg2.ToLong(&conversion, 10))
					{
						//Add the converted number to the time
						secs = secs + (604800 * conversion);
					}
					//Else, error
					else
					{
						//Error
						isError = true;
					}
				}
			}
		}
	}
	
	//If there is an error display
	if(isError == true)
	{
		//Display error
		wxMessageBox(L"Error: One of the parameters is not an integer.", L"Parameter Error", wxICON_ERROR | wxOK);
	}
	//If no error
	else
	{
		//If silent mode 
		if(mode == 1)
		{
			//Adjust the token privs of the current process
			adjustTokenPrivsGlobal();
			
			//Message in the shutdown dialog
			wchar_t message[250];

			//Create the message
			swprintf(message, L"The computer will be shutdown in %d seconds.", secs);

			//Attempt to shutdown
			unsigned long res = InitiateSystemShutdown(NULL, message, secs, true, 
				false);
				
			//If not successful, process
			if(res == 0)
			{
				//Character pointer
				wchar_t * buf;
			
				//Formats the error message
				FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(), 0 ,
					(wchar_t *)&buf, 0, NULL);
				
				//Error string
				wxString errorString(buf);
				
				//Alert the user to the error
				wxMessageBox(buf, L"Error", wxOK | wxICON_ERROR, NULL);
			}
		}
	}
	
	//Return the mode
	return mode;
}

//Constructor for the main class
tsFrame::tsFrame() : wxFrame(NULL, wxID_ANY, L"Timed Shutdown", wxDefaultPosition, wxSize(450,120)) 
{
	//Create the menubar
	wxMenuBar * menuBar = new wxMenuBar();
	
	//Create the file menu, help menu, edit menu
	wxMenu * fileMenu = new wxMenu();
	wxMenu * helpMenu = new wxMenu();
	
	//Add to the menus
	fileMenu->Append(idSh, L"Initiate Timed &Shutdown\tCtrl+S"), wxT("Initiates the timed shutdown");
	fileMenu->Append(idAb, L"&Abort Timed Shutdown\tCtrl+A"), wxT("Aborts the timed shutdown");
	fileMenu->AppendSeparator();
	fileMenu->Append(idEx, L"E&xit\t Ctrl+F4"), wxT("Exits the program");
	
	//Append to the help menu
	helpMenu->Append(wxID_ABOUT, L"&About...\tF1"), wxT("Show about dialog");
	
	//Add the file menu to the menu bar
	menuBar->Append(fileMenu, L"&File");
	menuBar->Append(helpMenu, L"&Help");
	
	//Set the menubar
	SetMenuBar(menuBar);
	
	//Panel for the dialog
	wxPanel * tPanel = new wxPanel(this, wxID_ANY);
	
	//Initialize the combo box and text control
	wxTextCtrl * timeField = new wxTextCtrl(tPanel, idTF, "0", wxDefaultPosition, wxDefaultSize,  
		wxTE_RICH | wxTE_PROCESS_ENTER, wxDefaultValidator, wxTextCtrlNameStr);
		
	//Create the list of units
	wxArrayString unitsList;
	
	//Add to the list
	unitsList.Add("Seconds");
	unitsList.Add("Minutes");
	unitsList.Add("Hours");
	unitsList.Add("Days");
	unitsList.Add("Weeks");
		
	//Create the combo box
	wxComboBox * unitsCombo = new wxComboBox(tPanel, idUC,
		L"Seconds", wxDefaultPosition, wxDefaultSize, unitsList, wxCB_READONLY);
		
	//Create the button
	wxButton * shutDButton = new wxButton(tPanel, idSb, L"Initiate &Shutdown");
	
	//Create the static text
	wxStaticText * lText = new wxStaticText(tPanel, wxID_ANY, L"Amount of Time:", 
		wxDefaultPosition, wxDefaultSize, 0, L"staticText");
	
	//Create the sizers
	wxBoxSizer * hSizer = new wxBoxSizer(wxHORIZONTAL);
	
	//Set the sizer for the panel
	tPanel->SetSizer(hSizer);
	
	//Add the items to the panel
	hSizer->Add(lText, 1, wxALL | wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER, 5);
	hSizer->Add(timeField, 1, wxEXPAND | wxALL, 5);
	hSizer->Add(unitsCombo, 1, wxEXPAND | wxALL, 5);
	hSizer->Add(shutDButton, 1, wxEXPAND | wxALL, 5);
	
	//Create the status bar
    CreateStatusBar(1);
    SetStatusText(L"Timed Shutdown");
	
	//Set the icon
	SetIcon(wxIcon(L"id"));
	
	//Fit the sizer to the panel
	hSizer->Fit(tPanel);
	
	//Show the dialog
	//Show(true);
}

//Get the number of seconds
int tsFrame::seconds(int seconds)
{
	//Return the number of seconds
	return seconds;
}
	
//Convert the number of minutes to seconds
int tsFrame::minutesToSeconds(int minutes)
{
	//Return the equal number of seconds
	return (60 * minutes);
}

//Convert the number of days to seconds
int tsFrame::daysToSeconds(int days)
{
	//Return the equal number of seconds
	return (86400 * days);
}

//Convert the number of hours to seconds
int tsFrame::hoursToSeconds(int hours)
{
	//Return the equal number of seconds
	return (3600 * hours);
}

//Convert the number of days to seconds
int tsFrame::weeksToSeconds(int weeks)
{
	//Return the equal number of seconds
	return (604800 * weeks);
}

//Perform the shutdown
void tsFrame::shutdown(int numSeconds)
{
	//Attempt to adjust the process token privileges
	adjustTokenPrivs();
	
	//Find the text control
	wxTextCtrl * timeField = (wxTextCtrl * )FindWindow(idTF);
	
	//Get the value in the text control
	wxString tValue = timeField->GetValue();
	
	//Long value
	long lVal;
	
	//Attempt to convert to long
	tValue.ToLong(&lVal);
	
	//Caste to integer
	int iVal = (int)lVal;
	
	//Find the combo box
	wxComboBox * unitsCombo = (wxComboBox *)FindWindow(idUC);
	
	//Get the value in the combo box
	wxString selection = unitsCombo->GetValue();
	
	//Message in the shutdown dialog
	wchar_t message[250];
	
	//If seconds, just call shutdown with value
	if(selection.Cmp(L"Seconds") == 0)
	{
		//Create the message
		swprintf(message, L"The computer will be shutdown in %d seconds.", iVal);
	}
	//Else if, minutes
	else if(selection.Cmp(L"Minutes") == 0)
	{
		//Create the message
		swprintf(message, L"The computer will be shutdown in %d minutes.", iVal);
	}
	//Else if, hours
	else if(selection.Cmp(L"Hours") == 0)
	{
		//Create the message
		swprintf(message, L"The computer will be shutdown in %d hours.", iVal);
	}
	//Else if, days
	else if(selection.Cmp(L"Days") == 0)
	{
		//Create the message
		swprintf(message, L"The computer will be shutdown in %d days.", iVal);
	}
	//Else if, weeks
	else if(selection.Cmp(L"Weeks") == 0)
	{
		//Create the message
		swprintf(message, L"The computer will be shutdown in %d weeks.", iVal);
	}
	
	//Attempt to shutdown
	unsigned long res = InitiateSystemShutdown(NULL, message, numSeconds, true, 
		false);
		
	//If not successful, process
	if(res == 0)
	{
		//Character pointer
		wchar_t * buf;
	
		//Formats the error message
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(), 0 ,
			(wchar_t *)&buf, 0, NULL);
		
		//Error string
		wxString errorString(buf);
		
		//Alert the user to the error
		wxMessageBox(buf, L"Error", wxOK | wxICON_ERROR, this);
	}
}

//Method that displays the about dialog
void tsFrame::about(wxCommandEvent& event)
{
	//Creates the dialog and sets the properties
	wxAboutDialogInfo aboutInfo;
	aboutInfo.SetName(L"Timed Shutdown");
	aboutInfo.SetVersion(L"1.0");
	aboutInfo.SetDescription(_(L"Initiates a Timed Shutdown"));
	//aboutInfo.SetCopyright(L"(C) 2013");
	aboutInfo.SetWebSite(L"https://github.com/Joshvanburen");
	aboutInfo.AddDeveloper(L"Josh Van Buren");
	aboutInfo.SetIcon(wxIcon(L"id"));

	//Displays the box
	wxAboutBox(aboutInfo);
}

//Method that exits the application
void tsFrame::exitApp(wxCommandEvent &event)
{
	//Destroy and close the window
	this->Destroy();
}

//Method that calls the shutdown method
void tsFrame::initiateShutdown(wxCommandEvent &event)
{
	//Check the input to make sure that it is an integer
	if(checkInput() == true)
	{
		//Find the text control
		wxTextCtrl * timeField = (wxTextCtrl * )FindWindow(idTF);
	
		//Get the value in the text control
		wxString tValue = timeField->GetValue();
		
		//Long value
		long lVal;
		
		//Attempt to convert to long
		tValue.ToLong(&lVal);
		
		//Caste to integer
		int iVal = (int)lVal;
		
		//Find the combo box
		wxComboBox * unitsCombo = (wxComboBox *)FindWindow(idUC);
		
		//Get the value in the combo box
		wxString selection = unitsCombo->GetValue();
		
		//If seconds, just call shutdown with value
		if(selection.Cmp(L"Seconds") == 0)
		{
			//Calls the shutdown function
			shutdown(iVal);
		}
		//Else if, minutes
		else if(selection.Cmp(L"Minutes") == 0)
		{
			//Calls the shutdown function
			shutdown(minutesToSeconds(iVal));
		}
		//Else if, hours
		else if(selection.Cmp(L"Hours") == 0)
		{
			//Calls the shutdown function
			shutdown(hoursToSeconds(iVal));
		}
		//Else if, days
		else if(selection.Cmp(L"Days") == 0)
		{
			//Calls the shutdown function
			shutdown(daysToSeconds(iVal));
		}
		//Else if, weeks
		else if(selection.Cmp(L"Weeks") == 0)
		{
			//Calls the shutdown function
			shutdown(weeksToSeconds(iVal));
		}
	}
	//Else, input error, alert user
	else
	{
		//Incorrect input
		wxMessageBox(L"Error: Please Enter an Integer", L"Error: Incorrect Input", wxOK | wxICON_ERROR, this);
	}
}

//Method that calls the shutdown method
void tsFrame::abortShutdown(wxCommandEvent &event)
{
	//Attempt to adjust the process token privileges
	adjustTokenPrivs();
	
	//Attempt to shutdown
	unsigned long res = AbortSystemShutdown(NULL);
		
	//If not successful, process
	if(res == 0)
	{
		//Character pointer
		wchar_t * buf;
	
		//Formats the error message
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(), 0 ,
			(wchar_t *)&buf, 0, NULL);
		
		//Error string
		wxString errorString(buf);
		
		//Alert the user to the error
		wxMessageBox(buf, L"Error", wxOK | wxICON_ERROR, this);
	}
}

//Method that handles the drop down event change
void tsFrame::unitsChange(wxCommandEvent &event)
{
	//Nothing needed at this time
}

//Checks what the user has input in the text box
bool tsFrame::checkInput()
{
	//Find the text control
	wxTextCtrl * timeField = (wxTextCtrl * )FindWindow(idTF);
	
	//Get the value in the text control
	wxString tValue = timeField->GetValue();
	
	//Long value
	long lVal;
	
	//Attempt to convert to long
	if(tValue.ToLong(&lVal) == true)
		//Successful conversion, return true
		return true;
	//Else, cannot convert
	else
		//Return false
		return false;
}

//Attempts to get the privileges to shutdown the machine
void tsFrame::adjustTokenPrivs()
{
	//Handle for the current process
	HANDLE currToken;

	//Token privs
	TOKEN_PRIVILEGES pPrivs; 
 
   //Get the token for the current process
   if(!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &currToken)) 
   {
		//Failed
		wxMessageBox(L"Error: Cannot get the handle for the current process", L"Error", wxOK | wxICON_ERROR, this);
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

