// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    Encodable.h
//
// = DESCRIPTION
//     Defines the interface for classes that wish to be
//     encodable/decodable into/from a CDR representation.
//
// = AUTHOR
//     Angelo Corsaro <corsaro@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_ENCODABLE_H_
#define TAO_ENCODABLE_H_

#include "ace/pre.h"
#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif // ACE_LACKS_PRAGMA_ONCE

class TAO_Export TAO_Encodable
{
// = TITLE
//    TAO_Encodable Interface
//
// = DESCRIPTION
//    This interface should be inherited by classes that wish to be
//    encoded/decoded into/from a CDR stream. Implementation of the
//    streaming methods is deferred to the subclasses.
//
public:

  virtual ~TAO_Encodable (void);

  virtual CORBA::Boolean _tao_encode (TAO_OutputCDR &out_cdr) = 0;
  //  Encodes the object implementing this method into a CDR stream.
  //  Returns true on success and false on failure.

  virtual CORBA::Boolean _tao_decode (TAO_InputCDR &in_cdr) = 0;
  //  Decodes the object implementing this method from a CDR stream.
  //  Returns true on success and false on failure.
};

#include "ace/post.h"

#endif /* TAO_ENCODABLE_H_ */
