// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file    Parser_Registry.h
 *
 *  $Id$
 *
 *  @author  Priyanka Gontla <pgontla@uci.edu>
 *  @author  Carlos O'Ryan <coryan@uci.edu>
 */
//=============================================================================


#ifndef TAO_PARSER_REGISTRY_H
#define TAO_PARSER_REGISTRY_H

#include "ace/pre.h"

#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

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
  // = Initialization and termination methods.
  /// Default constructor.
  TAO_Parser_Registry (void);

  /// Dstructor.
  ~TAO_Parser_Registry (void);

  /// Initialize the parser registry with the list of known protocols.
  /// Invoked by the ORB during startup.
  int open (TAO_ORB_Core *orb_core);

  /// Find the parser that can parse <ior_string>
  /// The lookup is based on the prefix in the string
  TAO_IOR_Parser *match_parser (const char *ior_string);

  // = Iterator.
  typedef TAO_IOR_Parser** Parser_Iterator;
  Parser_Iterator begin (void) const;
  Parser_Iterator end (void) const;

private:

  // The parser registry should not be copied.
  TAO_Parser_Registry (const TAO_Parser_Registry&);
  void operator= (const TAO_Parser_Registry&);

private:

  /// List of parsers
  TAO_IOR_Parser **parsers_;

  /// Number of parsers
  size_t size_;

};

#if defined(__ACE_INLINE__)
#include "tao/Parser_Registry.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_PARSER_REGISTRY_H */
