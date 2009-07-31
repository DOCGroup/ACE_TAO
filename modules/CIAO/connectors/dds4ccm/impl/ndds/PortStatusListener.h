/**
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */
 
#ifndef DDS4CCM_PORTSTATUSLISTENERI_H_
#define DDS4CCM_PORTSTATUSLISTENERI_H_

#include "dds4ccm_PortStatusListenerS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class  CCM_DDS_PortStatusListener_i
  : public virtual POA_CCM_DDS::PortStatusListener
{
public:
  // Constructor 
  CCM_DDS_PortStatusListener_i (void);
  
  // Destructor 
  virtual ~CCM_DDS_PortStatusListener_i (void);
  
  virtual
  void on_requested_deadline_missed (
    ::DDS::DataReader_ptr the_reader,
    const ::DDS::RequestedDeadlineMissedStatus & status);
  
  virtual
  void on_sample_lost (
    ::DDS::DataReader_ptr the_reader,
    const ::DDS::SampleLostStatus & status);
};


#endif /* DDS4CCM_PORTSTATUSLISTENERI_H_  */

