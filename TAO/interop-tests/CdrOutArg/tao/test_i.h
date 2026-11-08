// -*- C++ -*-

//=============================================================================
/**
 *  @file   test_i.h
 *
 *  @author Phil Mesnier
 */
//=============================================================================

#ifndef CDR_OUT_ARG_TEST_I_H
#define CDR_OUT_ARG_TEST_I_H

#include "testS.h"

/**
 * @class CDR_Out_Arg_i
 *
 * Implements the CDR_Out_Arg interface
 */
class CDR_Out_Arg_i : public POA_Interop::CDR_Out_Arg
{
public:
  explicit CDR_Out_Arg_i (CORBA::ORB_ptr orb);
  CDR_Out_Arg_i (CDR_Out_Arg_i const &) = delete;
  CDR_Out_Arg_i (CDR_Out_Arg_i &&) = delete;
  CDR_Out_Arg_i &operator= (CDR_Out_Arg_i const &) = delete;
  CDR_Out_Arg_i &operator= (CDR_Out_Arg_i &&) = delete;

  void get_out (CORBA::Long_out arg) override;

  void shutdown () override;

private:
  /// The ORB pseudo-reference (for shutdown).
  CORBA::ORB_var orb_;
};

#endif /* TAO_INTERCEPTOR_TEST_I_H */
