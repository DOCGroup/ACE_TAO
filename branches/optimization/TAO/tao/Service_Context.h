// This may look like C, but it's really -*- C++ -*-
// -*- C++ -*-

// ===================================================================
/**
 *  @file   Service_Context.h
 *
 *  $Id$
 *
 *  @author Balachandran Natarajan <bala@cs.wustl.edu>
 *
 */
// ===================================================================

#ifndef TAO_SERVICE_CONTEXT_H
#define TAO_SERVICE_CONTEXT_H
#include "ace/pre.h"

#include "tao/IOPC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


/**
 * @class Service_Context.h
 *
 * @brief Helper class for managing the service context list
 *  information.
 *
 * This class is used to manipulate and access the service context
 * list that is passed around with every GIOP request/reply. The
 * definition of the service context list is simply a sequence of the
 * following structures:
 * typedef unsigned long ServiceId;
 * struct ServiceContext
 * {
 *   ServiceId          context_id;
 *   sequence <octet>   context_data;
 * };
 * typedef sequence <ServiceContext> ServiceContextList;
 *
 * the real motivation behind this class is to consolidate all the
 * marshalling and unmarshalling information pertaining to service
 * context list
 *
 * Note: Somewhere down the line we may want to use this class for
 * fast access to the info that we need from the Service Context
 * List.
 */


class TAO_Export TAO_Service_Context
{
public:
  /// Constructor
  TAO_Service_Context (void);


  /// = Generic components

  /// Insert the component into the list, making a copy of the octet
  /// sequence.
  void set_context (const IOP::ServiceContext &context);

  /// Insert the component into the list, but efficiently stealing the
  /// contents of the octet sequence.
  void set_context (IOP::ServiceContext &context);

  /// Get a copy of the context identified by <context.context_id>, return
  /// 0 if the component is not present.
  int get_context (IOP::ServiceContext &context) const;

  /// Set the context from the CDR stream and add that to the service
  /// Context list
  void set_context (IOP::ServiceId id, TAO_OutputCDR &cdr);

  /// Set the context from the CDR stream and return the context back
  /// to the caller. *Does not* modify the underlying service context
  /// list.
  void set_context (IOP::ServiceContext &context, TAO_OutputCDR &cdr);

  /// Is the <id> available in the underlying service context list? If
  /// so return 1, else return 0
  int is_service_id (IOP::ServiceId id);

  /// = Marshaling and demarshaling the list
  int encode (TAO_OutputCDR& cdr) const;
  int decode (TAO_InputCDR& cdr);

  /// Return the underlying service context list
  IOP::ServiceContextList &service_info (void);
  const IOP::ServiceContextList &service_info (void) const;

  // @@ Note: The above method is only for backward comptiblity. We
  // need to get this removed once RT folks have their service
  // addition info done through this interface

private:

  /// Helper methods to implement set_context()
  void set_context_i (const IOP::ServiceContext &context);
  void set_context_i (IOP::ServiceContext &context);
  void add_context_i (const IOP::ServiceContext &context);
  void add_context_i (IOP::ServiceContext &context);
  void set_context_i (IOP::ServiceId id, TAO_OutputCDR &cdr);
  void set_context_i (IOP::ServiceContext &context, TAO_OutputCDR &cdr);

  /// Helper methods to implement set_context()
  int get_context_i (IOP::ServiceContext &context) const;

private:

  /// The ServiceContextList info
  IOP::ServiceContextList service_context_;
};

#if defined (__ACE_INLINE__)
# include "tao/Service_Context.inl"
#endif /* ! __ACE_INLINE__ */


#include "ace/post.h"
#endif  /* TAO_SERVICE_CONTEXT_H */
