// $Id$

// ============================================================================
//
// = LIBRARY
//   TAO
//
// = FILENAME
//   CORBALOC_Parser.h
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
// ============================================================================

#ifndef TAO_CORBALOC_PARSER_H
#define TAO_CORBALOC_PARSER_H
#include "ace/pre.h"

#include "tao/IOR_Parser.h"
#include "ior_corbaloc_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_IOR_CORBALOC_Export TAO_CORBALOC_Parser : public TAO_IOR_Parser
{
  // = TITLE
  //   Implements the <corbaloc:> IOR format
  //
  // = DESCRIPTION
  //   This class implements the <corbaloc:> IOR format.
  //   It is dynamically loaded by the ORB and used to parse the
  //   string to separate the individual <obj_addr> from the list of object
  //   addresses <obj_addr_list>.
  //
public:
  TAO_CORBALOC_Parser (void);
  // Constructor

  virtual ~TAO_CORBALOC_Parser (void);
  // The destructor

  // = The IOR_Parser methods, please read the documentation in
  //   IOR_Parser.h
  virtual int match_prefix (const char *ior_string) const;
  virtual CORBA::Object_ptr parse_string (const char *ior,
                                          CORBA::ORB_ptr orb,
                                          CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException));
};

#if defined (__ACE_INLINE__)
# include "CORBALOC_Parser.i"
#endif /* __ACE_INLINE__ */

ACE_FACTORY_DECLARE (TAO_IOR_CORBALOC, TAO_CORBALOC_Parser)

#include "ace/post.h"
#endif /* TAO_CORBALOC_PARSER_H */
