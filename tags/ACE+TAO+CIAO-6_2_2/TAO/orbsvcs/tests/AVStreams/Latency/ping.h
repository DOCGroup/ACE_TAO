
//=============================================================================
/**
 *  @file   ping.h
 *
 *  $Id$
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
  Ping_Recv_Callback (void);
  virtual int handle_stop (void);
  virtual int receive_frame (ACE_Message_Block *frame,
                             TAO_AV_frame_info *frame_info = 0,
                             const ACE_Addr &peer_address = ACE_Addr::sap_any);
  virtual int handle_destroy (void);
 protected:
  int count_;
};

class Ping_Recv : public TAO_FlowConsumer
{
public:
  Ping_Recv (void);

  virtual int get_callback (const char *flowname,
                            TAO_AV_Callback *&callback);

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

  virtual int handle_timeout (void *arg);
  virtual int handle_end_stream (void);
  virtual void get_timeout (ACE_Time_Value *&tv,
                            void *&arg);
};

class Pong_Send : public TAO_FlowProducer
{
public:
  Pong_Send (void);
  virtual int get_callback (const char *flowname,
                            TAO_AV_Callback *&callback);
};

typedef TAO_AV_Endpoint_Reactive_Strategy_B <TAO_StreamEndPoint_B,TAO_VDev,AV_Null_MediaCtrl> Reactive_Strategy;

typedef TAO_FDev<TAO_FlowProducer,Ping_Recv> Ping_Recv_FDev;
typedef TAO_FDev<Pong_Send,TAO_FlowConsumer> Pong_Send_FDev;

#endif /* TAO_PING_H */
