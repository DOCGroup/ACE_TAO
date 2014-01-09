// -*- C++ -*-
// $Id$

#include "test_i.h"
#include "ace/OS_NS_string.h"

char *
foo_i::foo_op (const char * inarg)
{
  CORBA::String_var retval = CORBA::string_dup ("bad");

  if (ACE_OS::strcmp (inarg, "foo_op") == 0)
    {
      retval = CORBA::string_dup ("good");
    }
  else
    {
      throw BadInput ("expected \"foo_op\"\n");
    }

  return retval._retn ();
}

char *
foo_i::base_op (const char * inarg)
{
  CORBA::String_var retval = CORBA::string_dup ("bad");

  if (ACE_OS::strcmp (inarg, "base_op") == 0)
    {
      retval = CORBA::string_dup ("good");
    }
  else
    {
      throw BadInput ("expected \"base_op\"\n");
    }

  return retval._retn ();
}

passer_i::passer_i (CORBA::ORB_ptr orb, PortableServer::POA_ptr poa)
 : orb_ (CORBA::ORB::_duplicate (orb))
 , poa_ (PortableServer::POA::_duplicate (poa))
{
}

void
passer_i::shutdown (void)
{
  this->orb_->shutdown (0);
}

void
passer_i::pass_ops (base_out outarg)
{
  foo_i *servant = 0;
  ACE_NEW (servant,
           foo_i);

  PortableServer::ObjectId_var id =
    this->poa_->activate_object (servant);

  CORBA::Object_var object = this->poa_->id_to_reference (id.in ());

  PortableServer::ServantBase_var safety (servant);
  outarg = foo::_narrow (object.in ());
}

void
passer_i::pass_state (base_out outarg)
{
  TreeController_var tc;
  ACE_NEW (tc.inout (),
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

void
passer_i::pass_nil (base_out outarg)
{
  outarg = foo::_nil ();
}
