// $Id$

// ============================================================================
//
// = LIBRARY
//   TAO
//
// = FILENAME
//   RT_Current.h
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
// ============================================================================

#ifndef TAO_RT_CURRENT_H
#define TAO_RT_CURRENT_H
#include "ace/pre.h"

#include "tao/rtcorbafwd.h"

#if (TAO_HAS_RT_CORBA == 1)

#include "tao/Exception.h"
#include "tao/CurrentC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_RT_Current_var;
class TAO_RT_Current;
typedef TAO_RT_Current *TAO_RT_Current_ptr;

class TAO_Export TAO_RT_Current : public CORBA_Current
{
  // = TITLE
  //   The interface for accessing and setting the priority of the
  //   current thread.
  //
  // = DESCRIPTION
  //   Check the RT-CORBA spec (orbos/99-02-12) secions 4.6
  //
public:
  TAO_RT_Current (TAO_Stub *stub);
  // Default constructor

  virtual ~TAO_RT_Current (void);
  // The destructor

#if !defined(__GNUC__) || !defined (ACE_HAS_GNUG_PRE_2_8)
  typedef TAO_RT_Current_ptr _ptr_type;
  typedef TAO_RT_Current_var _var_type;
#endif /* ! __GNUC__ || g++ >= 2.8 */

    // the static operations
    static TAO_RT_Current_ptr _duplicate (TAO_RT_Current_ptr obj);
    static TAO_RT_Current_ptr _narrow (
        CORBA::Object_ptr obj,
        CORBA::Environment &env =
          TAO_default_environment ()
      );
    static TAO_RT_Current_ptr _nil (void)
    {
      return (RTCORBA::Current_ptr)0;
    }

  virtual RTCORBA::Priority the_priority (
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));

  virtual void the_priority (
        RTCORBA::Priority the_priority,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));

  virtual CORBA::Boolean _is_a (
        const char *type_id,
        CORBA::Environment &env =
          TAO_default_environment ()
      );
  virtual const char* _remote_interface_repository_id (void) const;

private:
  TAO_RT_Current (const TAO_RT_Current &);
  void operator= (const TAO_RT_Current &);
};

// ****************************************************************

class TAO_Export TAO_RT_Current_var
{
public:
  TAO_RT_Current_var (void);
  TAO_RT_Current_var (TAO_RT_Current_ptr);
  TAO_RT_Current_var (const TAO_RT_Current_var &);
  ~TAO_RT_Current_var (void);

  TAO_RT_Current_var &operator= (TAO_RT_Current_ptr);
  TAO_RT_Current_var &operator= (const TAO_RT_Current_var &);
  TAO_RT_Current_ptr operator-> (void) const;

  operator const TAO_RT_Current_ptr &() const;
  operator TAO_RT_Current_ptr &();
  // in, inout, out, _retn
  TAO_RT_Current_ptr in (void) const;
  TAO_RT_Current_ptr &inout (void);
  TAO_RT_Current_ptr &out (void);
  TAO_RT_Current_ptr _retn (void);
  TAO_RT_Current_ptr ptr (void) const;

private:
  TAO_RT_Current_ptr ptr_;
};

// ****************************************************************

class TAO_Export TAO_RT_Current_out
{
public:
  TAO_RT_Current_out (TAO_RT_Current_ptr &);
  TAO_RT_Current_out (TAO_RT_Current_var &);
  TAO_RT_Current_out (const TAO_RT_Current_out &);
  TAO_RT_Current_out &operator= (const TAO_RT_Current_out &);
  TAO_RT_Current_out &operator= (const TAO_RT_Current_var &);
  TAO_RT_Current_out &operator= (TAO_RT_Current_ptr);
  operator TAO_RT_Current_ptr &();
  TAO_RT_Current_ptr &ptr (void);
  TAO_RT_Current_ptr operator-> (void);

private:
  TAO_RT_Current_ptr &ptr_;
};

#if defined (__ACE_INLINE__)
# include "tao/RT_Current.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_HAS_RT_CORBA == 1 */

#include "ace/post.h"
#endif /* TAO_RT_CURRENT_H */
