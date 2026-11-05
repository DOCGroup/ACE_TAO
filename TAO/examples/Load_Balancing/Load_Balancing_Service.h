// -*- C++ -*-

//=============================================================================
/**
 *  @file    Load_Balancing_Service.h
 *
 *  Driver for Load Balancing service.
 *
 *  @author Marina Spivak <marina@cs.wustl.edu>
 */
//=============================================================================

#ifndef LOAD_BALANCING_SERVICE_H_
#define LOAD_BALANCING_SERVICE_H_

#include "tao/Utils/ORB_Manager.h"

#include <cstdio>
#include <memory>

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class Load_Balancing_Service
 *
 * Server, which creates and initializes a
 * <Load_Balancer::Object_Group_Factory>
 * object, and runs the orb loop.
 */
class Load_Balancing_Service
{
public:
  /// Default constructor.
  Load_Balancing_Service () = default;

  /// Destructor.
  ~Load_Balancing_Service () = default;

  Load_Balancing_Service (Load_Balancing_Service const &) = delete;
  Load_Balancing_Service (Load_Balancing_Service &&) = delete;
  Load_Balancing_Service &operator= (Load_Balancing_Service const &) = delete;
  Load_Balancing_Service &operator= (Load_Balancing_Service &&) = delete;

  /// Initialize the <Load_Balancing_Service>: initializes the ORB, parses
  /// arguments, creates a servant ...
  int init (int argc, ACE_TCHAR *argv[]);

  /// Run the server.
  int run ();

private:
  /// Parses the commandline arguments.
  int parse_args (int argc, ACE_TCHAR *argv[]);

  /// The ORB manager.
  TAO_ORB_Manager orb_manager_;

  /// File to output the <Object_Group_Factory> IOR.
  using File_Ptr = std::unique_ptr<FILE, int (*) (FILE *)>;
  File_Ptr ior_output_file_ {nullptr, &std::fclose};
};

#endif /* LOAD_BALANCING_SERVICE_H_ */
