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

#include "test_config.h"
#include "ace/Stream.h"
#include "ace/UPIPE_Acceptor.h"
#include "ace/UPIPE_Connector.h"

ACE_RCSID(tests, UPIPE_SAP_Test, "$Id$")

#if defined(__BORLANDC__) && __BORLANDC__ >= 0x0530
USELIB("..\ace\aced.lib");
//---------------------------------------------------------------------------
#endif /* defined(__BORLANDC__) && __BORLANDC__ >= 0x0530 */

#if defined (ACE_HAS_THREADS) && (defined (ACE_HAS_STREAM_PIPES) || defined (ACE_WIN32))

// Global pattern
static ACE_UPIPE_Addr addr (__TEXT ("pattern"));

// connector thread.

static void *
connector (void *)
{
  ACE_UPIPE_Stream c_stream;

  ACE_OS::sleep (5);

  ACE_DEBUG ((LM_DEBUG, "(%t) connector starting connect\n"));
  ACE_UPIPE_Connector con;

  if (con.connect (c_stream, addr) == -1)
    ACE_DEBUG ((LM_DEBUG, "(%t) connector ACE_UPIPE_Connector failed\n"));

  ACE_Message_Block *mb;

  ACE_NEW_RETURN (mb, ACE_Message_Block (sizeof ("hello thanks") * sizeof (char)), 0);

  mb->copy ("hello");

  if (c_stream.send (mb) == -1)
    ACE_DEBUG ((LM_DEBUG, "(%t) error connector send\n"));

  if (c_stream.recv (mb) == -1)
    ACE_DEBUG ((LM_DEBUG, "(%t) error connector recv\n"));

  ACE_ASSERT (ACE_OS::strcmp (mb->rd_ptr (), "thanks") == 0);

  // Free up the memory block.
  mb->release ();

  // Now try the send()/recv() interface.
  char mytext[] = "This string is sent by connector as a buffer";

  ACE_DEBUG ((LM_DEBUG, "(%t) connector sending text\n"));
  if (c_stream.send (mytext, sizeof (mytext)) == -1)
    ACE_DEBUG ((LM_DEBUG,
                "(%t) buffer send from connector failed\n"));

  char conbuf[BUFSIZ];  // Buffer to receive response.

  int i = 0;

  for (char c = ' '; c != '!'; i++)
    {
      if (c_stream.recv (&c, 1) == -1)
        ACE_DEBUG ((LM_DEBUG,
                    "(%t) buffer recv from connector failed\n"));
      else
        conbuf[i] = c;
    }

  conbuf[i] = '\0';
  ACE_DEBUG ((LM_DEBUG, "(%t) conbuf = %s\n", conbuf));
  ACE_ASSERT (ACE_OS::strcmp (conbuf, "this is the acceptor response!") == 0);

  c_stream.close ();
  ACE_DEBUG ((LM_DEBUG, "(%t) exiting thread\n"));
  return 0;
}

static void *
acceptor (void *args)
{
  ACE_UPIPE_Acceptor *acceptor = (ACE_UPIPE_Acceptor *) args;
  ACE_UPIPE_Stream s_stream;

  if (acceptor->accept (s_stream) == -1)
    ACE_DEBUG ((LM_DEBUG,
                "(%t) ACE_UPIPE_Acceptor.accept failed\n"));

  ACE_Message_Block *mb = 0;

  if (s_stream.recv (mb) == -1)
    ACE_DEBUG ((LM_DEBUG, "(%t) acceptor recv failed\n"));

  ACE_ASSERT (ACE_OS::strcmp (mb->rd_ptr (), "hello") == 0);

  mb->wr_ptr (mb->rd_ptr ());
  mb->copy ("thanks");

  if (s_stream.send (mb) == -1)
    ACE_DEBUG ((LM_DEBUG, "(%t) acceptor send failed\n"));

  char s_buf[BUFSIZ];
  ACE_DEBUG ((LM_DEBUG, "(%t) acceptor sleeping on recv\n"));

  if (s_stream.recv (s_buf, sizeof (s_buf)) == -1)
    ACE_DEBUG ((LM_DEBUG, "(%t) acceptor recv failed\n"));
  else
    ACE_ASSERT (ACE_OS::strcmp (s_buf,
                                "This string is sent by connector as a buffer") == 0);

  const char svr_response[] = "this is the acceptor response!";
  ACE_OS::strcpy (s_buf, svr_response);

  if (s_stream.send (s_buf, sizeof (svr_response)) == -1)
    ACE_DEBUG ((LM_DEBUG, "(%t) acceptor send failed\n"));

  s_stream.close ();
  ACE_DEBUG ((LM_DEBUG, "(%t) exiting thread\n"));
  return 0;
}
#endif /* ACE_HAS_THREADS && defined ACE_HAS_STREAM_PIPES || ACE_WIN32 */

int
main (int, char *[])
{
  ACE_START_TEST ("UPIPE_SAP_Test");

#if defined (ACE_HAS_THREADS) && (defined (ACE_HAS_STREAM_PIPES) || defined (ACE_WIN32))
  ACE_UPIPE_Acceptor acc (addr);

  // Spawn a acceptor thread.
  if (ACE_Thread_Manager::instance ()->spawn (ACE_THR_FUNC (acceptor),
                                              (void *) &acc,
                                              THR_NEW_LWP,
                                              0) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "spawn"), 1);

  // Spawn a connector thread.
  if (ACE_Thread_Manager::instance ()->spawn (ACE_THR_FUNC (connector),
                                              (void *) 0,
                                              THR_NEW_LWP,
                                              0) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "spawn"), 1);

  ACE_Thread_Manager::instance ()->wait ();
  ACE_DEBUG ((LM_DEBUG, "(%t) joined with acceptor thread\n"));

  // Close the acceptor
  acc.close ();

#else

#if !defined (ACE_HAS_THREADS)
  ACE_ERROR ((LM_ERROR, "threads not supported on this platform\n"));
#else
#if !defined (ACE_HAS_STREAM_PIPES) && !defined (ACE_WIN32)
  ACE_ERROR ((LM_ERROR, "UPIPE is not supported on this platform\n"));
#endif /* !defined (ACE_HAS_STREAM_PIPES) && ! defined (ACE_WIN32) */
#endif /* !defined (ACE_HAS_THREADS) */
#endif /* defined (ACE_HAS_THREADS) && (defined (ACE_HAS_STREAM_PIPES) || defined (ACE_WIN32)) */

  ACE_END_TEST;
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
#if defined (__BORLANDC__)
// Borland C++ doesn't link with these instantiations in the ACE library.
template class ACE_Stream<ACE_SYNCH>;
template class ACE_Double_Linked_List<ACE_Thread_Descriptor>;
template class ACE_Unbounded_Queue<ACE_Thread_Descriptor*>;
template class ACE_Unbounded_Queue<ACE_Thread_Descriptor_Base>;
#endif /* defined (__BORLANDC__) */
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#if defined (__BORLANDC__)
// Borland C++ doesn't link with these instantiations in the ACE library.
#pragma instantiate ACE_Stream<ACE_SYNCH>
#pragma instantiate ACE_Double_Linked_List<ACE_Thread_Descriptor>
#pragma instantiate ACE_Unbounded_Queue<ACE_Thread_Descriptor*>
#pragma instantiate ACE_Unbounded_Queue<ACE_Thread_Descriptor_Base>
#endif /* defined (__BORLANDC__) */
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
