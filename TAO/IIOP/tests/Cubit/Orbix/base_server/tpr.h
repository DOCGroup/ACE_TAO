#include "cubit.h"
#include "ace/Service_Config.h"
#include "ace/Thread_Manager.h"

//
// Create a thread filter to dispatch incoming calls
//
class tpr_filter : public CORBA::ThreadFilter {
	virtual int inRequestPreMarshal(CORBA::Request &, CORBA::Environment&);
};
