/* -*- C++ -*- */

// ========================================================================
// $Id$
// 
// = LIBRARY
//    orbsvcs
// 
// = FILENAME
//   Service_Type_Repository.h
//
// = AUTHOR
//    Marina Spivak <marina@cs.wustl.edu>
//    Seth Widoff <sbw1@cs.wustl.edu>
// 
// ========================================================================

#ifndef TAO_SERVICE_TYPE_REPOSITORY_H
#define TAO_SERVICE_TYPE_REPOSITORY_H

#include "stl.h"
#include "Trader.h"
#include "Monitor.h"

class TAO_Service_Type_Repository :
  public POA_CosTradingRepos::ServiceTypeRepository
  //
  // = TITLE
  //     This class implements CosTradingRepos::ServiceTypeRepository
  //     IDL interface. 
{
public:
  
  TAO_Service_Type_Repository (ACE_Lock* lock = 0);
  // Parameterize the Service_Type_Repository with a lock to serialize
  // access to the type repository map. A reader/writer lock is
  // probably best. The Service_Type_Repository assumes control of the 
  // lock.
  
  ~TAO_Service_Type_Repository (void);
  
  virtual CosTradingRepos::ServiceTypeRepository::IncarnationNumber 
    incarnation (CORBA::Environment& _env);  

  virtual CosTradingRepos::ServiceTypeRepository::IncarnationNumber 
    add_type (const char * name, 
	      const char * if_name, 
	      const CosTradingRepos::ServiceTypeRepository::PropStructSeq& props,
	      const SERVICE_TYPE_REPOS::ServiceTypeNameSeq& super_types,
	      CORBA::Environment& _env)
    TAO_THROW_SPEC ((CORBA::SystemException,
		    CosTrading::IllegalServiceType, 
		    SERVICE_TYPE_REPOS::ServiceTypeExists, 
		    SERVICE_TYPE_REPOS::InterfaceTypeMismatch, 
		    CosTrading::IllegalPropertyName, 
		    CosTrading::DuplicatePropertyName, 
		    SERVICE_TYPE_REPOS::ValueTypeRedefinition, 
		    CosTrading::UnknownServiceType, 
		    SERVICE_TYPE_REPOS::DuplicateServiceTypeName));

  // BEGIN SPEC
  // The add_type operation enables the creation of new service types
  // in the service type repository. The caller supplies the "name"
  // for the new type, the identifier for the interface associated
  // with instances of this service type, the properties definitions
  // for this service type, and the service type names of the
  // immediate super-types to this service type.

  // If the type creation is successful, an incarnation number is
  // returned as the value of the operation. Incarnation numbers are
  // opaque values that are assigned to each modification to the
  // repository's state. An incarnation number can be quoted when
  // invoking the list_types operation to retrieve all changes to the
  // service repository since a particular logical time. (Note:
  // IncarnationNumber is currently declared as a struct consisting of
  // two unsigned longs; what we really want here is an unsigned hyper
  // [64-bit integer]. A future revision task force should modify this
  // when CORBA systems support IDL 64-bit integers.)

  // If the "name" parameter is malformed, then the
  // CosTrading::IllegalServiceType exception is raised. � If the type
  // already exists, then the ServiceTypeExists exception is raised. �
  // If the "if_name" parameter is not a sub-type of the interface
  // associated with a service type from which this service type is
  // derived, such that substitutability would be violated, then the
  // InterfaceTypeMismatch exception is raised. � If a property name
  // supplied in the "props" parameter is malformed, the
  // CosTrading::IllegalPropertyName exception is raised. � If the
  // same property name appears two or more times in the "props"
  // parameter, the CosTrading::DuplicatePropertyName exception is
  // raised. � If a property value type associated with this service
  // type illegally modifies the value type of a super-type's
  // property, or if two super-types incompatibly declare value types
  // for the same property name, then the ValueTypeRedefinition
  // exception is raised. � If one of the ServiceTypeNames in
  // "super_types" is malformed, then the
  // CosTrading::IllegalServiceType exception is raised. If one of the
  // ServiceTypeNames in "super_types" does not exist, then the
  // CosTrading::UnknownServiceType exception is raised. � If the same
  // service type name is included two or more times in this
  // parameter, the DuplicateServiceTypeName exception is raised. 
  // END SPEC
  

  virtual void remove_type (const char * name,
			    CORBA::Environment& _env)
    TAO_THROW_SPEC ((CORBA::SystemException,
		    CosTrading::IllegalServiceType, 
		    CosTrading::UnknownServiceType, 
		    SERVICE_TYPE_REPOS::HasSubTypes));

  // BEGIN SPEC
  // The remove_type operation removes the named type from the service
  // type repository. � If "name" is malformed, then the
  // CosTrading::IllegalServiceType exception is raised. � If "name"
  // does not exist within the repository, then the
  // CosTrading::UnknownServiceType exception is raised. � If "name"
  // has a service type which has been derived from it, then the
  // HasSubTypes exception is raised. 
  // END SPEC
  
  virtual SERVICE_TYPE_REPOS::ServiceTypeNameSeq* 
    list_types (const SERVICE_TYPE_REPOS::SpecifiedServiceTypes& which_types,
		CORBA::Environment& _env)
    TAO_THROW_SPEC ((CORBA::SystemException));

  // BEGIN SPEC
  // The list_types operation permits a client to obtain the names of
  // service types which are in the repository. The "which_types"
  // parameter permits the client to specify one of two possible
  // values: � all types known to the repository � all types
  // added/modified since a particular incarnation number The names of
  // the requested types are returned by the operation for subsequent
  // querying via the describe_type or the fully_describe_type
  // operation. 
  // END SPEC 
           
  virtual SERVICE_TYPE_REPOS::TypeStruct*
    describe_type (const char * name,
		   CORBA::Environment& _env) 
    TAO_THROW_SPEC ((CORBA::SystemException,
		     CosTrading::IllegalServiceType, 
		     CosTrading::UnknownServiceType));
  // BEGIN SPEC
  // The describe_type operation permits a client to obtain the
  // details for a particular service type. � If "name" is malformed,
  // then the CosTrading::IllegalServiceType exception is raised. � If
  // "name" does not exist within the repository, then the
  // CosTrading::UnknownServiceType exception is raised. 
  // END SPEC
  
  virtual SERVICE_TYPE_REPOS::TypeStruct* 
    fully_describe_type (const char * name,
			 CORBA::Environment& _env) 
    TAO_THROW_SPEC ((CORBA::SystemException, 
		     CosTrading::IllegalServiceType, 
		     CosTrading::UnknownServiceType));
  // BEGIN SPEC
  // The fully_describe_type operation permits a client to obtain the
  // details for a particular service type. The property sequence
  // returned in the TypeStruct includes all properties inherited from
  // the transitive closure of its super types; the sequence of super
  // types in the TypeStruct contains the names of the types in the
  // transitive closure of the super type relation. � If "name" is
  // malformed, then the CosTrading::IllegalServiceType exception is
  // raised. � If "name" does not exist within the repository, then
  // the CosTrading::UnknownServiceType exception is raised. 
  // END SPEC 

  virtual void mask_type (const char * name,
			  CORBA::Environment& _env) 
    TAO_THROW_SPEC ((CORBA::SystemException,
		     CosTrading::IllegalServiceType, 
		     CosTrading::UnknownServiceType, 
		     SERVICE_TYPE_REPOS::AlreadyMasked));
  // BEGIN SPEC
  // The mask_type operation permits the deprecation of a particular
  // type (i.e., after being masked, exporters will no longer be able
  // to advertise offers of that particular type). The type continues
  // to exist in the service repository due to other service types
  // being derived from it. � If "name" is malformed, then the
  // CosTrading::IllegalServiceType exception is raised. � If "name"
  // does not exist within the repository, then the
  // CosTrading::UnknownServiceType exception is raised. � If the type
  // is currently in the masked state, then the AlreadyMasked
  // exception is raised. 
  // END SPEC 
       
  virtual void unmask_type (const char * name,
			    CORBA::Environment& _env) 
    TAO_THROW_SPEC ((CORBA::SystemException, 
		    CosTrading::IllegalServiceType, 
		    CosTrading::UnknownServiceType, 
		    SERVICE_TYPE_REPOS::NotMasked));
  // BEGIN SPEC
  // The unmask_type undeprecates a type (i.e., after being unmasked,
  // exporters will be able to resume advertisement of offers of that
  // particular type). � If "name" is malformed, then the
  // CosTrading::IllegalServiceType exception is raised. � If "name"
  // does not exist within the repository, then the
  // CosTrading::UnknownServiceType exception is raised. � If the type
  // is not currently in the masked state, then the NotMasked
  // exception is raised. 
  // END SPEC

private:
  
  struct Type_Info {
    // storage structure for information pertinent to the type.

    typedef list<string> TYPE_LIST;

    SERVICE_TYPE_REPOS::TypeStruct type_info_; 
    // standard type info.

    SERVICE_TYPE_REPOS::PropStructSeq all_prop_; 
    // complete listing of the type's properties (including inherited).
    
    TYPE_LIST sub_types_;
    // names of subtypes.
  };
  
  typedef map< string, Type_Info, less <string> > SERVICE_TYPE_MAP;
  typedef SERVICE_TYPE_MAP::iterator SERVICE_TYPE_ITERATOR;
  
  typedef map
    <
    string,
    SERVICE_TYPE_REPOS::PropStruct*,
    less < string >
    > PROP_MAP;
  
  typedef map
    <
    string,
    SERVICE_TYPE_MAP::iterator,
    less < string >
    > SUPER_TYPE_MAP;
  
  void validate_properties (const SERVICE_TYPE_REPOS::PropStructSeq& props,
			    PROP_MAP& prop_map,
			    CORBA::Environment& _env)
    TAO_THROW_SPEC ((CosTrading::IllegalPropertyName,
		     CosTrading::DuplicatePropertyName));
  // Confirm that the properties in props have valid names, and aren't
  // duplicated. Cram those properties into the prop_map.
  
  void validate_supertypes (const SERVICE_TYPE_REPOS::ServiceTypeNameSeq& super_types,
			    SUPER_TYPE_MAP& super_map,
			    CORBA::Environment& _env)
    TAO_THROW_SPEC ((CosTrading::IllegalServiceType,
		    CosTrading::UnknownServiceType,
		    CosTrading::DuplicatePropertyName));   
  // Confirm that the each super type exists, and cram them into super_map. 
  
  void validate_inheritance (PROP_MAP& prop_map,
			     SUPER_TYPE_MAP& super_map,
			     CORBA::Environment& _env)
    TAO_THROW_SPEC ((SERVICE_TYPE_REPOS::ValueTypeRedefinition));
  // Ensure that properties of a super_type aren't having their types
  // or retstrictions redefined.

  //  void validate_interface (const char* if_name,
  //			   SUPER_TYPE_MAP& super_map,
  //			   CORBA::Environment& _env)
  //    TAO_THROW_SPEC ((SERVICE_TYPE_REPOS::InterfaceTypeMismatch));
  // Ensure that the interface type derives from its superclasses'.
  
  void update_type_map (const char* name,
			SERVICE_TYPE_REPOS::TypeStruct& type,
			PROP_MAP& prop_map,
			SUPER_TYPE_MAP& super_map);
  // Update the type map with the information contained in the
  // TypeStruct, prop_map, and super_map.

  ACE_Lock* lock_;
  // Lock with which to serialize access to the service type map.
  
  SERVICE_TYPE_MAP type_map_;
  // Stores information for each service type in the repository.
  // This is a mapping from service type name to a Type_Info struct
  // which serves as a storage for various information for the given type.

  SERVICE_TYPE_REPOS::IncarnationNumber incarnation_;
  // incarnation number to be used for the next modification to the repository.
};

// This are temporary functions, until the format of the incarnation number
// will be changed in the spec.

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Service_Type_Repository.cpp"
#endif  /* ACE_TEMPLATES_REQUIRE_SOURCE */

#endif /* TAO_SERVICE_TYPE_REPOSITORY_H */
