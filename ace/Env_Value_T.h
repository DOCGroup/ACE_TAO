/* This may look like C, but it's really -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ACE
//
// = DESCRIPTION
//    Template to encapsulate getting a value from an environment variable
//    and using a supplied default value if not in the environment.
//
// = AUTHOR
//    Chris Cleeland (derived from work by Carlos O'Ryan)
//
// ============================================================================

#ifndef ACE_ENV_VALUE_T_H
#define ACE_ENV_VALUE_T_H

#include "ace/OS.h"             // Need to get ACE_static_cast definition

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

template <class T>
class ACE_Env_Value
{
  // = TITLE
  //    Enviroment Variable Value
  //
  // = DESCRIPTION
  //    Reads a variable from the user enviroment, providing a default
  //    value.
  //
  // = AUTHOR
  //    Chris Cleeland, Carlos O'Ryan
public:
  ACE_Env_Value (void);
  // Default constructor which isn't bound to a specific environment
  // variable name or a default value.  Before being useful it must
  // <open()>ed.

  ACE_Env_Value (const char *varname,
                 const T &vardefault);
  // Constructor that calls <open>.

  ~ACE_Env_Value (void);
  // Destroy the value.

  operator T (void);
  // Returns the value as type T.

  void open (const char *varname, const T &defval);
  // The constructor, read <varname> from the enviroment, using
  // <vardefault> as its value if it is not defined.

  const char *varname (void) const;
  // Returns the name of the variable being tracked.

private:
  ACE_UNIMPLEMENTED_FUNC (ACE_Env_Value(const ACE_Env_Value<T> &))
  ACE_UNIMPLEMENTED_FUNC (ACE_Env_Value<T> operator=(const ACE_Env_Value<T> &))
  // Disallow copying and assignment.

  void fetch_value (void);

  const char *varname_;
  T value_;
};

template <class T> void ACE_Convert (const char *s, T &t);
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
ACE_Convert (const char *s, T &t)
{
  t = T (s);
}

inline void
ACE_Convert (const char *s, char *&v)
{
  v = (char *) s;
}

inline void
ACE_Convert (const char *s, short &si)
{
  si = ACE_static_cast (short, ACE_OS::strtol (s, 0, 10));
}

inline void
ACE_Convert (const char *s, u_short &us)
{
  us = ACE_static_cast (u_short, ACE_OS::strtol (s, 0, 10));
}

inline void
ACE_Convert (const char *s, u_int &i)
{
  i = ACE_static_cast (u_int,
                       ACE_OS::strtol (s, 0, 10));
}

inline void
ACE_Convert (const char *s, long &l)
{
  l = ACE_OS::strtol (s, 0, 10);
}

inline void
ACE_Convert (const char *s, int &i)
{
  i = ACE_static_cast (int, ACE_OS::strtol (s, 0, 10));
}

inline void
ACE_Convert (const char *s, u_long &ul)
{
  ul = ACE_OS::strtoul (s, 0, 10);
}

inline void
ACE_Convert (const char *s, double &d)
{
  d = ACE_OS::strtod (s, 0);
}

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Env_Value_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* ACE_ENV_VALUE_T_H */
