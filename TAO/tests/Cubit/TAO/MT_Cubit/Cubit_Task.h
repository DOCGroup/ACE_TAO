/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests
//
// = FILENAME
//    Cubit_Task.h
//
// = AUTHOR
//    Andy Gokhale, Sumedh Mungee,Sergio Flores-Gaitan and Nagarajan Surendran.
//
// ============================================================================

#if !defined (MT_CUBIT_TASK_H)
#define MT_CUBIT_TASK_H

#include "ace/Task.h"
#include "ace/ARGV.h"
#include "tao/TAO.h"
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/Naming/Naming_Utils.h"
#include "cubit_i.h"
#include "Globals.h"

class Cubit_Task : public ACE_Task<ACE_MT_SYNCH>
{
  // = TITLE
  //    Encapsulates an ORB for the Cubit application.
public:
  Cubit_Task (const char *args,
              const char* orbname,
              u_int num_of_objs,
	      ACE_Thread_Manager *thr_mgr,
	      u_int task_id);
  // Constructor.

  virtual int svc (void);
  // Active Object entry point.

  CORBA::String get_servant_ior (u_int index);
  // Gets the ior of the cubit servant in this task.

protected:
  Cubit_Task (void);
  // No-op constructor.

private:
  int initialize_orb (void);
  // Initialize the ORB, and POA.

  int create_servants (void);
  // Create the servants.

  CORBA::String key_;
  // All cubit objects will have this as prefix to its key.

  char *orbname_;
  // Name of the ORB.

  char *orbargs_;
  // ORB arguments.

  u_int num_of_objs_;
  // Number of objects we're managing.

  CORBA::ORB_var orb_;
  // Pointer to the ORB

  Cubit_i **servants_;
  // Array to hold the servants.

  CORBA::String *servants_iors_;
  // IOR strings of the servants.

  //CosNaming::NamingContext_var naming_context_;
  // Object reference to the naming service.

  u_int task_id_;
  // ID used for naming service object name.

  CosNaming::NamingContext_var mt_cubit_context_;
  // Context where all MT Cubit objects will be created.

  TAO_ORB_Manager orb_manager_;
  // The TAO ORB Manager.

  TAO_Naming_Client my_name_client_;
  // An instance of the name client used for resolving the factory
  // objects.
};

#endif /* MT_CUBIT_TASK_H */
