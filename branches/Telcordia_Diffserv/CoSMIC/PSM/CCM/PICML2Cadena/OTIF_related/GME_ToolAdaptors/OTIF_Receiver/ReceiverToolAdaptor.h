//ReceiverToolAdaptor.h

#ifndef RECEIVER_TOOL_ADAPTOR_H
#define RECEIVER_TOOL_ADAPTOR_H

#include <string>
#include <deque>
#include <algorithm>
#include "UdmCorba.h"
#include <afxmt.h>

using std::string;
using std::deque;


class UML_ToolAdaptor : public UdmCORBA::ToolAdaptor 
{
public:

	UML_ToolAdaptor(int argc, char **argv) : ToolAdaptor(argc, argv) {};

	void notify(const char *paradigm_name, const char *paradigm_version,
		const char *name, const char *version, const char *remarks, long docId, long keepAlive) {};
};

class ReceiverToolAdaptor : public UdmCORBA::ToolAdaptor 
{
public:

	struct AvailableDocument;

    ReceiverToolAdaptor(const char *type, int argc, char **argv);
	~ReceiverToolAdaptor();
    void notify(const char *par_name, const char *par_version, const char *name,
                const char *version, const char *remarks, long docId, long keepAlive); //Implementation of UdmCORBA::ToolAdaptor virtual function
	deque<AvailableDocument> getAvailableDocuments(); //Call to get the available documents (member accessor)
	void run();  //Runs in a separate thread
	void terminate();  //Terminates the run on the worker thread and waits that the worker thread exits before returning


	struct AvailableDocument
	{
		AvailableDocument();
		AvailableDocument(string paradigmName_, string paradigmVersion_, string name_, 
			string version_, string remarks_, long docId_, long keepAlive_);		  

		string paradigmName;
		string paradigmVersion;
		string name;
		string version;
		string remarks;
		long docId;
		long keepAlive;
	};


protected:
	static bool expired(AvailableDocument &a) {return a.keepAlive < time(NULL);}
	static UINT inner_run(LPVOID pThis);

	deque<AvailableDocument> _documents;
	CEvent runExited;
	CCriticalSection alloc_sync;
};








#endif //RECEIVER_TOOL_ADAPTOR_H