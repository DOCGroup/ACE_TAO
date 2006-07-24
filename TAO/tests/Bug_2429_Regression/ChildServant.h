#ifndef CHILDSERVANT_H
#define CHILDSERVANT_H

// $Id$

#include "ChildS.h"

class ChildServant
  : public virtual POA_Child
{
 public:
   ChildServant (CORBA::ORB_ptr orb);
   virtual void parentMethod ()
        ACE_THROW_SPEC ((CORBA::SystemException));
   virtual void childMethod ()
        ACE_THROW_SPEC ((CORBA::SystemException));
   virtual void shutdown ()
        ACE_THROW_SPEC ((CORBA::SystemException));

 private:
   CORBA::ORB_var orb_;
};

#endif
