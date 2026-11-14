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
  Ping_Recv_Callback () = default;
  Ping_Recv_Callback (Ping_Recv_Callback const &) = delete;
  Ping_Recv_Callback &operator= (Ping_Recv_Callback const &) = delete;
  Ping_Recv_Callback (Ping_Recv_Callback &&) = delete;
  Ping_Recv_Callback &operator= (Ping_Recv_Callback &&) = delete;

  int handle_stop () override;
  int receive_frame (ACE_Message_Block *frame,
                     TAO_AV_frame_info *frame_info = nullptr,
                     ACE_Addr const &peer_address = ACE_Addr::sap_any) override;
  int handle_destroy () override;

protected:
  int count_ {};
};

class Ping_Recv : public TAO_FlowConsumer
{
public:
  Ping_Recv ();
  Ping_Recv (Ping_Recv const &) = delete;
  Ping_Recv &operator= (Ping_Recv const &) = delete;
  Ping_Recv (Ping_Recv &&) = delete;
  Ping_Recv &operator= (Ping_Recv &&) = delete;

  int get_callback (char const *flowname,
                    TAO_AV_Callback *&callback) override;

private:
  /// The callback object.
  Ping_Recv_Callback callback_;
};

class Pong_Send_Callback : public TAO_AV_Callback
{
public:
  Pong_Send_Callback () = default;
  Pong_Send_Callback (Pong_Send_Callback const &) = delete;
  Pong_Send_Callback &operator= (Pong_Send_Callback const &) = delete;
  Pong_Send_Callback (Pong_Send_Callback &&) = delete;
  Pong_Send_Callback &operator= (Pong_Send_Callback &&) = delete;

  /// Ad-hoc method to send a response outside the context of a
  /// handle_timeout.
  int send_response (ACE_hrtime_t stamp);

  int handle_timeout (void *arg) override;
  void get_timeout (ACE_Time_Value *&tv,
                    void *&arg) override;
};

class Pong_Send : public TAO_FlowProducer
{
public:
  Pong_Send ();
  Pong_Send (Pong_Send const &) = delete;
  Pong_Send &operator= (Pong_Send const &) = delete;
  Pong_Send (Pong_Send &&) = delete;
  Pong_Send &operator= (Pong_Send &&) = delete;

  int get_callback (char const *flowname,
                    TAO_AV_Callback *&callback) override;
};

using Reactive_Strategy = TAO_AV_Endpoint_Reactive_Strategy_B
  <TAO_StreamEndPoint_B, TAO_VDev, AV_Null_MediaCtrl>;

using Ping_Recv_FDev = TAO_FDev<TAO_FlowProducer, Ping_Recv>;
using Pong_Send_FDev = TAO_FDev<Pong_Send, TAO_FlowConsumer>;

#endif /* TAO_PING_H */
