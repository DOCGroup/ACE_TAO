// $Id$

#include "test_i.h"

ACE_RCSID (Abstract_Interface, 
           test_i, 
           "$Id$")

char * 
foo_i::foo_op (const char * inarg
               ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    BadInput
  ))
{
  CORBA::String_var retval = CORBA::string_dup ("bad");

  if (ACE_OS::strcmp (inarg, "foo_op") == 0)
    {
      retval = CORBA::string_dup ("good");
    }
  else
    {
      ACE_THROW_RETURN (BadInput ("expected \"foo_op\"\n"),
                        retval._retn ());
    }

  return retval._retn ();
}

char * 
foo_i::base_op (const char * inarg
                ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      BadInput
    ))
{
  CORBA::String_var retval = CORBA::string_dup ("bad");

  if (ACE_OS::strcmp (inarg, "base_op") == 0)
    {
      retval = CORBA::string_dup ("good");
    }
  else
    {
      ACE_THROW_RETURN (BadInput ("expected \"base_op\"\n"),
                        retval._retn ());
    }

  return retval._retn ();
}

void 
passer_i::pass_ops (base_out outarg
                    ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  foo_i *servant = 0;
  ACE_NEW (servant,
           foo_i);
  PortableServer::ServantBase_var safety (servant);
  outarg = servant->_this (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

void 
passer_i::pass_state (base_out outarg
                      ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  TreeController_var tc;
  ACE_NEW (tc,
           OBV_TreeController);

  // Create the root node.
  StringNode_var sn;
  ACE_NEW (sn.inout (),
           OBV_StringNode);
  sn->name ((const char *) "RootNode");
  tc->root (sn.in ());

  // Create the left leaf.
  StringNode_var l_dummy;
  ACE_NEW (l_dummy.inout (),
           OBV_StringNode);
  l_dummy->name ((const char *) "LeftNode");
  sn->left (l_dummy.in ());

  // Create the right leaf.
  StringNode_var r_dummy;
  ACE_NEW (r_dummy.inout (),
           OBV_StringNode);
  r_dummy->name ((const char *) "RightNode");
  sn->right (r_dummy.in ());

  outarg = tc._retn ();
}

