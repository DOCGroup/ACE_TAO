/* -*- C++ -*- */

// $Id$

// ============================================================================
//
// = LIBRARY
//   ORBSVCS AVStreams
//
// = FILENAME
//   Policy.h
//
// = AUTHOR
//    Nagarajan Surendran <naga@cs.wustl.edu>
//
//
// ============================================================================

#ifndef TAO_AV_POLICY_H
#define TAO_AV_POLICY_H

#include "ace/Addr.h"
#include "tao/TAO.h"
#include "tao/debug.h"
#include "orbsvcs/orbsvcs_export.h"

struct TAO_AV_frame_info
{
  CORBA::Boolean boundary_marker;
  CORBA::Octet format;
  // @@ Shouldn't this be a string.
  CORBA::ULong timestamp;
  CORBA::ULong ssrc;
  CORBA::ULong sequence_num;
};

class TAO_ORBSVCS_Export TAO_AV_Policy
{
public:
  struct sdes
  {
    CORBA::String_var name_;
    CORBA::String_var value_;
  };

  enum PolicyType
  {
    TAO_AV_SSRC_POLICY,
    TAO_AV_PAYLOAD_TYPE_POLICY,
    TAO_AV_TIMEOUT_POLICY,
    TAO_AV_RTCP_SDES_POLICY,
    TAO_AV_TIMESTAMP_POLICY
  };

  TAO_AV_Policy (PolicyType type);
  PolicyType type (void);
protected:
  PolicyType type_;
};

class TAO_AV_SSRC_Policy
  :public TAO_AV_Policy
{
public:
  TAO_AV_SSRC_Policy (CORBA::ULong ssrc = 0);
  CORBA::ULong value (void);
  void value (CORBA::ULong ssrc);
protected:
  CORBA::ULong ssrc_;
};

class TAO_AV_Payload_Type_Policy
  :public TAO_AV_Policy
{
public:
  TAO_AV_Payload_Type_Policy (int payload_type = -1);
  int value (void);
  void value (int pt);
protected:
  int payload_type_;
};


class TAO_AV_RTCP_Sdes_Policy
  :public TAO_AV_Policy
{
public:
  TAO_AV_RTCP_Sdes_Policy (void);
  TAO_AV_Policy::sdes &value (void);
  void value (const TAO_AV_Policy::sdes& sdes_val);
protected:
  TAO_AV_Policy::sdes sdes_;
};

class TAO_AV_Timestamp_Policy
  :public TAO_AV_Policy
{
public:
  TAO_AV_Timestamp_Policy (void);
  ACE_UINT32 value (void);
  void value (ACE_UINT32 timestamp);
protected:
  ACE_UINT32 timestamp_;
};

typedef TAO_Unbounded_Sequence<TAO_AV_Policy*> PolicyList;

class TAO_AV_Policy_Manager
{
public:
  TAO_AV_Policy *create_policy (TAO_AV_Policy::PolicyType type,
                                void *value);
};

class TAO_AV_Protocol_Object;
class TAO_AV_Transport;
class TAO_AV_Flow_Handler;

class TAO_AV_Callback
{
  // = TITLE
  //     Callback class that the user will be implementing for receiving
  //     frames from the network and also for timer events.
public:
  TAO_AV_Callback (void);

  int open (TAO_AV_Protocol_Object *object,
            TAO_AV_Flow_Handler *handler);
  // Called for opening the callback.

  virtual int handle_start (void);
  // Called during Streamctrl->start.

  virtual int handle_stop (void);
  // Called during Streamctrl->stop.

  virtual int handle_timeout (void *arg);
  // Called during timeout for Flow Producers.

  virtual int schedule_timer (void);

  virtual int receive_frame (ACE_Message_Block *frame,
                             TAO_AV_frame_info *frame_info = 0,
                             const ACE_Addr &address = ACE_Addr::sap_any);

  // Called when a frame arrives for a FlowConsumer.

  virtual int receive_control_frame (ACE_Message_Block *frame,
                                     const ACE_Addr &address = ACE_Addr::sap_any);
  // address from which the frame was received.

  virtual int handle_destroy (void);
  // Called during Streamctrl->destroy i.e tear_down  of the stream

  virtual void get_timeout (ACE_Time_Value *&tv,
                            void *&arg);
  // Called to get the timeout. If tv is 0 then the framework stop
  // calling this. This will be called during the start of the frame
  // and also if schedule_timer is called to get the timeout.

  TAO_AV_Protocol_Object *protocol_object (void);
  // Accessor to protocol object.
protected:
  TAO_AV_Protocol_Object *protocol_object_;
  TAO_AV_Flow_Handler *handler_;
};

#if defined(__ACE_INLINE__)
#include "Policy.i"
#endif /* __ACE_INLINE__ */
#endif /* TAO_AV_POLICY_H */
