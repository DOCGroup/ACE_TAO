/* -*- C++ -*- */
// $Id$

#include <iostream.h>
#include "../../ImplRepo_Service/Repository.h"

// Just tests the functionality of the Repository

int main (int argc, char *argv[])
{
  Repository rep;
  Repository::Record new_rec, rec;

  new_rec.comm_line = "cl value";
  new_rec.env = "env value";
  new_rec.host = 100;
  new_rec.port = 1;
  new_rec.ping_ior = "ping_ior value";
  new_rec.wdir = "wdir value";

  // Add our one record
  int result = rep.add ("simple", new_rec);
  cout << "Registration: " << result << endl;

  if (result == 1) // We need to get rid of it
  {
    rep.remove ("simple");
    result = rep.add ("simple", new_rec);
    cout << "2nd Registration: " << result << endl;
  }

  // Bring it back!
  if (rep.resolve ("simple", rec) == 0)
  {
    cout << "Command line: " << rec.comm_line << endl;
    cout << "Environment: " << rec.env << endl;
    cout << "Working directory: " << rec.wdir << endl;
    cout << "Host and Port: " << rec.host << ":" << rec.port << endl;
    cout << "Ping IOR: " << rec.ping_ior << endl;

    delete [] rec.comm_line;
    delete [] rec.env;
    delete [] rec.wdir;
    delete [] rec.ping_ior;
  }

  // Try some of the single accessors

  char *cl, *env, *ping_ior;

  if (rep.get_comm_line ("simple", cl) == 0
      && rep.get_env ("simple", env) == 0
      && rep.get_ping_ior ("simple", ping_ior) == 0)
  {
    cout << "Command line: " << cl << endl;
    cout << "Environment: " << env << endl;
    cout << "Ping IOR: " << ping_ior << endl;
  
    delete [] cl;
    delete [] env;
    delete [] ping_ior;
  }

  // So what happens when we try to reregister?
  cout << "Re-Registration: " << rep.add ("simple", new_rec) << endl;

  //rep.remove ("simple");
  return 0;
}
