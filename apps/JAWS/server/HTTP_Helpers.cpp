// HTTP_Helpers.cpp -- Helper utilities for both server and client

#include "JAWS/server/HTTP_Helpers.h"

// = Static initialization.
ACE_SYNCH_MUTEX HTTP_Status_Code::lock_;
int HTTP_Status_Code::instance_ = 0;
const char *HTTP_Status_Code::Reason[HTTP_Status_Code::MAX_STATUS_CODE + 1];

const char **
HTTP_Status_Code::instance (void)
{
  if (HTTP_Status_Code::instance_ == 0) 
    {
      ACE_Guard<ACE_SYNCH_MUTEX> g (lock_);
      
      if (HTTP_Status_Code::instance_ == 0) 
	{
	  for (size_t i = 0; i < HTTP_Status_Code::MAX_STATUS_CODE + 1; i++) 
	    {
	      switch (i) 
		{
		case STATUS_OK:
		  HTTP_Status_Code::Reason[i] = "OK"; break;
		case STATUS_CREATED:
		  HTTP_Status_Code::Reason[i] = "Created"; break;
		case STATUS_ACCEPTED:
		  HTTP_Status_Code::Reason[i] = "Accepted"; break;
		case STATUS_NO_CONTENT:
		  HTTP_Status_Code::Reason[i] = "No Content"; break;
		case STATUS_MOVED_PERMANENTLY:
		  HTTP_Status_Code::Reason[i] = "Moved Permanently"; break;
		case STATUS_MOVED_TEMPORARILY:
		  HTTP_Status_Code::Reason[i] = "Moved Temporarily"; break;
		case STATUS_NOT_MODIFIED:
		  HTTP_Status_Code::Reason[i] = "Not Modified"; break;
		case STATUS_BAD_REQUEST:
		  HTTP_Status_Code::Reason[i] = "Bad Request"; break;
		case STATUS_UNAUTHORIZED:
		  HTTP_Status_Code::Reason[i] = "Unauthorized"; break;
		case STATUS_FORBIDDEN:
		  HTTP_Status_Code::Reason[i] = "Forbidden"; break;
		case STATUS_NOT_FOUND:
		  HTTP_Status_Code::Reason[i] = "Not Found"; break;
		case STATUS_INTERNAL_SERVER_ERROR:
		  HTTP_Status_Code::Reason[i] = "Internal Server Error"; break;
		case STATUS_NOT_IMPLEMENTED:
		  HTTP_Status_Code::Reason[i] = "Not Implemented"; break;
		case STATUS_BAD_GATEWAY:
		  HTTP_Status_Code::Reason[i] = "Bad Gateway"; break;
		case STATUS_SERVICE_UNAVAILABLE:
		  HTTP_Status_Code::Reason[i] = "Service Unavailable"; break;
		default:
		  HTTP_Status_Code::Reason[i] = "Unknown";
		}
	    }

	  HTTP_Status_Code::instance_ = 1;
	}

      // GUARD released
    }

  return HTTP_Status_Code::Reason;
}
