// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
// 
// = FILENAME
//    UPIPE_SAP_Test.cpp
//
// = DESCRIPTION
//      This is a test that uses ACE_UPIPE_SAP and ACE_Thread for
//      intra-process communication. 
//
// = AUTHOR
//    Gerhard Lenzer, Douglas C. Schmidt, and Prashant Jain
// 
// ============================================================================

#include "ace/Log_Msg.h"
#include "ace/Stream.h"
#include "ace/UPIPE_Acceptor.h"
#include "ace/UPIPE_Connector.h"
#include "ace/Service_Config.h"
#include "test_config.h"

#if defined (ACE_HAS_THREADS)

// Global pattern
static ACE_UPIPE_Addr addr ("pattern");

// client thread.

static void *
client (void *)
{
  // Insert thread into thr_mgr.
  ACE_NEW_THREAD;

  ACE_UPIPE_Stream c_stream;

  ACE_DEBUG ((LM_DEBUG, "(%t) client starting connect\n"));
  ACE_UPIPE_Connector con;

  if (con.connect (c_stream, addr) == -1)
    ACE_DEBUG ((LM_DEBUG, "(%t) client ACE_UPIPE_Connector failed\n"));

  ACE_Message_Block *mb;

  ACE_NEW_RETURN (mb, ACE_Message_Block (sizeof "hello thanks"), 0);

  mb->copy ("hello", sizeof "hello");

  if (c_stream.send (mb) == -1)
    ACE_DEBUG ((LM_DEBUG, "(%t) error client send\n"));

  if (c_stream.recv (mb) == -1)
    ACE_DEBUG ((LM_DEBUG, "(%t) error client recv\n"));

  ACE_ASSERT (ACE_OS::strcmp (mb->rd_ptr (), "thanks") == 0);

  // Free up the memory block.
  delete mb;

  // Now try the send()/recv() interface.
  char mytext[] = "This string is sent by client as a buffer";

  ACE_DEBUG ((LM_DEBUG, "(%t) client sending text\n"));
  if (c_stream.send (mytext, sizeof mytext) == -1)
    ACE_DEBUG ((LM_DEBUG, 
		"(%t) buffer send from client failed\n"));

  char conbuf[BUFSIZ];  // Buffer to receive response.

  int i = 0;

  for (char c = ' '; c != '!'; i++)
    {
      if (c_stream.recv (&c, 1) == -1)
	ACE_DEBUG ((LM_DEBUG, 
		    "(%t) buffer recv from client failed\n"));
      else
	conbuf[i] = c;
    }

  conbuf[i] = '\0';
  ACE_DEBUG ((LM_DEBUG, "(%t) conbuf = %s", conbuf));
  ACE_ASSERT (ACE_OS::strcmp (conbuf, "this is the server response!") == 0);

  c_stream.close ();
  ACE_DEBUG ((LM_DEBUG, "\n(%t) exiting thread\n"));
  return 0;
}

static void *
server (void *args)
{
  // Insert thread into thr_mgr.
  ACE_NEW_THREAD;

  ACE_UPIPE_Acceptor *acceptor = (ACE_UPIPE_Acceptor *) args;
  ACE_UPIPE_Stream s_stream;
  ACE_hthread_t thr_handle;

  // Spawn a client thread.
  if (ACE_Thread::spawn (ACE_THR_FUNC (client), 
			 (void *) 0,
			 THR_NEW_LWP | THR_DETACHED,
			 0,
			 &thr_handle) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "spawn"), 0);

  ACE_DEBUG ((LM_DEBUG, "(%t) server starting accept\n"));

  if (acceptor->accept (s_stream) == -1)
    ACE_DEBUG ((LM_DEBUG, 
		"(%t) ACE_UPIPE_Acceptor.accept failed\n"));

  ACE_Message_Block *mb = 0;

  if (s_stream.recv (mb) == -1)
    ACE_DEBUG ((LM_DEBUG, "(%t) server recv failed\n"));

  ACE_ASSERT (ACE_OS::strcmp (mb->rd_ptr (), "hello") == 0);

  mb->wr_ptr (mb->rd_ptr ());
  mb->copy ("thanks", sizeof "thanks");

  if (s_stream.send (mb) == -1)
    ACE_DEBUG ((LM_DEBUG, "(%t) server send failed\n"));

  char s_buf[BUFSIZ];
  ACE_DEBUG ((LM_DEBUG, "(%t) server sleeping on recv\n"));

  if (s_stream.recv (s_buf, sizeof s_buf) == -1)
    ACE_DEBUG ((LM_DEBUG, "(%t) server recv failed\n"));
  else
    ACE_ASSERT (ACE_OS::strcmp (s_buf, 
				"This string is sent by client as a buffer") == 0);

  const char svr_response[] = "this is the server response!";
  ACE_OS::strcpy (s_buf, svr_response);

  if (s_stream.send (s_buf, sizeof svr_response) == -1)
    ACE_DEBUG ((LM_DEBUG, "(%t) server send failed\n"));

  // Wait until the client shuts down and the shutdown.
  if (ACE_Thread::join (thr_handle) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "join"));
  s_stream.close ();
  ACE_DEBUG ((LM_DEBUG, "(%t) exiting thread\n"));
  return 0;
}

#endif /* ACE_HAS_THREADS */

int 
main (int, char *argv[])
{
  ACE_START_TEST ("UPIPE_SAP_Test.cpp");

#if defined (ACE_HAS_THREADS) && (defined (ACE_HAS_STREAM_PIPES) || defined (ACE_WIN32))
  // Spawn a server thread.
  ACE_hthread_t thr_handle;
  ACE_UPIPE_Acceptor acceptor (addr);

  if (ACE_Thread::spawn (ACE_THR_FUNC (server), 
			 (void *) &acceptor,
			 THR_NEW_LWP | THR_DETACHED,
			 0,
			 &thr_handle) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "spawn"), 1);

  // Wait for server thread to exit.
  ACE_Thread::join (thr_handle);
#else
  ACE_ERROR ((LM_ERROR, "threads and/or UPIPE not supported on this platform\n"));
#endif /* ACE_HAS_THREADS */
  ACE_END_TEST;
  return 0;
}
