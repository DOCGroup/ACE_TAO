#ifndef CHILDSERVANT_H
#define CHILDSERVANT_H

#include "ChildS.h"

class ChildServant
  : public virtual POA_Child
{
 public:
   explicit ChildServant (CORBA::ORB_ptr orb);
   ChildServant (ChildServant const &) = delete;
   ChildServant &operator= (ChildServant const &) = delete;
   ChildServant (ChildServant &&) = delete;
   ChildServant &operator= (ChildServant &&) = delete;

   void parentMethod () override;
   void childMethod () override;
   void shutdown () override;

 private:
   CORBA::ORB_var orb_;
};

#endif
