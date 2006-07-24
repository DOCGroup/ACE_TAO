// -*- C++ -*-

//=============================================================================
/**
 *  @file    Encodable.h
 *
 *  $Id$
 *
 *   Defines the interface for classes that wish to be
 *   encodable/decodable into/from a CDR representation.
 *
 *  @author  Angelo Corsaro <corsaro@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_ENCODABLE_H_
#define TAO_ENCODABLE_H_

#include /**/ "ace/pre.h"

#include "tao/Basic_Types.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Utils/utils_export.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_OutputCDR;
class TAO_InputCDR;

/**
 * @class TAO_Encodable
 *
 * @brief TAO_Encodable Interface
 *
 * This interface should be inherited by classes that wish to be
 * encoded/decoded into/from a CDR stream. Implementation of the
 * streaming methods is deferred to the subclasses.
 */
class TAO_UTILS_Export TAO_Encodable
{
public:

  virtual ~TAO_Encodable (void);

  ///  Encodes the object implementing this method into a CDR stream.
  ///  Returns true on success and false on failure.
  virtual CORBA::Boolean _tao_encode (TAO_OutputCDR &out_cdr) = 0;

  ///  Decodes the object implementing this method from a CDR stream.
  ///  Returns true on success and false on failure.
  virtual CORBA::Boolean _tao_decode (TAO_InputCDR &in_cdr) = 0;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_ENCODABLE_H_ */
