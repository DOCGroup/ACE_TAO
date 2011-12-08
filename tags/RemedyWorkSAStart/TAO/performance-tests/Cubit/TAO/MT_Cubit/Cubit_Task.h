/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Cubit_Task.h
 *
 *  $Id$
 *
 *  @author Andy Gokhale
 *  @author Sumedh Mungee
 *  @author Sergio Flores-Gaitan and Nagarajan Surendran.
 */
//=============================================================================


#ifndef MT_CUBIT_TASK_H
#define MT_CUBIT_TASK_H

#include "ace/Task.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/ARGV.h"
#include "tao/Utils/ORB_Manager.h"
#include "cubit_i.h"
#include "Globals.h"

/**
 * @class Cubit_Task
 *
 * @brief Encapsulates an ORB for the Cubit application.
 */
class Cubit_Task : public ACE_Task<ACE_SYNCH>
{
public:
  /// Constructor.
  Cubit_Task (const ACE_TCHAR *args,
              const char* orbname,
              u_int num_of_objs,
              ACE_Thread_Manager *thr_mgr,
              u_int task_id);

  /// Active Object entry point.
  virtual int svc (void);

  /// Gets the ior of the cubit servant in this task.
  char* get_servant_ior (u_int index);

protected:
  /// No-op constructor.
  Cubit_Task (void);

private:
  /// Initialize the ORB, and POA.
  int initialize_orb (void);

  /// Create the servants.
  int create_servants (void);

  /// All cubit objects will have this as prefix to its key.
  const char *key_;

  /// Name of the ORB.
  const char *orbname_;

  /// ORB arguments.
  const ACE_TCHAR *orbargs_;

  /// Number of objects we're managing.
  u_int num_of_objs_;

  /// Pointer to the ORB
  CORBA::ORB_var orb_;

  /// Array to hold the servants.
  Cubit_i **servants_;

  /// IOR strings of the servants.
  char **servants_iors_;

  /// ID used for naming service object name.
  u_int task_id_;

  /// The TAO ORB Manager.
  TAO_ORB_Manager orb_manager_;
};

#endif /* MT_CUBIT_TASK_H */
