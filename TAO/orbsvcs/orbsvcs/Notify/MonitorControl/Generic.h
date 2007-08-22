// $Id$
#ifndef GENERIC_H
#define GENERIC_H

#include /**/ "ace/pre.h"
#include "orbsvcs/Notify/MonitorControl/notify_mc_export.h"

#include "tao/Versioned_Namespace.h"
#include "ace/SString.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Notify_MC_Export TAO_NS_Generic
{
public:
  /// Ensure that the correct destructor is called by making it virtual
  virtual ~TAO_NS_Generic (void);

  /// Return the name of this control object
  const ACE_CString& name (void) const;

protected:
  /// Construct a control object.
  TAO_NS_Generic (const char* name);

private:
  ACE_CString name_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* GENERIC_H */
