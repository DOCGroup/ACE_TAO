//$Id$
#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "tao/corbafwd.h"
#include "tao/ORB.h"
#include "tao/Object.h"


class Manager
{
public:

  Manager (void);

  // Ctor

  int init (int argc,
            char *argv[],
            CORBA::Environment & ACE_TRY_ENV);

  // Initialize the ORB, POA etc.

  int make_merged_iors (CORBA::Environment &);
  // Merges the different IORS

  int set_properties (CORBA::Environment &);
  // Sets the properties for the profiles

  int run (CORBA::Environment &ACE_TRY_ENV);
  // Run the  ORB event loop..

  int write_to_file (void);
  // Write the merged IOR to a file
private:
  CORBA::ORB_var orb_;
  // Our ORB

  CORBA::Object_var merged_set_;
  // The merged IOR set
};
#endif /*_MANAGER_H_ */
