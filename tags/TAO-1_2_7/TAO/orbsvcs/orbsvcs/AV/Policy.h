/* -*- C++ -*- */


//=============================================================================
/**
 *  @file   Policy.h
 *
 *  $Id$
 *
 *  @author Nagarajan Surendran <naga@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_AV_POLICY_H
#define TAO_AV_POLICY_H
#include "ace/pre.h"

#include "ace/Addr.h"
#include "tao/PortableServer/ORB_Manager.h"
#include "orbsvcs/AV/AV_export.h"

struct TAO_AV_frame_info
{
  CORBA::Boolean boundary_marker;
  CORBA::Octet format;
  // @@ Shouldn't this be a string.
  CORBA::ULong timestamp;
  CORBA::ULong ssrc;
  CORBA::ULong sequence_num;
};

#define TAO_AV_SSRC_POLICY 100
#define TAO_AV_PAYLOAD_TYPE_POLICY 101
#define TAO_AV_TIMEOUT_POLICY 102
#define TAO_AV_RTCP_SDES_POLICY 103
#define TAO_AV_SFP_CREDIT_POLICY 104

struct TAO_AV_RTCP_Sdes
{
  CORBA::String_var name_;
  CORBA::String_var value_;
};

class TAO_AV_Export TAO_AV_Policy
{
public:
  TAO_AV_Policy (CORBA::ULong type);
  CORBA::ULong type (void);
protected:
  CORBA::ULong type_;
};

class  TAO_AV_Export TAO_AV_SSRC_Policy : public TAO_AV_Policy
{
public:
  TAO_AV_SSRC_Policy (CORBA::ULong ssrc = 0);
  CORBA::ULong value (void);
  void value (CORBA::ULong ssrc);
protected:
  CORBA::ULong ssrc_;
};

class  TAO_AV_Export TAO_AV_Payload_Type_Policy : public TAO_AV_Policy
{
public:
  TAO_AV_Payload_Type_Policy (int payload_type = -1);
  int value (void);
  void value (int pt);
protected:
  int payload_type_;
};

class TAO_AV_Export TAO_AV_RTCP_Sdes_Policy : public TAO_AV_Policy
{
public:
  TAO_AV_RTCP_Sdes_Policy (void);
  TAO_AV_RTCP_Sdes &value (void);
  void value (const TAO_AV_RTCP_Sdes& sdes_val);
protected:
  TAO_AV_RTCP_Sdes sdes_;
};

class TAO_AV_Export TAO_AV_SFP_Credit_Policy : public TAO_AV_Policy
{
public:
  TAO_AV_SFP_Credit_Policy (void);
  int value (void);
  void value (int val);
protected:
  int value_;
};

typedef TAO_Unbounded_Sequence<TAO_AV_Policy*> TAO_AV_PolicyList;

class TAO_AV_Protocol_Object;
class TAO_AV_Transport;
class TAO_AV_Flow_Handler;

/**
 * @class TAO_AV_Callback
 *
 * @brief Callback class that the user will be implementing for receiving
 * frames from the network and also for timer events.
 */
class TAO_AV_Export TAO_AV_Callback
{
public:
  TAO_AV_Callback (void);
  virtual ~TAO_AV_Callback (void);

  /// Called for opening the callback.
  int open (TAO_AV_Protocol_Object *object,
            TAO_AV_Flow_Handler *handler);

  /// Called during Streamctrl->start.
  virtual int handle_start (void);

  /// Called during Streamctrl->stop.
  virtual int handle_stop (void);

  /// Called during timeout for Flow Producers.
  virtual int handle_timeout (void *arg);

  virtual int schedule_timer (void);

  virtual int receive_frame (ACE_Message_Block *frame,
                             TAO_AV_frame_info *frame_info = 0,
                             const ACE_Addr &address = ACE_Addr::sap_any);

  // Called when a frame arrives for a FlowConsumer.

  /// address from which the frame was received.
  virtual int receive_control_frame (ACE_Message_Block *frame,
                                     const ACE_Addr &address = ACE_Addr::sap_any);

  /// Called during Streamctrl->destroy i.e tear_down  of the stream
  virtual int handle_destroy (void);

  /**
   * Called to get the timeout. If tv is 0 then the framework stop
   * calling this. This will be called during the start of the frame
   * and also if schedule_timer is called to get the timeout.
   */
  virtual void get_timeout (ACE_Time_Value *&tv,
                            void *&arg);

  /// Accessor to protocol object.
  TAO_AV_Protocol_Object *protocol_object (void);

  /// get the policies for the protocol object.
  virtual TAO_AV_PolicyList get_policies (void);
protected:
  TAO_AV_Protocol_Object *protocol_object_;
  TAO_AV_Flow_Handler *handler_;
};

#if defined(__ACE_INLINE__)
#include "Policy.i"
#endif /* __ACE_INLINE__ */
#include "ace/post.h"
#endif /* TAO_AV_POLICY_H */
