// $Id$

// ============================================================================
//
// = LIBRARY
//   TAO/tests/Object_Loader
//
// = FILENAME
//   Test_i.h
//
// = AUTHOR
//   Carlos O'Ryan
//
// ============================================================================

#ifndef TAO_OBJECT_LOADER_TEST_I_H
#define TAO_OBJECT_LOADER_TEST_I_H

#include "TestS.h"
#include "OLT_Export.h"

class OLT_Export Test_i : public POA_Test
{
  // = TITLE
  //   Implement the Test interface
  //
  // = DESCRIPTION
  //
public:
  Test_i (PortableServer::POA_ptr poa);
  // ctor

  CORBA::Long instance_count (CORBA::Environment &)
    ACE_THROW_SPEC (());

  void destroy (CORBA::Environment &)
    ACE_THROW_SPEC (());

  PortableServer::POA_ptr _default_POA (CORBA::Environment &)
    ACE_THROW_SPEC (());

private:
  PortableServer::POA_var poa_;
  // The default POA used for this object

  static CORBA::Long instance_count_;
};

#if defined(__ACE_INLINE__)
#include "Test_i.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_OBJECT_LOADER_TEST_I_H */
