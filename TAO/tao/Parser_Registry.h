//=============================================================================
/**
 *  @file    Parser_Registry.h
 *
 *  @author  Priyanka Gontla <pgontla@uci.edu>
 *  @author  Carlos O'Ryan <coryan@uci.edu>
 */
//=============================================================================

#ifndef TAO_PARSER_REGISTRY_H
#define TAO_PARSER_REGISTRY_H

#include /**/ "ace/pre.h"

#include /**/ "tao/TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "tao/Versioned_Namespace.h"

#include "ace/os_include/os_stddef.h"
#include <memory>
#include <vector>

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// Forward declarations.
class TAO_ORB_Core;
class TAO_IOR_Parser;

/**
 * @class TAO_Parser_Registry
 *
 * @brief Maintain the collection of known IOR format parsers
 *
 * The ORB dynamically loads a collection of IOR parsers (check
 * the IOR_Parser class).  The collection is kept in this class
 * for easy lookup and use.
 */
class TAO_Export TAO_Parser_Registry
{
public:
  /// Default constructor.
  TAO_Parser_Registry ();

  /// Dstructor.
  ~TAO_Parser_Registry ();

  /// Initialize the parser registry with the list of known protocols.
  /// Invoked by the ORB during startup.
  int open (TAO_ORB_Core *orb_core);

  /// Find the parser that can parse @a ior_string
  /// The lookup is based on the prefix in the string
  TAO_IOR_Parser *match_parser (const char *ior_string);

  // = Iterator.
  using const_iterator = std::vector<TAO_IOR_Parser *>::const_iterator;
  const_iterator begin () const;
  const_iterator end () const;

private:
  // The parser registry should not be copied.
  TAO_Parser_Registry (TAO_Parser_Registry const &) = delete;
  TAO_Parser_Registry &operator= (TAO_Parser_Registry const &) = delete;
  TAO_Parser_Registry (TAO_Parser_Registry &&) = delete;
  TAO_Parser_Registry &operator= (TAO_Parser_Registry &&) = delete;

private:
  /// Non-owning pointers to the registered parser services.
  std::vector<TAO_IOR_Parser *> parsers_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(__ACE_INLINE__)
#include "tao/Parser_Registry.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_PARSER_REGISTRY_H */
