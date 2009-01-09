
/* -*- C++ -*- */

//=============================================================================
/**
 *  @file   HTTP_Parser.h
 *
 *  $Id$
 *
 *  @author Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================


#ifndef TAO_HTTP_PARSER_H
#define TAO_HTTP_PARSER_H

#include /**/ "ace/pre.h"
#include "ace/Service_Config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/IOR_Parser.h"

#if (TAO_HAS_HTTP_PARSER == 1)

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_HTTP_Parser
 *
 * @brief Implements the <http:> IOR format
 *
 * This class implements the <http:> IOR format.
 * It is dynamically loaded by the ORB and used to get an IOR
 * from a http server and then interprete the data as an IOR (that
 * can be in any valid format).
 */
class TAO_HTTP_Parser : public TAO_IOR_Parser
{
public:

  /// The destructor
  virtual ~TAO_HTTP_Parser (void);

  // = The IOR_Parser methods, please read the documentation in
  //   IOR_Parser.h
  virtual bool match_prefix (const char *ior_string) const;
  virtual CORBA::Object_ptr parse_string (const char *ior, CORBA::ORB_ptr orb);
};

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_STATIC_SVC_DECLARE_EXPORT (TAO, TAO_HTTP_Parser)
ACE_FACTORY_DECLARE (TAO, TAO_HTTP_Parser)

#endif /* TAO_HAS_HTTP_PARSER == 1 */

#include /**/ "ace/post.h"
#endif /* TAO_HTTP_PARSER_H */

