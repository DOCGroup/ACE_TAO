/**
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef CIAO_RTI_ENTITYFACTORYQOSPOLICY_H
#define CIAO_RTI_ENTITYFACTORYQOSPOLICY_H

inline void
operator<<= (::DDS::EntityFactoryQosPolicy &ddsqos, const ::DDS_EntityFactoryQosPolicy & qos)
{
  ddsqos.autoenable_created_entities = qos.autoenable_created_entities;
}

inline void
operator<<= (::DDS_EntityFactoryQosPolicy &ddsqos, const ::DDS::EntityFactoryQosPolicy & qos)
{
  ddsqos.autoenable_created_entities = qos.autoenable_created_entities;
}

inline void
operator>>= (const ::DDS_EntityFactoryQosPolicy &qos, ::DDS::EntityFactoryQosPolicy & ddsqos)
{
  ddsqos.autoenable_created_entities = qos.autoenable_created_entities;
}

inline void
operator>>= (const ::DDS::EntityFactoryQosPolicy &qos, ::DDS_EntityFactoryQosPolicy & ddsqos)
{
  ddsqos.autoenable_created_entities = qos.autoenable_created_entities;
}

#endif /* CIAO_RTI_ENTITYFACTORYQOSPOLICY_H */
