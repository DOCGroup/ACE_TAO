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

#include "tao/TAO.h"
#include "Transport.h"

struct TAO_AV_frame_info
{
  CORBA::Boolean boundary_marker;
  CORBA::Octet format;
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

class TAO_AV_Callback
{
public:
  virtual int handle_start (void);
  virtual int handle_stop (void);
  virtual int receive_frame (ACE_Message_Block *frame);
  virtual int handle_end_stream (void);
  TAO_AV_Transport *transport (void);
  void transport (TAO_AV_Transport *transport);
protected:
  TAO_AV_Transport *transport_;
};

class TAO_AV_Protocol_Object
{
public:
  TAO_AV_Protocol_Object (TAO_AV_Callback *callback,
                          TAO_AV_Transport *transport = 0);
  // constructor.

  virtual ~TAO_AV_Protocol_Object (void);
  // Destructor

  virtual int set_policies (const PolicyList &policy_list);
  virtual PolicyList get_policies (void);
  // set/get policies.

  virtual int start (void);
  virtual int stop (void);
  // start/stop the flow.

  virtual int send_frame (ACE_Message_Block *frame,
                          TAO_AV_frame_info *frame_info = 0) = 0;
  // send a data frame.

  void transport (TAO_AV_Transport *transport);
  TAO_AV_Transport *transport (void);
  // set/get the transport.

  virtual int end_stream (void) = 0;
  // end the stream.
protected:
  TAO_AV_Transport *transport_;
  PolicyList policy_list_;
  TAO_AV_Callback *callback_;
};

#if defined(__ACE_INLINE__)
#include "Policy.i"
#endif /* __ACE_INLINE__ */
#endif /* TAO_AV_POLICY_H */
