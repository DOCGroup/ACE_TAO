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
//     Defines the  interface that should  be implemented by the
//     classes that whish to  be  encodable into a CDR representation.
//
//     NOTE: This interface is different in purpose from the 
//           operaror << and operator >>. In fact those operator
//           usually create and object reference, while this interface
//           is meant to store and retrieve an object state (i.e. member
//           variable) into or from CDR. 
//
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

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif // ACE_LACKS_PRAGMA_ONCE 


class TAO_Export TAO_Encodable 
{

// = TITLE
//    TAO_Encodable Interface
//
// = DESCRIPTION
//    This interface has to be implemented by class that
//    whish to  be encoded into  CDR. The responsability
//    or implementing the streaming methods is deferred to 
//    the subclasses.
//
public:

  virtual ~TAO_Encodable (void);

  virtual CORBA::Boolean _tao_encode (TAO_OutputCDR &out_cdr) = 0;
  //  Encode the Object into a CDR representation. 

  virtual CORBA::Boolean _tao_decode (TAO_InputCDR &in_cdr) = 0;
  //  Decode the Object from a CDR representation. If the CDR
  //  given has the wrong format than the method will return
  //  false.
}; 


#endif // defined (TAO_ENCODABLE_H_)
