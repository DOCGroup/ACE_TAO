// DialogBackplaneMonitor.cpp : implementation file
//

#include "stdafx.h"
#include "DialogBackplaneMonitor.h"
#include "DialogCompletionStatus.h"
#include "Utils.h"
#include <sstream>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// OTIF classes

namespace
{
	//Fake argc, argv
	int ac = 1;
	char *av[2]={"GME.exe",NULL};

	//Dummy function for the Udm::UdmDiagram
	void dummy() {}
}


/////////////////////////////////////////////////////////////////////////////
// CDialogBackplaneMonitor dialog

CDialogBackplaneMonitor::CDialogBackplaneMonitor(const string &paradigmname, 
												 const string &projectfullpath,
												 HWND & pushOutDialogHWND,
												 CWnd* pParent /*=NULL*/)
	: CDialog(CDialogBackplaneMonitor::IDD, pParent), 
	  _paradigmname(paradigmname), 
	  _projectfullpath(projectfullpath),
	  _pushOutDialogHWND(pushOutDialogHWND),
	  _ta((paradigmname+"_Receiver").c_str(), ac, av)
{
	//{{AFX_DATA_INIT(CDialogBackplaneMonitor)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT


	// --- Application code begins

	CDialogCompletionStatus dcs(NULL, "Setting-Up Notification Sink ToolAdaptor", 0, 30);

	dcs.Create(CDialogCompletionStatus::IDD);
	dcs.ShowWindow(SW_SHOW);

	dcs.UpdateState("Getting Working Paradigms from the backplane", 10);

	//Let's fetch the Diagram from the OTIF backplane. 
	map<long, UdmCORBA::PARADIGM_STRUCT> workingparadigms; _ta.getWorkingParadigms(workingparadigms);



	dcs.UpdateState("Filtering names matching this GME project's paradigm name", 2);

	{for (std::map<long, UdmCORBA::PARADIGM_STRUCT>::iterator iter = workingparadigms.begin(); iter != workingparadigms.end(); )
	{
		map<long, UdmCORBA::PARADIGM_STRUCT>::iterator olditer = iter++;
		if (olditer->second.name != _paradigmname) workingparadigms.erase(olditer);
	}}
	if (workingparadigms.empty()) throw exception("Current paradigm not found on OTIF backplane! Check the workflow please.");

	dcs.UpdateState("Searching for highest version number", 2);

	//We are looking for the latest version of our paradigm from the backplane.
	string maxversion_str; double maxversion_number = 0;
	{for (std::map<long, UdmCORBA::PARADIGM_STRUCT>::iterator iter = workingparadigms.begin(); iter != workingparadigms.end(); iter++)
	{
		if (maxversion_number < atof(iter->second.version.c_str()))
		{					
			maxversion_number = atof(iter->second.version.c_str());
			maxversion_str = iter->second.version;
		}
	}}

	_paradigm_version = maxversion_str;



	dcs.UpdateState("Creating UML ToolAdaptor", 4);
	UML_ToolAdaptor uml_ta(ac, av);

	dcs.UpdateState("Getting Uml::diagram datanetwork", 4);
	uml_ta.getDatanetwork(Uml::diagram);

	dcs.UpdateState((string()+"Fetching remote paradigm \""+_paradigmname+"\" version \""+_paradigm_version+"\" with the UML ToolAdaptor").c_str(), 110);
	_myUmlDiagram = uml_ta.remoteFetchMyParadigm(_paradigmname.c_str(), _paradigm_version.c_str());

	_myUdmDiagram.dgr=&_myUmlDiagram; _myUdmDiagram.init=dummy;

	dcs.UpdateState("Getting CORBADataNetwork from OTIF backplane", 2);
	_dn = _ta.getDatanetwork(_myUdmDiagram);
	


	_ta.subscribe(_myUdmDiagram, _paradigmname.c_str(), _paradigm_version.c_str());
	_ta.run(); //Runs in a separate thread
}

CDialogBackplaneMonitor::~CDialogBackplaneMonitor()
{
}

void CDialogBackplaneMonitor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogBackplaneMonitor)
	DDX_Control(pDX, IDC_LIST_DATANETWORKS, m_ListDataNetworks);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogBackplaneMonitor, CDialog)
	//{{AFX_MSG_MAP(CDialogBackplaneMonitor)
	ON_BN_CLICKED(IDC_BUTTON_GET_SELECTED_DOCUMENT, OnButtonGetSelectedDocument)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogBackplaneMonitor message handlers

BOOL CDialogBackplaneMonitor::OnInitDialog() 
{
	CDialog::OnInitDialog();

	_pushOutDialogHWND = m_hWnd; //At this point, m_hWnd is valid

	SetWindowText((string()+"Monitoring backplane for: paradigm \""+_paradigmname+"\" version \""+_paradigm_version+'"').c_str());

	//Let's take care of the list control now

	//Add columns to the list control
	m_ListDataNetworks.InsertColumn(0,"Name",	LVCFMT_LEFT, 120);
	m_ListDataNetworks.InsertColumn(1,"Version",LVCFMT_LEFT, 50);
	m_ListDataNetworks.InsertColumn(2,"Remarks",LVCFMT_LEFT, 235);
	m_ListDataNetworks.InsertColumn(3,"DocID",	LVCFMT_LEFT, 48);
	m_ListDataNetworks.InsertColumn(4,"KeepAlive",LVCFMT_LEFT,65);

	//Fullrow selection is nicer
	m_ListDataNetworks.SetExtendedStyle(m_ListDataNetworks.GetExtendedStyle() | LVS_EX_FULLROWSELECT);


	SetTimer(TIMERID_POLL_NOTIFIED, 500, NULL);


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogBackplaneMonitor::OnButtonGetSelectedDocument() 
{
	// TODO: Add your control notification handler code here



	POSITION pos = m_ListDataNetworks.GetFirstSelectedItemPosition();
	if (pos == NULL) 
	{
		MessageBox("No items are selected!"); 
		return;
	}

	int nItem = m_ListDataNetworks.GetNextSelectedItem(pos);
	char docid_cstr[20];
	char docname_cstr[20];
	m_ListDataNetworks.GetItemText(nItem, 3, docid_cstr, sizeof(docid_cstr));
	m_ListDataNetworks.GetItemText(nItem, 0, docname_cstr, sizeof(docname_cstr));

	int docid_int;
	string docid_str;
	stringstream ss;
	docid_int = atoi(docid_cstr);
	ss << docid_int; ss >> docid_str; //Reflect the number we got into the string to the user

	if (IDCANCEL == MessageBox((string()+"Document named = "+docname_cstr+" , ID = "+docid_str+" was selected.\r\n\
If you fetch this document it will overwrite your current project file!\r\n\
Please confirm that you intend to do so.").c_str(),
		"ATTENTION: your project file is going to be overwritten!",
		MB_OKCANCEL)) return;


	
	//We need to create another tooladaptor otherwise it does not appear to work correctly

	CDialogCompletionStatus dcs(NULL, "Fetching file", 0, 102);
	dcs.Create(CDialogCompletionStatus::IDD);
	dcs.ShowWindow(SW_SHOW);
	
	ReceiverToolAdaptor downloader_ta((_paradigmname+"_Receiver").c_str(), ac, av);

	dcs.UpdateState("Getting DataNetwork", 10);
	UdmCORBA::CORBADataNetwork * downloader_dn = downloader_ta.getDatanetwork(_myUdmDiagram);


	dcs.UpdateState("Getting RootObject meta", 10);
	Uml::Class rootobject = downloader_ta.getDocumentRootobjectMeta(docid_int, _myUmlDiagram);
	if (!rootobject) throw exception("During document fetching operation - could not find the meta of the rootobject of the document");

	if (rootobject == &Udm::_null) AfxMessageBox("rootobject e' null");
	if (downloader_dn==NULL) AfxMessageBox("Datanetwork is NULL!");

	dcs.UpdateState("Creating new datanetwork over current project file", 10);
	downloader_dn->CreateNew(_projectfullpath, _paradigmname.c_str(), rootobject, Udm::CHANGES_PERSIST_ALWAYS); //_projectfullpath.c_str()

	dcs.UpdateState("Fetching document", 40);
	downloader_dn->fetchDocument(docid_int);

	dcs.UpdateState("Saving and closing DataNetwork", 20);
	downloader_dn->CloseWithUpdate();



	//Since GME does not support reloading the project from disk, we have to go dirty:
	//we save the fetched file on the same project which was open, then we launch 
	//another GME on it and we TERMINATE the current GME (so that it does not prompt the user to save it)
	//There is no other way than terminating it because we cannot access any COM interfaces of GME from here
	//(because the component InvokeEx in facts has already returned, and here we are in another thread).

	dcs.UpdateState("Terminating messageloop for notification sink tooladapter", 5);
	_ta.terminate();

	dcs.UpdateState("Launching a new instance of GME on the newly downloaded project", 5);
	ShellExecute(NULL, NULL, _projectfullpath.c_str(), NULL, NULL, SW_SHOW); //We launch the new GME

	dcs.UpdateState("dcs.Terminating current process", 102);
	TerminateProcess(OpenProcess(PROCESS_TERMINATE, FALSE, GetCurrentProcessId()),0); //GME MUST NOT save the project
}



void CDialogBackplaneMonitor::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
	if (nIDEvent==TIMERID_POLL_NOTIFIED)
	{
		std::deque<ReceiverToolAdaptor::AvailableDocument> &documents = _ta.getAvailableDocuments();

		int n=1;
		std::deque<ReceiverToolAdaptor::AvailableDocument>::iterator iter = documents.begin();
		for ( ; iter != documents.end(); iter++, n++)
		{
			if (m_ListDataNetworks.GetItemCount() < n) m_ListDataNetworks.InsertItem(n-1,"dummy_string");

			
			std::stringstream sDocId; sDocId << iter->docId;
			std::stringstream sKeepAlive; sKeepAlive << iter->keepAlive;

			m_ListDataNetworks.SetItemText(n-1,0,iter->name.c_str());
			m_ListDataNetworks.SetItemText(n-1,1,iter->version.c_str());
			m_ListDataNetworks.SetItemText(n-1,2,iter->remarks.c_str());
			m_ListDataNetworks.SetItemText(n-1,3,sDocId.str().c_str());
			m_ListDataNetworks.SetItemText(n-1,4,sKeepAlive.str().c_str());
		}

		while ((n = m_ListDataNetworks.GetItemCount()) > documents.size()) m_ListDataNetworks.DeleteItem(n-1);

	}
	else CDialog::OnTimer(nIDEvent);
}
