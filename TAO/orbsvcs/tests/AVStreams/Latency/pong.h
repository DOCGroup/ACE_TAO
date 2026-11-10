//=============================================================================
/**
 *  @file   pong.h
 *
 *  @author Carlos O'Ryan
 */
//=============================================================================

#ifndef TAO_PONG_H
#define TAO_PONG_H

#include "orbsvcs/AV/AVStreams_i.h"
#include "orbsvcs/AV/Policy.h"
#include "orbsvcs/AV/Flows_T.h"

class Pong_Recv_Callback : public TAO_AV_Callback
{
public:
  Pong_Recv_Callback () = default;
  Pong_Recv_Callback (Pong_Recv_Callback const &) = delete;
  Pong_Recv_Callback &operator= (Pong_Recv_Callback const &) = delete;
  Pong_Recv_Callback (Pong_Recv_Callback &&) = delete;
  Pong_Recv_Callback &operator= (Pong_Recv_Callback &&) = delete;

  int handle_stop () override;
  int receive_frame (ACE_Message_Block *frame,
                     TAO_AV_frame_info *frame_info = nullptr,
                     ACE_Addr const &peer_address = ACE_Addr::sap_any) override;
  int handle_destroy () override;
};

class Pong_Recv : public TAO_FlowConsumer
{
public:
  Pong_Recv ();
  Pong_Recv (Pong_Recv const &) = delete;
  Pong_Recv &operator= (Pong_Recv const &) = delete;
  Pong_Recv (Pong_Recv &&) = delete;
  Pong_Recv &operator= (Pong_Recv &&) = delete;

  int get_callback (char const *flowname,
                    TAO_AV_Callback *&callback) override;

private:
  /// The callback object.
  Pong_Recv_Callback callback_;
};

class Ping_Send_Callback : public TAO_AV_Callback
{
public:
  Ping_Send_Callback ();
  Ping_Send_Callback (Ping_Send_Callback const &) = delete;
  Ping_Send_Callback &operator= (Ping_Send_Callback const &) = delete;
  Ping_Send_Callback (Ping_Send_Callback &&) = delete;
  Ping_Send_Callback &operator= (Ping_Send_Callback &&) = delete;

  int handle_timeout (void *arg) override;
  void get_timeout (ACE_Time_Value *&tv,
                    void *&arg) override;

private:
  /// The timeout value.
  ACE_Time_Value timeout_;

  /// Preallocated message block for sending.
  ACE_Message_Block frame_;
  int count_ {};
};

class Ping_Send : public TAO_FlowProducer
{
public:
  Ping_Send ();
  Ping_Send (Ping_Send const &) = delete;
  Ping_Send &operator= (Ping_Send const &) = delete;
  Ping_Send (Ping_Send &&) = delete;
  Ping_Send &operator= (Ping_Send &&) = delete;

  int get_callback (char const *flowname,
                    TAO_AV_Callback *&callback) override;

private:
  /// The callback object...
  Ping_Send_Callback callback_;
};

using Reactive_Strategy = TAO_AV_Endpoint_Reactive_Strategy_A
  <TAO_StreamEndPoint_A, TAO_VDev, AV_Null_MediaCtrl>;

using Pong_Recv_FDev = TAO_FDev<TAO_FlowProducer, Pong_Recv>;
using Ping_Send_FDev = TAO_FDev<Ping_Send, TAO_FlowConsumer>;

#endif /* TAO_PONG_H */
