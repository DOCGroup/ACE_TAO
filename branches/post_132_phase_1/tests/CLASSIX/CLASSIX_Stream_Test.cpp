/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
// 
// = FILENAME
//    Stream_Test.cpp
//
// = DESCRIPTION
//     This is a test of the <IPP_CLASSIX_Addr> class.
//
// = AUTHOR
//    Wei Chiang
// 
// ============================================================================

#include "ace/CLASSIX/CLASSIX_Stream.h"

#include "tests/test_config.h"

#define RCV_DELAY 1000        /* We should NOT wait in ipcReceive */

static char sndBody[]  = "The sea is calm, the tide is full ...\n";
static char rcvAnnex[K_CMSGANNEXSIZE];
static char rcvBody[1000];

int
main (int, char *[])
{
  ACE_START_TEST ("Stream_Test");

  /* ================================================================== */
  ACE_CLASSIX_Port_Core remote_port;

  // Sender's socket
  // Use my default port as the sending address
  ACE_CLASSIX_Stream send(remote_port);

  ACE_CLASSIX_Stream rcv(send.local_sap().get_addr(), 
			 ACE_CLASSIX_Port(remote_port));
  // make the rcv's port be one of the multiple receive ports  
  if (rcv.selectable() < 0)
      ACE_DEBUG((LM_DEBUG, "failed to make the port selectable\n"));

  send.open_writer();

  ACE_DEBUG((LM_DEBUG, "send and block on receive....\n"));
  if (send.send_n(&sndBody[0], sizeof(sndBody)) == sizeof(sndBody))
  {
      // Receiver's Socket
      int rslt = rcv.ipcRecv(rcvBody, 1000);
      if (rslt == sizeof (sndBody))
	  ACE_DEBUG((LM_DEBUG, "received %s\n", rcvBody));
      else
	  ACE_DEBUG((LM_ERROR, "???? Error in ipcReceive():%d\n", rslt));
  }
  else
  {
	  ACE_ERROR((LM_ERROR, "(%t)|%p\n", "???? Error in send_n()\n"));
  }

  ACE_DEBUG((LM_DEBUG, "send, peek then block on receive....\n"));
  if (send.send_n(&sndBody[0], sizeof(sndBody)) == sizeof(sndBody))
  {
      // Receiver's Socket
      //  int rslt = rcv.recv(rcvBody, 1000, MSG_PEEK);
      // Equivalent to rcv.peek()
      int rslt = rcv.peek();
      if (rslt < 0)
	  ACE_DEBUG((LM_ERROR, "???? Error while peeking :%d\n", rslt));
      else
      {
	  char *buf = new char(rslt);
	  if (int n = rcv.recv(buf, rslt) == rslt)
	      ACE_DEBUG((LM_DEBUG, "received %s\n", buf));
	  else
	      ACE_DEBUG((LM_ERROR, "???? Error in ipcReceive(): %d\n", n));
	  delete buf;
      }
  }
  else
  {
	  ACE_DEBUG((LM_ERROR, "???? Error in send_n()\n"));
  }

  ACE_DEBUG((LM_DEBUG, "test recv_n()....\n"));
  if (send.send_n(&sndBody[0], sizeof(sndBody)) == sizeof(sndBody) &&
      send.send_n(&sndBody[0], sizeof(sndBody)) == sizeof(sndBody))
  {
      // Receiver's Socket

      int rslt = rcv.ipcRecv_n(rcvBody, 2*sizeof (sndBody) - 10);
      if (rslt == (2 * sizeof (sndBody) -10))
      {
	  rcvBody[rslt] = '\0';	// For %s printout format
	  ACE_DEBUG((LM_DEBUG, "received %d byte: %s + %s\n", rslt,
		     rcvBody,  rcvBody + sizeof(sndBody)));
      }
      else
	  ACE_DEBUG((LM_ERROR, "???(%P|%t) %p\n", "ipcRecv_n()"));
  }
  else
      ACE_DEBUG((LM_ERROR, "???(%P|%t) %p\n", "ipcRecv_n()"));

  ACE_END_TEST;
  return 0;
}

