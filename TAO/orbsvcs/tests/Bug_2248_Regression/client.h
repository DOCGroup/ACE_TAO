#ifndef BUG_2248_REGRESSION_CLIENT_H
#define BUG_2248_REGRESSION_CLIENT_H

#include "tao/ORB.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Object.h"
#include "tao/IORManipulation/IORManipulation.h"

struct Client_Options
{
  ACE_TCHAR const *first_ior {};
  ACE_TCHAR const *first_key {};
  ACE_TCHAR const *second_ior {};
  ACE_TCHAR const *second_key {};
  ACE_TCHAR const *first_iogr_file {};
  ACE_TCHAR const *second_iogr_file {};
  int sleep_delay {15};
};

class Manager
{
public:
  Manager () = default;

  void init (int& argc,
             ACE_TCHAR *argv[]);

  int make_merged_iors (Client_Options const &options);

  int write_to_files (Client_Options const &options);

  CORBA::ORB_ptr orb ();

private:
  int write_to_file (CORBA::Object_ptr ior, ACE_TCHAR const *ior_output_file);

  int set_properties (TAO_IOP::TAO_IOR_Manipulation_ptr iorm,
                      CORBA::Object_ptr merged_set,
                      CORBA::Object_ptr primary, CORBA::ULong version);

  CORBA::ORB_var orb_;

  CORBA::Object_var merged_set_10_;
  CORBA::Object_var merged_set_11_;
};

#endif /* BUG_2248_REGRESSION_CLIENT_H */
