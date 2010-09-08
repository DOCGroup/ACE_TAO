// -*- C++ -*-
//=============================================================================
/**
 *  @file    Service_Config_DLL.cpp
 *
 *  $Id$
 *
 *  This file is related to, and used with, Service_Config_Test. It's
 *  used when testing the reentrance/thread-safety of the
 *  Service Configurator, in addition to testing the Service
 *  Configurator's ability to handle nested processing of Service
 *  Configurator directives.
 *
 *  @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================

#include "Service_Config_DLL.h"
#include "ace/Service_Config.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"

static ACE_THR_FUNC_RETURN
invoke_service_config (void *arg)
{
  const ACE_TCHAR *directive = reinterpret_cast<const ACE_TCHAR *> (arg);


  // Process a Service Configurator directive in the current thread.
  if (ACE_Service_Config::process_directive (directive) != 0)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P|%t) Service_Config_DLL::svc() - ")
                ACE_TEXT ("process_directive() failed for:\n")
                ACE_TEXT ("\"%s\"\n"),
                directive));

  return 0;
}

Service_Config_DLL::Service_Config_DLL (void)
{
  ACE_OS::memset (this->directive_[0], 0, BUFSIZ * sizeof (ACE_TCHAR));
  ACE_OS::memset (this->directive_[1], 0, BUFSIZ * sizeof (ACE_TCHAR));
}

int
Service_Config_DLL::init (int argc, ACE_TCHAR *argv[])
{
  if (argc == 2)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Loading Test_Object_%s and Test_Object_%s\n"),
                  argv[0],
                  argv[1]));

      ACE_OS::sprintf (this->directive_[0],
#if (ACE_USES_CLASSIC_SVC_CONF == 1)
                       ACE_TEXT ("dynamic Test_Object_%s Service_Object * Service_Config_DLL:_make_Service_Config_DLL() \"Test_Object_%s\""),
#else
                       ACE_TEXT ("<?xml version='1.0'?> <dynamic id='Test_Object_%s' type='service_object'> <initializer init='_make_Service_Config_DLL' path='Service_Config_DLL' params='Test_Object_%s'/> </dynamic>"),
#endif
                       argv[0],
                       argv[0]);

      ACE_OS::sprintf (this->directive_[1],
#if (ACE_USES_CLASSIC_SVC_CONF == 1)
                       ACE_TEXT ("dynamic Test_Object_%s Service_Object * Service_Config_DLL:_make_Service_Config_DLL() \"Test_Object_%s\""),
#else
                       ACE_TEXT ("<?xml version='1.0'?> <dynamic id='Test_Object_%s' type='service_object'> <initializer init='_make_Service_Config_DLL' path='Service_Config_DLL' params='Test_Object_%s'/> </dynamic>"),
#endif

                       argv[1],
                       argv[1]);

      if (ACE_Service_Config::process_directive (this->directive_[0]) != 0)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("(%P|%t) Service_Config_DLL::init() - ")
                    ACE_TEXT ("process_directive() failed for:\n")
                    ACE_TEXT ("\"%s\": %m\n"),
                    this->directive_[0]));

#if defined (ACE_HAS_THREADS)

      // Become an Active Object if more than one argument passed.
      // Two arguments indicate two "test objects" to be dynamically
      // loaded.
      return this->activate ();

#endif  /* ACE_HAS_THREADS */

    }
  else if (argc == 1)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Service_Config_DLL::init () %@ - %s\n"),
                  this,
                  argv[0]));
    }
  else
   {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%P|%t) Incorrect number of arguments ")
                         ACE_TEXT ("(%d) passed to Service_Config_DLL::init ()"),
                         argc),
                        -1);
    }

  return 0;
}

int
Service_Config_DLL::fini (void)
{
  return 0;
}

int
Service_Config_DLL::svc (void)
{
  if (ACE_Thread_Manager::instance ()->spawn (invoke_service_config,
                                              this->directive_[1]) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("(%P|%t) Unable to spawn thread to ")
                       ACE_TEXT ("invoke Service Configurator.\n")),
                      -1);

  return 0;
}

// The same class (Service_Config_DLL) is used to implement each of the
// Service Objects whose service descriptors are defined below.

// -----------------------------------------------------------------

ACE_STATIC_SVC_DEFINE (Test_Object_1,
                       ACE_TEXT ("Test_Object_1"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (Service_Config_DLL),
                       ACE_Service_Type::DELETE_THIS
                       | ACE_Service_Type::DELETE_OBJ,
                       0)

// -----------------------------------------------------------------

ACE_STATIC_SVC_DEFINE (Test_Object_2,
                       ACE_TEXT ("Test_Object_2"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (Service_Config_DLL),
                       ACE_Service_Type::DELETE_THIS
                       | ACE_Service_Type::DELETE_OBJ,
                       0)

// -----------------------------------------------------------------

ACE_STATIC_SVC_DEFINE (Test_Object_3,
                       ACE_TEXT ("Test_Object_3"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (Service_Config_DLL),
                       ACE_Service_Type::DELETE_THIS
                       | ACE_Service_Type::DELETE_OBJ,
                       0)

// -----------------------------------------------------------------

ACE_STATIC_SVC_DEFINE (Test_Object_4,
                       ACE_TEXT ("Test_Object_4"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (Service_Config_DLL),
                       ACE_Service_Type::DELETE_THIS
                       | ACE_Service_Type::DELETE_OBJ,
                       0)

// -----------------------------------------------------------------

ACE_STATIC_SVC_DEFINE (Test_Object_5,
                       ACE_TEXT ("Test_Object_5"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (Service_Config_DLL),
                       ACE_Service_Type::DELETE_THIS
                       | ACE_Service_Type::DELETE_OBJ,
                       0)

// -----------------------------------------------------------------

ACE_STATIC_SVC_DEFINE (Test_Object_6,
                       ACE_TEXT ("Test_Object_6"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (Service_Config_DLL),
                       ACE_Service_Type::DELETE_THIS
                       | ACE_Service_Type::DELETE_OBJ,
                       0)

// -----------------------------------------------------------------

ACE_STATIC_SVC_DEFINE (Final_Object,
                       ACE_TEXT ("Final_Object"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (Service_Config_DLL),
                       ACE_Service_Type::DELETE_THIS
                       | ACE_Service_Type::DELETE_OBJ,
                       0)

// -----------------------------------------------------------------

ACE_STATIC_SVC_DEFINE (Test_Object_1_More,
                       ACE_TEXT ("Test_Object_1_More"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (Service_Config_DLL),
                       ACE_Service_Type::DELETE_THIS
                       | ACE_Service_Type::DELETE_OBJ,
                       0)

// -----------------------------------------------------------------

ACE_STATIC_SVC_DEFINE (Test_Object_2_More,
                       ACE_TEXT ("Test_Object_2_More"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (Service_Config_DLL),
                       ACE_Service_Type::DELETE_THIS
                       | ACE_Service_Type::DELETE_OBJ,
                       0)


// -----------------------------------------------------------------

// Same factory is used for all service descriptors defined above.
ACE_FACTORY_DEFINE (Service_Config_DLL, Service_Config_DLL)

/*
** Simple service which will refuse to load/initialize correctly.
** The main program should do:
**  1. Try to load this service (which should fail)
**  2. Try to look up this service and call its info() hook; if info()
**     can be called, the test has failed.
** Similarly, if fini() is called later, something is very wrong.
*/

/// Initializes object when dynamic linking occurs.
int
Refuses_Init::init (int, ACE_TCHAR *[])
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Refuses_Init::init - refusing to init\n")));
  return -1;
}

/// Terminates object when dynamic unlinking occurs.
int
Refuses_Init::fini (void)
{
  ACE_ERROR ((LM_ERROR,
              ACE_TEXT ("Refuses_Init::fini should not be called!\n")));
  return 0;
}

/// Returns information on a service object.
int
Refuses_Init::info (ACE_TCHAR **info_string, size_t length) const
{
  ACE_TCHAR const *msg =
    ACE_TEXT ("Refuses_Init service, shouldn't be here!\n");
  if (*info_string == 0)
    *info_string = ACE_OS::strdup (msg);
  else
    ACE_OS::strncpy (*info_string, msg, length);
  ACE_ERROR ((LM_ERROR,
              ACE_TEXT ("Refuses_Init::info() called - shouldn't be\n")));
  return ACE_OS::strlen (*info_string);
}

ACE_FACTORY_DEFINE (Service_Config_DLL, Refuses_Init)
