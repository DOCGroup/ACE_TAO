// $Id$

#include "POA_Extension_Initializer.h"
#include "Environment.h"

ACE_RCSID(tao, TAO_POA_Extension_Initializer, "$Id$")

TAO_POA_Extension_Initializer::TAO_POA_Extension_Initializer (void)
  : next_ (0)
{
  // No-Op.
}

TAO_POA_Extension_Initializer::~TAO_POA_Extension_Initializer (void)
{
  delete this->next_;
}

void
TAO_POA_Extension_Initializer::add_initializer (TAO_POA_Extension_Initializer *initializer)
{
  if (this->next_ == 0)
    this->next_ = initializer;
  else
    {
      initializer->next_ = this->next_;
      this->next_ = initializer;
    }
}

void
TAO_POA_Extension_Initializer::register_hooks (TAO_Object_Adapter &object_adapter,
                                           CORBA::Environment &ACE_TRY_ENV)
{
  this->register_hooks_impl (object_adapter, ACE_TRY_ENV);
  ACE_CHECK;

  if (this->next_ != 0)
    {
      this->next_->register_hooks (object_adapter, ACE_TRY_ENV);
      ACE_CHECK;
    }
}

