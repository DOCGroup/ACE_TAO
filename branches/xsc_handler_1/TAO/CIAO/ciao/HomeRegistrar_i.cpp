// $Id$

#include "HomeRegistrar_i.h"

#if !defined (__ACE_INLINE__)
#include "HomeRegistrar_i.inl"
#endif /* !defined INLINE */

CIAO::HomeRegistrar_Impl::HomeRegistrar_Impl (CosNaming::NamingContext_ptr ns)
  : ns_ (CosNaming::NamingContext::_duplicate (ns))
{
}

CIAO::HomeRegistrar_Impl::~HomeRegistrar_Impl ()
{
}

::Components::CCMHome_ptr
CIAO::HomeRegistrar_Impl::find_home_impl (const char *name
                                          ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::HomeNotFound))
{
  Components::CCMHome_var ahome;

  ACE_TRY
    {
      CosNaming::Name aname;
      aname.length (1);
      aname[0].id = CORBA::string_dup (name);

      CORBA::Object_var obj = this->ns_->resolve (aname
                                                  ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ahome = Components::CCMHome::_narrow (obj.in ()
                                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_TRY_THROW (Components::HomeNotFound ());
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);

  if (CORBA::is_nil (ahome.in ()))
    ACE_THROW_RETURN (Components::HomeNotFound (), 0);
  return ahome._retn ();
}

void
CIAO::HomeRegistrar_Impl::register_home (const char * home_repo_id,
                                         const char * comp_repo_id,
                                         const char * home_name,
                                         CORBA::Object_ptr objref
                                         ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   CIAO::AlreadyBound,
                   CIAO::InvalidName,
                   CIAO::HomeRegisterFailure))
{
  if (CORBA::is_nil (objref))
    ACE_THROW (CORBA::BAD_PARAM ());

  ACE_TRY
    {
      CosNaming::Name aname;
      aname.length (1);
      aname[0].id = CORBA::string_dup (home_repo_id);

      this->ns_->bind (aname, objref ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      aname[0].id = CORBA::string_dup (comp_repo_id);

      this->ns_->bind (aname, objref ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      aname[0].id = CORBA::string_dup (home_name);

      this->ns_->bind (aname, objref ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CosNaming::NamingContext::AlreadyBound, ex)
    {
      ACE_TRY_THROW (CIAO::AlreadyBound ());
    }
  ACE_CATCH (CosNaming::NamingContext::InvalidName, ex)
    {
      ACE_TRY_THROW (CIAO::InvalidName ());
    }
  ACE_CATCHANY
    {
      ACE_TRY_THROW (CIAO::HomeRegisterFailure ());
    }
  ACE_ENDTRY;
}
