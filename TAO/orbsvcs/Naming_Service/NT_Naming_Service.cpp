/* -*- C++ -*- */
// $Id$

#include /**/ "Naming_Service.h"
#include /**/ "NT_Naming_Service.h"

TAO_NT_Naming_Service::TAO_NT_Naming_Service (void)
  : argc_(0),
    argv_(0)
{
}

TAO_NT_Naming_Service::~TAO_NT_Naming_Service (void)
{
}

void
TAO_NT_Naming_Service::handle_control (DWORD control_code)
{
  if (control_code == SERVICE_CONTROL_SHUTDOWN
      || control_code == SERVICE_CONTROL_STOP)
    {
      report_status (SERVICE_STOP_PENDING);
      TAO_ORB_Core_instance ()->reactor ()->end_event_loop ();
      TAO_ORB_Core_instance ()->orb ()->shutdown (1);
      report_status (SERVICE_STOPPED);
    }
  else
    ACE_NT_Service::handle_control (control_code);
}

int
TAO_NT_Naming_Service::handle_exception (ACE_HANDLE)
{
  return 0;
}

int
TAO_NT_Naming_Service::init (int argc,
                             ASYS_TCHAR *argv[])
{
  argc_ = argc;
  argv_ = argv;

  return 0;
}

int
TAO_NT_Naming_Service::svc (void)
{
  TAO_Naming_Service naming_service;

  if (naming_service.init (argc_,
                           argv_) == -1)
    return -1;

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      report_status (SERVICE_RUNNING);
      naming_service.run (ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "TAO NT Naming Service");
      return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (1);

  return 0;
}

