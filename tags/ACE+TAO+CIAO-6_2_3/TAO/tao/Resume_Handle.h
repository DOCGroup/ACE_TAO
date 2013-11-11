// -*- C++ -*-

//=============================================================================
/**
 *  @file Resume_Handle.h
 *
 *  $Id$
 *
 *  @author Balachandran Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_RESUME_HANDLE_H
#define TAO_RESUME_HANDLE_H

#include /**/ "ace/pre.h"

#include /**/ "tao/TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "tao/Versioned_Namespace.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_ORB_Core;

/**
 * @class TAO_Resume_Handle
 *
 * @brief A utility class that helps in resuming handlers if TAO uses
 *  a TP Reactor from ACE.
 *
 * Please read the documentation in the bugzilla #575 in the bugzilla
 * database what we mean by handler resumption.
 *
 * When TAO uses a TP reactor, it takes care of resuming the handler
 * once it makes sure that it has read the whole message out of the
 * socket. During the process of reading the transport object would
 * have to deal with errors in 'read' from the socket, or errors in
 * the messages that has been received. Instead of calling
 * resume_handler () on the reactor at every point in the code, we
 * use this utility class to take care of the resumption.
 */
class TAO_Export TAO_Resume_Handle
{

public:
  /// Constructor.
  TAO_Resume_Handle (TAO_ORB_Core *orb_core = 0,
                     ACE_HANDLE h = ACE_INVALID_HANDLE);
  /// Destructor
  ~TAO_Resume_Handle (void);

  enum TAO_Handle_Resume_Flag
  {
    TAO_HANDLE_RESUMABLE = 0,
    TAO_HANDLE_ALREADY_RESUMED,
    TAO_HANDLE_LEAVE_SUSPENDED
  };

  /// Allow the users of this class to change the underlying flag.
  void set_flag (TAO_Handle_Resume_Flag fl);

  /// Assignment operator
  TAO_Resume_Handle &operator= (const TAO_Resume_Handle &rhs);

  /// Resume the handle in the reactor only if the ORB uses a TP
  /// reactor. Else we don't resume the handle.
  void resume_handle (void);

  /// Hook method called at the end of a connection handler's
  /// handle_input function.  Might override the handle_input
  /// return value or change the resume_handler's flag_ value.
  void handle_input_return_value_hook (int& return_value);

private:

  /// Our ORB Core.
  TAO_ORB_Core *orb_core_;

  /// The actual handle that needs resumption..
  ACE_HANDLE handle_;

  /// The flag for indicating whether the handle has been resumed or
  /// not. A value of '0' indicates that the handle needs resumption.
  TAO_Handle_Resume_Flag flag_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/Resume_Handle.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /*TAO_RESUME_HANDLE*/
