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

#if !defined (ACE_ENV_VALUE_T_H)
#define ACE_ENV_VALUE_T_H

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

  operator const T (void) const;
  // Returns the value as type T.

  void open (const char *varname, const T &defval);
  // The constructor, read <varname> from the enviroment, using
  // <vardefault> as its value if it is not defined.

  const char *varname (void) const;
  // Returns the name of the variable being tracked.

private:
  ACE_UNIMPLEMENTED_FUNC (ACE_Env_Value (const ACE_Env_Value &))
  ACE_UNIMPLEMENTED_FUNC (ACE_Env_Value &operator= (const ACE_Env_Value &))
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

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Env_Value_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* ACE_ENV_VALUE_T_H */
