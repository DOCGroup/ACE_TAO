/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Select_IO_Handler_Repository.h
 *
 *  $Id$
 *
 *  @author Balachandran Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_ACE_SELECT_IO_HANDLER_REPOSITORY_H
#define TAO_ACE_SELECT_IO_HANDLER_REPOSITORY_H
#include "ace/pre.h"
#include "IO_Handler_Repository.h"


#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// @@todo: This pulls OS.h. Need to figure out a way to decouple this
// from OS.h
#include "ace/Handle_Set.h"


class TAO_ACE_Select_IO_Handler_Repository_Iterator;

/**
 * @class TAO_ACE_Select_Reactor_Handle_Set
 *
 * @brief Track handles we are interested for various events.
 *
 * @todo: This class is the *same* as the
 *  ACE_Select_Reactor_Handle_Set.  As the ACE_Select_Yada is defined
 *  within some class which would set up a dependency to classes that
 *  we dont want, I am redefining this class here.
 */

class TAO_ACE_Export TAO_ACE_Select_Strategy_Handle_Set
{
public:
  /// Read events (e.g., input pending, accept pending).
  ACE_Handle_Set rd_mask_;

  /// Write events (e.g., flow control abated, non-blocking connection
  /// complete).
  ACE_Handle_Set wr_mask_;

  /// Exception events (e.g., SIG_URG).
  ACE_Handle_Set ex_mask_;
};


/**
 * @class TAO_ACE_Select_IO_Handler_Repository
 *
 * @brief Provides an abstract interface for handling socket events.
 *
 * @@ todo<Bala>: Write comments
 */
class TAO_ACE_Export TAO_ACE_Select_IO_Handler_Repository : public TAO_ACE_IO_Handler_Repository
{
public:
  friend class TAO_ACE_Select_IO_Handler_Repository_Iterator;

  /// Ctor
  TAO_ACE_Select_IO_Handler_Repository (void);

  /* Initialize the repository on construction. The constructor calls
   * the underlying open () method to initialize the repository. This
   * cannot be of much use for error checking.
   */
  TAO_ACE_Select_IO_Handler_Repository (size_t s = ACE_FD_SETSIZE);

  ~TAO_ACE_Select_IO_Handler_Repository (void);

  /* Initialize the <TAO_ACE_IO_Event_Handler_Repository> for
   * use. The <s> denotes the number of <IO_Event_Handlers> that
   * can be stored in it
   */
  int open (size_t s = ACE_FD_SETSIZE);

  /// Close down the repository and release the resources that we
  /// held.
  int close (void);

  virtual int bind (TAO_ACE_IO_Event_Handler *eh,
                    TAO_ACE_IO_Event_Masks::Event_Masks mask);


  virtual int unbind (TAO_ACE_IO_Event_Handler *eh,
                      TAO_ACE_IO_Event_Masks::Event_Masks mask);


  virtual int add_masks (ACE_HANDLE h,
                         TAO_ACE_IO_Event_Masks::Event_Masks mask);


  virtual int clr_masks (ACE_HANDLE h,
                         TAO_ACE_IO_Event_Masks::Event_Masks mask);

  virtual int find_state (ACE_HANDLE h,
                          TAO_ACE_IO_Handler_Repository::IO_Handler_State &st);

  virtual int rebind_state (
      TAO_ACE_IO_Handler_Repository::IO_Handler_State &st);

  /// Local methods ..
  int max_handlep1 (void) const;

  TAO_ACE_Select_Strategy_Handle_Set &wait_set (void);

private:

  int is_handle_valid (ACE_HANDLE handle);

  int is_event_mask_available (ACE_HANDLE handle);

private:

  ssize_t max_size_;

  /// The highest currently active handle, plus 1 (ranges between 0 and
  /// <max_size_>.
  int max_handlep1_;


  TAO_ACE_Select_Strategy_Handle_Set wait_set_;


#if defined (ACE_WIN32)

  /// Useful typedef to the place where we store the data.
  typedef ACE_Hash_Map_Manager <ACE_SOCKET,
    TAO_ACE_IO_Handler_Repository::IO_Handler_State,
    ACE_Null_Mutex> Handler_Map;

  typedef ACE_Hash_Map_Iterator <ACE_SOCKET,
    TAO_ACE_IO_Handler_Repository::IO_Handler_State,
    ACE_Null_Mutex> Handler_Map_Iter;

  /// Map where the sockets and the respective handler states are
  /// housed for easy access and something better than O(n) most of
  /// the time.
  Handler_Map handler_states_;

#else

  /// On Unix based platforms we can maintain an array of handler
  /// states and directly index into this for easy access.
  TAO_ACE_IO_Handler_Repository::IO_Handler_State *handler_states_;
#endif /*ACE_WIN32*/


  ///@


};


/**
 * @class TAO_ACE_Select_IO_Handler_Repository
 *
 * @brief Provides an abstract interface for handling socket events.
 *
 * @@ todo<Bala>: Write comments
 */

class TAO_ACE_Export TAO_ACE_Select_IO_Handler_Repository_Iterator
{
public:

  TAO_ACE_Select_IO_Handler_Repository_Iterator (
      const TAO_ACE_Select_IO_Handler_Repository  &repo);

  // = Iteration methods.

  /// Pass back the <next_item> that hasn't been seen in the Set.
  /// Returns 0 when all items have been seen, else 1.
  int next (
      TAO_ACE_IO_Handler_Repository::IO_Handler_State *&next_item);

  /// Returns 1 when all items have been seen, else 0.
  int done (void) const;

  /// Move forward by one element in the set.  Returns 0 when all the
  /// items in the set have been seen, else 1.
  int advance (void);

  /// Dump the state of an object.
  void dump (void) const;

private:

  const TAO_ACE_Select_IO_Handler_Repository &repo_;

#if defined (ACE_WIN32)
  TAO_ACE_Select_IO_Handler_Repository::Handler_Map_Iter iter_;
#else /* !ACE_WIN32*/
  ssize_t index_;
#endif /*ACE_WIN32*/

};

#if defined (__ACE_INLINE__)
#include "Select_IO_Handler_Repository.inl"
#endif /* __ACE_INLINE__ */


#include "ace/post.h"
#endif /*TAO_ACE_SELECT_IO_HANDLER_REPOSITORY_H*/
