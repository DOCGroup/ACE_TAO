
// -*- C++ -*-
// ===================================================================
/**
 *  @file   GIOP_Message_Generator_Parser_Impl.h
 *
 *  $Id$
 *
 *  @author Balachandran Natarajan <bala@cs.wustl.edu>
 */
// ===================================================================

#ifndef TAO_GIOP_MESSAGE_GENERATOR_PARSER_IMPL_H
#define TAO_GIOP_MESSAGE_GENERATOR_PARSER_IMPL_H
#include /**/ "ace/pre.h"

#include "tao/GIOP_Message_Generator_Parser_11.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/GIOP_Message_Generator_Parser_12.h"

/**
 * @class TAO_GIOP_Message_Generator_Parser_Impl
 *
 * @brief This class holds all the different GIOP message generators
 *  and parsers.
 *
 *  This class can be done away with if we want to use the svc.conf
 *  file to load the right GIOP protocol. But that would require some
 *  work as we need to make sure that we have the behaviours of lower
 *  versions in the higher versions.
 */

class TAO_Export TAO_GIOP_Message_Generator_Parser_Impl
{
public:

  /// Performs a check of the revision numbers
  static CORBA::Boolean check_revision (CORBA::Octet incoming_major,
                                        CORBA::Octet incoming_minor);

  /// Version 1.0 of GIOP
  TAO_GIOP_Message_Generator_Parser_10 tao_giop_10;

  /// Version 1.1 of GIOP
  TAO_GIOP_Message_Generator_Parser_11 tao_giop_11;

  /// Version 1.2 of GIOP
  TAO_GIOP_Message_Generator_Parser_12 tao_giop_12;
};

#if defined (__ACE_INLINE__)
# include "tao/GIOP_Message_Generator_Parser_Impl.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /*TAO_GIOP_MESSAGE_GENERATOR_PARSER_IMPL_H*/
