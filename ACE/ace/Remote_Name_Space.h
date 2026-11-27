/**
 *  @file    Remote_Name_Space.h
 *
 *  @author Prashant Jain
 */

#ifndef ACE_REMOTE_NAME_SPACE_H
#define ACE_REMOTE_NAME_SPACE_H

#include /**/ "ace/pre.h"

#include /**/ "ace/ACE_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Name_Proxy.h"
#include "ace/Name_Space.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

class ACE_NS_WString;

using ACE_WSTRING_SET = ACE_Unbounded_Set<ACE_NS_WString>;

/**
 * @class ACE_Remote_Name_Space
 *
 * @brief Maintaining accesses Remote Name Server Database.  Allows to
 * add NameBindings, change them, remove them and resolve
 * NameBindings.
 *
 * Manages a Naming Service for a remote name space which
 * includes bindings for net_local naming context.  All strings
 * are stored in wide character format.  A Name Binding consists
 * of a name (that's the key), a value string and an optional
 * type string (no wide chars).
 */
class ACE_Export ACE_Remote_Name_Space : public ACE_Name_Space
{
public:
  /// "Do-nothing" constructor.
  ACE_Remote_Name_Space ();

  /**
   * Specifies the scope of this namespace, opens and memory-maps the
   * associated file (if accessible) or contacts the dedicated name
   * server process for NET_LOCAL namespace.
   */
  ACE_Remote_Name_Space (ACE_TCHAR const *hostname, u_short port);

  /**
   * Specifies the scope of this namespace, opens and memory-maps the
   * associated file (if accessible) or contacts the dedicated name
   * server process for NET_LOCAL namespace.
   */
  int open (ACE_TCHAR const *servername, u_short port);

  /// destructor, do some cleanup :TBD: last dtor should "compress"
  /// file
  ~ACE_Remote_Name_Space () override;

  /// Bind a new name to a naming context (Wide character strings).
  int bind (ACE_NS_WString const &name_in,
                    ACE_NS_WString const &value_in,
                    char const *type_in = "") override;

  /**
   * Overwrite the value or type of an existing name in a
   * ACE_Remote_Name_Space or bind a new name to the context, if it
   * didn't exist yet. (Wide character strings interface).
   */
  int rebind (ACE_NS_WString const &name_in,
                      ACE_NS_WString const &value_in,
                      char const *type_in = "") override;

  /// Delete a name from a ACE_Remote_Name_Space (Wide character strings
  /// Interface).
  int unbind (ACE_NS_WString const &name_in) override;

  /// Get value and type of a given name binding (Wide chars).  The
  /// caller is responsible for deleting both @a value_out and @a type_out!
  int resolve (ACE_NS_WString const &name_in,
                       ACE_NS_WString &value_out,
                       char *&type_out) override;

  /// Get a set of names matching a specified pattern (wchars). Matching
  /// means the names must begin with the pattern string.
  int list_names (ACE_WSTRING_SET &set_out,
                          ACE_NS_WString const &pattern_in) override;

  /// Get a set of values matching a specified pattern (wchars). Matching
  /// means the values must begin with the pattern string.
  int list_values (ACE_WSTRING_SET &set_out,
                           ACE_NS_WString const &pattern_in) override;

  /// Get a set of types matching a specified pattern (wchars). Matching
  /// means the types must begin with the pattern string.
  int list_types (ACE_WSTRING_SET &set_out,
                          ACE_NS_WString const &pattern_in) override;

  /**
   * Get a set of names matching a specified pattern (wchars). Matching
   * means the names must begin with the pattern string. Returns the
   * complete binding associated each pattern match.
   */
  int list_name_entries (ACE_BINDING_SET &set,
                                 ACE_NS_WString const &pattern) override;

  /**
   * Get a set of values matching a specified pattern (wchars). Matching
   * means the values must begin with the pattern string. Returns the
   * complete binding associated each pattern match.
   */
  int list_value_entries (ACE_BINDING_SET &set,
                                  ACE_NS_WString const &pattern) override;

  /**
   * Get a set of types matching a specified pattern (wchars). Matching
   * means the types must begin with the pattern string. Returns the
   * complete binding associated each pattern match.
   */
  int list_type_entries (ACE_BINDING_SET &set,
                                 ACE_NS_WString const &pattern) override;

  /// Dump the state of the object.
  void dump () const override;

  ACE_ALLOC_HOOK_DECLARE;

private:
  /// Interface to Name server process for NET_LOCAL namespace.
  ACE_Name_Proxy ns_proxy_;
};

ACE_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* ACE_REMOTE_NAME_SPACE_H */
