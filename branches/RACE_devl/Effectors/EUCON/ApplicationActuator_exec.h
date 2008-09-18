// $Id$
#ifndef CIAO_APPLICATIONACTUATOR_EXEC_H
#define CIAO_APPLICATIONACTUATOR_EXEC_H

#include /**/ "ace/pre.h"

#include "EUCON_Effector_svnt.h"
#include "Logger.h"
#include "orbsvcs/CosNamingC.h"
#include "ace/Hash_Map_Manager.h"
#include "ace/SString.h"
#include "Effectors/Callback/CallbackC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "EUCON_Effector_exec_export.h"
#include "tao/LocalObject.h"

namespace CIAO
{
  namespace RACE
  {
    namespace Effector
    {
      namespace CIDL_EUCON_Effector_Impl
      {
        class EUCON_EFFECTOR_EXEC_Export ApplicationActuator_exec_i
          : public virtual ::CIAO::RACE::Effector::CCM_ApplicationActuator,
            public virtual TAO_Local_RefCounted_Object
        {
          public:
          ApplicationActuator_exec_i
            (EUCON_Effector_Context *context,
             Logger &logger);

          virtual ~ApplicationActuator_exec_i (void);


          virtual ::CORBA::Boolean
            init (const ::CIAO::RACE::OperationalStrings & opstrings);


          virtual ::CORBA::Boolean
          modifyApplications (
            const ::CIAO::RACE::OperationalStrings & opstrings);

          protected:

          //Helper functions.

          virtual bool
            resolve_naming_service ();

          virtual bool
            populate_callback_map
            (const ::CIAO::RACE::OperationalStrings & opstrings);

          private:

          Logger &logger_;

          EUCON_Effector_Context *context_;

          CosNaming::NamingContext_var naming_context_;

          typedef
          ACE_Hash_Map_Manager_Ex <ACE_CString,
            Callback_var,
            ACE_Hash<ACE_CString>,
            ACE_Equal_To<ACE_CString>,
            ACE_Null_Mutex> MAP;

          MAP callback_map_;
        };

      }
    }
  }
}

#include /**/ "ace/post.h"

#endif /* CIAO_APPLICATIONACTUATOR_EXEC_H */
