// $Id$

#include "Property_Exporter.h"

ACE_RCSID(server, Property_Exporter, "$Id$")

// *************************************************************
// DP_Adapter
// *************************************************************

class DP_Adapter : public TAO_Dynamic_Property
// = TITLE
//   This class links the a dynamic property in a Trading Service
//   Offer with its value in a CosPropertyService::PropertySet.
{
public:

  DP_Adapter (const char* prop_name,
              CosPropertyService::PropertySet_ptr prop_set);

  ~DP_Adapter (void);

  virtual CORBA::Any* evalDP (const char* name,
                              CORBA::TypeCode_ptr returned_type,
                              const CORBA::Any& extra_info,
                              CORBA::Environment& ACE_TRY_ENV)
    ACE_THROW_SPEC ((CosTradingDynamic::DPEvalFailure));
  // Call back to the Property Service interface. The Property
  // Service reference is contained in the extra_info -- an
  // Asynchronous Completion Token.

private:

  const char* prop_name_;
  CosPropertyService::PropertySet_var prop_set_;
};

DP_Adapter::DP_Adapter (const char* prop_name,
                        CosPropertyService::PropertySet_ptr prop_set)
  : prop_name_ (prop_name),
    prop_set_ (prop_set)
{
}

DP_Adapter::~DP_Adapter (void)
{
}

CORBA::Any*
DP_Adapter::evalDP (const char* name,
                    CORBA::TypeCode_ptr returned_type,
                    const CORBA::Any& extra_info,
                    CORBA::Environment& ACE_TRY_ENV)
  ACE_THROW_SPEC ((CosTradingDynamic::DPEvalFailure))
{
  ACE_TRY
    {
      CORBA::Any* return_value =
        this->prop_set_->get_property_value (this->prop_name_,
                                             ACE_TRY_ENV);
      ACE_TRY_CHECK;

      return return_value;
    }
  ACE_CATCHANY
    {
      ACE_THROW(CosTradingDynamic::DPEvalFailure
                        (this->prop_name_, returned_type, extra_info));
    }
  ACE_ENDTRY;
}

// *************************************************************
// TAO_Property_Exporter
// *************************************************************

TAO_Property_Exporter::
TAO_Property_Exporter (CosTrading::Lookup_var lookup_if,
                       CosPropertyService::PropertySet_ptr prop_set,
                       CORBA::ULong initial_size)
  : increment_ (initial_size),
    lookup_ (lookup_if),
    prop_set_ (prop_set),
    tprops_ (initial_size),
    pprops_ (initial_size),
    pcount_ (0),
    tcount_ (0)
{
}

TAO_Property_Exporter::~TAO_Property_Exporter (void)
{
}

void
TAO_Property_Exporter::add_static_property (const char* name,
                                            const CORBA::Any& value)
{
  CORBA::ULong plength = this->pprops_.length (),
    tlength = this->tprops_.length ();

  if (tlength == this->tcount_)
    this->tprops_.length (tlength + this->increment_);

  if (plength == this->pcount_)
    this->pprops_.length (plength + this->increment_);

  // Add the value to the property service sequence.
  this->tprops_[this->tcount_].name = name;
  this->tprops_[this->tcount_].value = value;
  this->tcount_++;

  // Add the value to the trading service sequence.
  this->pprops_[this->pcount_].property_name = name;
  this->pprops_[this->pcount_].property_value = value;
  this->pcount_++;
}


void
TAO_Property_Exporter::
add_dynamic_property (const char* name,
                      const CORBA::Any& value,
                      TAO_Dynamic_Property& dynamic_prop)
{
  // Add a property to the PropSet and a dynamic property to the
  // Offer. Have the dynamic property connect to the PropSet accessor
  // for that name.
  DP_Adapter* dp_adapter;
  ACE_NEW_RETURN (dp_adapter, DP_Adapter (name, this->prop_set_),);

  CORBA::ULong plength = this->pprops_.length (),
    tlength = this->tprops_.length ();

  if (tlength == this->tcount_)
    this->tprops_.length (tlength + this->increment_);

  if (plength == this->pcount_)
    this->pprops_.length (plength + this->increment_);

  CORBA::Any extra_info;

  CosTradingDynamic::DynamicProp_var dp_struct =
    dynamic_prop.construct_dynamic_prop (name, value.type (), extra_info);

  // Copy the dynamic property struct into the property value
  this->tprops_[this->tcount_].name = name;
  this->tprops_[this->tcount_].value <<= dp_struct.in ();
  this->tcount_++;

  this->pprops_[this->pcount_].property_name = name;
  this->pprops_[this->pcount_].property_value = value;
  this->pcount_++;
}

void
TAO_Property_Exporter::
add_dynamic_property (const char* name,
                      CosTradingDynamic::DynamicProp& dp_struct)
{
  CORBA::ULong tlength = this->tprops_.length ();

  if (tlength == this->tcount_)
    this->tprops_.length (tlength + this->increment_);

  this->tprops_[this->tcount_].name = name;
  this->tprops_[this->tcount_].value <<= dp_struct;
  this->tcount_++;
}

void
TAO_Property_Exporter::
add_dynamic_property (const char* name,
                      CosTradingDynamic::DynamicProp* dp_struct)
{
  CORBA::ULong tlength = this->tprops_.length ();

  if (tlength == this->tcount_)
    this->tprops_.length (tlength + this->increment_);

  this->tprops_[this->tcount_].name = name;
  this->tprops_[this->tcount_].value <<= dp_struct;
  this->tcount_++;
}


CosTrading::OfferId
TAO_Property_Exporter::_cxx_export (const CORBA::Object_ptr object_ref,
                                    const CosTrading::ServiceTypeName type,
                                    CORBA::Environment& ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   CosTrading::Register::InvalidObjectRef,
                   CosTrading::IllegalServiceType,
                   CosTrading::UnknownServiceType,
                   CosTrading::Register::InterfaceTypeMismatch,
                   CosTrading::IllegalPropertyName,
                   CosTrading::PropertyTypeMismatch,
                   CosTrading::ReadonlyDynamicProperty,
                   CosTrading::MissingMandatoryProperty,
                   CosTrading::DuplicatePropertyName))
{
  CosTrading::Register_var reg = this->lookup_->register_if (ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  // Export the offer to the trader under the given type.
  CosTrading::OfferId offer_id = 0;
  this->tprops_.length (this->tcount_);
  offer_id = reg->_cxx_export (object_ref, type, this->tprops_, ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  this->pprops_.length (this->pcount_);
  this->prop_set_->define_properties (this->pprops_, ACE_TRY_ENV);
  ACE_CHECK_RETURN (offer_id);

  return offer_id;
}

CosTrading::OfferId
TAO_Property_Exporter::_cxx_export (const CORBA::Object_ptr object_ref,
                                    const CosTrading::ServiceTypeName type,
                                    const TRADING_REPOS::PropStructSeq& props,
                                    const TRADING_REPOS::ServiceTypeNameSeq& stypes,
                                    CORBA::Environment& ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   CosTrading::IllegalServiceType,
                   TRADING_REPOS::ServiceTypeExists,
                   TRADING_REPOS::InterfaceTypeMismatch,
                   CosTrading::IllegalPropertyName,
                   CosTrading::DuplicatePropertyName,
                   TRADING_REPOS::ValueTypeRedefinition,
                   TRADING_REPOS::DuplicateServiceTypeName,
                   CosTrading::Register::InvalidObjectRef,
                   CosTrading::Register::InterfaceTypeMismatch,
                   CosTrading::ReadonlyDynamicProperty,
                   CosTrading::MissingMandatoryProperty,
                   CosTrading::DuplicatePropertyName))
{
  CosTrading::Register_var reg;
  CosTrading::OfferId offer_id = 0;

  ACE_TRY
    {
      reg = this->lookup_->register_if (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      this->pprops_.length (this->pcount_);
      this->prop_set_->define_properties (this->pprops_, ACE_TRY_ENV);
      ACE_TRY_CHECK;



      /***************** UTTER HACK: REMOVE WHEN EXCEPTIONS WORK! ****
      CosTrading::TypeRepository_ptr obj = this->lookup_->type_repos (_env);

      ACE_DEBUG ((LM_DEBUG, "Attempting add_type.\n"));
      CosTradingRepos::ServiceTypeRepository_var str =

        CosTradingRepos::ServiceTypeRepository::_narrow (obj, _env);
      ACE_CHECK_RETURN (0);

       str->add_type (type,

                     object_ref->_interface_repository_id (),
                     props,
                     stypes,

                     _env);
      ACE_CHECK_RETURN (0);
      ***************** UTTER HACK: REMOVE WHEN EXCEPTIONS WORK! ****/


      // Attempt to export the offer.
      ACE_DEBUG ((LM_DEBUG, "Attempting export.\n"));
      this->tprops_.length (this->tcount_);
      offer_id = reg->_cxx_export (object_ref,
                                   type,
                                   this->tprops_,
                                   ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CosTrading::UnknownServiceType, excp)
    {


      cout<<"Within the UnknownServiceType exception"<<endl;
      CosTrading::TypeRepository_ptr obj = this->lookup_->type_repos (ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      ACE_DEBUG ((LM_DEBUG, "Export failed. Attempting add_type.\n"));
      CosTradingRepos::ServiceTypeRepository_var str =
        CosTradingRepos::ServiceTypeRepository::_narrow (obj, ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      // If the ServiceTypeName wasn't found, we'll have to add the
      // type to the Service Type repository ourselves.
      str->add_type (type,
                     object_ref->_interface_repository_id (),
                     props,
                     stypes,
                     ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      // Now we'll try again to register the offer.
      ACE_DEBUG ((LM_DEBUG, "Attempting export again.\n"));

      this->tprops_.length (this->tcount_);
      offer_id = reg->_cxx_export (object_ref,
                                   type,
                                   this->tprops_,
                                   ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);
      cout<<"It reached here"<<endl;

    }
  /*
  ACE_CATCHANY
    {
      // Sigh, all our efforts were for naight.
      ACE_RETHROW;
    }
  */
  ACE_ENDTRY;

  return offer_id;
}

// *************************************************************
// TAO_Property_Modifier
// *************************************************************

TAO_Property_Modifier::
TAO_Property_Modifier (CosTrading::Lookup_var lookup_if,
                       CosPropertyService::PropertySet_ptr prop_set,
                       CORBA::ULong initial_size)
  : increment_ (initial_size),
    lookup_ (lookup_if),
    prop_set_ (prop_set),
    pdelete_ (initial_size),
    pmodify_ (initial_size),
    tdelete_ (initial_size),
    tmodify_ (initial_size),
    pdcount_ (0),
    pmcount_ (0),
    tdcount_ (0),
    tmcount_ (0)
{
}

void
TAO_Property_Modifier::delete_property (const char* name)
{
  CORBA::ULong plength = this->pdelete_.length (),
    tlength = this->tdelete_.length ();

  if (tlength == this->tdcount_)
    this->tdelete_.length (tlength + this->increment_);

  if (plength == this->pdcount_)
    this->pdelete_.length (plength + this->increment_);

  this->tdelete_[this->tdcount_] = name;
  this->tdcount_++;

  this->pdelete_[this->pdcount_] = name;
  this->pdcount_++;
}

void
TAO_Property_Modifier::modify_property (const char* name, const CORBA::Any& value)
{
  CORBA::ULong plength = this->pmodify_.length (),
    tlength = this->tmodify_.length ();

  if (tlength == this->tmcount_)
    this->tmodify_.length (tlength + this->increment_);

  if (plength == this->pmcount_)
    this->pmodify_.length (plength + this->increment_);

  this->tmodify_[this->tmcount_].name = name;
  this->tmodify_[this->tmcount_].value = value;
  this->tdcount_++;

  this->pmodify_[this->pmcount_].property_name = name;
  this->pmodify_[this->pmcount_].property_value = value;
  this->pdcount_++;
}

void
TAO_Property_Modifier::commit (CosTrading::OfferId id,
                               CORBA::Environment& ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   CosPropertyService::MultipleExceptions,
                   CosTrading::NotImplemented,
                   CosTrading::IllegalOfferId,
                   CosTrading::UnknownOfferId,
                   CosTrading::Register::ProxyOfferId,
                   CosTrading::IllegalPropertyName,
                   CosTrading::Register::UnknownPropertyName,
                   CosTrading::PropertyTypeMismatch,
                   CosTrading::ReadonlyDynamicProperty,
                   CosTrading::Register::MandatoryProperty,
                   CosTrading::Register::ReadonlyProperty,
                   CosTrading::DuplicatePropertyName))
{
  this->tdelete_.length (this->tdcount_);
  this->tmodify_.length (this->tmcount_);
  this->pdelete_.length (this->pdcount_);
  this->pmodify_.length (this->pmcount_);

  CosTrading::Register_var reg = this->lookup_->register_if (ACE_TRY_ENV);
  ACE_CHECK;

  reg->modify (id, this->tdelete_, this->tmodify_, ACE_TRY_ENV);
  ACE_CHECK;

  this->prop_set_->define_properties (this->pmodify_, ACE_TRY_ENV);
  ACE_CHECK;

  this->prop_set_->delete_properties (this->pdelete_, ACE_TRY_ENV);
  ACE_CHECK;
}
