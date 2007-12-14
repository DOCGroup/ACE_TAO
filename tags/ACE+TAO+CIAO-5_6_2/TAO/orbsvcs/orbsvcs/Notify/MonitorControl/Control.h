// $Id$
#ifndef CONTROL_H
#define CONTROL_H

#include /**/ "ace/pre.h"
#include "orbsvcs/Notify/MonitorControl/notify_mc_export.h"
#include "orbsvcs/Notify/MonitorControl/Generic.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

#define TAO_NS_CONTROL_SHUTDOWN "shutdown"

class TAO_Notify_MC_Export TAO_NS_Control: public TAO_NS_Generic
{
public:
  /// This is here due to virtual functions.
  virtual ~TAO_NS_Control (void);

  /// Execute the control object
  virtual void execute (const char* command) = 0;

protected:
  /// Construct a control object.
  TAO_NS_Control (const char* name);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* CONTROL_H */
