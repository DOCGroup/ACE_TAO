
//=============================================================================
/**
 *  @file   test_i.h
 *
 *  @author Carlos O'Ryan
 */
//=============================================================================


#ifndef TAO_OCTETSEQ_TEST_I_H
#define TAO_OCTETSEQ_TEST_I_H

#include "testS.h"

/**
 * @class Database_i
 *
 * Implements the Database interface in test.idl
 */
class Database_i : public POA_Test::Database
{
public:
  /// ctor
  Database_i (CORBA::ORB_ptr orb,
              Test::Index max_range);

  /// dtor
  ~Database_i ();

  Database_i (Database_i const &) = delete;
  Database_i &operator= (Database_i const &) = delete;
  Database_i (Database_i &&) = delete;
  Database_i &operator= (Database_i &&) = delete;

  // = The Simple_Server methods.
  void set (Test::Index i,
            Test::OctetSeq const &seq,
            CORBA::Double verification_token,
            CORBA::Double_out returned_token) override;

  Test::OctetSeq* get (Test::Index i) override;

  CORBA::ULong get_crc (Test::Index i) override;

  void shutdown () override;

private:
  /// The ORB pointer
  CORBA::ORB_var orb_;

  /// The maximum number of elements in the database
  Test::Index max_range_;

  /// The elements
  Test::OctetSeq* elements_;
};

#if defined(__ACE_INLINE__)
#include "test_i.inl"
#endif /* __ACE_INLINE__ */

#endif /* TAO_MT_CLIENT_TEST_I_H */
