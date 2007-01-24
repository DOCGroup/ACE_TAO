// $Id$

#include "test_i.h"
#include "tao/debug.h"

#if !defined(__ACE_INLINE__)
#include "test_i.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(DSI_Gateway, test_i, "$Id$")

CORBA::Long
Simple_Server_i::test_method (CORBA::Long x,
                              const Structure& the_in_structure,
                              Structure_out the_out_structure,
                              char *&name
                              ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  Structure *tmp = 0;
  ACE_NEW_RETURN (tmp, Structure (the_in_structure), -1);
  the_out_structure = tmp;

  if (TAO_debug_level > 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Simpler_Server_i ====\n"
                  "    x = %d\n"
                  "    i = %d\n"
                  "    length = %d\n"
                  "    name = <%s>\n",
                  x,
                  the_in_structure.i,
                  the_in_structure.seq.length (),
                  name));
    }

  return x;
}

void
Simple_Server_i::raise_user_exception (void)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     test_exception))
{
  ACE_THROW (test_exception (33,
                             "reactor meltdown",
                             "kaput"));
}

void
Simple_Server_i::raise_system_exception (void)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW (CORBA::NO_PERMISSION ());
}

void
Simple_Server_i::shutdown (void)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0 ACE_ENV_ARG_PARAMETER);
}

CORBA::Long
Simple_Server_i::test_val (void)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return vlong_;
}

void
Simple_Server_i::test_val (CORBA::Long tv ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (TAO_debug_level > 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Simpler_Server_i ====test_val: %d\n", tv
                 ));
    }
    vlong_ = tv;
}

