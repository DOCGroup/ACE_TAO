
//=============================================================================
/**
 *  @file   Lib_Find.h
 *
 *  $Id$
 *
 *  @author Priyanka Gontla <pgontla@ece.uci.edu>
 */
//=============================================================================


#ifndef ACE_LIB_FIND_H
#define ACE_LIB_FIND_H
#include "ace/pre.h"

#include "ace/OS/OS.h"

/**
 * @class ACE_Lib_Find
 *
 * This class includes all the static function calls needed to search
 * and open shared libraries. 
 */
class ACE_Export ACE_Lib_Find
{
 public:

  // = Methods for searching and opening shared libraries.

  /**
   * Finds the file <filename> either using an absolute path or using
   * a relative path in conjunction with ACE_LD_SEARCH_PATH (e.g.,
   * $LD_LIBRARY_PATH on UNIX or $PATH on Win32).  This function will
   * add appropriate suffix (e.g., .dll on Win32 or .so on UNIX)
   * according to the OS platform.  In addition, this function will
   * apply the appropriate prefix (e.g., "lib" on UNIX and "" on
   * Win32) if the <filename> doesn't match directly.
   */
  static int ldfind (const ACE_TCHAR *filename,
                     ACE_TCHAR *pathname,
                     size_t maxlen);

  /**
   * Uses <ldfind> to locate and open the appropriate <filename> and
   * returns a pointer to the file, else it returns a NULL
   * pointer. <type> specifies how the file should be open.
   */
  static FILE *ldopen (const ACE_TCHAR *filename,
                       const ACE_TCHAR *type);

  /**
   * Transforms <entry_point> into a form that can be located in a
   * dynamic library using <dlsym>. For example, with Win32/Borland
   * extern "C" functions which use the default calling convention
   * have a '_' prepended. Always returns a buffer that has been
   * dynamically allocated using <operator new>.
   */
  static ACE_TCHAR *ldname (const ACE_TCHAR *entry_point);

  /**
   * Returns the temporary directory including the trailing slash in
   * <buffer>.  Returns -1 for an error or if the buffer_len is not
   * long enough.
   */
  static int get_temp_dir (ACE_TCHAR *buffer, size_t buffer_len);

  /// Opening the temp file.  File is automagically unlinked when it is
  /// closed.  This is useful for have temp files.
  static ACE_HANDLE open_temp_file (const ACE_TCHAR *name,
                                    int mode,
                                    int perm = 0);

  // @@ Though the following functions dont come under the same category as
  // above, these are used only in the functions in this class. So it makes
  // more sense to move these functions too to this class.
  //
  /// Replace all instances of <search> in <s> with <replace>.  Returns
  /// the number of replacements made.
  static size_t strrepl (char *s, char search, char replace);

  /**
   * Splits string <s> into pieces separated by the string <token>.
   * <next_start> is an opaque cookie handed back by the call to store
   * its state for the next invocation, thus making it re-entrant.
   * This operates very similar to Perl's <split> function except that
   * it returns pieces one at a time instead of into an array.
   */
  static char *strsplit_r (char *s, const char *token, char *&next_start);

#if defined (ACE_HAS_WCHAR)
  /// As strrepl, but for wide characters.
  static size_t strrepl (wchar_t *s, wchar_t search, wchar_t replace);

  /// As strsplit_r, but for wide characters.
  static wchar_t *strsplit_r (wchar_t *s, const wchar_t *token,
                              wchar_t *&next_start);
#endif /* ACE_HAS_WCHAR */
};

#if !defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "Lib_Find.i"
#endif /* ACE_LACKS_INLINE_FUNCTIONS */

#include "ace/post.h"
#endif  /* ACE_LIB_FIND_H */
