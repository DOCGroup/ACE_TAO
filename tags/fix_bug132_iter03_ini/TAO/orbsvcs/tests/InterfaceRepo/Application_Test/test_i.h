// -*- C++ -*-
// $Id$

#include "testS.h"

class inventory_i : public POA_warehouse::inventory
{
public:
  CORBA::Boolean getCDinfo (const char * artist,
                            char *& title,
                            CORBA::Float_out price,
                            CORBA::Environment &ACE_TRY_ENV = 
                              TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));
};

