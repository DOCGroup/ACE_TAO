// -*- C++ -*-
//
// $Id$


ACE_RCSID (LoadBalancer_test, LoadBalancer_test.cpp "$Id$")

int
main (int argc, char *argv[])
{
  // @@ activate Replica servant
  // @@ Obtain reference to object
  MyReplica_var replica = replica_servant._this ();

  // @@ Is _interface_repository_id() portable?
  const char * type_id = replica->_interface_repository_id ();

  // Infrastructure-Controlled MembershipStyle

  // Obtain Reference to the TAO LoadBalancer ReplicationManager
  CORBA::Object_var obj = resolve_initial_references ("TAO_LoadBalancer");
  TAO_LoadBalancer_var lb = TAO_LoadBalancer::_narrow (obj);
  //@@ check if <lb> is nil

  // Set the default properties
  TAO_LoadBalancer::Properties props;
  // @@ actually set them  (InitialNumberReplicas,
  //    MinimumNumberReplicas, etc)

  lb->set_default_properties (props);

  // Set the properties specific to my Object type
  TAO_LoadBalancer::Properties overrides;
  // @@ actually set them  (InitialNumberReplicas,
  //    MinimumNumberReplicas, etc)

  lb->set_type_properties (type_id,
                           overrides);








  // The factory ID
  TAO_LoadBalancer::FactoryCreationID factory_creation_id;

  // Create a replicated object (object group)
  obj = lb->create_object (type_id,
                           the_criteria,
                           factory_creation_id);
  MyReplica_var replica_group = MyReplica::_narrow (obj);
  // @@ check if <replica_group> is nil

  replica_group->some_method_available_on_replica ();
  

  // Application-Controlled MembershipStyle


  return 0;
}
