/* -*- C++ -*- */
// @(#)Timer_Service.h	1.1	10/18/96

#include "ace/Service_Config.h"

class Timer_Service : public ACE_Service_Object
{
public:
  virtual int init (int argc, char *argv[]);
  
  virtual int handle_timeout (const ACE_Time_Value &,
			      const void *);
};

ACE_STATIC_SVC_DECLARE (Timer_Service)

