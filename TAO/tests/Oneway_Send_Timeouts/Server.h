// $Id$

#ifndef _SERVER_
#define _SERVER_

#include "Test_i.h"

#include "ORB_Task.h"

#include "tao/IORTable/IORTable.h"
#include "tao/PortableServer/POAC.h"
#include "tao/PortableServer/POAManagerC.h"

#include <string>

class Server
{
 public:
  Server (int argc, ACE_TCHAR* argv[]);
  ~Server ();

  bool run (bool management);
  void shutdown ();

  bool init_;

 private:
  bool parse_args (int argc, ACE_TCHAR* argv[]);

  CORBA::ORB_var orb_;
  CORBA::ORB_var management_orb_;
  bool shutdown_;
  TAO_SYNCH_MUTEX mutex_;
  std::auto_ptr<Test_i> test_i_;
  bool orb_shutdown_;
  bool management_orb_shutdown_;
};

#endif //_SERVER_
