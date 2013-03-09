
/**
 * @author Marcel Smit (msmit@remedy.nl)
 *
 * $Id$
 *
 * Configuration file for DDS4CCM.
 */

#ifndef DDS4CCM_CONF_H_
#define DDS4CCM_CONF_H_

/**
 * CIAO_DDS4CCM_CONTEXT_SWITCH defines whether DDS4CCM
 * does use the ORB core reactor thread when a DDS callback
 * has been received.
 *
 * If this is defined to 1 DDS4CCM will pass the DDS callback to the
 * ORB Core Reactor thread which means a thread context switch
 *
 * If this is defined to 0 no context switch takes place and the DDS
 * callback is delivered to the user code on the DDS thread
 *
 * If the user doesn't explicitly choose a value we will default to 1
 */
#if !defined (CIAO_DDS4CCM_CONTEXT_SWITCH)
#  define CIAO_DDS4CCM_CONTEXT_SWITCH 1
#endif /* CIAO_DDS4CCM_CONTEXT_SWITCH */

#if (CIAO_DDS4CCM_CONTEXT_SWITCH == 1) && !defined (ACE_HAS_REACTOR_NOTIFICATION_QUEUE)
#  error For using the DDS4CCM context switch ACE_HAS_REACTOR_NOTIFICATION_QUEUE must be defined
#endif

#if (CIAO_DDS4CCM_NDDS==1)
# include "dds4ccm/impl/Utils.h"
# define DDS_INSTANCE_HANDLE_T_IN const ::DDS::InstanceHandle_t &
# define DDS_INSTANCE_HANDLE_T_RETN ::DDS::InstanceHandle_t
# define DDS_INSTANCE_HANDLE_FORMAT_SPECIFIER ACE_TEXT ("<%x%x%x%x%x%x%x%x%x%x%x%x%x%x%x%x:%d:%d>")
# define DDS_INSTANCE_HANDLE_LOG(X) X.value[0], X.value[1], X.value[2], X.value[3], X.value[4], X.value[5], X.value[6], X.value[7], X.value[8], X.value[9], X.value[10], X.value[11], X.value[12], X.value[13], X.value[14], X.value[15], X.length, X.isValid
# define DDS_ENTITY_FORMAT_SPECIFIER ACE_TEXT ("<%x%x%x%x%x%x%x%x%x%x%x%x%x%x%x%x:%d:%d>")
# define DDS_ENTITY_LOG(X) X->get_instance_handle ().value[0], X->get_instance_handle ().value[1], X->get_instance_handle ().value[2], X->get_instance_handle ().value[3], X->get_instance_handle ().value[4], X->get_instance_handle ().value[5], X->get_instance_handle ().value[6], X->get_instance_handle ().value[7], X->get_instance_handle ().value[8], X->get_instance_handle ().value[9], X->get_instance_handle ().value[10], X->get_instance_handle ().value[11], X->get_instance_handle ().value[12], X->get_instance_handle ().value[13], X->get_instance_handle ().value[14], X->get_instance_handle ().value[15], X->get_instance_handle ().length, X->get_instance_handle ().isValid
# define DDS_XML_QOS_PARSER_TYPE CIAO::DDS4CCM::QOS_XML_Noop
#elif (CIAO_DDS4CCM_OPENDDS==1)
# define DDS_INSTANCE_HANDLE_T_IN ::DDS::InstanceHandle_t
# define DDS_INSTANCE_HANDLE_T_RETN ::DDS::InstanceHandle_t
# define DDS_INSTANCE_HANDLE_FORMAT_SPECIFIER ACE_TEXT ("<%d>")
# define DDS_INSTANCE_HANDLE_LOG(X) X
# define DDS_ENTITY_FORMAT_SPECIFIER ACE_TEXT ("<%d:%@>")
# define DDS_ENTITY_LOG(X) X->get_instance_handle (), X
# define DDS_XML_QOS_PARSER_TYPE OpenDDS::DCPS::QOS_XML_Loader
#endif
#endif /* DDS4CCM_CONF_H_ */
