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

#ifndef TAO_NS_SUPPLIER_CLIENT_H
#define TAO_NS_SUPPLIER_CLIENT_H
#include "ace/pre.h"

#include "ORB_Objects.h"
#include "ace/Task.h"
#include "tao/corba.h"

class TAO_NS_Supplier;

/**
 * @class TAO_NS_Supplier_Client
 *
 * @brief Supplier Client
 *
 */
class TAO_NS_Supplier_Client : public ACE_Task_Base
{
public:
  /// Constuctor
  TAO_NS_Supplier_Client (TAO_NS_ORB_Objects& orb_objects);

  /// Destructor
  ~TAO_NS_Supplier_Client ();

  /// Init
  void init (ACE_ENV_SINGLE_ARG_DECL);

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
  TAO_NS_ORB_Objects orb_objects_;

  /// Supplier that sends events.
  TAO_NS_Supplier* supplier_;

  /// The Number of consumers that we expect to send an event to.
  int consumer_count_;

  /// Name of the file to write the supplier ior to.
  ACE_CString ior_file_name_;
};

#include "ace/post.h"
#endif /* TAO_NS_SUPPLIER_CLIENT_H */
