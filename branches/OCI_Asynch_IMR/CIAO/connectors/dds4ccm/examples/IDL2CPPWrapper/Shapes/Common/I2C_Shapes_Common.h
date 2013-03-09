// $Id$

#ifndef I2C_SHAPES_COMMON_H_
#define I2C_SHAPES_COMMON_H_

#include "I2C_Shapes_Common_export.h"
#include "connectors/dds4ccm/impl/ndds/DomainParticipant.h"
#include "connectors/dds4ccm/impl/ndds/DomainParticipantFactory.h"
#include "connectors/dds4ccm/impl/ndds/TypeSupport.h"
#include "Base/I2C_Shapes_Traits.h"

/**
 * Pulse generator
 */

#define QOS_PROFILE "Shapes_Library#ShapesProfile"

class I2C_SHAPES_Export I2C_Shapes_Common
{
public:
  I2C_Shapes_Common ();
  ~I2C_Shapes_Common ();

  //@{
  /** Public methods. */
  ::DDS::Publisher_ptr create_publisher (void);
  ::DDS::Subscriber_ptr create_subscriber (void);
  //@}

  //@{
  /** Getters. */
  ::DDS::Topic_ptr get_topic (void);
  //@}

private:
  typedef ::CIAO::NDDS::DDS_TypeFactory_T <ShapeType_DDS_Traits,
                                           ShapeTypeSeq> dds_type_factory;

  //@{
  /** Members. */
  ::CIAO::NDDS::DDS_DomainParticipantFactory_i participant_factory_;
  ::DDS::DomainParticipant_var participant_;
  ::CIAO::NDDS::DDS_TypeFactory_i * factory_;
  ::DDS::Topic_var topic_;
  //@}

  //@{
  /** Private methods. */
  int get_domain_id (void);
  void initialize (void);
  //@}
};

#endif /* I2C_SHAPES_COMMON_H_ */
