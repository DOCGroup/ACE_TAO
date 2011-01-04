// $Id$

#include "CIF_Common.h"
#include "ciao/Base/Client_init.h"
#include "ace/Get_Opt.h"

CIF_Common::CIF_Common(void)
    : naming_(0)
{
}

CIF_Common::~CIF_Common(void)
{
  if (! ::CORBA::is_nil (this->orb_.in ()))
    {
      this->orb_->destroy ();
    }
}

//============================================================
// parse_args
//============================================================
int
CIF_Common::parse_args(int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts(argc, argv, ACE_TEXT("n:"));
  int c;
  while ((c = get_opts()) != -1)
    {
      switch (c)
        {
          case 'n':
            this->naming_ = get_opts.opt_arg();
            break;

          case '?':
          default:
            ACE_ERROR_RETURN((LM_ERROR,
                              "usage:  %s\n"
                              "-n (naming service)\n"
                              "\n",
                              argv [0]),
                             -1);
        }
    }

  return 0;
}

//============================================================
// init_provider_component
//============================================================
int
CIF_Common::init_provider_component(::CosNaming::NamingContext_ptr naming_context)
{
  ::CosNaming::Name_var name;
  ACE_NEW_THROW_EX (name,
                    ::CosNaming::Name,
                    ::CORBA::NO_MEMORY ());
  name->length(1);
  (*name)[0].id = ::CORBA::string_dup("ProviderComponentInstance");

  ::CORBA::Object_var obj_ref = naming_context->resolve(name);

  this->provider_object_ = ::CIF::CIF_Provider::_narrow(obj_ref.in());

  if (! ::CORBA::is_nil(this->provider_object_.in()))
    {
      ACE_DEBUG((LM_DEBUG, "Established connection with Provider Component.\n"));
    }
  else
    {
      ACE_ERROR_RETURN((LM_ERROR,
                        "Error resolving CIF component object for Provider component\n"),
                       1);
    }
  return 0;
}

//============================================================
// init_user_component
//============================================================
int
CIF_Common::init_user_component(::CosNaming::NamingContext_ptr naming_context)
{
  ::CosNaming::Name_var name;
  ACE_NEW_THROW_EX (name,
                    ::CosNaming::Name,
                    ::CORBA::NO_MEMORY ());
  name->length(1);
  (*name)[0].id = ::CORBA::string_dup("UserComponentInstance");

  ::CORBA::Object_var obj_ref = naming_context->resolve(name);

  this->user_object_ = ::CIF::CIF_User::_narrow(obj_ref.in());

  if (! ::CORBA::is_nil(this->user_object_.in()))
    {
      ACE_DEBUG((LM_DEBUG, "Established connection with User Component.\n"));
    }
  else
    {
      ACE_ERROR_RETURN((LM_ERROR,
                        "Error resolving CIF component object with User component\n"),
                       1);
    }
  return 0;
}

//============================================================
// init
//============================================================
int
CIF_Common::init(int argc, ACE_TCHAR *argv[])
{
  int ret = 0;

  this->orb_ = ::CORBA::ORB_init(argc, argv);
  ::CIAO::Client_init(this->orb_.in ());

  if (this->parse_args(argc, argv) != 0)
    return 1;

  try
    {
      // Resolving naming service
      ::CORBA::Object_var naming_context_object =
        this->orb_->string_to_object(this->naming_);
      if (::CORBA::is_nil(naming_context_object.in()))
        {
          return 1;
        }
      CosNaming::NamingContext_var naming_context =
        CosNaming::NamingContext::_narrow(naming_context_object.in());

      if (::CORBA::is_nil(naming_context.in()))
        {
          ACE_ERROR_RETURN((LM_ERROR,
                            "Unable to connect to naming service\n"),
                           1);
        }

      ret += init_provider_component(naming_context.in());
      ret += init_user_component(naming_context.in());
    }
  catch (const ::CORBA::Exception &ex)
    {
      ex._tao_print_exception("CIF_Common::init");
      return 1;
    }
  return ret;
}

//============================================================
// get_navigation_interface
//============================================================
::Components::Navigation_ptr
CIF_Common::get_navigation_interface()
{
  ::CORBA::Object_var provider_cmp = this->provider_object_->_get_component();
  ::Components::Navigation_var nav =
    ::Components::Navigation::_narrow(provider_cmp.in());

  if (::CORBA::is_nil(nav.in()))
    {
      ACE_ERROR((LM_ERROR,
                 "Narrow failed from CCMObject to Navigation\n"));
      return ::Components::Navigation::_nil();
    }
  return ::Components::Navigation::_duplicate(nav.in());
}

//============================================================
// get_receptacle_interface
//============================================================
Components::Receptacles_ptr
CIF_Common::get_receptacle_interface()
{
  ::CORBA::Object_var user_cmp = this->user_object_->_get_component();
  ::Components::Receptacles_var rec =
    ::Components::Receptacles::_narrow(user_cmp.in());

  if (::CORBA::is_nil(rec.in()))
    {
      ACE_ERROR((LM_ERROR,
                 "Narrow failed from CCMObject to Receptacles\n"));
      return ::Components::Receptacles::_nil();
    }
  return ::Components::Receptacles::_duplicate(rec.in());
}

//============================================================
// get_provider_cmp
//============================================================
::CORBA::Object_ptr
CIF_Common::get_provider_cmp()
{
  ::CORBA::Object_var provider_cmp = this->provider_object_->_get_component();
  if (! ::CORBA::is_nil(provider_cmp.in()))
    {
      return ::CORBA::Object::_duplicate(provider_cmp.in());
    }
  else
    {
      ACE_ERROR((LM_ERROR, "CIF_Common::get_provider_cmp - "
                 "Error: get_component returned NIL\n"));
      return ::CORBA::Object::_nil();
    }
}

//============================================================
// get_user_cmp
//============================================================
::CORBA::Object_ptr
CIF_Common::get_user_cmp()
{
  ::CORBA::Object_var user_cmp = this->user_object_->_get_component();
  if (! ::CORBA::is_nil(user_cmp.in()))
    {
      return ::CORBA::Object::_duplicate(user_cmp.in());
    }
  else
    {
      ACE_ERROR((LM_ERROR, "CIF_Common::get_user_cmp - "
                 "Error: get_component returned NIL\n"));
      return ::CORBA::Object::_nil();
    }
}

//============================================================
// shutdown
//============================================================
void
CIF_Common::shutdown()
{
  this->orb_->shutdown();
}

//============================================================
// test_provider_component
//============================================================
int
CIF_Common::test_provider_component()
{
  ACE_DEBUG((LM_DEBUG, "Navigation test_provider_component - "
             "Start test\n"));
  int ret = 0;
  ::CORBA::Object_var provider_cmp = this->provider_object_->_get_component();
  ::CIF::CIF_Provider_var provider_object = ::CIF::CIF_Provider::_narrow(provider_cmp.in ());

  if (::CORBA::is_nil(provider_cmp.in()))
    {
      ACE_ERROR_RETURN((LM_ERROR,
                        "CIF_Common::test_provider_component - "
                        "_get_component returned a nil component for Provider component\n"),
                       1);
    }
  try
    {
      if (provider_object->provider_id() != 4321)
        {
          ACE_ERROR((LM_ERROR, "CIF_Common::test_user_component - "
                     "Error: Attribute hasn't the right initial value: "
                     "expected <> - received < % d > \n",
                     provider_object->provider_id()));
          ++ret;
        }
      provider_object->provider_id(1234);
      if (provider_object->provider_id() != 1234)
        {
          ACE_ERROR((LM_ERROR, "CIF_Common::test_provider_component - "
                     "Error: Setting attribute failed\n"));
          ++ret;
        }
      ACE_DEBUG((LM_DEBUG, "Navigation test_provider_component - "
                 "Test passed\n"));
    }
  catch (const ::CORBA::Exception& e)
    {
      e._tao_print_exception("CIF_Common::test_provider_component - Error: ");
      return 1;
    }
  catch (...)
    {
      ACE_ERROR_RETURN((LM_ERROR,
                        "CIF_Common::test_provider_component - "
                        "Error: Unknown exception caught\n"),
                       1);
    }
  return ret;
}

//============================================================
// test_user_component
//============================================================
int
CIF_Common::test_user_component()
{
  ACE_DEBUG((LM_DEBUG, "Receptacle test_user_component - "
             "Start test\n"));
  int ret = 0;
  ::CORBA::Object_var user_cmp = this->user_object_->_get_component();
  ::CIF::CIF_User_var user_object = ::CIF::CIF_User::_narrow(user_cmp.in ());
  if (::CORBA::is_nil(user_object))
    {
      ACE_ERROR_RETURN((LM_ERROR,
                        "_get_component returned a nil component for User component\n"),
                       1);
    }
  try
    {
      if (user_object->user_id() != 1234)
        {
          ACE_ERROR((LM_ERROR, "CIF_Common::test_user_component - "
                     "Error: Attribute hasn't the right initial value: "
                     "expected <> - received < % d > \n",
                     user_object->user_id()));
          ++ret;
        }
      user_object->user_id(4321);
      if (user_object->user_id() != 4321)
        {
          ACE_ERROR((LM_ERROR, "CIF_Common::test_user_component - "
                     "Error: Setting attribute failed\n"));
          ++ret;
        }
      ACE_DEBUG((LM_DEBUG, "Receptacle test_user_component - "
                 "Test passed\n"));
    }
  catch (const ::CORBA::Exception& e)
    {
      e._tao_print_exception("CIF_Common::test_user_component - Error: ");
      return 1;
    }
  catch (...)
    {
      ACE_ERROR_RETURN((LM_ERROR,
                        "CIF_Common::test_user_component - "
                        "Error: Unknown exception caught\n"),
                       1);
    }
  return ret;
}
