// -*- C++ -*-
#ifndef FACTORY_H
#define FACTORY_H

#include "Factory_Map.h"
#include "orbsvcs/PortableGroupS.h"
#include "ace/Null_Mutex.h"

#if defined (_MSC_VER)
# pragma warning(push)
#endif /* _MSC_VER */

/// Implement the PortableGroup::GenericFactory Interface.
class Factory
  : public virtual POA_PortableGroup::GenericFactory
{
public:
  explicit Factory (int id, int pong_delay = 0);

  Factory (Factory const &) = delete;
  Factory &operator= (Factory const &) = delete;
  Factory (Factory &&) = delete;
  Factory &operator= (Factory &&) = delete;
  /// creates a Test::Simple servant reference.
  /// This reference is then passed as a FactoryInfos property
  /// when the LoadManager creates the object group.
  /// In this way, LoadManager can decide when to create the object group
  /// members.

  CORBA::Object_ptr create_object (
      char const* type_id,
      PortableGroup::Criteria const& the_criteria,
      PortableGroup::GenericFactory::FactoryCreationId_out
        factory_creation_id) override;

  /// The LoadManager is passed the FactoryCreationId, it received
  /// from the create_object () call.
  /// The LoadManager destroys the factory object created.

  void delete_object (
      PortableGroup::GenericFactory::FactoryCreationId const&
        factory_creation_id) override;

private:
  /// The factory creation id.
  CORBA::ULong fcid_ {};

  /// Tracks the FactoryCreationIds managed by this factory.
  Factory_Map factory_map_;

  /// The object id of the servant we are creating.
  PortableServer::ObjectId_var oid_;

  /// The POA with which we register the servant.
  PortableServer::POA_var poa_;

  TAO_SYNCH_MUTEX lock_;

  int const id_;
  int const pong_delay_;

  CORBA::Object_var obj_;
};

#if defined(_MSC_VER)
# pragma warning(pop)
#endif /* _MSC_VER */

#endif /* FACTORY_H */
