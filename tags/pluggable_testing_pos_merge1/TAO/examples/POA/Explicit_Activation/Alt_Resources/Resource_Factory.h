// $Id$

#include "tao/corba.h"
#include "Alt_Resource_Factory.h"

class Alt_Resource_Factory_Export My_Resource_Factory :  public TAO_Resource_Factory
{
public:
  virtual ACE_Reactor *get_reactor (void);
  // Return an <ACE_Reactor> to be utilized.

private:
  struct My_Reactors
  {
    My_Reactors (void);
    ACE_WFMO_Reactor wfmo_reactor_;
    ACE_Reactor reactor_;
  };

  typedef ACE_Singleton<My_Reactors, ACE_SYNCH_MUTEX> GLOBAL_REACTORS;
  typedef ACE_TSS_Singleton<My_Reactors, ACE_SYNCH_MUTEX> TSS_REACTORS;
};

ACE_FACTORY_DECLARE (Alt_Resource_Factory, My_Resource_Factory)

