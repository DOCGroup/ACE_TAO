
//=============================================================================
/**
 *  @file   ex1.cpp
 *
 *  $Id$
 *
 *  Example for using <ACE_UPIPE_SAP> and <ACE_Thread> for
 *  intra-process communication.
 *
 *
 *  @author Gerhard Lenzer and Douglas C. Schmidt
 */
//=============================================================================


#include "ace/OS_main.h"
#include "ace/OS_NS_string.h"
#include "ace/Stream.h"
#include "ace/UPIPE_Acceptor.h"
#include "ace/UPIPE_Connector.h"



#if defined (ACE_HAS_THREADS)

// Global pattern
static ACE_UPIPE_Addr addr (ACE_TEXT("pattern"));

// peer1 thread entry point.

static void *
peer1 (void *)
{
  ACE_UPIPE_Stream c_stream;

  ACE_DEBUG ((LM_DEBUG,
              "(%t) peer1 starting connect\n"));
  ACE_UPIPE_Connector con;

  if (con.connect (c_stream, addr) == -1)
    ACE_ERROR ((LM_ERROR,
                "(%t) peer1 ACE_UPIPE_Connector failed\n"));

  ACE_Message_Block *mb = 0;
  ACE_NEW_RETURN (mb,
                  ACE_Message_Block (20),
                  0);

  mb->copy ("hello", 6);

  if (c_stream.send (mb) == -1)
    ACE_ERROR ((LM_ERROR,
                "(%t) error peer1 send\n"));

  if (c_stream.recv (mb) == -1)
    ACE_ERROR ((LM_ERROR,
                "(%t) error peer1 recv\n"));

  ACE_ERROR ((LM_ERROR,
              "(%t) peer1 ack is \"%s\"\n",
              mb->rd_ptr ()));

  // Free up the memory block.
  mb->release ();

  // Now try the send()/recv() interface.
  char mytext[] = "This string is sent by peer1 as buffer";

  ACE_ERROR ((LM_ERROR,
              "(%t) peer1 sending text\n"));
  if (c_stream.send (mytext, sizeof mytext) == -1)
    ACE_ERROR ((LM_ERROR,
                "(%t) buffer send from peer1 failed\n"));

  char conbuf[30];  // Buffer to receive response.

  int i = 0;

  for (char c = ' '; c != '!'; i++)
    {
      if (c_stream.recv (&c, 1) == -1)
        ACE_ERROR ((LM_ERROR,
                    "(%t) buffer recv from peer1 failed\n"));
      else
        conbuf[i] = c;
    }

  conbuf[i] = '\0';
  ACE_DEBUG ((LM_DEBUG,
              "(%t) peer1 received buffer with \"%s\"\n",
              conbuf));
  c_stream.close ();
  return 0;
}

static void *
peer2 (void *)
{
  ACE_UPIPE_Acceptor acc (addr);
  ACE_UPIPE_Stream s_stream;

  // Spawn a peer1 thread.
  if (ACE_Thread_Manager::instance ()->spawn (ACE_THR_FUNC (peer1),
                                              (void *) 0,
                                              THR_NEW_LWP | THR_DETACHED) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "spawn"),
                      0);

  ACE_DEBUG ((LM_DEBUG,
              "(%t) peer2 starting accept\n"));

  if (acc.accept (s_stream) == -1)
    ACE_ERROR ((LM_ERROR,
                "(%t) ACE_UPIPE_Acceptor.accept failed\n"));

  ACE_Message_Block *mb = 0;

  if (s_stream.recv (mb) == -1)
    ACE_ERROR ((LM_ERROR,
                "(%t) peer2 recv failed\n"));

  ACE_DEBUG ((LM_DEBUG, "(%t) peer2 recv is \"%s\"\n",
              mb->rd_ptr ()));

  mb->wr_ptr (mb->rd_ptr ());
  mb->copy ("thanks", 7);

  if (s_stream.send (mb) == -1)
    ACE_ERROR ((LM_ERROR,
                "(%t) peer2 send failed\n"));

  char s_buf[42];
  ACE_DEBUG ((LM_DEBUG,
              "(%t) peer2 sleeping on recv\n"));

  if (s_stream.recv (s_buf, sizeof s_buf) == -1)
    ACE_ERROR ((LM_ERROR,
                "(%t) peer2 recv failed\n"));
  else
    ACE_DEBUG ((LM_DEBUG,
                "(%t) peer2 received buffer with \"%s\"\n",
                s_buf));

  ACE_OS::strcpy (s_buf,
                  "this is the peer2 response!");

  if (s_stream.send (s_buf, 30) == -1)
    ACE_ERROR ((LM_ERROR,
                "(%t) peer2 send failed\n"));
  s_stream.close ();
  return 0;
}

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  // Spawn a peer2 thread.
  if (ACE_Thread_Manager::instance ()->spawn (ACE_THR_FUNC (peer2),
                                              (void *) 0,
                                              THR_NEW_LWP | THR_DETACHED) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "spawn"),
                      1);
  // Wait for peer2 and peer1 threads to exit.
  ACE_Thread_Manager::instance ()->wait ();
  return 0;
}
#else
int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_ERROR_RETURN ((LM_ERROR,
                     "threads not supported on this platform\n"),
                    -1);
}
#endif /* ACE_HAS_THREADS */
