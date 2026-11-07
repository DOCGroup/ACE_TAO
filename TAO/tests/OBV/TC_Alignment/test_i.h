
//=============================================================================
/**
 *  @file   test_i.h
 *
 *  @author Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================


#ifndef TAO_OBV_TC_ALIGNMENT_TEST_I_H
#define TAO_OBV_TC_ALIGNMENT_TEST_I_H

#include "testS.h"

class tcOffset1_i : public OBV_obvmod::tcOffset1
{
public:
  tcOffset1_i () = default;
  tcOffset1_i (tcOffset1_i const&) = delete;
  tcOffset1_i (tcOffset1_i&&) = delete;
  tcOffset1_i& operator= (tcOffset1_i const&) = delete;
  tcOffset1_i& operator= (tcOffset1_i&&) = delete;
};

class tcOffset3_i : public OBV_obvmod::tcOffset003
{
public:
  tcOffset3_i () = default;
  tcOffset3_i (tcOffset3_i const&) = delete;
  tcOffset3_i (tcOffset3_i&&) = delete;
  tcOffset3_i& operator= (tcOffset3_i const&) = delete;
  tcOffset3_i& operator= (tcOffset3_i&&) = delete;
};

class tcOffset5_i : public OBV_obvmod::tcOffset00005
{
public:
  tcOffset5_i () = default;
  tcOffset5_i (tcOffset5_i const&) = delete;
  tcOffset5_i (tcOffset5_i&&) = delete;
  tcOffset5_i& operator= (tcOffset5_i const&) = delete;
  tcOffset5_i& operator= (tcOffset5_i&&) = delete;
};

class tcOffset7_i : public OBV_obvmod::tcOffset0000007
{
public:
  tcOffset7_i () = default;
  tcOffset7_i (tcOffset7_i const&) = delete;
  tcOffset7_i (tcOffset7_i&&) = delete;
  tcOffset7_i& operator= (tcOffset7_i const&) = delete;
  tcOffset7_i& operator= (tcOffset7_i&&) = delete;
};

class test_i : public virtual POA_test
{
public:
  explicit test_i (CORBA::ORB_ptr orb);

  test_i (test_i const&) = delete;
  test_i (test_i&&) = delete;
  test_i& operator= (test_i const&) = delete;
  test_i& operator= (test_i&&) = delete;

  CORBA::Any *get_value (CORBA::Long offset) override;

  void shutdown () override;

private:
  CORBA::ORB_ptr orb_;
};

#endif /* TAO_OBV_TC_ALIGNMENT_TEST_I_H */
