// $Id$

// ============================================================================
//
// = LIBRARY
//   TAO
//
// = FILENAME
//   iopfwd.h
//
// = DESCRIPTION
//   Forward declare the types in the IOP namespace.
//
// = AUTHOR
//   Carlos O'Ryan
//
// ============================================================================

#ifndef TAO_IOPFWD_H
#define TAO_IOPFWD_H

#include "tao/corbafwd.h"

class TAO_Export IOP
{
  // = TITLE
  //   Provides the IOP namespace.
  //
  // = DESCRIPTION
public:
  typedef CORBA::ULong ComponentId;
  typedef CORBA::ULong_out ComponentId_out;
  // static CORBA::TypeCode_ptr _tc_ComponentId;

  typedef CORBA::ULong ServiceId;
  typedef CORBA::ULong_out ServiceId_out;
  // static CORBA::TypeCode_ptr _tc_ServiceId;

  // @@ Add more types....
};

#endif /* TAO_IOPFWD_H */
