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
//     Defines the  interface for
//     classes that wish to be encodable/decodable into/from a CDR
//     representation.
//
// = AUTHOR
//     Angelo Corsaro <corsaro@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_ENCODABLE_H_
#define TAO_ENCODABLE_H_

#include "ace/pre.h"
#include "tao/orbconf.h"
#include "tao/corbafwd.h"

// @@ Angelo, I fixed several typos in class and file descriptions.
// Make sure to review what you write!!!

// @@ Angelo, when you type, many of the adjacent words have more than
// 1 space between them.  Please make sure to use just one space!

// @@ Angelo, do we need to include orbconf.h ?  If not, make sure to
// delete it.  We need to avoid creating unnecessary dependencies,
// since they increase compilation times.

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif // ACE_LACKS_PRAGMA_ONCE

class TAO_Export TAO_Encodable
{
// = TITLE
//    TAO_Encodable Interface
//
// = DESCRIPTION
//    This interface for use by classes that
//    whish to be encoded/decoded into/from a CDR
//    stream. Implementation of the streaming
//    methods is deferred to the subclasses.
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

#endif // defined (TAO_ENCODABLE_H_)

// @@ Angelo, please use #endif // TAO_ENCODABLE_H_
// In other words, don't include 'defined' and parens in the comment
// for endif.
