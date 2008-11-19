//
// $Id$
//

#ifndef TRIGGER_H
#define TRIGGER_H
#include /**/ "ace/pre.h"

#include <string>
#include "TriggerS.h"

class Trigger
  : public virtual POA_Test::Trigger
{
public:
  Trigger (CORBA::ORB_ptr orb,
	   const std::string & server_ior);

  virtual void start (void);

  virtual void shutdown (void);

private:
  CORBA::ORB_var orb_;
  std::string server_ior_;
};

#include /**/ "ace/post.h"
#endif /* TRIGGER_H */
