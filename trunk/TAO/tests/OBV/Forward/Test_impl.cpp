// $Id$

#include "Test_impl.h"
#include "TreeBaseC.h"

ACE_RCSID(Forward, Test_impl, "$Id$")

Test_impl::Test_impl (CORBA::ORB_ptr orb)
    : orb_ (CORBA::ORB::_duplicate (orb))
{
}


void
reflect_node (BaseNode_ptr bn)
{
  if(bn == 0) return;

  reflect_node (bn->left ());
  reflect_node (bn->right ());

  BaseNode_ptr old_right = bn->right ();
  BaseNode_ptr old_left = bn->left ();

  CORBA::add_ref (old_right);
  CORBA::add_ref (old_left);

  bn->right (old_left);
  bn->left (old_right);
}

TreeController *
Test_impl::reflect (TreeController * tc
                    TAO_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (tc != 0)
  {
    reflect_node (tc-> root());
    tc->_add_ref ();
  }
  return tc;
}

void
Test_impl::shutdown (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0 TAO_ENV_ARG_PARAMETER);
}
