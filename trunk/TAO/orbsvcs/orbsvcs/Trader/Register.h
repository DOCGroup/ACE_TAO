/* -*- C++ -*- */

// ============================================================================
// $Id$ 
//
// = LIBRARY
//    orbsvcs
// 
// = FILENAME
//   Register.h
//
// = AUTHOR
//    Marina Spivak <marina@cs.wustl.edu>
//    Seth Widoff   <sbw1@cs.wustl.edu>
// 
// ============================================================================
#ifndef ACE_REGISTER_H
#define ACE_REGISTER_H

#include "Attributes_T.h"
#include "Offer_Modifier.h"

template <class TRADER>
class TAO_Register :
  public TAO_Trader_Components<POA_CosTrading::Register>,
  public TAO_Support_Attributes<POA_CosTrading::Register>
  //
  // = TITLE
  //     This class implements CosTrading::Register IDL interface.
{
public:

  TAO_Register (TRADER &trader);

  virtual ~TAO_Register (void);

  virtual CosTrading::OfferId export (CORBA::Object_ptr reference, 
				      const char *type, 
				      const CosTrading::PropertySeq& properties,
				      CORBA::Environment& _env)
    TAO_THROW_SPEC ((CORBA::SystemException, 
		    CosTrading::Register::InvalidObjectRef, 
		    CosTrading::IllegalServiceType, 
		    CosTrading::UnknownServiceType, 
		    CosTrading::Register::InterfaceTypeMismatch, 
		    CosTrading::IllegalPropertyName, 
		    CosTrading::PropertyTypeMismatch, 
		    CosTrading::ReadonlyDynamicProperty, 
		    CosTrading::MissingMandatoryProperty, 
		    CosTrading::DuplicatePropertyName));

  // BEGIN SPEC
  // The export operation is the means by which a service is
  // advertised, via a trader, to a community of potential
  // importers. The OfferId returned is the handle with which the
  // exporter can identify the exported offer when attempting to
  // access it via other operations. The OfferId is only meaningful in
  // the context of the trader that generated it.

  //   The "reference" parameter is the information that enables a client
  // to interact with a remote server. If a trader implementation chooses
  // to consider certain types of object references (e.g., a nil object
  // reference) to be unexportable, then it may return the InvalidObjectRef
  // exception in such cases.

  // The "type" parameter
  // identifies the service type, which contains the interface type of
  // the "reference" and a set of named property types that may be
  // used in further describing this offer (i.e., it restricts what is
  // acceptable in the properties parameter). °

  // If the string
  // representation of the "type" does not obey the rules for
  // identifiers, then an IllegalServiceType exception is raised. ° If
  // the "type" is correct syntactically but a trader is able to
  // unambiguously determine that it is not a recognized service type,
  // then an UnknownServiceType exception is raised. °

  // If the trader
  // can determine that the interface type of the "reference"
  // parameter is not a subtype of the interface type specified in
  // "type," then an InterfaceTypeMismatch exception is raised. The
  // "properties" parameter is a list of named values that conform to
  // the property value types defined for those names. They describe
  // the service being offered. This description typically covers
  // behavioral, non-functional, and non-computational aspects of the
  // service. °

  // If any of the property names do not obey the syntax
  // rules for PropertyNames, then an IllegalPropertyName exception is
  // raised. °

  // If the type of any of the property values is not the
  // same as the declared type (declared in the service type), then a
  // PropertyTypeMismatch exception is raised. 

  // ° If an attempt is made to assign a dynamic property value to a
  // readonly property, then the ReadonlyDynamicProperty exception is
  // raised. ° If the "properties" parameter omits any property
  // declared in the service type with a mode of mandatory, then a
  // MissingMandatoryProperty exception is raised. ° If two or more
  // properties with the same property name are included in this
  // parameter, the DuplicatePropertyName exception is raised.
  // END SPEC
 
  virtual void withdraw (const char *id, CORBA::Environment& _env)
    TAO_THROW_SPEC ((CORBA::SystemException, 
		    CosTrading::IllegalOfferId, 
		    CosTrading::UnknownOfferId, 
		    CosTrading::Register::ProxyOfferId));

  // BEGIN SPEC
  // The withdraw operation removes the service offer from the trader
  // (i.e., after withdraw the offer can no longer be returned as the
  // result of a query). The offer is identified by the "id" parameter
  // which was originally returned by export. ° If the string
  // representation of "id" does not obey the rules for offer
  // identifiers, then an IllegalOfferId exception is raised. ° If the
  // "id" is legal but there is no offer within the trader with that
  // "id," then an UnknownOfferId exception is raised. ° If the "id"
  // identifies a proxy offer rather than an ordinary offer, then a
  // ProxyOfferId exception is raised. 
  // END SPEC
  
  virtual CosTrading::Register::OfferInfo*
    describe (const char * id,
	      CORBA::Environment& _env)
    TAO_THROW_SPEC ((CORBA::SystemException, 
		    CosTrading::IllegalOfferId, 
		    CosTrading::UnknownOfferId, 
		    CosTrading::Register::ProxyOfferId));

  // BEGIN SPEC
  // The describe operation returns the information about an offered
  // service that is held by the trader. It comprises the "reference"
  // of the offered service, the "type" of the service offer, and the
  // "properties" that describe this offer of service. The offer is
  // identified by the "id" parameter which was originally returned by
  // export. ° If the string representation of "id" does not obey the
  // rules for object identifiers, then an IllegalOfferId exception is
  // raised. ° If the "id" is legal but there is no offer within the
  // trader with that "id," then an UnknownOfferId exception is
  // raised. ° If the "id" identifies a proxy offer rather than an
  // ordinary offer, then a ProxyOfferId exception is raised. 
  // END SPEC
  
  virtual void modify (const char * id, 
		       const CosTrading::PropertyNameSeq& del_list, 
		       const CosTrading::PropertySeq& modify_list,
		       CORBA::Environment& _env)
    TAO_THROW_SPEC ((CORBA::SystemException, 
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
		    CosTrading::DuplicatePropertyName));

  // BEGIN SPEC
  // The modify operation is used to change the description of a
  // service as held within a service offer. The object reference and
  // the service type associated with the offer cannot be
  // changed. This operation may: ° add new (non-mandatory) properties
  // to describe an offer, ° change the values of some existing (not
  // readonly) properties, or ° delete existing (neither mandatory nor
  // readonly) properties.

  // The modify operation either succeeds completely or it fails
  // completely. The offer is identified by the "id" parameter which
  // was originally returned by export. ° If the string representation
  // of "id" does not obey the rules for offer identifiers, then an
  // IllegalOfferId exception is raised. ° If the "id" is legal but
  // there is no offer within the trader with that "id," then an
  // UnknownOfferId exception is raised. ° If the "id" identifies a
  // proxy offer rather than an ordinary offer, then a ProxyOfferId
  // exception is raised.

  // The "del_list" parameter gives the names of the properties that
  // are no longer to be recorded for the identified offer. Future
  // query and describe operations will not see these properties. ° If
  // any of the names within the "del_list" do not obey the rules for
  // PropertyName's, then an IllegalPropertyName exception is
  // raised. ° If a "name" is legal but there is no property for the
  // offer with that "name," then an UnknownPropertyName exception is
  // raised. ° If the list includes a property that has a mandatory
  // mode, then the MandatoryProperty exception is raised. ° If the
  // same property name is included two or more times in this
  // parameter, the DuplicatePropertyName exception is raised.

  // The "modify_list" parameter gives the names and values of
  // properties to be changed. If the property is not in the offer,
  // then the modify operation adds it. The modified (or added)
  // property values are returned in future query and describe
  // operations in place of the original values. ° If any of the names
  // within the "modify_list" do not obey the rules for
  // PropertyName's, then an IllegalPropertyName exception is
  // raised. ° If the list includes a property that has a readonly
  // mode, then the ReadonlyProperty exception is raised unless that
  // readonly property is not currently recorded for the offer. The
  // ReadonlyDynamicProperty exception is raised if an attempt is made
  // to assign a dynamic property value to a readonly property. ° If
  // the value of any modified property is of a type that is not the
  // same as the type expected, then the PropertyTypeMismatch
  // exception is raised. ° If two or more properties with the same
  // property name are included in this argument, the
  // DuplicatePropertyName exception is raised.

  // The NotImplemented exception shall be raised if and only if the
  // supports_modifiable_properties attribute yields FALSE. 
  // END SPEC
  
  virtual void withdraw_using_constraint (const char *type, 
					  const char *constr,
					  CORBA::Environment& _env)
    TAO_THROW_SPEC ((CORBA::SystemException, 
		    CosTrading::IllegalServiceType, 
		    CosTrading::UnknownServiceType, 
		    CosTrading::IllegalConstraint, 
		    CosTrading::Register::NoMatchingOffers));

  // BEGIN SPEC
  // The withdraw_using_constraint operation withdraws a set of offers
  // from within a single trader. This set is identified in the same
  // way that a query operation identifies a set of offers to be
  // returned to an importer.

  // The "type" parameter conveys the required service type. Each
  // offer of the specified type will have the constraint expression
  // applied to it. If it matches the constraint expression, then the
  // offer will be withdrawn.° If "type" does not obey the rules for
  // service types, then an IllegalServiceType exception is raised. °
  // If the "type" is correct syntactically but is not recognized as a
  // service type by the trader, then an UnknownServiceType exception
  // is raised. 

  // The constraint "constr" is the means by which the client
  // restricts the set of offers to those that are intended for
  // withdrawal. ° If "constr" does not obey the syntax rules for a
  // constraint then an IllegalConstraint exception is raised. ° If
  // the constraint fails to match with any offer of the specified
  // service type, then a NoMatchingOffers exception is raised. 
  // END SPEC
  
  virtual CosTrading::Register_ptr
    resolve (const CosTrading::TraderName &name,
	     CORBA::Environment& _env)
    TAO_THROW_SPEC ((CORBA::SystemException, 
		    CosTrading::Register::IllegalTraderName, 
		    CosTrading::Register::UnknownTraderName, 
		    CosTrading::Register::RegisterNotSupported));

  // BEGIN SPEC
  // This operation is used to resolve a context relative name for
  // another trader. In particular, it is used when exporting to a
  // trader that is known by a name rather than by an object
  // reference. The client provides the name, which will be a sequence
  // of name components. ° If the content of the parameter cannot
  // yield legal syntax for the first component, then the
  // IllegalTraderName exception is raised. Otherwise, the first name
  // component is compared against the name held in each link. ° If no
  // match is found, or the trader does not support links, the
  // UnknownTraderName exception is raised. Otherwise, the trader
  // obtains the register_if held as part of the matched link. ° If
  // the Register interface is not nil, then the trader binds to the
  // Register interface and invokes resolve but passes the TraderName
  // with the first component removed; if it is nil, then the
  // RegisterNotSupported exception is raised. When a trader is able
  // to match the first name component leaving no residual name, that
  // trader returns the reference for the Register interface for that
  // linked trader. In unwinding the recursion, intermediate traders
  // return the Register interface reference to their client (another
  // trader).
  // END SPEC

 protected:

  typedef TRADER::Service_Type_Map Service_Type_Map;
  
  void validate_properties (const char* type,
			    CosTradingRepos::ServiceTypeRepository::TypeStruct* type_struct,
			    CosTrading::PropertySeq& properties,
			    CORBA::Environment& _env)
    TAO_THROW_SPEC ((CosTrading::IllegalPropertyName, 
		    CosTrading::PropertyTypeMismatch, 
		    CosTrading::ReadonlyDynamicProperty, 
		    CosTrading::MissingMandatoryProperty, 
		    CosTrading::DuplicatePropertyName));
  
  // Type is a known service type.
  
  CORBA::Boolean find_property (CosTrading::PropertySeq &properties,
				const char *property_name,
				CORBA::ULong &position);
  
 private:
  TRADER &trader_;
};


#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Register.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#endif /* ACE_REGISTER_H */
