// -*- C++ -*-
//=============================================================================
/**
 *  @file   FtRtEvent_Test.h
 *
 *  $Id$
 *
 *  @author Huang-Ming Huang <hh1@cse.wustl.edu>
 */
//=============================================================================

#ifndef FTRTEVENT_TEST_H
#define FTRTEVENT_TEST_H

#include "orbsvcs/FtRtecEventChannelAdminC.h"

namespace TAO_FTRTEC {
  class FTEC_Gateway;
}

class FtRtEvent_Test_Base
{
public:
  FtRtEvent_Test_Base();
  virtual ~FtRtEvent_Test_Base();

  int run(int argc, ACE_TCHAR** argv);
protected:
  CORBA::ORB_var orb_;
  FtRtecEventChannelAdmin::EventChannel_var channel_;
  TAO_FTRTEC::FTEC_Gateway* gateway_;
  ACE_Time_Value timer_interval_;
  int num_iterations_;
  int use_gateway_;
private:
  int parse_args(int argc, ACE_TCHAR** argv 
                 ACE_ENV_ARG_DECL);
  RtecEventChannelAdmin::EventChannel_ptr
    get_event_channel(ACE_ENV_SINGLE_ARG_DECL);

  virtual int init(ACE_ENV_SINGLE_ARG_DECL)=0;
};

template <class T>
class FtRtEvent_Test : public FtRtEvent_Test_Base
{
  virtual int init();
  T client_;
};

template <class T>
int FtRtEvent_Test<T>::init(ACE_ENV_SINGLE_ARG_DECL)
{
  return client_.init(orb_, num_iterations_, channel_, timer_interval_ 
                      ACE_ENV_ARG_PARAMETER);
}

#endif
