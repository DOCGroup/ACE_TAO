/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    SString.h
//
// = AUTHOR
//    Douglas C. Schmidt (schmidt@cs.wustl.edu)
// 
// ============================================================================

#if !defined (ACE_SSTRING_H)
#define ACE_SSTRING_H

#include "ace/ACE.h"

// Forward decl.
class ACE_Allocator;

class ACE_Export ACE_CString
{
  // = TITLE
  //   A simple "C String" (ACE_CString) class.
  //
  // = DESCRIPTION
  //   This is a place holder until all compilers implement the
  //   ANSI/ISO C++ standard String class.  Note that we need to use
  //   this class since the ACE ACE_Map_Manager requires an object
  //   that supports the operator== and operator!=.  This class uses
  //   an ACE_Allocator to allocate memory.  The user can make this a
  //   persistant class by providing an ACE_Allocator with a
  //   persistable memory pool.  NOTE: if an instance of this class is
  //   constructed from or assigned an empty string (with first
  //   element of '\0'), then it is _not_ allocated new space.
  //   Instead, its internal representation is set equal to a global
  //   empty string.
public:
  friend ACE_Export ACE_CString operator+ (const ACE_CString &, const ACE_CString &);
#if !defined (ACE_HAS_WINCE)
  friend ACE_Export ostream &operator<< (ostream &, const ACE_CString &);
#endif /* ! ACE_HAS_WINCE */

  static const int npos;
  // No position constant

  ACE_CString (ACE_Allocator *alloc = 0);
  // Default constructor.

  ACE_CString (const char *s, ACE_Allocator *alloc = 0);
  // Constructor that copies <s> into dynamically allocated memory. 

  ACE_CString (const char *s, size_t len, ACE_Allocator *alloc = 0);
  // Constructor that copies <len> chars of <s> into dynamically
  // allocated memory (will NUL terminate the result).

  ACE_CString (const ACE_CString &);
  // Copy constructor.

  ACE_CString (const ACE_USHORT16 *s, ACE_Allocator *alloc = 0);
  // Constructor that copies <s> into dynamically allocated memory.
  // Probable loss of data. Please use with care.

  ACE_CString (char c, ACE_Allocator *alloc = 0);
  // Constructor that copies <c> into dynamically allocated memory. 

  ~ACE_CString (void);
  // Deletes the memory...

  char operator[] (size_t index) const;
  // Return the <index'th> character in the string (doesn't perform
  // bounds checking).

  char &operator[] (size_t index);
  // Return the <index'th> character by reference in the string
  // (doesn't perform bounds checking).

  ACE_CString &operator= (const ACE_CString &);
  // Assignment operator (does copy memory).

  ACE_CString substring (size_t offset, ssize_t length = -1) const;
  // Return a substring given an offset and length, if length == -1
  // use rest of str return empty substring if offset or offset/length
  // are invalid

  ACE_CString substr (size_t offset, ssize_t length = -1) const;
  // Same as substring

  ACE_CString &operator+= (const ACE_CString &);
  // Concat operator (copies memory).

  u_long hash (void) const;
  // Returns a hash value for this string.

  size_t length (void) const;
  // Return the length of the string.

  char *rep (void) const;
  // Get a copy of the underlying pointer.

  const char *fast_rep (void) const;
  // Get at the underlying representation directly!
  // _Don't_ even think about casting the result to (char *) and modifying it,
  // if it has length 0!

  const char *c_str (void) const;
  // Same as STL String's c_str()

  int strstr (const ACE_CString &s) const;
  // Comparison operator that will match substrings.  Returns the
  // index of the first location that matches, else -1.

  int find (const ACE_CString &str, int pos = 0) const;
  // Find <str> starting at pos.  Returns the index of the first
  // location that matches, else npos.

  int find (const char *s, int pos = 0) const;
  // Find <s> starting at pos.  Returns the index of the first
  // location that matches, else npos.

  int find (char c, int pos = 0) const;
  // Find <c> starting at pos.  Returns the index of the first
  // location that matches, else npos.
  
  int rfind (char c, int pos = npos) const;
  // Find <c> starting at pos (counting from the end).  Returns the
  // index of the first location that matches, else npos.
  
  int operator== (const ACE_CString &s) const;
  // Equality comparison operator (must match entire string).

  int operator!= (const ACE_CString &s) const;
  // Inequality comparison operator.

  int compare (const ACE_CString &s) const;
  // Performs a <strcmp>-style comparison.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  ACE_Allocator *allocator_;
  // Pointer to a memory allocator.

  size_t len_;
  // Length of the ACE_CString (not counting the trailing '\0').

  char *rep_;
  // Pointer to data.

  static char NULL_CString_;
  // Represents the "NULL" string to simplify the internal logic.
};

ACE_Export ACE_CString operator+ (const ACE_CString &, const ACE_CString &);
#if !defined (ACE_HAS_WINCE)
ACE_Export ostream &operator<< (ostream &, const ACE_CString &);
#endif /* ! ACE_HAS_WINCE */

class ACE_Export ACE_SString
  // = TITLE
  //   A very "Simple String" (ACE_SString) class.
  //
  // = DESCRIPTION
  //   This is *not* a general-purpose string class.  It is only
  //   intended for use with applications that understand how it
  //   works.  In particular, it has no destructor...  Note that we
  //   need to use this class since the ACE ACE_Map_Manager requires
  //   an object that supports the operator== and operator!=.
  //   This class uses an ACE_Allocator to allocate memory
  //   The user can make this a persistant class by providing an 
  //   ACE_Allocator with a persistable memory pool
{
public:
#if !defined (ACE_HAS_WINCE)
  friend ACE_Export ostream &operator<< (ostream &, const ACE_SString &);
#endif /* ! ACE_HAS_WINCE */

  static const int npos;
  // No position constant

  ACE_SString (ACE_Allocator *alloc = 0);
  // Default constructor.

  ACE_SString (const char *s, ACE_Allocator *alloc = 0);
  // Constructor that copies <s> into dynamically allocated memory. 

  ACE_SString (const char *s, size_t len, ACE_Allocator *alloc = 0);
  // Constructor that copies <len> chars of <s> into dynamically
  // allocated memory (will NUL terminate the result).

  ACE_SString (const ACE_SString &);
  // Copy constructor.

  ACE_SString (char c, ACE_Allocator *alloc = 0);
  // Constructor that copies <c> into dynamically allocated memory. 

  char operator[] (size_t index) const;
  // Return the <index'th> character in the string (doesn't perform
  // bounds checking).

  char &operator[] (size_t index);
  // Return the <index'th> character by reference in the string
  // (doesn't perform bounds checking).

  ACE_SString &operator= (const ACE_SString &);
  // Assignment operator (does copy memory).

  ACE_SString substring (size_t offset, ssize_t length = -1) const;
  // Return a substring given an offset and length, if length == -1
  // use rest of str return empty substring if offset or offset/length
  // are invalid

  ACE_SString substr (size_t offset, ssize_t length = -1) const;
  // Same as substring

  u_long hash (void) const;
  // Returns a hash value for this string.

  size_t length (void) const;
  // Return the length of the string.

  void rep (char *s);
  // Set the underlying pointer.  Since this does not copy memory or
  // delete existing memory use with extreme caution!!!

  const char *rep (void) const;
  // Get the underlying pointer.

  const char *fast_rep (void) const;
  // Get the underlying pointer.

  const char *c_str (void) const;
  // Get the underlying pointer.

  int strstr (const ACE_SString &s) const;
  // Comparison operator that will match substrings.  Returns the
  // index of the first location that matches, else -1.

  int find (const ACE_SString &str, int pos = 0) const;
  // Find <str> starting at pos.  Returns the index of the first
  // location that matches, else npos.

  int find (const char *s, int pos = 0) const;
  // Find <s> starting at pos.  Returns the index of the first
  // location that matches, else npos.

  int find (char c, int pos = 0) const;
  // Find <c> starting at pos.  Returns the index of the first
  // location that matches, else npos.
  
  int rfind (char c, int pos = npos) const;
  // Find <c> starting at pos (counting from the end).  Returns the
  // index of the first location that matches, else npos.
  
  int operator== (const ACE_SString &s) const;
  // Equality comparison operator (must match entire string).

  int operator!= (const ACE_SString &s) const;
  // Inequality comparison operator.

  int compare (const ACE_SString &s) const;
  // Performs a <strcmp>-style comparison.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  ACE_Allocator *allocator_;
  // Pointer to a memory allocator.

  size_t len_;
  // Length of the ACE_SString (not counting the trailing '\0').

  char *rep_;
  // Pointer to data.
};

#if !defined (ACE_HAS_WINCE)
ACE_Export ostream &operator<< (ostream &, const ACE_SString &);
#endif /* ! ACE_HAS_WINCE */

class ACE_Export ACE_WString
  // = TITLE
  //   A persistent wide string class.
  // 
  // = DESCRIPTION
  //   This is *not* a general-purpose string class.  It is only
  //   intended for use with applications that understand how it
  //   works.  Note that we need to use this class since the ACE
  //   ACE_Map_Manager requires an object that supports the operator==
  //   and operator!=.  This class uses an ACE_Allocator to allocate
  //   memory The user can make this a persistant class by providing
  //   an ACE_Allocator with a persistable memory pool
{
  friend ACE_Export ACE_WString operator+ (const ACE_WString &, const ACE_WString &);
public:
  static const int npos;
  // No position constant

  ACE_WString (ACE_Allocator *alloc = 0);
  // Default constructor.

  ACE_WString (const char *s, 
	       ACE_Allocator *alloc = 0);
  // Constructor that copies <s> into dynamically allocated memory.

  ACE_WString (const ACE_USHORT16 *s, 
	       ACE_Allocator *alloc = 0);
  // Constructor that copies <s> into dynamically allocated memory.

  ACE_WString (const ACE_USHORT16 *s, 
	       size_t len, 
	       ACE_Allocator *alloc = 0);
  // Constructor that copies <len> ACE_USHORT16's of <s> into dynamically
  // allocated memory (will NUL terminate the result).

  ACE_WString (size_t len, ACE_Allocator *alloc = 0);
  // Constructor that dynamically allocates memory for <len> + 1
  // ACE_USHORT16 characters. The newly created memory is set memset to 0.

  ACE_WString (const ACE_WString &s);
  // Copy constructor.

  ACE_WString (ACE_USHORT16 c, ACE_Allocator *alloc = 0);
  // Constructor that copies <c> into dynamically allocated memory. 

  ~ACE_WString (void);
  // Deletes the memory...

  ACE_USHORT16 operator[] (size_t index) const;
  // Return the <index'th> character in the string (doesn't perform
  // bounds checking).

  ACE_USHORT16 &operator[] (size_t index);
  // Return the <index'th> character by reference in the string
  // (doesn't perform bounds checking).

  ACE_WString &operator= (const ACE_WString &);
  // Assignment operator (does copy memory).

  ACE_WString substring (size_t offset, ssize_t length = -1) const;
  // Return a substring given an offset and length, if length == -1
  // use rest of str return empty substring if offset or offset/length
  // are invalid.

  ACE_WString substr (size_t offset, ssize_t length = -1) const;
  // Same as substring

  ACE_WString &operator+= (const ACE_WString &);
  // Concat operator (does copy memory).

  u_long hash (void) const;
  // Returns a hash value for this string.

  size_t length (void) const;
  // Return the length of the string.

  ACE_USHORT16 *rep (void) const;
  // Gets a copy of the underlying pointer.

  char *char_rep (void) const;
  // Transform into a copy of the ASCII character representation.

  const ACE_USHORT16 *fast_rep (void) const;
  // Get at the underlying representation directly!

  const ACE_USHORT16 *c_str (void) const;
  // Same as STL String's c_str()

  int strstr (const ACE_WString &s) const;
  // Comparison operator that will match substrings.  Returns the
  // index of the first location that matches, else -1.

  int find (const ACE_WString &str, int pos = 0) const;
  // Find <str> starting at pos.  Returns the index of the first
  // location that matches, else npos.

  int find (const ACE_USHORT16 *s, int pos = 0) const;
  // Find <s> starting at pos.  Returns the index of the first
  // location that matches, else npos.

  int find (ACE_USHORT16 c, int pos = 0) const;
  // Find <c> starting at pos.  Returns the index of the first
  // location that matches, else npos.
  
  int rfind (ACE_USHORT16 c, int pos = npos) const;
  // Find <c> starting at pos (counting from the end).  Returns the
  // index of the first location that matches, else npos.
  
  int operator== (const ACE_WString &s) const;
  // Equality comparison operator (must match entire string).

  int operator!= (const ACE_WString &s) const;
  // Inequality comparison operator.

  int compare (const ACE_WString &s) const;
  // Performs a <strcmp>-style comparison.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

  static size_t wstrlen (const ACE_USHORT16 *);
  // Computes the length of a "0" terminated ACE_USHORT16 *.
  
  static const ACE_USHORT16 *strstr (const ACE_USHORT16 *s1, const ACE_USHORT16 *s2);
  // Traditional style strstr
  
  void resize (size_t len);
  // This method is designed for high-performance. Please use with
  // care ;-) If the current size of the string is less than <len>,
  // the string is resized to the new length. The data is is zero'd
  // out after this operation.

private:
  ACE_Allocator *allocator_;
  // Pointer to a memory allocator.

  size_t len_;
  // Length of the ACE_WString.

  ACE_USHORT16 *rep_;
  // Pointer to data.
};

ACE_Export ACE_WString operator+ (const ACE_WString &, const ACE_WString &);

#if defined (__ACE_INLINE__)
#include "ace/SString.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_SSTRING_H */
