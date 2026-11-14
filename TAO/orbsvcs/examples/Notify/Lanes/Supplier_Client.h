/* -*- C++ -*- */
/**
 *  @file Supplier_Client.h
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
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
 */
class TAO_Notify_Lanes_Supplier_Client : public ACE_Task_Base
{
public:
  explicit TAO_Notify_Lanes_Supplier_Client (TAO_Notify_ORB_Objects& orb_objects);
  ~TAO_Notify_Lanes_Supplier_Client () override = default;
  TAO_Notify_Lanes_Supplier_Client (TAO_Notify_Lanes_Supplier_Client const&) = delete;
  TAO_Notify_Lanes_Supplier_Client& operator= (TAO_Notify_Lanes_Supplier_Client const&) = delete;
  TAO_Notify_Lanes_Supplier_Client (TAO_Notify_Lanes_Supplier_Client&&) = delete;
  TAO_Notify_Lanes_Supplier_Client& operator= (TAO_Notify_Lanes_Supplier_Client&&) = delete;

  /// Init
  void initialize ();

  /// Run
  void run ();

  /// Parse Args
  int parse_args (int argc, ACE_TCHAR *argv[]);

  /// The thread entry point.
  int svc () override;

protected:
  /// Create an EC
  CosNotifyChannelAdmin::EventChannel_ptr create_ec () const;

  /// Write ior to file.
  void write_ior () const;

  /// ORB Objects.
  TAO_Notify_ORB_Objects& orb_objects_;

  /// Supplier that sends events.
  // The POA retains the activated servant; this pointer is non-owning.
  TAO_Notify_Lanes_Supplier* supplier_ {};

  struct Options
  {
    int consumer_count {2};
    ACE_TString ior_file_name;
  };

  Options options_;
};

#include /**/ "ace/post.h"
#endif /* TAO_Notify_SUPPLIER_CLIENT_H */
