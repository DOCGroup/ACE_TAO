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
  explicit Country_i (CORBA::ORB_ptr orb);

  //Destructor
  ~Country_i () override = default;

  void update_population (char const *country,
                          CORBA::Long population) override;

  CORBA::Object_ptr get_typed_consumer () override;

  void push (CORBA::Any const &data) override;

  void disconnect_push_consumer () override;

  Country_i (Country_i const &) = delete;
  Country_i (Country_i &&) = delete;
  Country_i &operator= (Country_i const &) = delete;
  Country_i &operator= (Country_i &&) = delete;

private:
  /// The ORB
  CORBA::ORB_var orb_;
};

#endif /* COUNTRYI_H_  */
