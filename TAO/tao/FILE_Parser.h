
//=============================================================================
/**
 *  @file   FILE_Parser.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 */
//=============================================================================


#ifndef TAO_FILE_PARSER_H
#define TAO_FILE_PARSER_H
#include "ace/pre.h"

#include "tao/IOR_Parser.h"
#include "ace/Service_Config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class TAO_FILE_Parser
 *
 * @brief Implements the <file:> IOR format
 *
 * This class implements the <file:> IOR format.
 * It is dynamically loaded by the ORB and used to open a file,
 * read its contents and then interepret the file as an IOR (that
 * can be in any valid format).
 */
class TAO_Export TAO_FILE_Parser : public TAO_IOR_Parser
{
public:
  /// Constructor
  TAO_FILE_Parser (void);

  /// The destructor
  virtual ~TAO_FILE_Parser (void);

  // = The IOR_Parser methods, please read the documentation in
  //   IOR_Parser.h
  virtual int match_prefix (const char *ior_string) const;
  virtual CORBA::Object_ptr parse_string (const char *ior,
                                          CORBA::ORB_ptr orb
                                          ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));
};

#if defined (__ACE_INLINE__)
# include "FILE_Parser.i"
#endif /* __ACE_INLINE__ */

ACE_STATIC_SVC_DECLARE_EXPORT (TAO, TAO_FILE_Parser)
ACE_FACTORY_DECLARE (TAO, TAO_FILE_Parser)

#include "ace/post.h"
#endif /* TAO_FILE_PARSER_H */
