/* -*- C++ -*- */
/**
 *  @file Supplier_Client.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_Notify_SUPPLIER_CLIENT_H
#define TAO_Notify_SUPPLIER_CLIENT_H

#include /**/ "ace/pre.h"
#include "ace/Task.h"
#include "ace/SString.h"

#include "ORB_Objects.h"

class TAO_Notify_Lanes_Supplier;

/**
 * @class TAO_Notify_Lanes_Supplier_Client
 *
 * @brief Supplier Client
 *
 */
class TAO_Notify_Lanes_Supplier_Client : public ACE_Task_Base
{
public:
  /// Constuctor
  TAO_Notify_Lanes_Supplier_Client (TAO_Notify_ORB_Objects& orb_objects);

  /// Destructor
  ~TAO_Notify_Lanes_Supplier_Client ();

  /// Init
  void initialize (ACE_ENV_SINGLE_ARG_DECL);

  /// Run
  void run (ACE_ENV_SINGLE_ARG_DECL);

  /// Parse Args
  int parse_args (int argc, char *argv[]);

  /// The thread entry point.
  virtual int svc (void);

protected:
  /// Create an EC
  CosNotifyChannelAdmin::EventChannel_ptr create_ec (ACE_ENV_SINGLE_ARG_DECL);

  /// Write ior to file.
  void write_ior (ACE_ENV_SINGLE_ARG_DECL);

  /// ORB Objects.
  TAO_Notify_ORB_Objects orb_objects_;

  /// Supplier that sends events.
  TAO_Notify_Lanes_Supplier* supplier_;

  /// The Number of consumers that we expect to send an event to.
  int consumer_count_;

  /// Name of the file to write the supplier ior to.
  ACE_CString ior_file_name_;
};

#include /**/ "ace/post.h"
#endif /* TAO_Notify_SUPPLIER_CLIENT_H */
