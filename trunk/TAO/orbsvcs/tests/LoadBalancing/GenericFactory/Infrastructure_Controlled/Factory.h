// -*- C++ -*-
//
// $Id$

#ifndef FACTORY_H
#define FACTORY_H

#include "Factory_Map.h"
#include "orbsvcs/orbsvcs/PortableGroupS.h"

#if defined (_MSC_VER)
# if (_MSC_VER >= 1200)
#  pragma warning(push)
# endif /* _MSC_VER >= 1200 */
#endif /* _MSC_VER */

/// Implement the PortableGroup::GenericFactory Interface.
class Factory
  : public virtual POA_PortableGroup::GenericFactory
{
public:

  /// Constructor
  Factory (void);
  /// creates a Test::Simple servant reference.
  /// This reference is then passed as a FactoryInfos property
  /// when the LoadManager creates the object group.
  /// In this way, LoadManager can decide when to create the object group
  /// members.

  virtual CORBA::Object_ptr create_object (
      const char * type_id,
      const PortableGroup::Criteria & the_criteria,
      PortableGroup::GenericFactory::FactoryCreationId_out
        factory_creation_id
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableGroup::NoFactory,
                     PortableGroup::ObjectNotCreated,
                     PortableGroup::InvalidCriteria,
                     PortableGroup::InvalidProperty,
                     PortableGroup::CannotMeetCriteria));

  /// The LoadManager is passed the FactoryCreationId, it received
  /// from the create_object () call.
  /// The LoadManager destroys the factory object created.

  virtual void delete_object (
      const PortableGroup::GenericFactory::FactoryCreationId &
        factory_creation_id
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableGroup::ObjectNotFound));
private:
  /// The factory creation id.
  CORBA::ULong fcid_;

  /// Table that maps FactoryCreationId to Factory_Node
  Factory_Map factory_map_;

  /// The object id of the servant we are creating.
  PortableServer::ObjectId_var oid_;

  /// The POA with which we register the servant.
  PortableServer::POA_var poa_;

  /// Lock used to synchronize access to the factory creation id
  TAO_SYNCH_MUTEX lock_;

};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma warning(pop)
#endif /* _MSC_VER */

#endif /* FACTORY_H */
