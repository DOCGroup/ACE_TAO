// $Id$

#include "entity_factory_qos_test.h"

EntityFactoryPolicyTest::EntityFactoryPolicyTest ()
{
}

bool
EntityFactoryPolicyTest::check (const ::DDS_EntityFactoryQosPolicy & dds_qos,
                                const ::DDS::EntityFactoryQosPolicy & qos)
{
  return dds_qos.autoenable_created_entities == qos.autoenable_created_entities;
}
