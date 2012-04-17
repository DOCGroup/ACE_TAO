// $Id$

// ============================================================================
//
// = LIBRARY
//   examples
//
// = FILENAME
//   ex3.cpp
//
// = DESCRIPTION
//    Example for using <ACE_UPIPE_SAP> and <ACE_Thread> for intra-process
//    communication.  This example uses char buffers as input/output
//    interface to the <ACE_UPIPE_Stream>.
//
// = AUTHOR
//    Gerhard Lenzer and Prashant Jain.
//
// ============================================================================

#include "ace/OS_main.h"
#include "ace/UPIPE_Connector.h"
#include "ace/UPIPE_Acceptor.h"
#include "ace/Auto_Ptr.h"
#include "ace/OS_NS_time.h"



#if defined (ACE_HAS_THREADS)

// Data for testsuite.
static int size = 0;
static int iterations = 0;

static void *
supplier (void *)
{
  ACE_UPIPE_Stream s_stream;
  ACE_UPIPE_Addr c_addr (ACE_TEXT("pattern"));

  ACE_UPIPE_Connector con;

  ACE_DEBUG ((LM_DEBUG,
              "(%t) supplier starting connect thread\n"));

  if (con.connect (s_stream, c_addr) == -1)
    ACE_ERROR ((LM_ERROR,
                "(%t) %p\n",
                "ACE_UPIPE_Acceptor.connect failed"));

  ACE_Auto_Basic_Array_Ptr<char> mybuf (new char[size]);

  for (int i = 0; i < size; i++)
    mybuf[i] = 'a';

  for (int j = 0; j < iterations; j++)
    if (s_stream.send (mybuf.get (), size) == -1)
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%t) %p\n", "send failed"),
                         0);

  // Insert a 0-sized message block to signal the other side to shut
  // down.
  if (s_stream.send (new ACE_Message_Block ((size_t) 0)) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%t) %p\n", "error put"),
                      0);
  s_stream.close ();
  return 0;
}

static void *
consumer (void *)
{
  ACE_UPIPE_Stream c_stream;
  ACE_UPIPE_Addr serv_addr (ACE_TEXT("pattern"));

  // Accept will wait up to 4 seconds
  ACE_UPIPE_Acceptor acc (serv_addr);

  ACE_DEBUG ((LM_DEBUG,
              "(%t) consumer spawning the supplier thread\n"));

  // Spawn the supplier thread.
  if (ACE_Thread_Manager::instance ()->spawn (ACE_THR_FUNC (supplier),
                                              (void *) 0,
                                              THR_NEW_LWP | THR_DETACHED) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "spawn"),
                      0);
  ACE_DEBUG ((LM_DEBUG,
              "(%t) consumer starting accept\n"));

  if (acc.accept (c_stream) == -1)
    ACE_ERROR ((LM_ERROR, "(%t) %p\n",
                "ACE_UPIPE_Acceptor.accept failed"));

  // Ensure deletion upon exit.
  ACE_Auto_Basic_Array_Ptr<char> mybuf (new char[size]);
  time_t currsec;

  ACE_OS::time (&currsec);

  time_t start = (time_t) currsec;
  int result = 0;
  int blocks = 0;

  for (;; blocks++)
    {
      result = c_stream.recv (mybuf.get (), size);
      if (result <= 0)
        break;
    }

  if (result == -1)
    ACE_ERROR ((LM_ERROR,
                "(%t) %p\n", "recv failed"));

  ACE_OS::time (&currsec);
  time_t secs = (time_t) currsec - start;

  ACE_DEBUG ((LM_DEBUG,
              "(%t) Transferred %d blocks of size %d\n"
              "The program ran %d seconds\n",
              blocks,
              size,
              secs));
  c_stream.close ();
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  size = argc > 1 ? ACE_OS::atoi (argv[1]) : 32;
  iterations = argc > 2 ? ACE_OS::atoi (argv[2]) : 16;

  // Spawn the thread.
  if (ACE_Thread_Manager::instance ()->spawn (ACE_THR_FUNC (consumer),
                                              (void *) 0,
                                              THR_NEW_LWP | THR_DETACHED) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "spawn"),
                      1);
  // Wait for producer and consumer threads to exit.
  ACE_Thread_Manager::instance ()->wait ();
  return 0;
}
#else
int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_ERROR_RETURN ((LM_ERROR,
                     "threads not supported on this platform\n"),
                     0);
}
#endif /* ACE_HAS_THREADS */
