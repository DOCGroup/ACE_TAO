// $Id$

#include "tao/PortableServer/ServantBase_var.h"
#include "tao/PortableServer/Servant_Base.h"

ACE_RCSID (PortableServer,
           ServantBase_var,
           "$Id$")

TAO_ServantBase_var::TAO_ServantBase_var (void)
  : ptr_ (0)
{
}

TAO_ServantBase_var::TAO_ServantBase_var (TAO_ServantBase *p)
  : ptr_ (p)
{
}

TAO_ServantBase_var::TAO_ServantBase_var (const TAO_ServantBase_var &b)
  : ptr_ (b.ptr_)
{
  if (this->ptr_ != 0)
    {
      ACE_TRY_NEW_ENV
        {
          this->ptr_->_add_ref (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      ACE_CATCHALL
        {
          ACE_RE_THROW;
        }
      ACE_ENDTRY;
    }
}

TAO_ServantBase_var::~TAO_ServantBase_var (void)
{
  if (this->ptr_ != 0)
    {
      // We shold not allow exceptions to pass through
      ACE_TRY_NEW_ENV
        {
          this->ptr_->_remove_ref (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      ACE_CATCHALL
        {
        }
      ACE_ENDTRY;
    }
}

TAO_ServantBase_var &
TAO_ServantBase_var::operator= (TAO_ServantBase *p)
{
  if (this->ptr_ == p)
    return *this;

  ACE_TRY_NEW_ENV
    {
      if (this->ptr_ != 0)
        {
          this->ptr_->_remove_ref (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHALL
    {
      ACE_RE_THROW;
    }
  ACE_ENDTRY;

  this->ptr_ = p;

  return *this;
}

TAO_ServantBase_var &
TAO_ServantBase_var::operator= (const TAO_ServantBase_var &b)
{
  if (this->ptr_ != b.ptr_)
    {
      ACE_TRY_NEW_ENV
        {
          if (this->ptr_ != 0)
            {
              this->ptr_->_remove_ref (ACE_ENV_SINGLE_ARG_PARAMETER);
              ACE_TRY_CHECK;
            }

          if ((this->ptr_ = b.ptr_) != 0)
            {
              this->ptr_->_add_ref (ACE_ENV_SINGLE_ARG_PARAMETER);
              ACE_TRY_CHECK;
            }
        }
      ACE_CATCHALL
        {
          ACE_RE_THROW;
        }
      ACE_ENDTRY;
    }

  return *this;
}

TAO_ServantBase *
TAO_ServantBase_var::operator->() const
{
  return this->ptr_;
}

TAO_ServantBase *
TAO_ServantBase_var::in (void) const
{
  return this->ptr_;
}

TAO_ServantBase *&
TAO_ServantBase_var::inout (void)
{
  return this->ptr_;
}

TAO_ServantBase *&
TAO_ServantBase_var::out (void)
{
  ACE_TRY_NEW_ENV
    {
      if (this->ptr_ != 0)
        {
          this->ptr_->_remove_ref (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHALL
    {
      ACE_RE_THROW;
    }
  ACE_ENDTRY;

  this->ptr_ = 0;

  return this->ptr_;
}

TAO_ServantBase *
TAO_ServantBase_var::_retn (void)
{
  TAO_ServantBase *retval = this->ptr_;
  this->ptr_ = 0;
  return retval;
}

