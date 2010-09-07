#include "TimeStamp.h"
#include "orbsvcs/AV/AVStreams_i.h"
#include "ace/High_Res_Timer.h"

// $Id$
//TimeStamp Protocol Object

TimeStamp_Protocol_Object::TimeStamp_Protocol_Object (TAO_AV_Callback *callback,
                                                      TAO_AV_Transport *transport)
  :TAO_AV_Protocol_Object (callback,transport)
{
  ACE_DEBUG ((LM_DEBUG,
              "TimeStamp_Protocol_Object::TimeStamp_Protocol_Object\n"));
  ACE_NEW (this->frame_,
           ACE_Message_Block);

  this->frame_->size (4 * this->transport_->mtu ());
}


int
TimeStamp_Protocol_Object::handle_input (void)
{
  ssize_t n = this->transport_->recv (this->frame_->rd_ptr (),
                                      this->frame_->size ());
  if (n == -1)
    ACE_ERROR_RETURN ((LM_ERROR,"TAO_AV_UDP_Flow_Handler::handle_input recv failed\n"),-1);
  if (n == -1)
    ACE_ERROR_RETURN ((LM_ERROR,"TAO_AV_UDP_Flow_Handler::handle_input connection closed\n"),-1);
  this->frame_->wr_ptr (this->frame_->rd_ptr () + n);

  return this->callback_->receive_frame (this->frame_);
}

//  int
//  TimeStamp_Protocol_Object::handle_input (void)
//  {
//    iovec iov;
//    int iovcnt;
//    int n = this->transport_->recv (&iov, iovcnt);

//    int frame_size = BUFSIZ;

//    int begin = 1;
//    ACE_Message_Block* prev;
//    int iov_base = 0;
//    while (n >= frame_size)
//      {
//        ACE_DEBUG ((LM_DEBUG,
//                "(%N|%l) Frame Size %d %d\n",
//                n,
//                frame_size));

//        ACE_Message_Block* mb = 0;
//        ACE_NEW_RETURN (mb,
//                    ACE_Message_Block(frame_size),
//                    -1);

//        ACE_OS_String::memmove (mb->rd_ptr (), iov.iov_base, frame_size);
//        mb->wr_ptr (mb->rd_ptr () + frame_size);

//        //      iov_base += frame_size;

//        n -= frame_size;

//        if (begin)
//      {
//        prev = mb;
//        this->frame_ = mb;
//        begin = 0;
//      }
//        else
//      {
//        prev->cont (mb);
//        prev = mb;

//      }
//      }

//    if (n > 0)
//      if (begin)
//        {
//      ACE_DEBUG ((LM_DEBUG,
//                  "(%N|%l) Frame Size %d\n",
//                    n));
//      ACE_OS_String::memmove (this->frame_->rd_ptr (), iov.iov_base, n);
//      this->frame_->wr_ptr (this->frame_->rd_ptr () + n);
//        }
//      else
//        {

//      ACE_DEBUG ((LM_DEBUG,
//                  "(%N|%l) Frame Size %d\n",
//                  n));

//      ACE_Message_Block* mb = 0;
//      ACE_NEW_RETURN (mb,
//                      ACE_Message_Block (frame_size),
//                      -1);

//      ACE_OS_String::memmove (mb->rd_ptr (), iov.iov_base, n);
//      mb->wr_ptr (mb->rd_ptr () + n);
//      prev->cont (mb);
//        }

//    ACE_DEBUG ((LM_DEBUG,
//            "IOVEC SIZE %d %d\n",
//            n,
//            iov.iov_len));

//      if (n == -1)
//      ACE_ERROR_RETURN ((LM_ERROR,"TAO_AV_TCP_Flow_Handler::handle_input recv failed\n"),-1);
//    if (n == 0)
//      ACE_ERROR_RETURN ((LM_ERROR,"TAO_AV_TCP_Flow_Handler::handle_input connection closed\n"),-1);

//    return this->callback_->receive_frame (this->frame_);
//  }

/// send a data frame.
int
TimeStamp_Protocol_Object::send_frame (ACE_Message_Block *frame,
                                       TAO_AV_frame_info *)
{
  ACE_DEBUG ((LM_DEBUG,
              "TimeStamp_Protocol_Object::send_frame\n"));

  ACE_Message_Block* timestamp;
  ACE_NEW_RETURN (timestamp,
                  ACE_Message_Block (BUFSIZ),
                  -1);

  ACE_hrtime_t now = ACE_OS::gethrtime ();

  ACE_UINT64 usec = now;
  ACE_UINT32 val_1 = ACE_CU64_TO_CU32 (usec);
  ACE_DEBUG ((LM_DEBUG,
              "Time Stamp %u usecs\n",
              val_1));

  ACE_OS_String::memcpy (timestamp->wr_ptr (), &now, sizeof (now));
  timestamp->wr_ptr (sizeof (now));

  frame->cont (timestamp);

  ssize_t result = this->transport_->send (frame);
  if (result < 0)
    return result;
  return 0;
}

int
TimeStamp_Protocol_Object::send_frame (iovec const* iov,
                                       int iovcnt,
                                       TAO_AV_frame_info *)
{
  return this->transport_->send (iov,iovcnt);
}

int
TimeStamp_Protocol_Object::send_frame (const char* buf,
                                       size_t len)
{
  int result = this->transport_->send (buf, len, 0);
  if (result < 0)
    return result;
  return 0;
}

/// end the stream.
int
TimeStamp_Protocol_Object::destroy (void)
{
  this->callback_->handle_destroy ();
  return 0;
}


TimeStamp_Protocol_Factory::TimeStamp_Protocol_Factory (void)
{
}

TimeStamp_Protocol_Factory::~TimeStamp_Protocol_Factory (void)
{
}

int
TimeStamp_Protocol_Factory::init (int, ACE_TCHAR **)
{
  ACE_DEBUG ((LM_DEBUG,
              "TimeStamp_Protocol_Factory::init\n"));
  return 0;
}

int
TimeStamp_Protocol_Factory::match_protocol (const char *flow_string)
{
  ACE_DEBUG ((LM_DEBUG,
              "TimeStamp_Protocol_Factory::match_protocol\n"));
  if (ACE_OS::strcasecmp (flow_string,"TS") == 0)
    return 1;
  return 0;
}

TAO_AV_Protocol_Object*
TimeStamp_Protocol_Factory::make_protocol_object (TAO_FlowSpec_Entry *entry,
                                                  TAO_Base_StreamEndPoint *endpoint,
                                                  TAO_AV_Flow_Handler *handler,
                                                  TAO_AV_Transport *transport)
{
  TAO_AV_Callback *callback = 0;
  endpoint->get_callback (entry->flowname (),
                          callback);


  TimeStamp_Protocol_Object *object = 0;
  ACE_NEW_RETURN (object,
                  TimeStamp_Protocol_Object (callback,
                                             transport),
                  0);
  callback->open (object,
                  handler);
  endpoint->set_protocol_object (entry->flowname (),
                                 object);
  return object;
}

ACE_FACTORY_DEFINE (TAO_TS, TimeStamp_Protocol_Factory)
ACE_STATIC_SVC_DEFINE (TimeStamp_Protocol_Factory,
                       ACE_TEXT ("TimeStamp_Protocol_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TimeStamp_Protocol_Factory),
                       ACE_Service_Type::DELETE_THIS |
                       ACE_Service_Type::DELETE_OBJ,
                       0)







