/**
 * @author Stephen Torri
 * $Id$
 */
#include "ECConfig_Gateway.h"
#include "ace/OS_NS_stdio.h"

ECConfig_Gateway::ECConfig_Gateway (void) {}
ECConfig_Gateway::~ECConfig_Gateway (void) {}

void ECConfig_Gateway::push (const RtecEventComm::EventSet &events ACE_ENV_ARG_DECL_WITH_DEFAULTS)
{
  ACE_OS::printf ("ECConfig_Gateway::push - %p\n", &events);
  TAO_EC_Gateway_Sched::push(events);
}

void ECConfig_Gateway::update_consumer (const RtecEventChannelAdmin::ConsumerQOS& sub
                               ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_OS::printf ("ECConfig_Gateway::update_consumer - %p\n", &sub);
  ACE_OS::printf ("   Dependencies:\n");
  for (unsigned int i = 0; i < sub.dependencies.length(); ++i) {
    ACE_OS::printf("     Dep #%d\n",i);
    // RtecEventComm::Event event
    ACE_OS::printf("     - Event:\n");
    //   RtecEventComm::EventHeader header
    //       RtecEventComm::EventType type; (Ulong)
    //       RtecEventComm::EventSourceID source; (Long)
    //       CORBA::Long ttl;
    ACE_OS::printf("       type=%d  source_id=%d  ttl=%d\n",
           sub.dependencies[i].event.header.type,
           sub.dependencies[i].event.header.source,
           sub.dependencies[i].event.header.ttl);
    //       RtecEventComm::Time creation_time;(Ulong)
    //       RtecEventComm::Time ec_recv_time; (Ulong)
    //       RtecEventComm::Time ec_send_time; (Ulong)
    ACE_OS::printf("       creation_time=%llu\n       ec_recv_time=%llu\n       ec_send_time=%llu\n",
           sub.dependencies[i].event.header.creation_time,
           sub.dependencies[i].event.header.ec_recv_time,
           sub.dependencies[i].event.header.ec_send_time);
    //   //RtecEventComm::EventData data
    // RtecBase::handle_t rt_info
    ACE_OS::printf("     - RT_Info: %d", sub.dependencies[i].rt_info);
  }
  ACE_OS::printf ("   Is this a gateway:");
  if (sub.is_gateway) {
    ACE_OS::printf (" true");
  }
  else {
    ACE_OS::printf (" false");
  }
  ACE_OS::printf("\n");

  TAO_EC_Gateway_Sched::update_consumer(sub);
}

void ECConfig_Gateway::update_supplier (const RtecEventChannelAdmin::SupplierQOS& pub
                                ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_OS::printf ("ECConfig_Gateway::update_supplier - %p\n", &pub);
  // SupplierQOS
  //   RtecEventChannelAdmin::PublicationSet publications;
  ACE_OS::printf ("   Publications:\n");
  for (unsigned int i = 0; i < pub.publications.length(); ++i) {
    ACE_OS::printf("     Pub #%d\n",i);
    //  Publication
    //    RtecEventComm::Event event;
    ACE_OS::printf("     - Event:\n");
    //      RtecEventComm::EventHeader header
    //      RtecEventComm::EventType type; (Ulong)
    //      RtecEventComm::EventSourceID source; (Long)
    //      CORBA::Long ttl;
    ACE_OS::printf("       type=%d  source_id=%d  ttl=%d\n",
           pub.publications[i].event.header.type,
           pub.publications[i].event.header.source,
           pub.publications[i].event.header.ttl);
    //      RtecEventComm::Time creation_time;(Ulong)
    //      RtecEventComm::Time ec_recv_time; (Ulong)
    //      RtecEventComm::Time ec_send_time; (Ulong)
    ACE_OS::printf("       creation_time=%llu\n       ec_recv_time=%llu\n       ec_send_time=%llu\n",
           pub.publications[i].event.header.creation_time,
           pub.publications[i].event.header.ec_recv_time,
           pub.publications[i].event.header.ec_send_time);

    //   RtecBase::Dependency_Info dependency_info;
    ACE_OS::printf("     - Dependency_Info\n");
    //     RtecBase::Dependency_Type_t dependency_type;
    ACE_OS::printf("       type: %d\n", pub.publications[i].dependency_info.dependency_type);
    //     CORBA::Long number_of_calls;
    ACE_OS::printf("       number of calls: %d\n", pub.publications[i].dependency_info.number_of_calls);
    //     RtecBase::handle_t rt_info;
    ACE_OS::printf("       rt_info: %d\n", pub.publications[i].dependency_info.rt_info);
    //     RtecBase::handle_t rt_info_depended_on;
    ACE_OS::printf("       rt_info dependend on: %d\n", pub.publications[i].dependency_info.rt_info_depended_on);
    //     RtecBase::Dependency_Enabled_Type_t enabled;
    ACE_OS::printf("       dependency enabled type: %d\n", pub.publications[i].dependency_info.enabled);
  }


  TAO_EC_Gateway_Sched::update_supplier(pub);
}
