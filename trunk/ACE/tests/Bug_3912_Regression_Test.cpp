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
#include "Service_Config_Stream_DLL.h"

#include "ace/Log_Msg.h"
#include "ace/Service_Config.h"
#include "ace/Service_Repository.h"
#include "ace/Service_Object.h"
#include "ace/Service_Types.h"


/**
 * We use this Task to track if close was called.
 */
class Close_Handler : public virtual MT_Task
{
public:

  Close_Handler(int* close_called_arg) : close_called(close_called_arg)
  {
  }

  virtual int
  close(u_long flags)
  {
    *close_called = 1;
  }

private:
  int* close_called;

};

int
run_main(int, ACE_TCHAR *argv[])
{
  ACE_START_TEST (ACE_TEXT ("Bug_3912_Regression_Test"));

  ACE_TCHAR * _argv[3] = {argv[0],
                          const_cast<ACE_TCHAR*> (ACE_TEXT ("-f")),
                          const_cast<ACE_TCHAR*>
                          (ACE_TEXT ("Bug_3912_Regression_Test.conf"))};
  int status = 0;

  int close_called = 0;
  Close_Handler* close_handler = new Close_Handler (&close_called);

  if ((status = ACE_Service_Config::open (3,
                                          _argv,
                                          ACE_DEFAULT_LOGGER_KEY,
                                          true,
                                          true /*ignore def svc.conf*/)) == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("%p\n"),
                ACE_TEXT ("open"),
                1));

  const ACE_Service_Type* st = 0;
  if (ACE_Service_Repository::instance ()->find ("Close_Test_Module", &st) != 0)
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("%p\n"),
                ACE_TEXT ("Couldn't find Close_Test_Module in ASR"),
                1));
    status++;
  }
  else
  {
    //
    // Put our Close_Handler Task into the Module
    //
    MT_Module* close_test_module = static_cast<MT_Module*> (st->type ()->object ());
    close_test_module->reader (close_handler);
  }
  //
  // Find the Stream that the Module is in.
  //
  const ACE_Service_Type* st2 = 0;
  if (ACE_Service_Repository::instance ()->find ("Close_Test_Stream", &st2) != 0)
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("%p\n"),
                ACE_TEXT ("Couldn't find Close_Test_Stream in ASR"),
                1));
    status++;
  }
  else
  {
    //
    // Remove the Module from the Stream.
    // This is what happens during ACE_Service_Repository::fini
    // We want to make sure that close is called on Modules and their Tasks
    // at this time to ensure proper cleanup during the shutdown sequence
    // by getting the close methods called so user shutdown hooks can fire.
    //
    ACE_Stream_Type* close_test_stream = const_cast<ACE_Stream_Type*>(static_cast<const ACE_Stream_Type*> (st2->type ()));
    
    close_test_stream->remove (const_cast<ACE_Module_Type*>(dynamic_cast<const ACE_Module_Type*>(st->type ())));
//    close_test_stream->remove ("Close_Test_Module");
    if (close_called != 1)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("%p\n"),
                  ACE_TEXT ("Module::close not called"),
                  1));
      status++;
    }
  }

  ACE_Service_Config::fini_svcs ();

  ACE_END_TEST;
  return status;
}
