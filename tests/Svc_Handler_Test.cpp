// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Svc_Handler_Test.cpp
//
// = DESCRIPTION
//    This tests illustrates the "buffering" strategy of the
//    <ACE_Buffered_Svc_Handler>.
//
// = AUTHORS
//    Douglas C. Schmidt <schmidt@cs.wustl.edu>
//
// ============================================================================

#include "test_config.h"
#include "ace/FILE_Connector.h"
#include "ace/Svc_Handler.h"
#include "ace/Synch.h"

ACE_RCSID(tests, Svc_Handler_Test, "$Id$")

#if defined(__BORLANDC__) && __BORLANDC__ >= 0x0530
USELIB("..\ace\aced.lib");
//---------------------------------------------------------------------------
#endif /* defined(__BORLANDC__) && __BORLANDC__ >= 0x0530 */

typedef ACE_Buffered_Svc_Handler <ACE_FILE_STREAM, ACE_NULL_SYNCH> SVC_HANDLER;

static void
run_test (SVC_HANDLER &svc_handler,
               size_t iterations)
{
  // Create a whole slew of message blocks and pass them to the
  // <svc_handler>.
  for (size_t i = 0; i < iterations; i++)
    {
      ACE_Message_Block *mb;
      ACE_NEW (mb,
               ACE_Message_Block (sizeof ("hello ")));

      ACE_Message_Block *cb1;
      ACE_NEW (cb1,
               ACE_Message_Block (sizeof ("there\n")));

      ACE_Message_Block *cb2;
      ACE_NEW (cb2,
               ACE_Message_Block (sizeof ("there\n")));

      mb->copy ("hello ",
                ACE_OS::strlen ("hello "));
      cb1->copy ("there ",
                ACE_OS::strlen ("there "));
      cb2->copy ("doug\n",
                ACE_OS::strlen ("doug\n"));
      mb->cont (cb1);
      cb1->cont (cb2);
      
      // Note that this is a buffered call!
      if (svc_handler.put (mb) == -1)
        ACE_ERROR ((LM_ERROR,
                    ASYS_TEXT ("%p\n"),
                    ASYS_TEXT ("put")));
    }

  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("final flush\n")));

  // Make sure to flush everything out before we exit.
  if (svc_handler.flush () == -1)
    ACE_ERROR ((LM_ERROR,
                ASYS_TEXT ("%p\n"),
                ASYS_TEXT ("flush")));
}

int
main (int argc, ASYS_TCHAR *argv[])
{
  ACE_START_TEST (ASYS_TEXT ("Svc_Handler_Test"));
  {
    size_t max_buffer_size = BUFSIZ;
    size_t iterations = 10;

    if (argc > 1)
      max_buffer_size = ACE_OS::atoi (argv[1]);
    if (argc > 2)
      iterations = ACE_OS::atoi (argv[2]);

    ACE_FILE_Connector connector;
    ACE_FILE_IO file_io;
    // Create a temporary filename.
    ACE_FILE_Addr file (ACE_sap_any_cast (ACE_FILE_Addr &));

    // Open up the temp file.
    if (connector.connect (file_io,
                           file) == -1)
      ACE_ERROR_RETURN ((LM_ERROR, 
                         ASYS_TEXT ("open failed for %p\n"),
                         file.get_path_name ()),
                        1);
    else if (file_io.unlink () == -1)
      ACE_ERROR_RETURN ((LM_ERROR, 
                         ASYS_TEXT ("unlink failed for %p\n"),
                         file.get_path_name ()),
                        1);

    // Create the service handler and assign it <file_io> as its data
    // sink.
    SVC_HANDLER svc_handler (0,
                             0,
                             0,
                             max_buffer_size,
                             0);
    svc_handler.peer () = file_io;
    
    // Run the test.
    run_test (svc_handler, iterations);
  }
  ACE_END_TEST;
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Buffered_Svc_Handler <ACE_FILE_STREAM, ACE_NULL_SYNCH>;
template class ACE_Svc_Handler <ACE_FILE_STREAM, ACE_NULL_SYNCH>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Buffered_Svc_Handler <ACE_FILE_STREAM, ACE_NULL_SYNCH>
#pragma instantiate ACE_Svc_Handler <ACE_FILE_STREAM, ACE_NULL_SYNCH>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
