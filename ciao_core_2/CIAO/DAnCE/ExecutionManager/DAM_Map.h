/*=======================================================================
 *
 * @file DAM_Map.h
 *
 * $Id$
 *
 * @brief Map of DomainApplicationManager to UUID's
 *
 * @author Bala Natarajan <bala @ dre.vanderbilt.edu>
 *
 *======================================================================*/
#ifndef CIAO_DAM_MAP_H
#define CIAO_DAM_MAP_H
#include /**/ "ace/pre.h"

#include "DAnCE/Deployment/Deployment_DomainApplicationManagerC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Null_Mutex.h"
#include "ace/Hash_Map_Manager.h"
#include "ace/SString.h"

namespace CIAO
{
  namespace Execution_Manager
  {
    /**
     * @class DAM_Map
     *
     * @brief Map of DomainApplicationManager to ACE_CStrings
     *
     */
    class DAM_Map
    {
    public:
      DAM_Map (void);

      bool is_plan_available (const ACE_CString &str) const;

      ::Deployment::DomainApplicationManager_ptr
      fetch_dam_reference (const ACE_CString &str);

      bool bind_dam_reference (
        const ACE_CString &str,
        ::Deployment::DomainApplicationManager_ptr tmp);

      Deployment::DomainApplicationManagers *get_dams ();

      bool unbind_dam (const ACE_CString &str);

      size_t size (void) const;

    private:
      typedef
      ACE_Hash_Map_Manager_Ex < ACE_CString,
                                ::Deployment::DomainApplicationManager_var,
                                ACE_Hash<ACE_CString>,
                                ACE_Equal_To<ACE_CString>,
                                ACE_Null_Mutex> MAP;
      typedef MAP::iterator Iterator;

      MAP map_;
    };
  }
}

#include /**/ "ace/post.h"
#endif /*CIAO_DAM_MAP_H*/
