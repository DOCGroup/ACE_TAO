// $Id$

#include "CIF_Common.h"
#include "ace/Get_Opt.h"
#include "Component/CIF_ComponentC.h"

CIF_Common::CIF_Common (void)
  : naming_ ("")
{
}

CIF_Common::~CIF_Common (void)
{
}

int
CIF_Common::parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("n:"));
  int c;
  while ((c = get_opts ()) != -1)
    {
      switch (c)
        {
          case 'n':
            this->naming_ = get_opts.opt_arg ();
            break;

          case '?':
          default:
            ACE_ERROR_RETURN ((LM_ERROR,
                              "usage:  %s\n"
                              "-n (naming service)\n"
                              "\n",
                              argv [0]),
                              -1);
        }
    }

  return 0;
}

int
CIF_Common::init_provider_component (::CosNaming::NamingContext_ptr naming_context)
{
  ::CosNaming::Name name (1);
  name.length (1);
  name[0].id = ::CORBA::string_dup (ACE_TEXT_ALWAYS_CHAR ("ProviderComponentInstance"));

  ::CORBA::Object_var obj_ref = naming_context->resolve (name);

  ::CIF::CIF_Provider_var provider_object =
    ::CIF::CIF_Provider::_narrow (obj_ref.in ());

  if (! ::CORBA::is_nil (provider_object.in ()))
    {
      ACE_DEBUG ((LM_DEBUG, "Established connection with Provider Component.\n"));
      this->provider_cmp_ = provider_object->_get_component ();
      if (::CORBA::is_nil (this->provider_cmp_.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                            "_get_component returned a nil component for Provider component\n"),
                            1);
        }
    }
  else
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                        "Error resolving CIF component object for Provider component\n"),
                        1);
    }
  return 0;
}

int
CIF_Common::init_user_component (::CosNaming::NamingContext_ptr naming_context)
{
  ::CosNaming::Name name (1);
  name.length (1);
  name[0].id = ::CORBA::string_dup (ACE_TEXT_ALWAYS_CHAR ("UserComponentInstance"));

  ::CORBA::Object_var obj_ref = naming_context->resolve (name);

  ::CIF::CIF_User_var user_object =
    ::CIF::CIF_User::_narrow (obj_ref.in ());

  if (! ::CORBA::is_nil (user_object.in ()))
    {
      ACE_DEBUG ((LM_DEBUG, "Established connection with User Component.\n"));
      this->user_cmp_ = user_object->_get_component ();
      if (::CORBA::is_nil (this->user_cmp_.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                            "_get_component returned a nil component for User component\n"),
                            1);
        }
    }
  else
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                        "Error resolving CIF component object with User component\n"),
                        1);
    }
  return 0;
}


int
CIF_Common::init (int argc, ACE_TCHAR *argv[])
{
  int ret = 0;

  if (this->parse_args (argc, argv) != 0)
    return 1;

  try
    {
      this->orb_ = ::CORBA::ORB_init (argc, argv);
      // Resolving naming service
      ::CORBA::Object_var naming_context_object =
        this->orb_->string_to_object (naming_);
      if (::CORBA::is_nil (naming_context_object.in ()))
          return -1;
      CosNaming::NamingContext_var naming_context =
        CosNaming::NamingContext::_narrow (naming_context_object.in ());

      if (::CORBA::is_nil (naming_context.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                            "Unable to connect to naming service\n"),
                            1);
        }

      ret += init_provider_component (naming_context.in ());
      ret += init_user_component (naming_context.in ());
    }
  catch (const ::CORBA::Exception &ex)
    {
      ex._tao_print_exception ("CIF_Common::init");
      return 1;
    }
  return ret;
}

::Components::Navigation_ptr
CIF_Common::get_navigation_interface ()
{
  ::Components::Navigation_var nav =
    ::Components::Navigation::_narrow (this->provider_cmp_.in ());

  if (::CORBA::is_nil (nav.in ()))
    {
      ACE_ERROR ((LM_ERROR,
                  "Narrow failed from CCMObject to Navigation\n"));
      return ::Components::Navigation::_nil ();
    }
  return ::Components::Navigation::_duplicate (nav.in ());
}

Components::Receptacles_ptr
CIF_Common::get_receptacle_interface()
{
  ::Components::Receptacles_var rec =
    ::Components::Receptacles::_narrow (this->user_cmp_.in ());

  if (::CORBA::is_nil (rec.in ()))
    {
      ACE_ERROR ((LM_ERROR,
                  "Narrow failed from CCMObject to Receptacles\n"));
      return ::Components::Receptacles::_nil ();
    }
  return ::Components::Receptacles::_duplicate (rec.in ());
}

void
CIF_Common::shutdown ()
{
  this->orb_->shutdown ();
}
