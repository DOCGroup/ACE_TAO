//$Id$

#include "Multiple_Impl.h"
#include "Collocation_Tester.h"

ACE_RCSID (tests, Multiple_Impl, "$Id$")

///////////////////////////////////////////////////////////
//     Bottom_Impl Implementation
//

Bottom_Impl::Bottom_Impl (CORBA::ORB_ptr orb)
{
  this->orb_ = CORBA::ORB::_duplicate (orb);
}

Bottom_Impl::~Bottom_Impl (void)
{
  // No-Op.
}

char *
Bottom_Impl::top_quote (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_UNUSED_ARG (ACE_TRY_ENV);
  return CORBA::string_dup(Quote::top);
}

char *
Bottom_Impl::left_quote (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_UNUSED_ARG (ACE_TRY_ENV);
  return CORBA::string_dup(Quote::left);
}

char *
Bottom_Impl::right_quote (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_UNUSED_ARG (ACE_TRY_ENV);
  return CORBA::string_dup(Quote::right);
}

char *
Bottom_Impl::bottom_quote (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_UNUSED_ARG (ACE_TRY_ENV);
  return CORBA::string_dup(Quote::bottom);
}

void
Bottom_Impl::shutdown (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_UNUSED_ARG (ACE_TRY_ENV);
  this->orb_->shutdown (0, ACE_TRY_ENV);
}

///////////////////////////////////////////////////////////
//     Delegated_Bottom_Impl Implementation
//
Delegated_Bottom_Impl::Delegated_Bottom_Impl (Multiple::Bottom_ptr delegate,
                                              CORBA::ORB_ptr orb)
  : delegate_ (delegate)
{
  this->orb_ = CORBA::ORB::_duplicate (orb);
}

Delegated_Bottom_Impl::~Delegated_Bottom_Impl (void)
{
  // No-Op.
}

char *
Delegated_Bottom_Impl::top_quote (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Delegating the call: <top_quote>\n")));

  CORBA::String_var msg =
    this->delegate_->top_quote (ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);
  return msg._retn ();
}

char *
Delegated_Bottom_Impl::left_quote (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
    ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Delegating the call: <left_quote>\n")));

  CORBA::String_var msg =
    this->delegate_->left_quote (ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);
  return msg._retn ();
}

char *
Delegated_Bottom_Impl::right_quote (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Delegating the call: <right_quote>\n")));

  CORBA::String_var msg =
    this->delegate_->right_quote (ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);
  return msg._retn ();
}

char *
Delegated_Bottom_Impl::bottom_quote (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Delegating the call: <bottom_quote>\n")));

  CORBA::String_var msg =
    this->delegate_->bottom_quote (ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);
  return msg._retn ();
}

void
Delegated_Bottom_Impl::shutdown (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Delegating Shut-Down.\n")));
  this->delegate_->shutdown (ACE_TRY_ENV);
  //this->orb_->shutdown (0, ACE_TRY_ENV);
  ACE_CHECK;
}
