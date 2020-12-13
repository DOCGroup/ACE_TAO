// -*- C++ -*-

//=============================================================================
/**
 *  @file     Bind_Dispatcher_Guard.h
 *
 *  @author  Carlos O'Ryan <coryan@uci.edu>
 *  @author  Chad Elliott <elliott_c@ociweb.com>
 */
//=============================================================================

#ifndef TAO_BIND_DISPATCHER_GUARD_H
#define TAO_BIND_DISPATCHER_GUARD_H

#include /**/ "ace/pre.h"

#include "Basic_Types.h"
#include /**/ "TAO_Export.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Reply_Dispatcher;
class TAO_Transport_Mux_Strategy;

class TAO_Export TAO_Bind_Dispatcher_Guard
{
public:
  TAO_Bind_Dispatcher_Guard (CORBA::ULong request_id,
                             TAO_Reply_Dispatcher* rd,
                             TAO_Transport_Mux_Strategy* tms);

  ~TAO_Bind_Dispatcher_Guard ();

  /// Returns 0 if successful and non-zero otherwise.
  int unbind_dispatcher ();

  /// State information
  enum TAO_Bind_Dispatcher_Status
    {
      /// Unbind the dispatcher
      UNBIND = 0,
      /// Do not unbind dispatcher
      NO_UNBIND
    };

  /// Get status
  TAO_Bind_Dispatcher_Status status () const;

  /// Set status
  void status (TAO_Bind_Dispatcher_Status status);

private:
  TAO_Bind_Dispatcher_Status  status_;
  CORBA::ULong                request_id_;
  TAO_Transport_Mux_Strategy* tms_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "Bind_Dispatcher_Guard.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif
