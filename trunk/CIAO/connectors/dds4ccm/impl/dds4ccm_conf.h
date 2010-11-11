
/**
 * @author Marcel Smit <msmit@remedy.nl>
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

enum DDS4CCM_Vendor
{
  DDS4CCM_NDDS,
  DDS4CCM_OPENDDS
};

#define DDS_INSTANCE_HANDLE_T_IN const ::DDS::InstanceHandle_t &
#define DDS_INSTANCE_HANDLE_T_RETN ::DDS::InstanceHandle_t

#endif /* DDS4CCM_CONF_H_ */
