// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    Server_Request.h
//
// = DESCRIPTION
//    Header file for CORBA's Dynamic Server Skeleton Interface's
//    "Server Request" type. (for UIOP)
//
// = AUTHOR
//     Copyright 1994-1995 by Sun Microsystems, Inc. and Chris Cleeland.
//     Modifications by Aniruddha Gokhale based on CORBAv2.2 Feb 98
//     UIOP modifications by Ossama Othman -- Apr 99
// ============================================================================

#ifndef TAO_UIOP_SERVER_REQUEST_H
#define TAO_UIOP_SERVER_REQUEST_H

#include "tao/Server_Request.h"

class TAO_Export UIOP_ServerRequest : public CORBA_ServerRequest
{
  // = TITLE
  //    Class representing an UIOP ServerRequest object.
public:
  // = Initialization and termination methods.
  UIOP_ServerRequest (TAO_InputCDR &input,
                      TAO_OutputCDR &output,
                      TAO_ORB_Core *orb_core,
                      CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ());
  // Constructor
  UIOP_ServerRequest (CORBA::ULong &request_id,
                      CORBA::Boolean &response_expected,
                      TAO_ObjectKey &object_key,
                      const ACE_CString &operation,
                      TAO_OutputCDR &output,
                      TAO_ORB_Core *orb_core,
                      CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ());

  virtual ~UIOP_ServerRequest (void);
  // Destructor.

#if !defined (TAO_HAS_MINIMUM_CORBA)

  // = General ServerRequest operations
  void arguments (CORBA::NVList_ptr &list,
                  CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ());

  void set_result (const CORBA::Any &value,
                   CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ());

  void set_exception (const CORBA::Any &value,
                      CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ());

  virtual void dsi_marshal (CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ());
  // does the marshaling of outgoing parameters and is used by the DSI based
  // scheme

#endif /* TAO_HAS_MINIMUM_CORBA */

  // = Request attributes.

  const char *operation (void) const;
  // return the operation name

  unsigned int operation_length (void) const;
  // return the legnth of the operation

  //  CORBA::Context_ptr ctx (void);
  // return the context pointer

  // = TAO extensions

  CORBA::ORB_ptr  orb (void);
  // return the underlying ORB

  TAO_POA *oa (void);
  // return the Object Adapter

  //
  // = TAO extensions
  // meant to be used internally.
  //

  virtual void demarshal (CORBA_Environment &orb_env,
                          const TAO_Call_Data_Skel *info,
                          ...);
  // demarshal incoming parameters. Used by the SII skeleton (i.e., the IDL
  // compiler generated skeleton)

  virtual void marshal (CORBA_Environment &orb_env,
                        //                        CORBA_Environment &skel_env,
                        const TAO_Call_Data_Skel *info,
                        ...);
  // marshal outgoing parameters and return value. This is used by the SSI
  // i.e., by the IDL compiler generated skeletons.

  virtual void init_reply (CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ());
  // start a Reply message

  virtual TAO_InputCDR &incoming (void);
  // Retrieve the incoming stream.

  virtual TAO_OutputCDR &outgoing (void);
  // Retrieve the outgoing stream.

  virtual CORBA::Boolean response_expected (void) const;
  // is the response expected

  virtual CORBA::Principal_ptr principal (void) const;

  virtual const TAO_ObjectKey &object_key (void) const;

  virtual const TAO_GIOP_ServiceContextList &service_info (void) const;

  // The pseudo object methods, not really needed because the class is
  // not in the spec, but we add them for the sake of completeness.
  static UIOP_ServerRequest* _duplicate (UIOP_ServerRequest*);
  static UIOP_ServerRequest* _nil (void);

  // To handle System Exceptions at the lowest level,
  // a method returning the request_id_ is needed.

  CORBA::ULong request_id (void);

  CORBA::Object_ptr forward_location (void);
  // get the forward_location

  CORBA::ULong exception_type (void);
  // get the exception type

private:
   void parse_header (CORBA::Environment &TAO_IN_ENV);
  // Parse the request header and store the result on this object.

  void parse_header_std (CORBA::Environment &TAO_IN_ENV);
  // Parse the standard UIOP request header and store the result on
  // this object.

  void parse_header_lite (CORBA::Environment &TAO_IN_ENV);
  // Parse the lightweight version of the UIOP request header and
  // store the result on this object.

private:
  ACE_CString operation_;
  // Operation name.

  CORBA::Object_var forward_location_;

  TAO_InputCDR *incoming_;
  // Incoming stream.

  TAO_OutputCDR *outgoing_;
  // Outgoing stream.

  CORBA::Boolean response_expected_;
  // is it oneway or twoway

#if !defined (TAO_HAS_MINIMUM_CORBA)

  CORBA::NVList_ptr params_;
  // Incoming parameters.

#endif /* TAO_HAS_MINIMUM_CORBA */

  CORBA::Any_ptr retval_;
  // Return value.

  CORBA::Any_ptr exception_;
  // Any exception which might be raised.

  //  TAO_GIOP_ReplyStatusType exception_type_;
  CORBA::ULong exception_type_;
  // exception type (will be NO_EXCEPTION in the majority of the cases)

  TAO_ORB_Core* orb_core_;
  // A pointer to the ORB Core for the context where the request was
  // created.

  TAO_GIOP_ServiceContextList service_info_;
  // The service context for the request (CORBA Reference?)

  CORBA::ULong request_id_;
  // Unique identifier for a request

  TAO_ObjectKey object_key_;
  // The object key of the destination object.

  CORBA::Principal_ptr requesting_principal_;
  // Identifies the requester
};

#if defined (__ACE_INLINE__)
# include "tao/UIOP_Server_Request.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_UIOP_SERVER_REQUEST_H */
