/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    SString.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt (schmidt@cs.wustl.edu)
 */
//=============================================================================


#ifndef ACE_SSTRING_H
#define ACE_SSTRING_H
#include "ace/pre.h"

#include "ace/ACE.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (ACE_HAS_WCHAR)
typedef wchar_t ACE_WSTRING_TYPE;
#else /* ACE_HAS_WCHAR */
typedef ACE_USHORT16 ACE_WSTRING_TYPE;
#endif /* ACE_HAS_WCHAR */

// Forward decl.
class ACE_Allocator;

/**
 * @class ACE_CString
 *
 * @brief This class provides a wrapper facade for C strings.
 *
 * This class uses an <ACE_Allocator> to allocate memory.  The
 * user can make this a persistant class by providing an
 * ACE_Allocator with a persistable memory pool.  This class is
 * optimized for efficiency, so it doesn't provide any internal
 * locking.
 * NOTE: if an instance of this class is constructed from or
 * assigned an empty string (with first element of '\0'), then it
 * is _not_ allocated new space.  Instead, its internal
 * representation is set equal to a global empty string.
 * CAUTION: in cases when ACE_CString is constructed from a
 * provided buffer with the release parameter set to 0,
 * ACE_CString is not guaranteed to be '\0' terminated.
 */
class ACE_Export ACE_CString
{

public:
  /// No position constant
  static const int npos;

  /// Default constructor.
  ACE_CString (ACE_Allocator *alloc = 0);

  /**
   * Constructor that copies <s> into dynamically allocated memory.
   * If <release> is non-0 then the <ACE_allocator> is responsible for
   * freeing this memory. Memory is _not_ allocated/freed if <release>
   * is 0.
   */
  ACE_CString (const char *s,
               ACE_Allocator *alloc = 0,
               int release = 1);

  /**
   * Constructor that copies <len> chars of <s> into dynamically
   * allocated memory (will NUL terminate the result).  If <release>
   * is non-0 then the <ACE_allocator> is responsible for freeing this
   * memory.  Memory is _not_ allocated/freed if <release> is 0.
   */
  ACE_CString (const char *s,
               size_t len,
               ACE_Allocator *alloc = 0,
               int release = 1);

  /// Copy constructor.
  ACE_CString (const ACE_CString &);

  /// Constructor that copies <s> into dynamically allocated memory.
  /// Probable loss of data. Please use with care.
  ACE_CString (const ACE_WSTRING_TYPE *s,
               ACE_Allocator *alloc = 0);

  /// Constructor that copies <c> into dynamically allocated memory.
  ACE_CString (char c, ACE_Allocator *alloc = 0);

  /// Deletes the memory...
  ~ACE_CString (void);

  /// Return the <slot'th> character in the string (doesn't perform
  /// bounds checking).
  const char &operator [] (size_t slot) const;

  /// Return the <slot'th> character by reference in the string
  /// (doesn't perform bounds checking).
  char &operator [] (size_t slot);

  /// Assignment operator (does copy memory).
  ACE_CString &operator = (const ACE_CString &);

  /// Copy <s> into this <ACE_CString>.  Memory is _not_
  /// allocated/freed if <release> is 0.
  void set (const char *s, int release = 1);

  /// Copy <len> bytes of <s> (will NUL terminate the result).
  /// Memory is _not_ allocated/freed if <release> is 0.
  void set (const char *s,
            size_t len,
            int release);

  /**
   * Return a substring given an offset and length, if length == -1
   * use rest of str.  Return empty substring if offset or
   * offset/length are invalid.
   */
  ACE_CString substring (size_t offset, ssize_t length = -1) const;

  /// Same as <substring>.
  ACE_CString substr (size_t offset, ssize_t length = -1) const;

  /// Concat operator (copies memory).
  ACE_CString &operator += (const ACE_CString &);

  /// Returns a hash value for this string.
  u_long hash (void) const;

  /// Return the length of the string.
  size_t length (void) const;

  /// Get a copy of the underlying pointer.
  char *rep (void) const;

  /**
   * Get at the underlying representation directly!
   * _Don't_ even think about casting the result to (char *) and modifying it,
   * if it has length 0!
   */
  const char *fast_rep (void) const;

  /// Same as STL String's <c_str> and <fast_rep>.
  const char *c_str (void) const;

  /// Comparison operator that will match substrings.  Returns the
  /// slot of the first location that matches, else -1.
  int strstr (const ACE_CString &s) const;

  /// Find <str> starting at pos.  Returns the slot of the first
  /// location that matches (will be >= pos), else npos.
  int find (const ACE_CString &str, int pos = 0) const;

  /// Find <s> starting at pos.  Returns the slot of the first
  /// location that matches (will be >= pos), else npos.
  int find (const char *s, int pos = 0) const;

  /// Find <c> starting at pos.  Returns the slot of the first
  /// location that matches (will be >= pos), else npos.
  int find (char c, int pos = 0) const;

  /// Find <c> starting at pos (counting from the end).  Returns the
  /// slot of the first location that matches, else npos.
  int rfind (char c, int pos = npos) const;

  /// Equality comparison operator (must match entire string).
  int operator == (const ACE_CString &s) const;

  /// Less than comparison operator.
  int operator < (const ACE_CString &s) const;

  /// Greater than comparison operator.
  int operator > (const ACE_CString &s) const;

  /// Inequality comparison operator.
  int operator != (const ACE_CString &s) const;

  /// Performs a <strcmp>-style comparison.
  int compare (const ACE_CString &s) const;

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

private:
  /// Pointer to a memory allocator.
  ACE_Allocator *allocator_;

  /// Length of the ACE_CString data (not counting the trailing '\0').
  size_t len_;

  /// Length of the ACE_CString data buffer.  Keeping track of the
  /// length allows to avoid unnecessary dynamic allocations.
  size_t buf_len_;

  /// Pointer to data.
  char *rep_;

  /// Flag that indicates if we own the memory
  int release_;

  /// Represents the "NULL" string to simplify the internal logic.
  static char NULL_CString_;
};

ACE_Export ACE_INLINE ACE_CString operator + (const ACE_CString &,
                                              const ACE_CString &);
#if !defined (ACE_LACKS_IOSTREAM_TOTALLY)
ACE_Export ostream &operator << (ostream &, const ACE_CString &);
#endif /* ! ACE_LACKS_IOSTREAM_TOTALLY */

/**
 * @class ACE_WString
 *
 * @brief This class provides a wrapper facade for C wide strings.
 *
 * This class uses an <ACE_Allocator> to allocate memory.  The
 * user can make this a persistant class by providing an
 * <ACE_Allocator> with a persistable memory pool.  This class is
 * optimized for efficiency, so it doesn't provide any internal
 * locking.
 */
class ACE_Export ACE_WString
{
public:
  /// No position constant
  static const int npos;

  /// Default constructor.
  ACE_WString (ACE_Allocator *alloc = 0);

  /// Constructor that copies <s> into dynamically allocated memory.
  ACE_WString (const char *s,
               ACE_Allocator *alloc = 0);

  /// Constructor that copies <s> into dynamically allocated memory.
  ACE_WString (const ACE_WSTRING_TYPE *s,
               ACE_Allocator *alloc = 0);

#if defined (ACE_WSTRING_HAS_USHORT_SUPPORT)
  /// Constructor that takes in a ushort16 string (mainly used by the
  /// ACE Name_Space classes)
  ACE_WString (const ACE_USHORT16 *s,
               size_t len,
               ACE_Allocator *alloc = 0);
#endif /* ACE_WSTRING_HAS_USHORT_SUPPORT */

  /// Constructor that copies <len> ACE_WSTRING_TYPE's of <s> into dynamically
  /// allocated memory (will NUL terminate the result).
  ACE_WString (const ACE_WSTRING_TYPE *s,
               size_t len,
               ACE_Allocator *alloc = 0);

  /// Constructor that dynamically allocates memory for <len> + 1
  /// ACE_WSTRING_TYPE characters. The newly created memory is set memset to 0.
  ACE_WString (size_t len, ACE_Allocator *alloc = 0);

  /// Copy constructor.
  ACE_WString (const ACE_WString &s);

  /// Constructor that copies <c> into dynamically allocated memory.
  ACE_WString (ACE_WSTRING_TYPE c, ACE_Allocator *alloc = 0);

  /// Deletes the memory...
  ~ACE_WString (void);

  /// Return the <slot'th> character in the string (doesn't perform
  /// bounds checking).
  ACE_WSTRING_TYPE operator [] (size_t slot) const;

  /// Return the <slot'th> character by reference in the string
  /// (doesn't perform bounds checking).
  ACE_WSTRING_TYPE &operator [] (size_t slot);

  /// Assignment operator (does copy memory).
  ACE_WString &operator = (const ACE_WString &);

  /// Copy <s>
  void set (const ACE_WSTRING_TYPE *s);

  /// Copy <len> bytes of <s> (will NUL terminate the result)
  void set (const ACE_WSTRING_TYPE *s,
            size_t len);

  /**
   * Return a substring given an offset and length, if length == -1
   * use rest of str return empty substring if offset or offset/length
   * are invalid.
   */
  ACE_WString substring (size_t offset, ssize_t length = -1) const;

  /// Same as substring
  ACE_WString substr (size_t offset, ssize_t length = -1) const;

  /// Concat operator (does copy memory).
  ACE_WString &operator += (const ACE_WString &);

  /// Returns a hash value for this string.
  u_long hash (void) const;

  /// Return the length of the string.
  size_t length (void) const;

  /// Gets a copy of the underlying pointer.
  ACE_WSTRING_TYPE *rep (void) const;

  /// Transform into a copy of the ASCII character representation.
  /// (caller must delete)
  char *char_rep (void) const;

  /// Transform into a copy of a USHORT16 representation (caller must
  /// delete).  Note, behavior is undefined when sizeof (wchar_t) != 2.
  ACE_USHORT16 *ushort_rep (void) const;

  /// Get at the underlying representation directly!
  const ACE_WSTRING_TYPE *fast_rep (void) const;

  /// Same as STL String's <c_str> and <fast_rep>.
  const ACE_WSTRING_TYPE *c_str (void) const;

  /// Comparison operator that will match substrings.  Returns the
  /// slot of the first location that matches, else -1.
  int strstr (const ACE_WString &s) const;

  /// Find <str> starting at pos.  Returns the slot of the first
  /// location that matches (will be >= pos), else npos.
  int find (const ACE_WString &str, int pos = 0) const;

  /// Find <s> starting at pos.  Returns the slot of the first
  /// location that matches (will be >= pos), else npos.
  int find (const ACE_WSTRING_TYPE *s, int pos = 0) const;

  /// Find <c> starting at pos.  Returns the slot of the first
  /// location that matches (will be >= pos), else npos.
  int find (ACE_WSTRING_TYPE c, int pos = 0) const;

  /// Find <c> starting at pos (counting from the end).  Returns the
  /// slot of the first location that matches, else npos.
  int rfind (ACE_WSTRING_TYPE c, int pos = npos) const;

  /// Equality comparison operator (must match entire string).
  int operator == (const ACE_WString &s) const;

  /// Less than comparison operator.
  int operator < (const ACE_WString &s) const;

  /// Greater than comparison operator.
  int operator > (const ACE_WString &s) const;

  /// Inequality comparison operator.
  int operator != (const ACE_WString &s) const;

  /// Performs a <strcmp>-style comparison.
  int compare (const ACE_WString &s) const;

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

  /// Computes the length of a "0" terminated ACE_WSTRING_TYPE *.
  static size_t strlen (const ACE_WSTRING_TYPE *);

  /// Traditional style strstr
  static const ACE_WSTRING_TYPE *strstr (const ACE_WSTRING_TYPE *s1,
                                       const ACE_WSTRING_TYPE *s2);

  /**
   * This method is designed for high-performance. Please use with
   * care ;-) If the current size of the string is less than <len>,
   * the string is resized to the new length. The data is is zero'd
   * out after this operation.
   */
  void resize (size_t len);

private:
  /// Pointer to a memory allocator.
  ACE_Allocator *allocator_;

  /// Length of the ACE_WString.
  size_t len_;

  /// Pointer to data.
  ACE_WSTRING_TYPE *rep_;
};

ACE_Export ACE_INLINE ACE_WString operator+ (const ACE_WString &,
                                             const ACE_WString &);
#if !defined (ACE_LACKS_IOSTREAM_TOTALLY)
ACE_Export ostream &operator << (ostream &, const ACE_WString &);
#endif /* ! ACE_LACKS_IOSTREAM_TOTALLY */

/**
 * @class ACE_SString
 *
 * @brief A very Simple String <ACE_SString> class.  This is not a
 * general-purpose string class, and you should probably consider
 * using <ACE_CString> is you don't understand why this class
 * exists...
 *
 * This class is optimized for efficiency, so it doesn't provide
 * any internal locking.
 * CAUTION: This class is only intended for use with applications
 * that understand how it works.  In particular, its destructor
 * does not deallocate its memory when it is destroyed...  We need
 * this class since the <ACE_Map_Manager> requires an object that
 * supports the operator == and operator !=.  This class uses an
 * <ACE_Allocator> to allocate memory.  The user can make this a
 * persistant class by providing an <ACE_Allocator> with a
 * persistable memory pool.
 */
class ACE_Export ACE_SString
{
public:
  /// No position constant
  static const int npos;

  /// Default constructor.
  ACE_SString (ACE_Allocator *alloc = 0);

  /// Constructor that copies <s> into dynamically allocated memory.
  ACE_SString (const char *s, ACE_Allocator *alloc = 0);

  /// Constructor that copies <len> chars of <s> into dynamically
  /// allocated memory (will NUL terminate the result).
  ACE_SString (const char *s, size_t len, ACE_Allocator *alloc = 0);

  /// Copy constructor.
  ACE_SString (const ACE_SString &);

  /// Constructor that copies <c> into dynamically allocated memory.
  ACE_SString (char c, ACE_Allocator *alloc = 0);

  /// Default dtor.
  ~ACE_SString (void);

  /// Return the <slot'th> character in the string (doesn't perform
  /// bounds checking).
  char operator [] (size_t slot) const;

  /// Return the <slot'th> character by reference in the string
  /// (doesn't perform bounds checking).
  char &operator [] (size_t slot);

  /// Assignment operator (does copy memory).
  ACE_SString &operator = (const ACE_SString &);

  /**
   * Return a substring given an offset and length, if length == -1
   * use rest of str return empty substring if offset or offset/length
   * are invalid
   */
  ACE_SString substring (size_t offset, ssize_t length = -1) const;

  /// Same as substring
  ACE_SString substr (size_t offset, ssize_t length = -1) const;

  /// Returns a hash value for this string.
  u_long hash (void) const;

  /// Return the length of the string.
  size_t length (void) const;

  /// Set the underlying pointer.  Since this does not copy memory or
  /// delete existing memory use with extreme caution!!!
  void rep (char *s);

  /// Get the underlying pointer.
  const char *rep (void) const;

  /// Get the underlying pointer.
  const char *fast_rep (void) const;

  /// Same as STL String's <c_str> and <fast_rep>.
  const char *c_str (void) const;

  /// Comparison operator that will match substrings.  Returns the
  /// slot of the first location that matches, else -1.
  int strstr (const ACE_SString &s) const;

  /// Find <str> starting at pos.  Returns the slot of the first
  /// location that matches (will be >= pos), else npos.
  int find (const ACE_SString &str, int pos = 0) const;

  /// Find <s> starting at pos.  Returns the slot of the first
  /// location that matches (will be >= pos), else npos.
  int find (const char *s, int pos = 0) const;

  /// Find <c> starting at pos.  Returns the slot of the first
  /// location that matches (will be >= pos), else npos.
  int find (char c, int pos = 0) const;

  /// Find <c> starting at pos (counting from the end).  Returns the
  /// slot of the first location that matches, else npos.
  int rfind (char c, int pos = npos) const;

  /// Equality comparison operator (must match entire string).
  int operator == (const ACE_SString &s) const;

  /// Less than comparison operator.
  int operator < (const ACE_SString &s) const;

  /// Greater than comparison operator.
  int operator > (const ACE_SString &s) const;

  /// Inequality comparison operator.
  int operator != (const ACE_SString &s) const;

  /// Performs a <strcmp>-style comparison.
  int compare (const ACE_SString &s) const;

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

private:
  /// Pointer to a memory allocator.
  ACE_Allocator *allocator_;

  /// Length of the ACE_SString (not counting the trailing '\0').
  size_t len_;

  /// Pointer to data.
  char *rep_;
};

#if !defined (ACE_LACKS_IOSTREAM_TOTALLY)
ACE_Export ostream &operator << (ostream &, const ACE_SString &);
#endif /* ! ACE_LACKS_IOSTREAM_TOTALLY */

// This allows one to use W or C String based on the Unicode
// setting
#if defined (ACE_USES_WCHAR)
# define ACE_TString ACE_WString
#else /* ACE_USES_WCHAR */
# define ACE_TString ACE_CString
#endif /* ACE_USES_WCHAR */


// ************************************************************

/**
 * @class ACE_Tokenizer
 *
 * @brief Tokenizer
 *
 * Tokenizes a buffer.  Allows application to set delimiters and
 * preserve designators.  Does not allow special characters, yet
 * (e.g., printf ("\"like a quoted string\"")).
 */
class ACE_Export ACE_Tokenizer
{
public:
  /// <buffer> will be parsed.
  ACE_Tokenizer (ACE_TCHAR *buffer);

  /// <d> is a delimiter.  Returns 0 on success, -1 if there is no
  /// memory left.
  int delimiter (ACE_TCHAR d);

  /**
   * <d> is a delimiter and, when found, will be replaced by
   * <replacement>.  Returns 0 on success, -1 if there is no memory
   * left.
   */
  int delimiter_replace (ACE_TCHAR d, ACE_TCHAR replacement);

  /**
   * For instance, quotes, or '(' and ')'.  Returns 0 on success, -1
   * if there is no memory left.  If <strip> == 1, then the preserve
   * designators will be stripped from the tokens returned by next.
   */
  int preserve_designators (ACE_TCHAR start, ACE_TCHAR stop, int strip=1);

  /// Returns the next token.
  ACE_TCHAR *next (void);

  enum {
    MAX_DELIMITERS=16,
    MAX_PRESERVES=16
  };

protected:
  /// Returns 1 if <d> is a delimiter, 0 otherwise.  If <d> should be
  /// replaced with <r>, <replace> is set to 1, otherwise 0.
  int is_delimiter (ACE_TCHAR d, int &replace, ACE_TCHAR &r);

  /**
   * If <start> is a start preserve designator, returns 1 and sets
   * <stop> to the stop designator.  Returns 0 if <start> is not a
   * preserve designator.
   */
  int is_preserve_designator (ACE_TCHAR start, ACE_TCHAR &stop, int &strip);

  ACE_TCHAR *buffer_;
  int index_;

  /**
   * @class Preserve_Entry
   *
   * @brief Preserve Entry
   *
   * Defines a set of characters that designate an area that
   * should not be parsed, but should be treated as a complete
   * token.  For instance, in: (this is a preserve region), start
   * would be a left paren -(- and stop would be a right paren
   * -)-.  The strip determines whether the designators should be
   * removed from the token.
   */
  class Preserve_Entry
  {
  public:
    /**
     * E.g., "(".
     * E.g., ")".
     * Whether the designators should be removed from the token.
     */
    ACE_TCHAR start_;
    ACE_TCHAR stop_;
    int strip_;
  };

  /// The application can specify MAX_PRESERVES preserve designators.
  Preserve_Entry preserves_[MAX_PRESERVES];

  /// Pointer to the next free spot in preserves_.
  int preserves_index_;

  /**
   * @class Delimiter_Entry
   *
   * @brief Delimiter Entry
   *
   * Describes a delimiter for the tokenizer.
   */
  class Delimiter_Entry
  {
  public:
    /**
     * Most commonly a space ' '.
     * What occurrences of delimiter_ should be replaced with.
     * Whether replacement_ should be used.  This should be replaced
     * with a technique that sets replacement_ = delimiter by
     * default.  I'll do that next iteration.
     */
    ACE_TCHAR delimiter_;
    ACE_TCHAR replacement_;
    int replace_;
  };

  /// The tokenizer allows MAX_DELIMITERS number of delimiters.
  Delimiter_Entry delimiters_[MAX_DELIMITERS];

  /// Pointer to the next free space in delimiters_.
  int delimiter_index_;
};

// ****************************************************************

/**
 * @class ACE_Auto_String_Free
 *
 * @brief Simple class to automatically de-allocate strings
 *
 * Keeps a pointer to a string and deallocates it (using
 * <ACE_OS::free>) on its destructor.
 * If you need to delete using "delete[]" the
 * ACE_Auto_Array_Ptr<char*> is your choice.
 * The class plays the same role as auto_ptr<>
 */
class ACE_Export ACE_Auto_String_Free
{
public:
  ACE_EXPLICIT ACE_Auto_String_Free (char* p = 0);
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

#include "ace/post.h"
#endif /* ACE_SSTRING_H */
