// -*- C++ -*-

#ifndef TAO_TEST_I_H
#define TAO_TEST_I_H

#include "testS.h"

class A_i : public POA_Test::A
{
public:
  A_i (CORBA::ORB_ptr orb,
       CORBA::ValueFactoryBase *vtp_factory);

  A_i (A_i const &) = delete;
  A_i (A_i &&) = delete;
  A_i &operator= (A_i const &) = delete;
  A_i &operator= (A_i &&) = delete;

  char *method (::Test::A::FailOn where,
                char const *arg1,
                ::CORBA::String_out arg2,
                char *&arg3) override;
  char *method_s (::Test::A::FailOn where,
                  char const *arg1,
                  ::CORBA::String_out arg2,
                  char *&arg3) override;

  ::Test::seq_bd_str *seq_method (::Test::A::FailOn where,
                                  ::Test::seq_bd_str const &arg1,
                                  ::Test::seq_bd_str_out arg2,
                                  ::Test::seq_bd_str &arg3) override;
  ::Test::seq_bds_str *seq_method_s (::Test::A::FailOn where,
                                     ::Test::seq_bds_str const &arg1,
                                     ::Test::seq_bds_str_out arg2,
                                     ::Test::seq_bds_str &arg3) override;

  ::Test::arr_bd_str_slice *arr_method (::Test::A::FailOn where,
                                        ::Test::arr_bd_str const arg1,
                                        ::Test::arr_bd_str_out arg2,
                                        ::Test::arr_bd_str arg3) override;
  ::Test::arr_bds_str_slice *arr_method_s (::Test::A::FailOn where,
                                           ::Test::arr_bds_str const arg1,
                                           ::Test::arr_bds_str_out arg2,
                                           ::Test::arr_bds_str arg3) override;

  ::Test::sct *sct_method (::Test::A::FailOn where,
                           ::Test::sct const &arg1,
                           ::Test::sct_out arg2,
                           ::Test::sct &arg3) override;

  ::Test::unn *unn_method (::Test::A::FailOn where,
                           ::Test::unn const &arg1,
                           ::Test::unn_out arg2,
                           ::Test::unn &arg3) override;

  ::Test::val_bd_str *vbx_method (::Test::A::FailOn where,
                                  ::Test::val_bd_str * arg1,
                                  ::Test::val_bd_str_out arg2,
                                  ::Test::val_bd_str *&arg3) override;

  ::Test::val_bds_str *vbx_method_s (::Test::A::FailOn where,
                                     ::Test::val_bds_str * arg1,
                                     ::Test::val_bds_str_out arg2,
                                     ::Test::val_bds_str *&arg3) override;

  ::Test::vtp *vtp_method (::Test::A::FailOn where,
                           ::Test::vtp * arg1,
                           ::Test::vtp_out arg2,
                           ::Test::vtp *&arg3) override;

  void exc_method (::Test::string_field f) override;

  void shutdown () override;

private:
  /// The ORB pseudo-reference (for shutdown).
  CORBA::ORB_var orb_;

  /// Factories for valuetypes.
  CORBA::ValueFactoryBase *vtp_factory_;
};

#endif /* TAO_TEST_I_H */
