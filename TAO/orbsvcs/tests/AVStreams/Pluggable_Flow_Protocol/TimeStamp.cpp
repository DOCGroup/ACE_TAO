#include "TimeStamp.h"

#include "orbsvcs/AV/AVStreams_i.h"
#include "ace/High_Res_Timer.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_strings.h"

#include <algorithm>

TimeStamp_Protocol_Object::TimeStamp_Protocol_Object (
    TAO_AV_Callback *callback,
    TAO_AV_Transport *transport)
  : TAO_AV_Protocol_Object (callback, transport),
    frame_ (std::make_unique<ACE_Message_Block> (
      std::max (static_cast<size_t> (BUFSIZ),
                4 * static_cast<size_t> (this->transport_->mtu ())) +
      sizeof (ACE_hrtime_t)))
{
}

TimeStamp_Protocol_Object::~TimeStamp_Protocol_Object () = default;

int
TimeStamp_Protocol_Object::handle_input ()
{
  this->frame_->reset ();
  ssize_t const bytes_received = this->transport_->recv (this->frame_->wr_ptr (),
                                                         this->frame_->size ());
  if (bytes_received < 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "TimeStamp_Protocol_Object::handle_input recv failed\n"),
                      -1);
  if (bytes_received == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "TimeStamp_Protocol_Object::handle_input connection closed\n"),
                      -1);

  this->frame_->wr_ptr (bytes_received);
  return this->callback_->receive_frame (this->frame_.get ());
}

int
TimeStamp_Protocol_Object::send_frame (ACE_Message_Block *frame,
                                       TAO_AV_frame_info *)
{
  ACE_hrtime_t const timestamp_value = ACE_OS::gethrtime ();
  ACE_Message_Block timestamp (sizeof timestamp_value);
  ACE_OS::memcpy (timestamp.wr_ptr (), &timestamp_value, sizeof timestamp_value);
  timestamp.wr_ptr (sizeof timestamp_value);

  ACE_Message_Block * const continuation = frame->cont ();
  frame->cont (&timestamp);
  ssize_t const result = this->transport_->send (frame);
  frame->cont (continuation);
  return result < 0 ? static_cast<int> (result) : 0;
}

int
TimeStamp_Protocol_Object::send_frame (iovec const *iov,
                                       int iovcnt,
                                       TAO_AV_frame_info *)
{
  return this->transport_->send (iov, iovcnt);
}

int
TimeStamp_Protocol_Object::send_frame (char const *buffer,
                                       size_t length)
{
  int const result = this->transport_->send (buffer, length, 0);
  return result < 0 ? result : 0;
}

int
TimeStamp_Protocol_Object::destroy ()
{
  this->callback_->handle_destroy ();
  return 0;
}

TimeStamp_Protocol_Factory::TimeStamp_Protocol_Factory () = default;

TimeStamp_Protocol_Factory::~TimeStamp_Protocol_Factory () = default;

int
TimeStamp_Protocol_Factory::init (int,
                                  ACE_TCHAR *[])
{
  return 0;
}

int
TimeStamp_Protocol_Factory::match_protocol (char const *flow_string)
{
  return ACE_OS::strcasecmp (flow_string, "TS") == 0;
}

TAO_AV_Protocol_Object *
TimeStamp_Protocol_Factory::make_protocol_object (TAO_FlowSpec_Entry *entry,
                                                  TAO_Base_StreamEndPoint *endpoint,
                                                  TAO_AV_Flow_Handler *handler,
                                                  TAO_AV_Transport *transport)
{
  TAO_AV_Callback *callback {};
  if (endpoint->get_callback (entry->flowname (), callback) != 0 || callback == nullptr)
    return nullptr;

  TimeStamp_Protocol_Object *object {};
  ACE_NEW_RETURN (object,
                  TimeStamp_Protocol_Object (callback, transport),
                  nullptr);
  callback->open (object, handler);
  endpoint->set_protocol_object (entry->flowname (), object);
  endpoint->protocol_object_set ();
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
