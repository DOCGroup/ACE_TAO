// $Id$
#include "FT_DetectorFactory_i.h"
#include "ace/Get_Opt.h"
#include "orbsvcs/CosNamingC.h"
#include "tao/PortableServer/ORB_Manager.h"

class PropertySetDecoder
{
public:
  class PropertyValue
  {
   protected:
    PropertyValue()
    {
    }
   public:
    virtual ~PropertyValue()
    {
    }
  };

  template<typename TYPE>
  class TypedPropertyValue : public PropertyValue
  {
   public:
    TypedPropertyValue (const TYPE & value, int type)
      : value_(value)
      , type_(type)
    {
    }

    virtual ~TypedPropertyValue ()
    {
    }

    int type ()const
    {
      return type_;
    }

    const TYPE & value ()const
    {
      return value_;
    }
   private:
    TYPE value_;
    int type_;
  };

  PropertySetDecoder (const FT::Properties & property_set);
  ~PropertySetDecoder ();

  // convenience methods
  int find (const std::string & key, long & value)const;
  int find (const std::string & key, double & value)const;
  int find (const std::string & key, std::string & value)const;

  // general purpose method
  int find (const std::string & key, PropertyValue *& pValue)const;

  // note: if we had templated members:
  // template <typename TYPE>
  // int find (const std::string & key, TypedPropertyValue<TYPE> *& pValue)const;

private:
  PropertySetDecoder();
  PropertySetDecoder(const PropertySetDecoder & rhs);
  PropertySetDecoder & operator = (const PropertySetDecoder & rhs);

};


FT_FaultDetectorFactory_i::FT_FaultDetectorFactory_i (void)
  : ior_output_file_(0)
  , nsName_(0)
{
}


FT_FaultDetectorFactory_i::~FT_FaultDetectorFactory_i (void)
{
}

int FT_FaultDetectorFactory_i::parse_args (int argc, char * argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "o:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file_ = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-o <iorfile>"
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

void FT_FaultDetectorFactory_i::identity (ostream & out) const
{
  if (ior_output_file_)
  {
    out << " file:" << ior_output_file_;
  }
  else if (nsName_ != 0)
  {
    out << " name:" << nsName_;
  }
}


int FT_FaultDetectorFactory_i::self_register (TAO_ORB_Manager & orbManager)
{
  int result = 0;
  orb_ = orbManager.orb();

  // Register with the ORB.
  ior_ = orbManager.activate (this
      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (ior_output_file_ != 0)
  {
    result = write_IOR();
  }
  else
  {
    // if no IOR file specified,
    // then always try to register with name service
    nsName_ = "FT_FaultDetectorFactory";
  }

  if(nsName_ != 0)
  {
    CORBA::Object_var naming_obj =
      orb_->resolve_initial_references ("NameService" ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    if (CORBA::is_nil(naming_obj.in ())){
      ACE_ERROR_RETURN ((LM_ERROR,
                         " (%P|%t) Unable to find the Naming Service\n"),
                        1);
    }

    CosNaming::NamingContext_var naming_context =
      CosNaming::NamingContext::_narrow (naming_obj.in () ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    CosNaming::Name this_name (1);
    this_name.length (1);
    this_name[0].id = CORBA::string_dup (nsName_);

    naming_context->rebind (this_name, _this()
                            ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;
  }

  return result;
}


/////////////////
// Everything you need to know about properties
//  typedef sequence<Property> Properties;
//  struct Property {
//      Name nam;
//    Value val;
//  };
//  typedef CosNaming::Name Name;
//  typedef any Value;
//  typedef sequence <NameComponent> CosNaming::Name;
//  struct NameComponent
//  {
//    // = TITLE
//    //   This is a 'simple' name.
//    //
//    // = DESCRIPTION
//    //   Both id and kind fields are used in resolving names.
//
//    Istring id;
//    // This is the name that is used to identify object references.
//
//    Istring kind;
//    // Stores any addtional info about the object reference.
//  };



void FT_FaultDetectorFactory_i::change_properties (
    const FT::Properties & property_set
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  int todo;
  // Add your implementation here
}

void FT_FaultDetectorFactory_i::shutdown (  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  int todo;
  // Add your implementation here
}

CORBA::Object_ptr FT_FaultDetectorFactory_i::create_object (
    const char * type_id,
    const FT::Criteria & the_criteria,
    FT::GenericFactory::FactoryCreationId_out factory_creation_id
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
    , FT::NoFactory
    , FT::ObjectNotCreated
    , FT::InvalidCriteria
    , FT::InvalidProperty
    , FT::CannotMeetCriteria
  ))
{
  // Add your implementation here
  CORBA::Object_ptr ptr = CORBA::Object::_nil();
  return ptr;
}

void FT_FaultDetectorFactory_i::delete_object (
    const FT::GenericFactory::FactoryCreationId & factory_creation_id
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
    , FT::ObjectNotFound
  ))
{
  int todo;
  // Add your implementation here
}

int FT_FaultDetectorFactory_i::write_IOR()
{
  int result = -1;
  FILE* out = ACE_OS::fopen (nsName_, "w");
  if (out)
  {
    ACE_OS::fprintf (out, "%s", static_cast<const char *>(ior_));
    ACE_OS::fclose (out);
    result = 0;
  }
  return result;
}

