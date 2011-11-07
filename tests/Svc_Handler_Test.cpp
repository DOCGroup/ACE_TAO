
//=============================================================================
/**
 *  @file    Svc_Handler_Test.cpp
 *
 *  $Id$
 *
 *  This tests illustrates the "buffering" strategy of the
 *  <ACE_Buffered_Svc_Handler>.  This test also illustrates how the
 *  <ACE_FILE_IO> classes work.
 *
 *
 *  @author Douglas C. Schmidt <schmidt@cs.wustl.edu>
 */
//=============================================================================


#include "test_config.h"
#include "ace/OS_NS_string.h"
#include "ace/FILE_Connector.h"
#include "ace/Null_Condition.h"
#include "ace/Null_Mutex.h"
#include "ace/Svc_Handler.h"
#include "ace/Synch_Traits.h"

typedef ACE_Buffered_Svc_Handler <ACE_FILE_IO, ACE_NULL_SYNCH> SVC_HANDLER;

static void
run_test (SVC_HANDLER &svc_handler,
          size_t iterations)
{
  // Create a whole slew of message blocks and pass them to the
  // <svc_handler>.
  for (size_t i = 0; i < iterations; i++)
    {
      ACE_Message_Block *mb = 0;
      ACE_NEW (mb,
               ACE_Message_Block (sizeof (ACE_TEXT("hello "))));

      ACE_Message_Block *cb1 = 0;
      ACE_NEW (cb1,
               ACE_Message_Block (sizeof (ACE_TEXT("there\n"))));

      ACE_Message_Block *cb2 = 0;
      ACE_NEW (cb2,
               ACE_Message_Block (sizeof (ACE_TEXT("there\n"))));

      mb->copy ("hello ",
                ACE_OS::strlen (ACE_TEXT("hello ")));
      cb1->copy ("there ",
                 ACE_OS::strlen (ACE_TEXT("there ")));
      mb->cont (cb1);
      cb2->copy ("doug\n",
                ACE_OS::strlen (ACE_TEXT("doug\n")));
      cb1->cont (cb2);

      // Note that this is a buffered call!
      if (svc_handler.put (mb) == -1)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("%p\n"),
                    ACE_TEXT ("put")));
    }

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("final flush\n")));

  // Make sure to flush everything out before we exit.
  if (svc_handler.flush () == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("%p\n"),
                ACE_TEXT ("flush")));
}

int
run_main (int argc, ACE_TCHAR *argv[])
{
  ACE_START_TEST (ACE_TEXT ("Svc_Handler_Test"));
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
    if (connector.connect (file_io, file) == -1)
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("connect failed for %p\n"),
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

    file_io.close ();

    // Open up the temp file.
    if (connector.connect (file_io, file) == -1)
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("connect failed for %p\n"),
                         file.get_path_name ()),
                        1);
    char buf[ACE_MAXLOGMSGLEN + 1];
    ACE_LOG_MSG->clr_flags (ACE_Log_Msg::VERBOSE_LITE);

    ACE_FILE_Info info;
    file_io.get_info (info);
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT("file size = %d\n"), info.size_));

    for (ssize_t n_bytes; (n_bytes = file_io.recv (buf, ACE_MAXLOGMSGLEN)) > 0; )
      {
        buf[n_bytes] = '\0';
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT("%C"), buf));
      }

    ACE_DEBUG ((LM_DEBUG, ACE_TEXT("\n")));

    file_io.close ();

    if (file_io.unlink () == -1)
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("unlink failed for %p\n"),
                         file.get_path_name ()),
                        1);
  }

  ACE_END_TEST;
  return 0;
}

