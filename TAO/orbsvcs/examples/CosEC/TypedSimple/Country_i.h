// -*- C++ -*-
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
  virtual ~Country_i ();

  virtual void update_population (const char * country,
                                  CORBA::Long population);

  virtual CORBA::Object_ptr get_typed_consumer ();

  virtual void push (const CORBA::Any & data);

  virtual void disconnect_push_consumer ();

private:
  /// The ORB
  CORBA::ORB_var orb_;
};

#endif /* COUNTRYI_H_  */
