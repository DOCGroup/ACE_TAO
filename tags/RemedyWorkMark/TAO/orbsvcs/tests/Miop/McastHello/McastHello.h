//
// $Id$
//

#ifndef MCASTHELLO_H
#define MCASTHELLO_H
#include /**/ "ace/pre.h"

#include "TestS.h"

/// Implement the Test::McastHello interface
class McastHello
  : public virtual POA_Test::McastHello
{
public:
  /// Constructor
  McastHello (CORBA::ORB_ptr orb,
              int instance);

  // = The skeleton methods
  virtual void send_forty_two (CORBA::Long forty_two);

  virtual void shutdown (void);

  /// Return whether all test requests have been received successfully.
  CORBA::Boolean get_status ();

  // Silence the builds
  void send_large_octet_array (const Test::Octets &);

private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;

  /// The instance number. (Useful when debugging multiple servants)
  int instance_;

  /// Track the success/failure of the small request.
  CORBA::Boolean small_request_status_;

  /// Track the success/failure of the large request.
  CORBA::Boolean large_request_status_;
};

#include /**/ "ace/post.h"
#endif /* MCASTHELLO_H */
