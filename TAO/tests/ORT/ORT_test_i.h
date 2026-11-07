// -*- C++ -*-

//=============================================================================
/**
 * @file ORT_test_i.h
 *
 * Implementation header for the "server" IDL interface for the
 * ORT example.
 *
 * @author Priyanka Gontla <gontla_p@ociweb.com>
 */
//=============================================================================


#ifndef ORT_TEST_I_H
#define ORT_TEST_I_H

#include "ORT_testS.h"


class ORT_test_i : public virtual POA_ObjectReferenceTemplate::ORT_test
{
public:
  explicit ORT_test_i (CORBA::ORB_ptr orb);

  ORT_test_i (ORT_test_i const &) = delete;
  ORT_test_i &operator= (ORT_test_i const &) = delete;
  ORT_test_i (ORT_test_i &&) = delete;
  ORT_test_i &operator= (ORT_test_i &&) = delete;

  CORBA::Boolean request_server () override;

  void shutdown () override;

private:
  CORBA::ORB_var orb_;
};


#endif  /* ORT_TEST_I_H */
