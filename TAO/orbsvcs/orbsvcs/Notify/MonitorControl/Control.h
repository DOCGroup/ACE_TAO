// $Id$

#ifndef CONTROL_H
#define CONTROL_H

#include /**/ "ace/pre.h"

#include "ace/SString.h"

#include "tao/Versioned_Namespace.h"

#include "orbsvcs/orbsvcs/Notify/MonitorControl/notify_mc_export.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

#define TAO_NS_CONTROL_SHUTDOWN "shutdown"
#define TAO_NS_CONTROL_REMOVE_CONSUMER "remove_consumer"
#define TAO_NS_CONTROL_REMOVE_SUPPLIER "remove_supplier"
#define TAO_NS_CONTROL_REMOVE_CONSUMERADMIN "remove_consumeradmin"
#define TAO_NS_CONTROL_REMOVE_SUPPLIERADMIN "remove_supplieradmin"

class TAO_Notify_MC_Export TAO_NS_Control
{
public:
  /// This is here due to virtual functions.
  virtual ~TAO_NS_Control (void);

  /// Return the name of this control object.
  const ACE_CString& name (void) const;

  /// Execute the control object.  Only return false if the command
  /// specified is not supported.
  virtual bool execute (const char* command) = 0;

protected:
  /// Construct a control object.
  TAO_NS_Control (const char* name);

private:
  ACE_CString name_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* CONTROL_H */
