#ifndef TIMESTAMP_H
#define TIMESTAMP_H
#include /**/ "ace/pre.h"

#include "orbsvcs/AV/Protocol_Factory.h"
#include "ace/Dynamic_Service.h"

// $Id$

class TAO_AV_Export TimeStamp_Protocol_Object : public TAO_AV_Protocol_Object
{
 public:
  TimeStamp_Protocol_Object (TAO_AV_Callback *callback,
                             TAO_AV_Transport *transport);

  virtual int handle_input (void);

  /// send a data frame.
  virtual int send_frame (ACE_Message_Block *frame,
                          TAO_AV_frame_info *frame_info = 0);

  virtual int send_frame (iovec const* iov,
                          int iovcnt,
                          TAO_AV_frame_info *frame_info = 0);

  virtual int send_frame (const char*buf,
                          size_t len);

  /// end the stream.
  virtual int destroy (void);

 private:
  /// Pre-allocated memory to receive the data...
  ACE_Message_Block* frame_;

};

class TAO_AV_Export TimeStamp_Protocol_Factory : public TAO_AV_Flow_Protocol_Factory
{
 public:
  /// Initialization hook.
  TimeStamp_Protocol_Factory (void);
  virtual ~TimeStamp_Protocol_Factory (void);
  virtual int init (int argc, char *argv[]);
  virtual int match_protocol (const char *flow_string);
  // Note : Some platforms still don't support Covariant returns
  virtual TAO_AV_Protocol_Object* make_protocol_object (TAO_FlowSpec_Entry *entry,
                                                        TAO_Base_StreamEndPoint *endpoint,
                                                        TAO_AV_Flow_Handler *handler,
                                                        TAO_AV_Transport *transport);
};

ACE_STATIC_SVC_DECLARE (TimeStamp_Protocol_Factory)
ACE_FACTORY_DECLARE (TAO_AV, TimeStamp_Protocol_Factory)

#include /**/ "ace/post.h"
#endif /*TIMESTAMP_H*/







