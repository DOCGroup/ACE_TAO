// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//     TAO
//
// = AUTHOR
//     Priyanka Gontla <pgontla@uci.edu>
//     Carlos O'Ryan <coryan@uci.edu>
//
// ============================================================================

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

class TAO_Export TAO_Parser_Registry
{
  // = TITLE
  //   Maintain the collection of known IOR format parsers
  //
  // = DESCRIPTION
  //   The ORB dynamically loads a collection of IOR parsers (check
  //   the IOR_Parser class).  The collection is kept in this class
  //   for easy lookup and use.
  //
public:
  // = Initialization and termination methods.
  TAO_Parser_Registry (void);
  // Default constructor.

  ~TAO_Parser_Registry (void);
  // Dstructor.

  int open (TAO_ORB_Core *orb_core);
  // Initialize the parser registry with the list of known protocols.
  // Invoked by the ORB during startup.

  TAO_IOR_Parser *match_parser (const char *ior_string);
  // Find the parser that can parse <ior_string>
  // The lookup is based on the prefix in the string

  // = Iterator.
  typedef TAO_IOR_Parser** Parser_Iterator;
  Parser_Iterator begin (void) const;
  Parser_Iterator end (void) const;

private:
  // The parser registry should not be copied.
  ACE_UNIMPLEMENTED_FUNC (TAO_Parser_Registry (const TAO_Parser_Registry&))
  ACE_UNIMPLEMENTED_FUNC (void operator= (const TAO_Parser_Registry&))

private:
  TAO_IOR_Parser **parsers_;
  // List of parsers

  size_t size_;
  // Number of parsers
};

#if defined(__ACE_INLINE__)
#include "tao/Parser_Registry.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_PARSER_REGISTRY_H */
