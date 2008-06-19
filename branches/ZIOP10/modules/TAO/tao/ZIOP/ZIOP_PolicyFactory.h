+// -*- C++ -*-
+//
+// ===================================================================
+/**
+ *  @file ZIOP_PolicyFactory.h
+ *
+ *  $Id$
+ *
+ *  @author Johnny Willemsen <jwillemsen@remedy.nl>
+ */
+// ===================================================================
+#ifndef TAO_ZIOP_POLICY_FACTORY_H
+#define TAO_ZIOP_POLICY_FACTORY_H
+
+#include /**/ "ace/pre.h"
+
+#include "tao/ZIOP/ziop_export.h"
+
+#if !defined (ACE_LACKS_PRAGMA_ONCE)
+# pragma once
+#endif /* ACE_LACKS_PRAGMA_ONCE */
+
+
+#include "tao/PI/PI.h"
+#include "tao/LocalObject.h"
+
+// This is to remove "inherits via dominance" warnings from MSVC.
+// MSVC is being a little too paranoid.
+#if defined(_MSC_VER)
+# pragma warning(push)
+# pragma warning(disable:4250)
+#endif /* _MSC_VER */
+
+TAO_BEGIN_VERSIONED_NAMESPACE_DECL
+
+/// Policy factory for all Messaging related policies.
+class TAO_ZIOP_PolicyFactory
+  : public PortableInterceptor::PolicyFactory
+  , public TAO_Local_RefCounted_Object
+{
+public:
+
+  virtual CORBA::Policy_ptr create_policy (CORBA::PolicyType type,
+                                           const CORBA::Any & value
+                                           ACE_ENV_ARG_DECL_WITH_DEFAULTS)
+    ACE_THROW_SPEC ((CORBA::SystemException,
+                     CORBA::PolicyError));
+
+};
+
+TAO_END_VERSIONED_NAMESPACE_DECL
+
+#if defined(_MSC_VER)
+# pragma warning(pop)
+#endif /* _MSC_VER */
+
+
+#include /**/ "ace/post.h"
+
+#endif /* TAO_ZIOP_POLICY_FACTORY_H */

