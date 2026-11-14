#ifndef TIMESTAMP_H
#define TIMESTAMP_H
#include /**/ "ace/pre.h"

#include <cstddef>
#include <memory>

#include "export.h"

#include "orbsvcs/AV/Protocol_Factory.h"

#include "ace/Dynamic_Service.h"
#include "ace/Service_Config.h"

class TAO_TS_Export TimeStamp_Protocol_Object : public TAO_AV_Protocol_Object
{
public:
  TimeStamp_Protocol_Object (TAO_AV_Callback *callback,
                             TAO_AV_Transport *transport);
  ~TimeStamp_Protocol_Object () override;
  TimeStamp_Protocol_Object (TimeStamp_Protocol_Object const &) = delete;
  TimeStamp_Protocol_Object &operator= (TimeStamp_Protocol_Object const &) = delete;
  TimeStamp_Protocol_Object (TimeStamp_Protocol_Object &&) = delete;
  TimeStamp_Protocol_Object &operator= (TimeStamp_Protocol_Object &&) = delete;

  int handle_input () override;

  /// send a data frame.
  int send_frame (ACE_Message_Block *frame,
                  TAO_AV_frame_info *frame_info = nullptr) override;

  int send_frame (iovec const *iov,
                  int iovcnt,
                  TAO_AV_frame_info *frame_info = nullptr) override;

  int send_frame (char const *buf,
                  size_t len) override;

  /// end the stream.
  int destroy () override;

private:
  std::unique_ptr<ACE_Message_Block> frame_;
};

class TAO_TS_Export TimeStamp_Protocol_Factory : public TAO_AV_Flow_Protocol_Factory
{
public:
  /// Initialization hook.
  TimeStamp_Protocol_Factory ();
  ~TimeStamp_Protocol_Factory () override;
  TimeStamp_Protocol_Factory (TimeStamp_Protocol_Factory const &) = delete;
  TimeStamp_Protocol_Factory &operator= (TimeStamp_Protocol_Factory const &) = delete;
  TimeStamp_Protocol_Factory (TimeStamp_Protocol_Factory &&) = delete;
  TimeStamp_Protocol_Factory &operator= (TimeStamp_Protocol_Factory &&) = delete;
  int init (int argc, ACE_TCHAR *argv[]) override;
  int match_protocol (char const *flow_string) override;
  // Note : Some platforms still don't support Covariant returns
  TAO_AV_Protocol_Object *make_protocol_object (TAO_FlowSpec_Entry *entry,
                                                 TAO_Base_StreamEndPoint *endpoint,
                                                 TAO_AV_Flow_Handler *handler,
                                                 TAO_AV_Transport *transport) override;
};

ACE_STATIC_SVC_DECLARE (TimeStamp_Protocol_Factory)
ACE_FACTORY_DECLARE (TAO_TS, TimeStamp_Protocol_Factory)

#include /**/ "ace/post.h"
#endif /*TIMESTAMP_H*/
