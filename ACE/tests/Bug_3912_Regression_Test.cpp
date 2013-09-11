// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = DESCRIPTION
//    This test asserts that close is called on Modules during
//    ACE_Service_Repository shutdown
//
// = AUTHOR
//    Chad Beaulac <chad@objectivesolutions.com>
//
// ============================================================================

#include "test_config.h"

#include "ace/Log_Msg.h"
#include "ace/Service_Config.h"
#include "ace/Service_Repository.h"
#include "ace/Service_Object.h"
#include "ace/Service_Types.h"
#include "ace/Task.h"
#include "ace/Module.h"

typedef ACE_Task<ACE_SYNCH> MT_Task;
typedef ACE_Module<ACE_SYNCH> MT_Module;

/**
 * We use this Task to track if close was called.
 */
class Close_Handler : public virtual MT_Task
{
public:

  Close_Handler(bool* close_called_arg)
  : close_called_ (close_called_arg)
  {
  }

  virtual int close(u_long );

private:
  bool* close_called_;
};


int Close_Handler::close(u_long )
{
  ACE_DEBUG ((LM_DEBUG,"Close_Handler::close \n"));
  *close_called_ = true;
  return 0;
}

int
run_test (int argc, ACE_TCHAR *argv[])
{
  int status = 0;
  bool close_called = false;
  Close_Handler* close_handler = 0;
  ACE_NEW_RETURN(close_handler, Close_Handler (&close_called), -1);

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Opening service config\n")));

  status = ACE_Service_Config::open (argc,
                                     argv,
                                     ACE_DEFAULT_LOGGER_KEY,
                                     true,
                                     true /*ignore def svc.conf*/);
  if (status != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                        ACE_TEXT ("run_test, %p\n")
                        ACE_TEXT ("ACE_Service_Config::open")),
                        status);
    }
  else
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Service config opened\n")));


  ACE_Service_Repository *asr = ACE_Service_Repository::instance ();
  if (asr == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("run_test, no service repository\n")),
                       -1);

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Finding close test module\n")));

  const ACE_Service_Type* st = 0;
  status = asr->find (ACE_TEXT ("Close_Test_Module"), &st);
  if (status != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("run_test, %p on Close_Test_Module\n")
                       ACE_TEXT ("ACE_Service_Repository::find")),
                       status);

  //
  // Put our Close_Handler Task into the Module
  //
  MT_Module* close_test_module =
    static_cast <MT_Module *> (st->type()->object ());

  if (close_test_module == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("run_test, no close test module\n")),
                       -1);
  close_test_module->reader (close_handler);

  //
  // Remove the Module from the Stream.
  // This is what happens during ACE_Service_Repository::fini
  // We want to make sure that close is called on Modules and their Tasks
  // at this time to ensure proper cleanup during the shutdown sequence
  // by getting the close methods called so user shutdown hooks can fire.
  //

  const ACE_Module_Type* module_type =
    static_cast< const ACE_Module_Type*>(st->type ());

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Finding close test stream\n")));

  status = asr->find (ACE_TEXT ("Close_Test_Stream"), &st);
  if (status != 0)
  {
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("run_test, %p on Close_Test_Stream\n")
                       ACE_TEXT ("ACE_Service_Repository::find")),
                       status);
  }
  const ACE_Stream_Type* close_test_stream =
    static_cast<const ACE_Stream_Type*> (st->type ());

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Removing module\n")));

  ACE_Stream_Type *nc_stream = const_cast<ACE_Stream_Type*>(close_test_stream);
  ACE_Module_Type *nc_module = const_cast<ACE_Module_Type*>(module_type);
  nc_stream->remove (nc_module);

  if (!close_called)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("close not called\n")));
      ++status;
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT (" SUCCESS: close called\n")));
    }
  return status;
}

int
run_main(int, ACE_TCHAR *argv[])
{
  ACE_START_TEST (ACE_TEXT ("Bug_3912_Regression_Test"));

  ACE_TCHAR conf_file_name [MAXPATHLEN];
#if defined (TEST_DIR)
  ACE_OS::strcpy (conf_file_name, TEST_DIR);
  ACE_OS::strcat (conf_file_name, ACE_DIRECTORY_SEPARATOR_STR);
  ACE_OS::strcat (conf_file_name, ACE_TEXT ("Bug_3912_Regression_Test.conf"));
#else
  ACE_OS::strcpy (conf_file_name, ACE_TEXT ("Bug_3912_Regression_Test.conf"));
#endif

  ACE_TCHAR * _argv[3] = {argv[0],
                          const_cast<ACE_TCHAR*> (ACE_TEXT ("-f")),
                          const_cast<ACE_TCHAR*> (conf_file_name)};

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Starting test\n")));

  int status = run_test (3,_argv);

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Closing service config\n")));

  ACE_Service_Config::fini_svcs ();

  ACE_END_TEST;
  return status;
}
