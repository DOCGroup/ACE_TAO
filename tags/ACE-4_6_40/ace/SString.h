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

#ifndef ACE_SSTRING_H
#define ACE_SSTRING_H

#include "ace/ACE.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// Forward decl.
class ACE_Allocator;

class ACE_Export ACE_CString
{
  // = TITLE
  //   This class provides a wrapper facade for C strings.
  //
  // = DESCRIPTION
  //   This class uses an <ACE_Allocator> to allocate memory.  The
  //   user can make this a persistant class by providing an
  //   ACE_Allocator with a persistable memory pool.  NOTE: if an
  //   instance of this class is constructed from or assigned an empty
  //   string (with first element of '\0'), then it is _not_ allocated
  //   new space.  Instead, its internal representation is set equal
  //   to a global empty string.  CAUTION: in cases when ACE_CString
  //   is constructed from provided buffer with release parameter set
  //   to 0, ACE_CString is not guaranteed to be '\0' terminated.
  //   This is a place holder until all compilers implement the
  //   ANSI/ISO C++ standard String class.

public:
  static const int npos;
  // No position constant

  ACE_CString (ACE_Allocator *alloc = 0);
  // Default constructor.

  ACE_CString (const char *s,
               ACE_Allocator *alloc = 0,
               int release = 1);
  // Constructor that copies <s> into dynamically allocated memory.
  // If <release> is non-0 then the <ACE_allocator> is responsible for
  // freeing this memory. Memory is _not_ allocated/freed if <release>
  // is 0. 

  ACE_CString (const char *s,
               size_t len,
               ACE_Allocator *alloc = 0,
               int release = 1);
  // Constructor that copies <len> chars of <s> into dynamically
  // allocated memory (will NUL terminate the result).  If <release>
  // is non-0 then the <ACE_allocator> is responsible for freeing this
  // memory. Memory is _not_ allocated/freed if <release> is 0. 

  ACE_CString (const ACE_CString &);
  // Copy constructor.

  ACE_CString (const ACE_USHORT16 *s, ACE_Allocator *alloc = 0);
  // Constructor that copies <s> into dynamically allocated memory.
  // Probable loss of data. Please use with care.

  ACE_CString (char c, ACE_Allocator *alloc = 0);
  // Constructor that copies <c> into dynamically allocated memory.

  ~ACE_CString (void);
  // Deletes the memory...

  const char &operator [] (size_t index) const;
  // Return the <index'th> character in the string (doesn't perform
  // bounds checking).

  char &operator [] (size_t index);
  // Return the <index'th> character by reference in the string
  // (doesn't perform bounds checking).

  ACE_CString &operator = (const ACE_CString &);
  // Assignment operator (does copy memory).

  void set (const char *s, int release);
  // Copy <s> into this <ACE_CString>.

  void set (const char *s, size_t len, int release);
  // Copy <len> bytes of <s> (will NUL terminate the result).

  ACE_CString substring (size_t offset, ssize_t length = -1) const;
  // Return a substring given an offset and length, if length == -1
  // use rest of str.  Return empty substring if offset or
  // offset/length are invalid.

  ACE_CString substr (size_t offset, ssize_t length = -1) const;
  // Same as <substring>.

  ACE_CString &operator += (const ACE_CString &);
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
  // Same as STL String's <c_str> and <fast_rep>.

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

  int operator == (const ACE_CString &s) const;
  // Equality comparison operator (must match entire string).

  int operator < (const ACE_CString &s) const;
  // Less than comparison operator.

  int operator > (const ACE_CString &s) const;
  // Greater than comparison operator.

  int operator != (const ACE_CString &s) const;
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
  // Length of the ACE_CString data (not counting the trailing '\0').

  size_t buf_len_;
  // Length of the ACE_CString data buffer.  Keeping track of the
  // length allows to avoid unnecessary dynamic allocations.

  char *rep_;
  // Pointer to data.

  int release_;
  // Flag that indicates if we own the memory

  static char NULL_CString_;
  // Represents the "NULL" string to simplify the internal logic.
};

ACE_Export ACE_INLINE ACE_CString operator + (const ACE_CString &, const ACE_CString &);
#if !defined (ACE_LACKS_IOSTREAM_TOTALLY)
ACE_Export ostream &operator << (ostream &, const ACE_CString &);
#endif /* ! ACE_LACKS_IOSTREAM_TOTALLY */

class ACE_Export ACE_WString
{
  // = TITLE
  //   This class provides a wrapper facade for C wide strings. 
  //
  // = DESCRIPTION 
  //     This class uses an <ACE_Allocator> to allocate memory.  The
  //     user can make this a persistant class by providing an
  //     <ACE_Allocator> with a persistable memory pool.  This is a
  //     place holder until all compilers implement the ANSI/ISO C++
  //     standard String class.
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

  ACE_USHORT16 operator [] (size_t index) const;
  // Return the <index'th> character in the string (doesn't perform
  // bounds checking).

  ACE_USHORT16 &operator [] (size_t index);
  // Return the <index'th> character by reference in the string
  // (doesn't perform bounds checking).

  ACE_WString &operator = (const ACE_WString &);
  // Assignment operator (does copy memory).

  void set (const ACE_USHORT16 *s);
  // Copy <s>

  void set (const ACE_USHORT16 *s, size_t len);
  // Copy <len> bytes of <s> (will NUL terminate the result)

  ACE_WString substring (size_t offset, ssize_t length = -1) const;
  // Return a substring given an offset and length, if length == -1
  // use rest of str return empty substring if offset or offset/length
  // are invalid.

  ACE_WString substr (size_t offset, ssize_t length = -1) const;
  // Same as substring

  ACE_WString &operator += (const ACE_WString &);
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
  // Same as STL String's <c_str> and <fast_rep>.

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

  int operator == (const ACE_WString &s) const;
  // Equality comparison operator (must match entire string).

  int operator < (const ACE_WString &s) const;
  // Less than comparison operator.

  int operator > (const ACE_WString &s) const;
  // Greater than comparison operator.

  int operator != (const ACE_WString &s) const;
  // Inequality comparison operator.

  int compare (const ACE_WString &s) const;
  // Performs a <strcmp>-style comparison.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

  static size_t strlen (const ACE_USHORT16 *);
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

ACE_Export ACE_INLINE ACE_WString operator + (const ACE_WString &, const ACE_WString &);
#if !defined (ACE_LACKS_IOSTREAM_TOTALLY)
ACE_Export ostream &operator << (ostream &, const ACE_WString &);
#endif /* ! ACE_LACKS_IOSTREAM_TOTALLY */

class ACE_Export ACE_SString
{
  // = TITLE
  //   A very Simple String <ACE_SString> class.  This is not a
  //   general-purpose string class, and you should probably consider
  //   using <ACE_CString> is you don't understand why this class
  //   exists...
  //
  // = DESCRIPTION
  //   This class is only intended for use with applications that
  //   understand how it works.  In particular, its destructor does
  //   not deallocate its memory when it is destroyed...  We need this
  //   class since the <ACE_Map_Manager> requires an object that
  //   supports the operator == and operator !=.  This class uses an
  //   <ACE_Allocator> to allocate memory.  The user can make this a
  //   persistant class by providing an <ACE_Allocator> with a
  //   persistable memory pool.
public:
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

  ~ACE_SString (void);
  // Default dtor.

  char operator [] (size_t index) const;
  // Return the <index'th> character in the string (doesn't perform
  // bounds checking).

  char &operator [] (size_t index);
  // Return the <index'th> character by reference in the string
  // (doesn't perform bounds checking).

  ACE_SString &operator = (const ACE_SString &);
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
  // Same as STL String's <c_str> and <fast_rep>.

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

  int operator == (const ACE_SString &s) const;
  // Equality comparison operator (must match entire string).

  int operator < (const ACE_SString &s) const;
  // Less than comparison operator.

  int operator > (const ACE_SString &s) const;
  // Greater than comparison operator.

  int operator != (const ACE_SString &s) const;
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

#if !defined (ACE_LACKS_IOSTREAM_TOTALLY)
ACE_Export ostream &operator << (ostream &, const ACE_SString &);
#endif /* ! ACE_LACKS_IOSTREAM_TOTALLY */

class ACE_Export ACE_Tokenizer
{
  // = TITLE
  //    Tokenizer
  //
  // = DESCRIPTION
  //    Tokenizes a buffer.  Allows application to set delimiters and
  //    preserve designators.  Does not allow special characters, yet
  //    (e.g., printf ("\"like a quoted string\"")).
public:
  ACE_Tokenizer (LPTSTR buffer);
  // <buffer> will be parsed.

  int delimiter (TCHAR d);
  // <d> is a delimiter.  Returns 0 on success, -1 if there is no
  // memory left.

  int delimiter_replace (TCHAR d, TCHAR replacement);
  // <d> is a delimiter and, when found, will be replaced by
  // <replacement>.  Returns 0 on success, -1 if there is no memory
  // left.

  int preserve_designators (TCHAR start, TCHAR stop, int strip=1);
  // For instance, quotes, or '(' and ')'.  Returns 0 on success, -1
  // if there is no memory left.  If <strip> == 1, then the preserve
  // designators will be stripped from the tokens returned by next.

  LPTSTR next (void);
  // Returns the next token.

  enum {
    MAX_DELIMITERS=16,
    MAX_PRESERVES=16
  };

protected:
  int is_delimiter (TCHAR d, int &replace, TCHAR &r);
  // Returns 1 if <d> is a delimiter, 0 otherwise.  If <d> should be
  // replaced with <r>, <replace> is set to 1, otherwise 0.

  int is_preserve_designator (TCHAR start, TCHAR &stop, int &strip);
  // If <start> is a start preserve designator, returns 1 and sets
  // <stop> to the stop designator.  Returns 0 if <start> is not a
  // preserve designator.

private:
  LPTSTR buffer_;
  int index_;

  class Preserve_Entry
  {
    // = TITLE
    //    Preserve Entry
    //
    // = DESCRIPTION
    //    Defines a set of characters that designate an area that
    //    should not be parsed, but should be treated as a complete
    //    token.  For instance, in: (this is a preserve region), start
    //    would be a left paren -(- and stop would be a right paren
    //    -)-.  The strip determines whether the designators should be
    //    removed from the token.
  public:
    TCHAR start_;
    // E.g., "(".
    TCHAR stop_;
    // E.g., ")".
    int strip_;
    // Whether the designators should be removed from the token.
  };

  Preserve_Entry preserves_[MAX_PRESERVES];
  // The application can specify MAX_PRESERVES preserve designators.

  int preserves_index_;
  // Pointer to the next free spot in preserves_.

  class Delimiter_Entry
  {
    // = TITLE
    //    Delimiter Entry
    //
    // = DESCRIPTION
    //    Describes a delimiter for the tokenizer.
  public:
    TCHAR delimiter_;
    // Most commonly a space ' '.
    TCHAR replacement_;
    // What occurrences of delimiter_ should be replaced with.
    int replace_;
    // Whether replacement_ should be used.  This should be replaced
    // with a technique that sets replacement_ = delimiter by
    // default.  I'll do that next iteration.
  };

  Delimiter_Entry delimiters_[MAX_DELIMITERS];
  // The tokenizer allows MAX_DELIMITERS number of delimiters.

  int delimiter_index_;
  // Pointer to the next free space in delimiters_.
};

// ****************************************************************

class ACE_Auto_String_Free
{
  // = TITLE
  //   Simple class to automatically de-allocate strings
  //
  // = DESCRIPTION
  //   Keeps a pointer to a string and deallocates it (using
  //   ACE_OS::free()) on its destructor.
  //   If you need to delete using "delete[]" the
  //   ACE_Auto_Array_Ptr<char*> is your choice.
  //   The class plays the same role as auto_ptr<>
  //
public:
  /* explicit */ ACE_Auto_String_Free (char* p = 0);
  ACE_Auto_String_Free (ACE_Auto_String_Free &rhs);
  ACE_Auto_String_Free& operator= (ACE_Auto_String_Free &rhs);
  ~ACE_Auto_String_Free (void);

  char* operator* () const;
  char operator[] (int i) const;
  char* get (void) const;
  char* release (void);
  void reset (char* p = 0);

private:
  char* p_;
};

#if defined (__ACE_INLINE__)
#include "ace/SString.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_SSTRING_H */
