/* -*- C++ -*- $Id$ */
// ============================================================================
//
// = LIBRARY
//     FT_Service(aka. DOORS, CASO etc.)
//
// = FILENAME
//     FT_CORBA_ServiceS.h
//
// = DESCRIPTION
//     This is the generated code by the IDL compiler for the FT
//     specification. The generated code here would be used by the
//     service layer. The service layer essentially consists of the
//     Replication Manager, Property Manager, Generic Factory, Fault
//     Monitors, and Fault Notfifiers.
//
// = AUTHOR
//     Balachandran Natarajan <bala@cs.wustl.edu>
//
// ============================================================================
#ifndef TAO_FT_CORBA_SERVICES_H
#define TAO_FT_CORBA_SERVICES_H
#include "ace/pre.h"
#include "tao/ftpoafwd.h"
#include "tao/FT_CORBAS.h"
#include "FT_CORBA_ServiceC.h"


#if (TAO_HAS_FT_CORBA == 1)

#if defined (ACE_HAS_MINIMUM_IOSTREAMH_INCLUSION)
#include "ace/streams.h"
#endif /* ACE_HAS_MINIMUM_IOSTREAMH_INCLUSION */

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (TAO_EXPORT_MACRO)
#undef TAO_EXPORT_MACRO
#endif
#define TAO_EXPORT_MACRO TAO_Export

#if defined (TAO_EXPORT_NESTED_CLASSES)
#  if defined (TAO_EXPORT_NESTED_MACRO)
#    undef TAO_EXPORT_NESTED_MACRO
#  endif /* defined (TAO_EXPORT_NESTED_MACRO) */
#  define TAO_EXPORT_NESTED_MACRO TAO_Export
#endif /* TAO_EXPORT_NESTED_CLASSES */

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_Export FT_POA_PropertyManager :  public virtual PortableServer::ServantBase
{
protected:
  FT_POA_PropertyManager (void);

public:
  FT_POA_PropertyManager (const FT_POA_PropertyManager& rhs);
  virtual ~FT_POA_PropertyManager (void);


  virtual CORBA::Boolean _is_a (
                                const char* logical_type_id,
                                CORBA::Environment &ACE_TRY_ENV =
                                TAO_default_environment ()
      );

    virtual void* _downcast (
        const char* logical_type_id
      );

    static void _is_a_skel (
        CORBA::ServerRequest &req,
        void *obj,
        void *context,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      );

    static void _non_existent_skel (
        CORBA::ServerRequest &req,
        void *obj,
        void *context,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      );

    virtual void _dispatch (
        CORBA::ServerRequest &_tao_req,
        void *_tao_context,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      );

    FT::PropertyManager *_this (
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      );

    virtual const char* _interface_repository_id (void) const;

    virtual void set_default_properties (
        const FT::Properties & props,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        FT::InvalidProperty,
        FT::UnsupportedProperty
      )) = 0;

    static void set_default_properties_skel (
        CORBA::ServerRequest &_tao_req,
        void *_tao_obj,
        void *_tao_context,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      );

    virtual FT::Properties * get_default_properties (
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      )) = 0;

    static void get_default_properties_skel (
        CORBA::ServerRequest &_tao_req,
        void *_tao_obj,
        void *_tao_context,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      );

    virtual void remove_default_properties (
        const FT::Properties & props,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        FT::InvalidProperty,
        FT::UnsupportedProperty
      )) = 0;

    static void remove_default_properties_skel (
        CORBA::ServerRequest &_tao_req,
        void *_tao_obj,
        void *_tao_context,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      );

    virtual void set_type_properties (
        const char * type_id,
        const FT::Properties & overrides,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        FT::InvalidProperty,
        FT::UnsupportedProperty
      )) = 0;

    static void set_type_properties_skel (
        CORBA::ServerRequest &_tao_req,
        void *_tao_obj,
        void *_tao_context,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      );

    virtual FT::Properties * get_type_properties (
        const char * type_id,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      )) = 0;

    static void get_type_properties_skel (
        CORBA::ServerRequest &_tao_req,
        void *_tao_obj,
        void *_tao_context,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      );

    virtual void remove_type_properties (
        const char * type_id,
        const FT::Properties & props,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        FT::InvalidProperty,
        FT::UnsupportedProperty
      )) = 0;

    static void remove_type_properties_skel (
        CORBA::ServerRequest &_tao_req,
        void *_tao_obj,
        void *_tao_context,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      );

    virtual void set_properties_dynamically (
        FT::ObjectGroup_ptr object_group,
        const FT::Properties & overrides,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        FT::ObjectGroupNotFound,
        FT::InvalidProperty,
        FT::UnsupportedProperty
      )) = 0;

    static void set_properties_dynamically_skel (
        CORBA::ServerRequest &_tao_req,
        void *_tao_obj,
        void *_tao_context,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      );

    virtual FT::Properties * get_properties (
        FT::ObjectGroup_ptr object_group,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        FT::ObjectGroupNotFound
      )) = 0;

    static void get_properties_skel (
        CORBA::ServerRequest &_tao_req,
        void *_tao_obj,
        void *_tao_context,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      );


};


class TAO_Export FT_POA_tao_thru_poa_collocated_PropertyManager :
  public virtual FT::PropertyManager
{
public:
  FT_POA_tao_thru_poa_collocated_PropertyManager (
      TAO_Stub *stub
    );

  virtual CORBA::Boolean _is_a(
      const CORBA::Char *logical_type_id,
      CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ()
    );

  virtual CORBA::Boolean _non_existent(
      CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ()
    );

  virtual void set_default_properties (
      const FT::Properties & props,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      FT::InvalidProperty,
      FT::UnsupportedProperty
    ));

  virtual FT::Properties * get_default_properties (
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));

  virtual void remove_default_properties (
      const FT::Properties & props,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
        CORBA::SystemException,
        FT::InvalidProperty,
        FT::UnsupportedProperty
      ));

  virtual void set_type_properties (
      const char * type_id,
      const FT::Properties & overrides,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      FT::InvalidProperty,
      FT::UnsupportedProperty
    ));

  virtual FT::Properties * get_type_properties (
      const char * type_id,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));

  virtual void remove_type_properties (
      const char * type_id,
      const FT::Properties & props,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      FT::InvalidProperty,
      FT::UnsupportedProperty
    ));

  virtual void set_properties_dynamically (
      FT::ObjectGroup_ptr object_group,
      const FT::Properties & overrides,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      FT::ObjectGroupNotFound,
      FT::InvalidProperty,
      FT::UnsupportedProperty
    ));

  virtual FT::Properties * get_properties (
      FT::ObjectGroup_ptr object_group,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      FT::ObjectGroupNotFound
    ));

};




class TAO_Export FT_POA_ObjectGroupManager :  public virtual PortableServer::ServantBase
{
protected:
  FT_POA_ObjectGroupManager (void);

public:
  FT_POA_ObjectGroupManager (const FT_POA_ObjectGroupManager& rhs);
  virtual ~FT_POA_ObjectGroupManager (void);


  virtual CORBA::Boolean _is_a (
      const char* logical_type_id,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    );

  virtual void* _downcast (
      const char* logical_type_id
    );

  static void _is_a_skel (
      CORBA::ServerRequest &req,
      void *obj,
      void *context,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    );

  static void _non_existent_skel (
      CORBA::ServerRequest &req,
      void *obj,
      void *context,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    );

  virtual void _dispatch (
      CORBA::ServerRequest &_tao_req,
      void *_tao_context,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    );

  FT::ObjectGroupManager *_this (
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    );

  virtual const char* _interface_repository_id (void) const;

  virtual FT::ObjectGroup_ptr create_member (
      FT::ObjectGroup_ptr object_group,
      const FT::Location & the_location,
      const char * type_id,
      const FT::Criteria & the_criteria,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      FT::ObjectGroupNotFound,
      FT::MemberAlreadyPresent,
      FT::NoFactory,
      FT::ObjectNotCreated,
      FT::InvalidCriteria,
      FT::CannotMeetCriteria
    )) = 0;

  static void create_member_skel (
      CORBA::ServerRequest &_tao_req,
      void *_tao_obj,
      void *_tao_context,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    );

  virtual FT::ObjectGroup_ptr add_member (
      FT::ObjectGroup_ptr object_group,
      const FT::Location & the_location,
      CORBA::Object_ptr member,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      FT::ObjectGroupNotFound,
      FT::MemberAlreadyPresent,
      FT::ObjectNotAdded
    )) = 0;

  static void add_member_skel (
      CORBA::ServerRequest &_tao_req,
      void *_tao_obj,
      void *_tao_context,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    );

  virtual FT::ObjectGroup_ptr remove_member (
      FT::ObjectGroup_ptr object_group,
      const FT::Location & the_location,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      FT::ObjectGroupNotFound,
      FT::MemberNotFound
    )) = 0;

  static void remove_member_skel (
      CORBA::ServerRequest &_tao_req,
      void *_tao_obj,
      void *_tao_context,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    );

  virtual FT::ObjectGroup_ptr set_primary_member (
      FT::ObjectGroup_ptr object_group,
      const FT::Location & the_location,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      FT::ObjectGroupNotFound,
      FT::MemberNotFound,
      FT::PrimaryNotSet,
      FT::BadReplicationStyle
    )) = 0;

  static void set_primary_member_skel (
      CORBA::ServerRequest &_tao_req,
      void *_tao_obj,
      void *_tao_context,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    );

  virtual FT::Locations * locations_of_members (
      FT::ObjectGroup_ptr object_group,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      FT::ObjectGroupNotFound
    )) = 0;

  static void locations_of_members_skel (
      CORBA::ServerRequest &_tao_req,
      void *_tao_obj,
      void *_tao_context,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    );

  virtual FT::ObjectGroupId get_object_group_id (
      FT::ObjectGroup_ptr object_group,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      FT::ObjectGroupNotFound
    )) = 0;

  static void get_object_group_id_skel (
      CORBA::ServerRequest &_tao_req,
      void *_tao_obj,
      void *_tao_context,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    );

  virtual FT::ObjectGroup_ptr get_object_group_ref (
        FT::ObjectGroup_ptr object_group,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        FT::ObjectGroupNotFound
      )) = 0;

  static void get_object_group_ref_skel (
        CORBA::ServerRequest &_tao_req,
        void *_tao_obj,
        void *_tao_context,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      );

  virtual CORBA::Object_ptr get_member_ref (
        FT::ObjectGroup_ptr object_group,
        const FT::Location & loc,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        FT::ObjectGroupNotFound,
        FT::MemberNotFound
      )) = 0;

  static void get_member_ref_skel (
        CORBA::ServerRequest &_tao_req,
        void *_tao_obj,
        void *_tao_context,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      );


};


class TAO_Export FT_POA_tao_thru_poa_collocated_ObjectGroupManager : public virtual FT::ObjectGroupManager
{
public:
  FT_POA_tao_thru_poa_collocated_ObjectGroupManager (
                                               TAO_Stub *stub
                                               );
  virtual CORBA::Boolean _is_a(
      const CORBA::Char *logical_type_id,
      CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ()
    );

  virtual CORBA::Boolean _non_existent(
      CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ()
    );

  virtual FT::ObjectGroup_ptr create_member (
        FT::ObjectGroup_ptr object_group,
        const FT::Location & the_location,
        const char * type_id,
        const FT::Criteria & the_criteria,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        FT::ObjectGroupNotFound,
        FT::MemberAlreadyPresent,
        FT::NoFactory,
        FT::ObjectNotCreated,
        FT::InvalidCriteria,
        FT::CannotMeetCriteria
      ));

  virtual FT::ObjectGroup_ptr add_member (
        FT::ObjectGroup_ptr object_group,
        const FT::Location & the_location,
        CORBA::Object_ptr member,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        FT::ObjectGroupNotFound,
        FT::MemberAlreadyPresent,
        FT::ObjectNotAdded
      ));

  virtual FT::ObjectGroup_ptr remove_member (
        FT::ObjectGroup_ptr object_group,
        const FT::Location & the_location,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        FT::ObjectGroupNotFound,
        FT::MemberNotFound
      ));

  virtual FT::ObjectGroup_ptr set_primary_member (
        FT::ObjectGroup_ptr object_group,
        const FT::Location & the_location,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        FT::ObjectGroupNotFound,
        FT::MemberNotFound,
        FT::PrimaryNotSet,
        FT::BadReplicationStyle
      ));

  virtual FT::Locations * locations_of_members (
        FT::ObjectGroup_ptr object_group,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        FT::ObjectGroupNotFound
      ));

  virtual FT::ObjectGroupId get_object_group_id (
        FT::ObjectGroup_ptr object_group,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        FT::ObjectGroupNotFound
      ));

  virtual FT::ObjectGroup_ptr get_object_group_ref (
        FT::ObjectGroup_ptr object_group,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        FT::ObjectGroupNotFound
      ));

  virtual CORBA::Object_ptr get_member_ref (
        FT::ObjectGroup_ptr object_group,
        const FT::Location & loc,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        FT::ObjectGroupNotFound,
        FT::MemberNotFound
      ));

};





class TAO_Export FT_POA_GenericFactory :  public virtual PortableServer::ServantBase
{
protected:
  FT_POA_GenericFactory (void);

public:
  FT_POA_GenericFactory (const FT_POA_GenericFactory& rhs);
  virtual ~FT_POA_GenericFactory (void);


  virtual CORBA::Boolean _is_a (
      const char* logical_type_id,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
      );

  virtual void* _downcast (
        const char* logical_type_id
      );

  static void _is_a_skel (
        CORBA::ServerRequest &req,
        void *obj,
        void *context,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      );

  static void _non_existent_skel (
        CORBA::ServerRequest &req,
        void *obj,
        void *context,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      );

  virtual void _dispatch (
        CORBA::ServerRequest &_tao_req,
        void *_tao_context,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      );

  FT::GenericFactory *_this (
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    );

  virtual const char* _interface_repository_id (void) const;

  virtual CORBA::Object_ptr create_object (
      const char * type_id,
      const FT::Criteria & the_criteria,
      FT::GenericFactory::FactoryCreationId_out factory_creation_id,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      FT::NoFactory,
      FT::ObjectNotCreated,
      FT::InvalidCriteria,
      FT::InvalidProperty,
      FT::CannotMeetCriteria
    )) = 0;

  static void create_object_skel (
        CORBA::ServerRequest &_tao_req,
        void *_tao_obj,
        void *_tao_context,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      );

  virtual void delete_object (
      const FT::GenericFactory::FactoryCreationId & factory_creation_id,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      FT::ObjectNotFound
    )) = 0;

  static void delete_object_skel (
      CORBA::ServerRequest &_tao_req,
      void *_tao_obj,
      void *_tao_context,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    );
};



class TAO_Export FT_POA_tao_thru_poa_collocated_GenericFactory:
  public virtual FT::GenericFactory
{
public:
  FT_POA_tao_thru_poa_collocated_GenericFactory (
      TAO_Stub *stub
    );
  virtual CORBA::Boolean _is_a(
      const CORBA::Char *logical_type_id,
      CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ()
    );

  virtual CORBA::Boolean _non_existent(
        CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ()
      );

  virtual CORBA::Object_ptr create_object (
        const char * type_id,
        const FT::Criteria & the_criteria,
        FT::GenericFactory::FactoryCreationId_out factory_creation_id,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        FT::NoFactory,
        FT::ObjectNotCreated,
        FT::InvalidCriteria,
        FT::InvalidProperty,
        FT::CannotMeetCriteria
      ));

  virtual void delete_object (
      const FT::GenericFactory::FactoryCreationId & factory_creation_id,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      FT::ObjectNotFound
    ));

};




class TAO_Export FT_POA_ReplicationManager :
  public virtual FT_POA_PropertyManager,
  public virtual FT_POA_ObjectGroupManager,
  public virtual FT_POA_GenericFactory
{
protected:
    FT_POA_ReplicationManager (void);

public:
  FT_POA_ReplicationManager (const FT_POA_ReplicationManager& rhs);
  virtual ~FT_POA_ReplicationManager (void);


  virtual CORBA::Boolean _is_a (
        const char* logical_type_id,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      );

  virtual void* _downcast (
        const char* logical_type_id
      );

  static void _is_a_skel (
        CORBA::ServerRequest &req,
        void *obj,
        void *context,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      );

  static void _non_existent_skel (
        CORBA::ServerRequest &req,
        void *obj,
        void *context,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      );

  virtual void _dispatch (
        CORBA::ServerRequest &_tao_req,
        void *_tao_context,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      );

    ::FT::ReplicationManager *_this (
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      );

  virtual const char* _interface_repository_id (void) const;

  virtual void register_fault_notifier (
        FT::FaultNotifier_ptr fault_notifier,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      )) = 0;

  static void register_fault_notifier_skel (
        CORBA::ServerRequest &_tao_req,
        void *_tao_obj,
        void *_tao_context,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      );

  virtual FT::FaultNotifier_ptr get_fault_notifier (
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        FT::InterfaceNotFound
      )) = 0;

  static void get_fault_notifier_skel (
        CORBA::ServerRequest &_tao_req,
        void *_tao_obj,
        void *_tao_context,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      );

  static void set_default_properties_skel (
        CORBA::ServerRequest &req,
        void *obj,
        void *context,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      );

  static void get_default_properties_skel (
        CORBA::ServerRequest &req,
        void *obj,
        void *context,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      );

  static void remove_default_properties_skel (
        CORBA::ServerRequest &req,
        void *obj,
        void *context,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      );

  static void set_type_properties_skel (
        CORBA::ServerRequest &req,
        void *obj,
        void *context,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      );

  static void get_type_properties_skel (
        CORBA::ServerRequest &req,
        void *obj,
        void *context,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      );

  static void remove_type_properties_skel (
        CORBA::ServerRequest &req,
        void *obj,
        void *context,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      );

  static void set_properties_dynamically_skel (
        CORBA::ServerRequest &req,
        void *obj,
        void *context,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      );

  static void get_properties_skel (
        CORBA::ServerRequest &req,
        void *obj,
        void *context,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      );

  static void create_member_skel (
        CORBA::ServerRequest &req,
        void *obj,
        void *context,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      );

  static void add_member_skel (
        CORBA::ServerRequest &req,
        void *obj,
        void *context,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      );

  static void remove_member_skel (
        CORBA::ServerRequest &req,
        void *obj,
        void *context,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      );

  static void set_primary_member_skel (
        CORBA::ServerRequest &req,
        void *obj,
        void *context,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      );

  static void locations_of_members_skel (
        CORBA::ServerRequest &req,
        void *obj,
        void *context,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      );

  static void get_object_group_id_skel (
        CORBA::ServerRequest &req,
        void *obj,
        void *context,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      );

  static void get_object_group_ref_skel (
        CORBA::ServerRequest &req,
        void *obj,
        void *context,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      );

  static void get_member_ref_skel (
        CORBA::ServerRequest &req,
        void *obj,
        void *context,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      );

  static void create_object_skel (
        CORBA::ServerRequest &req,
        void *obj,
        void *context,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      );

  static void delete_object_skel (
        CORBA::ServerRequest &req,
        void *obj,
        void *context,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      );


};

class TAO_Export FT_POA_tao_thru_poa_collocated_ReplicationManager :
  public virtual FT::ReplicationManager,
  public virtual FT_POA_tao_thru_poa_collocated_PropertyManager,
  public virtual FT_POA_tao_thru_poa_collocated_ObjectGroupManager,
  public virtual FT_POA_tao_thru_poa_collocated_GenericFactory
{
public:
  FT_POA_tao_thru_poa_collocated_ReplicationManager (
      TAO_Stub *stub
    );
  virtual CORBA::Boolean _is_a(
      const CORBA::Char *logical_type_id,
      CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ()
    );

  virtual CORBA::Boolean _non_existent(
      CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ()
    );

  virtual void register_fault_notifier (
      FT::FaultNotifier_ptr fault_notifier,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));

  virtual FT::FaultNotifier_ptr get_fault_notifier (
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      FT::InterfaceNotFound
    ));

};



class TAO_Export FT_POA_PullMonitorable :  public virtual PortableServer::ServantBase
{
protected:
  FT_POA_PullMonitorable (void);

public:
  FT_POA_PullMonitorable (const FT_POA_PullMonitorable& rhs);
  virtual ~FT_POA_PullMonitorable (void);


  virtual CORBA::Boolean _is_a (
      const char* logical_type_id,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    );

  virtual void* _downcast (
        const char* logical_type_id
      );

  static void _is_a_skel (
        CORBA::ServerRequest &req,
        void *obj,
        void *context,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      );

  static void _non_existent_skel (
        CORBA::ServerRequest &req,
        void *obj,
        void *context,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      );

  virtual void _dispatch (
        CORBA::ServerRequest &_tao_req,
        void *_tao_context,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      );

  FT::PullMonitorable *_this (
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      );

  virtual const char* _interface_repository_id (void) const;

  virtual CORBA::Boolean is_alive (
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      )) = 0;

  static void is_alive_skel (
      CORBA::ServerRequest &_tao_req,
      void *_tao_obj,
      void *_tao_context,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    );

};


class TAO_Export FT_POA_tao_thru_poa_collocated_PullMonitorable :
  public virtual FT::PullMonitorable
{
public:
  FT_POA_tao_thru_poa_collocated_PullMonitorable (
        TAO_Stub *stub
      );
  virtual CORBA::Boolean _is_a(
      const CORBA::Char *logical_type_id,
      CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ()
      );

  virtual CORBA::Boolean _non_existent(
      CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ()
    );

  virtual CORBA::Boolean is_alive (
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));

};




class TAO_Export FT_POA_FaultNotifier :
  public virtual PortableServer::ServantBase
{
protected:
  FT_POA_FaultNotifier (void);

public:
  FT_POA_FaultNotifier (const FT_POA_FaultNotifier& rhs);
  virtual ~FT_POA_FaultNotifier (void);


  virtual CORBA::Boolean _is_a (
      const char* logical_type_id,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    );

  virtual void* _downcast (
        const char* logical_type_id
      );

  static void _is_a_skel (
        CORBA::ServerRequest &req,
        void *obj,
        void *context,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      );

  static void _non_existent_skel (
        CORBA::ServerRequest &req,
        void *obj,
        void *context,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      );

  virtual void _dispatch (
        CORBA::ServerRequest &_tao_req,
        void *_tao_context,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      );

  FT::FaultNotifier *_this (
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    );

  virtual const char* _interface_repository_id (void) const;

  virtual void push_structured_fault (
        const CosNotification::StructuredEvent & event,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      )) = 0;

  static void push_structured_fault_skel (
        CORBA::ServerRequest &_tao_req,
        void *_tao_obj,
        void *_tao_context,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      );

  virtual void push_sequence_fault (
        const CosNotification::EventBatch & events,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      )) = 0;

  static void push_sequence_fault_skel (
        CORBA::ServerRequest &_tao_req,
        void *_tao_obj,
        void *_tao_context,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      );

  virtual CosNotifyFilter::Filter_ptr create_subscription_filter (
        const char * constraint_grammar,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        CosNotifyFilter::InvalidGrammar
      )) = 0;

  static void create_subscription_filter_skel (
        CORBA::ServerRequest &_tao_req,
        void *_tao_obj,
        void *_tao_context,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      );

  virtual FT::FaultNotifier::ConsumerId connect_structured_fault_consumer (
        CosNotifyComm::StructuredPushConsumer_ptr push_consumer,
        CosNotifyFilter::Filter_ptr filter,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      )) = 0;

  static void connect_structured_fault_consumer_skel (
        CORBA::ServerRequest &_tao_req,
        void *_tao_obj,
        void *_tao_context,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      );

  virtual FT::FaultNotifier::ConsumerId connect_sequence_fault_consumer (
        CosNotifyComm::SequencePushConsumer_ptr push_consumer,
        CosNotifyFilter::Filter_ptr filter,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      )) = 0;

  static void connect_sequence_fault_consumer_skel (
        CORBA::ServerRequest &_tao_req,
        void *_tao_obj,
        void *_tao_context,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      );

  virtual void disconnect_consumer (
      FT::FaultNotifier::ConsumerId connection,
      CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      CosEventComm::Disconnected
      )) = 0;

  static void disconnect_consumer_skel (
      CORBA::ServerRequest &_tao_req,
      void *_tao_obj,
      void *_tao_context,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
      );
};


class TAO_Export FT_POA_tao_thru_poa_collocated_FaultNotifier :
  public virtual FT::FaultNotifier
{
public:
  FT_POA_tao_thru_poa_collocated_FaultNotifier (
      TAO_Stub *stub
    );
  virtual CORBA::Boolean _is_a(
      const CORBA::Char *logical_type_id,
      CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ()
    );

  virtual CORBA::Boolean _non_existent(
      CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ()
    );

  virtual void push_structured_fault (
      const CosNotification::StructuredEvent & event,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));

  virtual void push_sequence_fault (
      const CosNotification::EventBatch & events,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));

  virtual CosNotifyFilter::Filter_ptr create_subscription_filter (
      const char * constraint_grammar,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      CosNotifyFilter::InvalidGrammar
    ));

  virtual FT::FaultNotifier::ConsumerId connect_structured_fault_consumer (
      CosNotifyComm::StructuredPushConsumer_ptr push_consumer,
      CosNotifyFilter::Filter_ptr filter,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));

  virtual FT::FaultNotifier::ConsumerId connect_sequence_fault_consumer (
      CosNotifyComm::SequencePushConsumer_ptr push_consumer,
      CosNotifyFilter::Filter_ptr filter,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));

  virtual void disconnect_consumer (
      FT::FaultNotifier::ConsumerId connection,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      CosEventComm::Disconnected
      ));
};




class TAO_Export FT_POA_Checkpointable :
  public virtual PortableServer::ServantBase
{
protected:
  FT_POA_Checkpointable (void);

public:
  FT_POA_Checkpointable (const FT_POA_Checkpointable& rhs);
  virtual ~FT_POA_Checkpointable (void);


  virtual CORBA::Boolean _is_a (
        const char* logical_type_id,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      );

  virtual void* _downcast (
        const char* logical_type_id
      );

  static void _is_a_skel (
        CORBA::ServerRequest &req,
        void *obj,
        void *context,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      );

  static void _non_existent_skel (
        CORBA::ServerRequest &req,
        void *obj,
        void *context,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      );

  virtual void _dispatch (
        CORBA::ServerRequest &_tao_req,
        void *_tao_context,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      );

  FT::Checkpointable *_this (
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
      );

  virtual const char* _interface_repository_id (void) const;

  virtual FT::State * get_state (
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        FT::NoStateAvailable
      )) = 0;

  static void get_state_skel (
        CORBA::ServerRequest &_tao_req,
        void *_tao_obj,
        void *_tao_context,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      );

  virtual void set_state (
        const FT::State & s,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        FT::InvalidState
      )) = 0;

  static void set_state_skel (
        CORBA::ServerRequest &_tao_req,
        void *_tao_obj,
        void *_tao_context,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      );


};


class TAO_Export FT_POA_tao_thru_poa_collocated_Checkpointable :
  public virtual FT::Checkpointable
{
public:
  FT_POA_tao_thru_poa_collocated_Checkpointable (
        TAO_Stub *stub
      );
  virtual CORBA::Boolean _is_a(
        const CORBA::Char *logical_type_id,
        CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ()
      );

  virtual CORBA::Boolean _non_existent(
        CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ()
      );

  virtual FT::State * get_state (
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        FT::NoStateAvailable
      ));

  virtual void set_state (
        const FT::State & s,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        FT::InvalidState
      ));

};



class TAO_Export FT_POA_Updateable :
  public virtual FT_POA_Checkpointable
{
protected:
  FT_POA_Updateable (void);

public:
  FT_POA_Updateable (const FT_POA_Updateable& rhs);
  virtual ~FT_POA_Updateable (void);


  virtual CORBA::Boolean _is_a (
        const char* logical_type_id,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      );

  virtual void* _downcast (
        const char* logical_type_id
      );

  static void _is_a_skel (
        CORBA::ServerRequest &req,
        void *obj,
        void *context,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      );

  static void _non_existent_skel (
        CORBA::ServerRequest &req,
        void *obj,
        void *context,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      );

  virtual void _dispatch (
        CORBA::ServerRequest &_tao_req,
        void *_tao_context,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      );

  FT::Updateable *_this (
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      );

  virtual const char* _interface_repository_id (void) const;

  virtual FT::State * get_update (
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        FT::NoUpdateAvailable
      )) = 0;

  static void get_update_skel (
        CORBA::ServerRequest &_tao_req,
        void *_tao_obj,
        void *_tao_context,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      );

  virtual void set_update (
        const FT::State & s,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        FT::InvalidUpdate
      )) = 0;

  static void set_update_skel (
        CORBA::ServerRequest &_tao_req,
        void *_tao_obj,
        void *_tao_context,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      );

  static void get_state_skel (
        CORBA::ServerRequest &req,
        void *obj,
        void *context,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      );

  static void set_state_skel (
        CORBA::ServerRequest &req,
        void *obj,
        void *context,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      );
};


class TAO_Export FT_POA_tao_thru_poa_collocated_Updateable     :
  public virtual FT::Updateable,
  public virtual FT_POA_tao_thru_poa_collocated_Checkpointable
{
public:
  FT_POA_tao_thru_poa_collocated_Updateable (
        TAO_Stub *stub
      );
  virtual CORBA::Boolean _is_a(
        const CORBA::Char *logical_type_id,
        CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ()
      );

  virtual CORBA::Boolean _non_existent(
        CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ()
      );

  virtual FT::State * get_update (
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        FT::NoUpdateAvailable
      ));

  virtual void set_update (
        const FT::State & s,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        FT::InvalidUpdate
      ));
};



#if defined (__ACE_INLINE__)
#include "FT_CORBA_ServiceS.i"
#endif /* defined INLINE */

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /*TAO_HAS_FT_CORBA == 1 */

#endif /*TAO_FT_CORBA_SERVICES_H */
