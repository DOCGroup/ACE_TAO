/* -*- C++ -*- */
// $Id$

/*-*- C++ -*- */

// ============================================================================
//
// = LIBRARY
//    ACE
// 
// = FILENAME
//    ACE_Local_Name_Space
//
// = AUTHOR
//    Prashant Jain (pjain@cs.wustl.edu), Irfan Pyarali
//    (irfan@wuerl.wustl.edu), and Douglas C. Schmidt
//    (schmidt@cs.wustl.edu). 
// 
// ============================================================================

#if !defined (ACE_LOCAL_NAME_SPACE_H)
#define ACE_LOCAL_NAME_SPACE_H

#include "ace/Map_Manager.h"
#include "ace/Service_Config.h"
#include "ace/SString.h"
#include "ace/Set.h"
#include "ace/Malloc.h"
#include "ace/Synch.h"

class ACE_Export ACE_NS_String
  // = TITLE
  //     This class and ACE_NS_Internal are used as Adapters to work
  //     with the Map_Manager.  
  //
  // = DESCRIPTION
  //     In order to work correctly, this class must be able to
  //     convert back and forth with ACE_WStrings.  Note that this
  //     class must not have a destructor since otherwise we'll have
  //     problems... 
{
public:
  // = Initialization.
  ACE_NS_String (void);
  // Default "no-op" constructor.

  ACE_NS_String (ACE_USHORT16 *dst, 
		 const ACE_USHORT16 *src, 
		 size_t len);
  // Initialization method.

  ACE_NS_String (const ACE_WString &);
  // Converts an ACE_WString to an ACE_NS_String;

  operator ACE_WString () const;
  // Converts an ACE_NS_String to fresh copy of an ACE_WString;

  char *char_rep (void) const;
  // Return the ASCII character representation.

  int strstr (const ACE_NS_String &) const;
  // Matches on substrings.

  int operator == (const ACE_NS_String &) const;
  // Compare an ACE_NS_String.

  size_t len (void) const;
  // Returns length of the string

  ACE_USHORT16 *fast_rep (void) const;
  // Returns the underlying representation.

private:
  size_t len_;
  // Length of the string.

  ACE_USHORT16 *rep_;
  // This actually points into shared/persistent memory.
};

class ACE_Export ACE_NS_Internal
  // = TITLE
  //     This class and ACE_NS_String are used as Adapters to work
  //     with the Map_Manager.  
{
public:
  ACE_NS_Internal (void);
  // No-op constructor.

  ACE_NS_Internal (ACE_NS_String &value, const char *type);
  // Constructor.

  int operator == (const ACE_NS_Internal &) const;
  // Compare an ACE_NS_Internal

  ACE_NS_String value (void);
  // Return value.

  const char *type (void);
  // Return type.

private:
  ACE_NS_String value_;
  // Contains the value of the string.
  
  const char *type_;
  // Contains the type of the string.
};

// Include the ACE_Local_Name_Space templates stuff at this point.
#include "ace/Local_Name_Space_T.h"

#endif /* ACE_LOCAL_NAME_SPACE_H */
