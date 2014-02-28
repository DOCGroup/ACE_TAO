// -*- C++ -*-

//=============================================================================
/**
 *  @file   test_i.h
 *
 *  $Id$
 *
 *  @author Phil Mesnier
 */
//=============================================================================


#ifndef CDR_OUT_ARG_TEST_I_H
#define CDR_OUT_ARG_TEST_I_H

#include "testS.hh"

/**
 * @class CDR_Out_Arg_i
 *
 * Implements the CDR_Out_Arg interface
 */
class CDR_Out_Arg_i : public POA_Interop::CDR_Out_Arg
{

public:

  CDR_Out_Arg_i (CORBA::ORB_ptr orb);

  void get_out (CORBA::Long_out arg) IT_THROW_DECL((CORBA::SystemException));

  void shutdown (void) IT_THROW_DECL((CORBA::SystemException));

private:

  /// The ORB pseudo-reference (for shutdown).
  CORBA::ORB_var orb_;

};

#endif /* TAO_INTERCEPTOR_TEST_I_H */
