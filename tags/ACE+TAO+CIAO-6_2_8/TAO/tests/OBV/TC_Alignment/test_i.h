
//=============================================================================
/**
 *  @file   test_i.h
 *
 *  $Id$
 *
 *  @author Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================


#ifndef TAO_OBV_TC_ALIGNMENT_TEST_I_H
#define TAO_OBV_TC_ALIGNMENT_TEST_I_H

#include "testS.h"

class tcOffset1_i : public OBV_obvmod::tcOffset1
{
};

class tcOffset3_i : public OBV_obvmod::tcOffset003
{
};

class tcOffset5_i : public OBV_obvmod::tcOffset00005
{
};

class tcOffset7_i : public OBV_obvmod::tcOffset0000007
{
};

class test_i :public virtual POA_test
{
public:
  test_i (CORBA::ORB_ptr orb);

  virtual CORBA::Any *get_value (CORBA::Long offset);

  virtual void shutdown (void);

private:
  CORBA::ORB_ptr orb_;
};

#endif /* TAO_OBV_TC_ALIGNMENT_TEST_I_H */
