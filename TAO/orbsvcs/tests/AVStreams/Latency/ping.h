
//=============================================================================
/**
 *  @file   ping.h
 *
 *  @author Carlos O'Ryan
 */
//=============================================================================


#ifndef TAO_PING_H
#define TAO_PING_H

#include "orbsvcs/AV/AVStreams_i.h"
#include "orbsvcs/AV/Policy.h"
#include "orbsvcs/AV/Flows_T.h"
#include "ace/OS_NS_time.h"

class Ping_Recv_Callback : public TAO_AV_Callback
{
public:
  Ping_Recv_Callback ();
  int handle_stop () override;
  int receive_frame (ACE_Message_Block *frame,
                     TAO_AV_frame_info *frame_info = nullptr,
                     ACE_Addr const &peer_address = ACE_Addr::sap_any) override;
  int handle_destroy () override;
 protected:
  int count_;
};

class Ping_Recv : public TAO_FlowConsumer
{
public:
  Ping_Recv ();

  int get_callback (const char *flowname,
                    TAO_AV_Callback *&callback) override;

private:
  /// The callback object...
  Ping_Recv_Callback callback_;
};

class Pong_Send_Callback : public TAO_AV_Callback
{
public:
  /// Ad-hoc method to send a response outside the context of a
  /// handle_timeout.
  int send_response (ACE_hrtime_t stamp);

  int handle_timeout (void *arg) override;
  int handle_end_stream ();
  void get_timeout (ACE_Time_Value *&tv,
                    void *&arg) override;
};

class Pong_Send : public TAO_FlowProducer
{
public:
  Pong_Send ();
  int get_callback (const char *flowname,
                    TAO_AV_Callback *&callback) override;
};

using Reactive_Strategy = TAO_AV_Endpoint_Reactive_Strategy_B
  <TAO_StreamEndPoint_B, TAO_VDev, AV_Null_MediaCtrl>;

using Ping_Recv_FDev = TAO_FDev<TAO_FlowProducer, Ping_Recv>;
using Pong_Send_FDev = TAO_FDev<Pong_Send, TAO_FlowConsumer>;

#endif /* TAO_PING_H */
