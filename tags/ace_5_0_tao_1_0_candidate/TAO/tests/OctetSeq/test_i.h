// $Id$

// ============================================================================
//
// = LIBRARY
//   TAO/tests/OctetSeq
//
// = FILENAME
//   test_i.h
//
// = AUTHOR
//   Carlos O'Ryan
//
// ============================================================================

#ifndef TAO_OCTETSEQ_TEST_I_H
#define TAO_OCTETSEQ_TEST_I_H

#include "testS.h"

class Database_i : public POA_Test::Database
{
  // = DESCRIPTION
  //   Implements the Database interface in test.idl
  //
public:
  Database_i (CORBA::ORB_ptr orb,
              Test::Index max_range);
  // ctor

  // = The Simple_Server methods.
  void set (Test::Index i,
            const Test::OctetSeq& seq,
            CORBA::Environment&)
    ACE_THROW_SPEC ((Test::OutOfRange));

  Test::OctetSeq* get (Test::Index i,
                       CORBA::Environment&)
    ACE_THROW_SPEC ((CORBA::SystemException,Test::OutOfRange));

  CORBA::ULong get_crc (Test::Index i,
                        CORBA::Environment&)
    ACE_THROW_SPEC ((CORBA::SystemException,Test::OutOfRange));

  void shutdown (CORBA::Environment&)
    ACE_THROW_SPEC (());

private:
  CORBA::ORB_var orb_;
  // The ORB pointer

  Test::Index max_range_;
  // The maximum number of elements in the database

  Test::OctetSeq* elements_;
  // The elements
};

#if defined(__ACE_INLINE__)
#include "test_i.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_MT_CLIENT_TEST_I_H */
