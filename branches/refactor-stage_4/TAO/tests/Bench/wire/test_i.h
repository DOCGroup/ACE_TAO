
// $Id$

#ifndef TAO_TEST_I_H
#define TAO_TEST_I_H

#include "testS.h"

class foo_i : public virtual POA_foo
{
  virtual char * all_str (const char * inarg,
                          char *& inoutarg,
                          CORBA::String_out outarg
                          ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));
};

#if defined(__ACE_INLINE__)
#include "test_i.inl"
#endif /* __ACE_INLINE__ */

#endif /* TAO_TEST_I_H */
