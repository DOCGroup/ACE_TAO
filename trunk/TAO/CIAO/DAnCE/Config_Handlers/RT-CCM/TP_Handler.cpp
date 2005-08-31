// $Id$Exp lseibert

#include "TP_Handler.h"
#include "ciao/ServerResourcesC.h"
#include "CIAOServerResources.hpp"

namespace CIAO
{
    namespace Config_Handlers
    {
	
	bool TP_Handler::thread_pool (
	    const ThreadpoolDef &src,
	    ::CIAO::DAnCE::ORS_Threadpool &dest)
	{
          dest.Id = CORBA::String_var (src.id().c_str ());
	    
          dest.stacksize = src.stacksize();
	    
          dest.static_threads = src.static_threads();
	    
          dest.dynamic_threads = src.dynamic_threads();
          
          dest.default_priority = src.default_priority ();
                      
          dest.allow_request_buffering = src.allow_request_buffering();
	    
          dest.max_buffered_requests = src.max_buffered_requests ();
	    
          dest.max_request_buffer_size = src.max_request_buffered_size();
          
          return true;
	}

	ThreadpoolDef 
        TP_Handler::thread_pool (
	    const ::CIAO::DAnCE::ORS_Threadpool &src)
	{
          ThreadpoolDef tp  (XMLSchema::unsignedLong(src.stacksize),
                             XMLSchema::unsignedLong(src.static_threads),
                             XMLSchema::unsignedLong(src.dynamic_threads),
                             Priority (src.default_priority),
                             XMLSchema::boolean (src.allow_request_buffering),
                             XMLSchema::unsignedLong(src.max_buffered_requests),
                             XMLSchema::unsignedLong(src.max_request_buffer_size));
          
          if (src.Id.in () == 0)
            tp.id (src.Id.in ());          

          return tp;
	}
    }
}
