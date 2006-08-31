//ReceiverToolAdaptor.cpp

#include "stdafx.h"

#include "ReceiverToolAdaptor.h"


//AvailableDocument constructors
ReceiverToolAdaptor::AvailableDocument::AvailableDocument() 
{
}

ReceiverToolAdaptor::AvailableDocument::AvailableDocument(string paradigmName_, string paradigmVersion_, string name_, 
		string version_, string remarks_, long docId_, long keepAlive_)
		: paradigmName(paradigmName_),
		  paradigmVersion(paradigmVersion_),
		  name(name_),
		  version(version_),
		  remarks(remarks_),
		  docId(docId_),
		  keepAlive(keepAlive_) 
{
}



//Creates a specific tooladaptor
ReceiverToolAdaptor::ReceiverToolAdaptor(const char *type, int argc, char **argv) 
	: ToolAdaptor(type, argc, argv),
	  runExited(TRUE,TRUE)
{
};


ReceiverToolAdaptor::~ReceiverToolAdaptor()
{
	terminate();
};

void ReceiverToolAdaptor::notify(const char *par_name, const char *par_version, const char *name,
            const char *version, const char *remarks, long docId, long keepAlive) 
{ 
	//We offset the keepAlive with the time() so that we actually record the absolute instant at which they die
	AvailableDocument avl (par_name, par_version, name, version, remarks, docId, keepAlive+time(NULL));

	{
		CSingleLock lock(&alloc_sync, TRUE);
		_documents.push_back(avl);
	}
};


deque<ReceiverToolAdaptor::AvailableDocument> ReceiverToolAdaptor::getAvailableDocuments()
{
	deque<AvailableDocument> ret;

	{
		CSingleLock lock(&alloc_sync, TRUE);

		_documents.erase(remove_if(_documents.begin(), _documents.end(), expired), _documents.end());

		for (deque<AvailableDocument>::iterator iter = _documents.begin(); iter != _documents.end(); iter++)
		{
			ret.push_back(AvailableDocument(
					iter->paradigmName,
					iter->paradigmVersion,
					iter->name,
					iter->version,
					iter->remarks,
					iter->docId,
					iter->keepAlive-time(NULL)));
		};
	}

	return ret;
};

void ReceiverToolAdaptor::run()
{
	CSingleLock lock(&runExited);
	if (TRUE==lock.Lock(0)) AfxBeginThread(inner_run, (LPVOID) this); //Can be made better
}

void ReceiverToolAdaptor::terminate()
{
	ToolAdaptor::terminate();
	CSingleLock lock(&runExited, TRUE);
}

UINT ReceiverToolAdaptor::inner_run(LPVOID pThis)
{
	ReceiverToolAdaptor &ta = *(ReceiverToolAdaptor*) pThis;

	ta.runExited.ResetEvent();
	ta.ToolAdaptor::run();
	ta.runExited.SetEvent();

	return 1;
}

