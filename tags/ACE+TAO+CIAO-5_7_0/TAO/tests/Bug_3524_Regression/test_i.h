// $Id: test_i.h,v 1.1 2003/08/27 22:43:35 vz Exp
// -*- C++ -*-

#ifndef TAO_TEST_I_H
#define TAO_TEST_I_H

#include "testS.h"

class A_i : public POA_Test::A
{
public:
  A_i (CORBA::ORB_ptr orb,
       CORBA::ValueFactoryBase *vtp_factory);

  char *method (::Test::A::FailOn where,
                const char *arg1,
                ::CORBA::String_out arg2,
                char *&arg3);
  char *method_s (::Test::A::FailOn where,
                  const char *arg1,
                  ::CORBA::String_out arg2,
                  char *&arg3);

  ::Test::seq_bd_str *seq_method (::Test::A::FailOn where,
                                  const ::Test::seq_bd_str & arg1,
                                  ::Test::seq_bd_str_out arg2,
                                  ::Test::seq_bd_str & arg3);
  ::Test::seq_bds_str *seq_method_s (::Test::A::FailOn where,
                                     const ::Test::seq_bds_str & arg1,
                                     ::Test::seq_bds_str_out arg2,
                                     ::Test::seq_bds_str & arg3);

  ::Test::arr_bd_str_slice *arr_method (::Test::A::FailOn where,
                                        const ::Test::arr_bd_str arg1,
                                        ::Test::arr_bd_str_out arg2,
                                        ::Test::arr_bd_str arg3);
  ::Test::arr_bds_str_slice *arr_method_s (::Test::A::FailOn where,
                                           const ::Test::arr_bds_str arg1,
                                           ::Test::arr_bds_str_out arg2,
                                           ::Test::arr_bds_str arg3);

  ::Test::sct *sct_method (::Test::A::FailOn where,
                           const ::Test::sct & arg1,
                           ::Test::sct_out arg2,
                           ::Test::sct & arg3);

  ::Test::unn *unn_method (::Test::A::FailOn where,
                           const ::Test::unn & arg1,
                           ::Test::unn_out arg2,
                           ::Test::unn & arg3);

  ::Test::val_bd_str *vbx_method (::Test::A::FailOn where,
                                  ::Test::val_bd_str * arg1,
                                  ::Test::val_bd_str_out arg2,
                                  ::Test::val_bd_str *& arg3);

  ::Test::val_bds_str *vbx_method_s (::Test::A::FailOn where,
                                     ::Test::val_bds_str * arg1,
                                     ::Test::val_bds_str_out arg2,
                                     ::Test::val_bds_str *& arg3);

  ::Test::vtp *vtp_method (::Test::A::FailOn where,
                           ::Test::vtp * arg1,
                           ::Test::vtp_out arg2,
                           ::Test::vtp *& arg3);

  void exc_method (::Test::string_field f);

  void shutdown ();

private:

  /// The ORB pseudo-reference (for shutdown).
  CORBA::ORB_var orb_;

  /// Factories for valuetypes.
  CORBA::ValueFactoryBase *vtp_factory_;
};

#endif /* TAO_TEST_I_H */
