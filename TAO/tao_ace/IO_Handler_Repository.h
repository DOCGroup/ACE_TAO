/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    IO_Handler_Repository.h
 *
 *  $Id$
 *
 *  @author Balachandran Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_ACE_IO_HANDLER_REPOSITORY_H
#define TAO_ACE_IO_HANDLER_REPOSITORY_H
#include "ace/pre.h"
#include "TAO_ACE_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "IO_Event_Masks.h"
#include "ace/Global_Macros.h"

class TAO_ACE_IO_Event_Handler;


/**
 * @class TAO_ACE_IO_Handler_Repository
 *
 * @brief Provides an abstract interface for handling socket events.
 *
 * @@ todo<Bala>: Write comments
 */

class TAO_ACE_Export TAO_ACE_IO_Handler_Repository
{
public:

  virtual ~TAO_ACE_IO_Handler_Repository (void) = 0;

  class IO_Handler_State
  {
  public:

    /// Ctor
    IO_Handler_State (void);

    /// Number of threads actually dispatched to the socket handler
    long thread_count_;

    /// Has the handler been marked for deletion?
    int marked_for_deletion_;


    /// The IO event handler
    TAO_ACE_IO_Event_Handler *event_handler_;
  };


  /// Some common methods go here..

  virtual int bind (TAO_ACE_IO_Event_Handler *eh,
                    TAO_ACE_IO_Event_Masks::Event_Masks mask) = 0;


  virtual int unbind (TAO_ACE_IO_Event_Handler *eh,
                      TAO_ACE_IO_Event_Masks::Event_Masks mask) = 0;


  virtual int add_masks (ACE_HANDLE io,
                         TAO_ACE_IO_Event_Masks::Event_Masks mask) = 0;


  virtual int clr_masks (ACE_HANDLE io,
                         TAO_ACE_IO_Event_Masks::Event_Masks mask) = 0;

  virtual int find_state (ACE_HANDLE h,
                          TAO_ACE_IO_Handler_Repository::IO_Handler_State &st) = 0;

  virtual int rebind_state (
      TAO_ACE_IO_Handler_Repository::IO_Handler_State &st) = 0;

protected:

  /// @todo: Cant this be defined here?
  virtual int is_handle_valid (ACE_HANDLE handle) = 0;

private:

  ACE_UNIMPLEMENTED_FUNC (void operator= (const TAO_ACE_IO_Handler_Repository &));
};


#if defined (__ACE_INLINE__)
#include "IO_Handler_Repository.inl"
#endif /* __ACE_INLINE__ */


#include "ace/post.h"
#endif /*TAO_ACE_IO_HANDLER_REPOSITORY_H*/
