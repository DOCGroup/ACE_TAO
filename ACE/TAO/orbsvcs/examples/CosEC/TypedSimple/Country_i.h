// -*- C++ -*-
//
// $Id$

#ifndef COUNTRYI_H_
#define COUNTRYI_H_

#include "CountryS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

//Class Country_i
class Country_i : public virtual POA_Country
{
public:
  //Constructor 
  Country_i (CORBA::ORB_ptr orb);
  
  //Destructor 
  virtual ~Country_i (void);
  
  virtual void update_population (const char * country,
                                  CORBA::Long population
                                  ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::Object_ptr get_typed_consumer (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void push (const CORBA::Any & data
                     ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     CosEventComm::Disconnected));

  virtual void disconnect_push_consumer (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  /// The ORB
  CORBA::ORB_var orb_;
};

#endif /* COUNTRYI_H_  */
