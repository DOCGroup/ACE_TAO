
//=============================================================================
/**
 *  @file   DLL_Parser.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 */
//=============================================================================


#ifndef TAO_DLL_PARSER_H
#define TAO_DLL_PARSER_H
#include "ace/pre.h"

#include "tao/IOR_Parser.h"
#include "ace/Service_Config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class TAO_DLL_Parser
 *
 * @brief Defines the interface for the pluggable DLL parser components
 *
 * The ORB is able to dynamically load the list of DLL formats it
 * understands.  That allow us to integrate formats such as
 * corbaname:, http: or ftp: only when required.
 * This class provides a uniform interface for all the DLL parsers
 */
class TAO_Export TAO_DLL_Parser : public TAO_IOR_Parser
{
public:
  /// Constructor
  TAO_DLL_Parser (void);

  /// The destructor
  virtual ~TAO_DLL_Parser (void);

  // = The IOR_Parser methods, please read the documentation in
  //   IOR_Parser.h
  virtual int match_prefix (const char *ior_string) const;
  virtual CORBA::Object_ptr parse_string (const char *ior,
                                          CORBA::ORB_ptr orb,
                                          CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException));
};

#if defined (__ACE_INLINE__)
# include "DLL_Parser.i"
#endif /* __ACE_INLINE__ */

ACE_STATIC_SVC_DECLARE (TAO_DLL_Parser)
ACE_FACTORY_DECLARE (TAO, TAO_DLL_Parser)

#include "ace/post.h"
#endif /* TAO_DLL_PARSER_H */
