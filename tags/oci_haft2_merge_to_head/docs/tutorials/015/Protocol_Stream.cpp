// $Id$

#include "Protocol_Stream.h"
#include "Protocol_Task.h"

#include "Xmit.h"
#include "Recv.h"

#include "Compressor.h"

#include "ace/Stream_Modules.h"

/* You can choose at compile time to include/exclude the protocol
   pieces.
*/

// The usual typedefs to make things easier to type.
typedef ACE_Module<ACE_MT_SYNCH> Module;
typedef ACE_Thru_Task<ACE_MT_SYNCH> Thru_Task;

/* An ACE_Stream is a collection of ACE_Modules.  You can think of it
   as a doubly-linked list if you like.  Each Module contains two
   ACE_Task derivatives.  One of these tasks is used when sending data
   "upstream", the other is used for "downstream" operation.  In some
   cases, you'll only need to move data in one direction.  To provide
   a placeholder for the other direction, ACE_Thru_Task can be used.
   ACE_Thru_Task responds to the put() by simply invoking put_next()
   to send the data to the next module.
 */

/* Do-nothing constructor and destructor
 */

Protocol_Stream::Protocol_Stream (void)
{
}

Protocol_Stream::~Protocol_Stream (void)
{
}

/* Even opening the stream is rather simple.  The important thing to
   remember is that the modules you push onto the stream first will be
   at the tail (eg -- most downstream) end of things when you're
   done.
 */
int
Protocol_Stream::open (ACE_SOCK_Stream &peer,
                       Protocol_Task *reader,
                       bool isOriginator)
{
  // Initialize our peer() to read/write the socket we're given
  peer_.set_handle (peer.get_handle ());

  // Construct (and remember) the Recv object so that we can read from
  // the peer().
  ACE_NEW_RETURN (recv_,
                  Recv ( this->peer ()),
                  -1);

  // Add the transmit and receive tasks to the head of the stream.  As
  // we add more modules these will get pushed downstream and end up
  // nearest the tail by the time we're done.
  if (stream ().push (new Module ("Xmit/Recv",
                                  new Xmit ( this->peer ()),
                                  recv_)) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "stream().push(xmit/recv)"),
                      -1);

  // Add any other protocol tasks to the stream.  Each one is added at
  // the head.  The net result is that Xmit/Recv are at the tail.
  if (this->open (isOriginator) == -1)
    return -1;

  // If a reader task was provided then push that in as the upstream
  // side of the next-to-head module.  Any data read from the peer()
  // will be sent through here last.  Server applications will
  // typically use this task to do the actual processing of data.
  // Note the use of Thru_Task.  Since a module must always have a
  // pair of tasks we use this on the writer side as a no-op.
  if (reader)
    {
      if (stream ().push (new Module ("Reader",
                                      new Thru_Task (),
                                      reader)) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%p\n",
                           "stream().push(reader)"),
                          -1);
    }

  return 0;
}

/* Add the necessary protocol objects to the stream.
*/

int
Protocol_Stream::open (bool isOriginator)
{
    // We are protocol version 1, we only support ZLIB compression currently.

    ProtocolVersion pversion = PROTOCOL_VERSION_1;
    Compressor::AlgorithmCode acode = Compressor::COMPRESSION_ZLIB;

     // If we are the originator, we put a message asking for
    // what compression is to be used.

    if (isOriginator)
    {
        ACE_Message_Block * message = new ACE_Message_Block( 1024 );

        CompressionNegotiation neg;
        neg.protocolVersion = pversion;
        neg.algorithmCount = 1;
        neg.algorithm[0] = acode;

        memcpy(message->wr_ptr(), &neg, sizeof(neg));
        message->wr_ptr(sizeof(neg));

        if (put(message, 0) < 0)
        {
            return -1;
        }

        ACE_Message_Block * response;
        if (get(response, 0) < 0)
        {
            return -1;
        }

        CompressionNegotiationReply negResponse;
        memcpy(&negResponse, response->rd_ptr(), sizeof(negResponse));

        response->release();
        if (negResponse.errorCode != NEGOTIATE_OK)
        {
            return -1;
        }
    }
    else
    {
        // Read the message to see what algorithm the originator speaks.

        ACE_Message_Block* message;
        if (get(message, 0) < 0)
        {
            return -1;
        }

        CompressionNegotiation neg;
        memcpy(&neg, message->rd_ptr(), sizeof(neg));
        message->release();

        // Reply to handshake message.

        CompressionNegotiationReply negResponse;
        negResponse.errorCode = NEGOTIATE_OK;
        negResponse.protocolVersion = PROTOCOL_VERSION_1;

        if (neg.protocolVersion == PROTOCOL_VERSION_1)
        {
            // Ensure that incoming algorithms include the one we support.

            negResponse.algorithm = Compressor::COMPRESSION_NONE;
            for (int i = 0; i < neg.algorithmCount; i++)
            {
                if (neg.algorithm[i] == acode)
                {
                    negResponse.algorithm = acode;
                    break;
                }
            }
            if (negResponse.algorithm == Compressor::COMPRESSION_NONE)
            {
                negResponse.errorCode = NEGOTIATE_UNSUPPORTED_ALGORITHM;
            }
        }
        else
        {
            negResponse.errorCode = NEGOTIATE_UNSUPPORTED_VERSION;
        }

        ACE_Message_Block* reply = new ACE_Message_Block( 1024 );
        memcpy(reply->wr_ptr(), &negResponse, sizeof(negResponse));
        reply->wr_ptr(sizeof(neg));

        if (put(reply, 0) < 0)
        {
            return -1;
        }
    }

    // Add the compression component to the pipeline.

    if (stream ().push (new Module ("compress",
                                  new Compressor (acode),
                                  new Compressor (acode))) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                          "%p\n",
                           "stream().push(compressor)"),
                         -1);
    return 0;
}

// Closing the Protocol_Stream is as simple as closing the ACE_Stream.
int
Protocol_Stream::close (void)
{
  return stream ().close ();
}

// Simply pass the data directly to the ACE_Stream.
int
Protocol_Stream::put (ACE_Message_Block *&message,
                      ACE_Time_Value *timeout)
{
  return stream ().put (message,
                        timeout);
}

/* Tell the Recv module to read some data from the peer and pass it
   upstream.  Servers will typically use this method in a
   handle_input() method to tell the stream to get a client's request.  */

int
Protocol_Stream::get(void)
{
  // If there is no Recv module, we're in big trouble!
  if (recv_ == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) No Recv object!\n"),
                      -1);

  // This tells the Recv module to go to it's peer() and read some
  // data.  Once read, that data will be pushed upstream.  If there is
  // a reader object then it will have a chance to process the data.
  // If not, the received data will be available in the message queue
  // of the stream head's reader object (eg --
  // stream().head()->reader()->msg_queue()) and can be read with our
  // other get() method below.
  if (recv_->get () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) Cannot queue read request\n"),
                      -1);

  // For flexibility I've added an error() method to tell us if
  // something bad has happened to the Recv object.
  if (recv_->error ())
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) Recv object error!\n"),
                      -1);

  return 0;
}

/* Take a message block off of the stream head reader's message queue.
   If the queue is empty, use get() to read from the peer.  This is
   most often used by client applications.  Servers will generaly
   insert a reader that will prevent the data from getting all the way
   upstream to the head.  */
int
Protocol_Stream::get (ACE_Message_Block *&response,
                      ACE_Time_Value *timeout )
{
  if (stream ().head ()->reader ()->msg_queue ()->is_empty ()
      && this->get () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) Cannot get data into the stream.\n"),
                      -1);

  return stream ().head ()->reader ()->getq (response,
                                             timeout);
}
