// -*- C++ -*-

//=============================================================================
/**
 *  @file    Env_Value_T.h
 *
 *  $Id$
 *
 *  Template to encapsulate getting a value from an environment variable
 *  and using a supplied default value if not in the environment.
 *
 *
 *  @author Chris Cleeland (derived from work by Carlos O'Ryan)
 */
//=============================================================================

#ifndef ACE_ENV_VALUE_T_H
#define ACE_ENV_VALUE_T_H

#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/OS.h"

/**
 * @class ACE_Env_Value
 *
 * @brief Enviroment Variable Value
 *
 * Reads a variable from the user enviroment, providing a default
 * value.
 */
template <class T>
class ACE_Env_Value
{
public:
  /**
   * Default constructor which isn't bound to a specific environment
   * variable name or a default value.  Before being useful it must
   * <open>'d.
   */
  ACE_Env_Value (void);

  /// Constructor that calls <open>.
  ACE_Env_Value (const ACE_TCHAR *varname,
                 const T &vardefault);

  /// Destroy the value.
  ~ACE_Env_Value (void);

  /// Returns the value as type T.
  operator T (void);

  /// The constructor, read <varname> from the environment, using
  /// <vardefault> as its value if it is not defined.
  void open (const ACE_TCHAR *varname, const T &defval);

  /// Returns the name of the variable being tracked.
  const ACE_TCHAR *varname (void) const;

private:
  /// Disallow copying and assignment.
  ACE_UNIMPLEMENTED_FUNC (ACE_Env_Value(const ACE_Env_Value<T> &))
  ACE_UNIMPLEMENTED_FUNC (ACE_Env_Value<T> operator=(const ACE_Env_Value<T> &))

  void fetch_value (void);

  const ACE_TCHAR *varname_;
  T value_;
};

template <class T> void ACE_Convert (const ACE_TCHAR *s, T &t);
// Function to convert a string <s> into type <T>.

#if defined (__ACE_INLINE__)
#include "ace/Env_Value_T.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/Env_Value_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */


// Default calls a CTOR on type T of the form 'T::T(const char*)', but
// users can feel free to create their own specialized conversion
// functions if necessary, as shown below.  Note that for 'char*' the
// default is used because a simple cast will be performed and no
// conversion will be necessary.

template <class T> inline void
ACE_Convert (const ACE_TCHAR *s, T &t)
{
  t = T (s);
}

inline void
ACE_Convert (const ACE_TCHAR *s, ACE_TCHAR *&v)
{
  v = (ACE_TCHAR *) s;
}

inline void
ACE_Convert (const ACE_TCHAR *s, const ACE_TCHAR *&v)
{
  v = (const ACE_TCHAR *) s;
}

inline void
ACE_Convert (const ACE_TCHAR *s, short &si)
{
  si = ACE_static_cast (short, ACE_OS::strtol (s, 0, 10));
}

inline void
ACE_Convert (const ACE_TCHAR *s, u_short &us)
{
  us = ACE_static_cast (u_short, ACE_OS::strtol (s, 0, 10));
}

inline void
ACE_Convert (const ACE_TCHAR *s, u_int &i)
{
  i = ACE_static_cast (u_int,
                       ACE_OS::strtol (s, 0, 10));
}

inline void
ACE_Convert (const ACE_TCHAR *s, long &l)
{
  l = ACE_OS::strtol (s, 0, 10);
}

inline void
ACE_Convert (const ACE_TCHAR *s, int &i)
{
  i = ACE_static_cast (int, ACE_OS::strtol (s, 0, 10));
}

inline void
ACE_Convert (const ACE_TCHAR *s, u_long &ul)
{
  ul = ACE_OS::strtoul (s, 0, 10);
}

inline void
ACE_Convert (const ACE_TCHAR *s, double &d)
{
  d = ACE_OS::strtod (s, 0);
}

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Env_Value_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include "ace/post.h"
#endif /* ACE_ENV_VALUE_T_H */
