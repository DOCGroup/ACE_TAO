// $Id$

#ifndef NAMESERVICEREDIRECTION_H_
#define NAMESERVICEREDIRECTION_H_

#include "ace/SString.h"
#include "ace/Vector_T.h"
#include "ace/Map_Manager.h"
#include "ace/Null_Mutex.h"
#include "orbsvcs/orbsvcs/CosNamingC.h"

#include "RedirectionService_Export.h"

namespace DAnCE
  {

  class RedirectionService_Export NameServiceRedirection
    {
    public:
      NameServiceRedirection (CosNaming::NamingContext_ptr hosting_naming,
                              CosNaming::NamingContext_ptr domain_naming);

      ~NameServiceRedirection();

      void start_binding (const ACE_CString& node, const ACE_CString& plan);
      ///  Registration for port object
      void bind (const ACE_CString& node, const ACE_CString& plan, const ACE_CString& component, const ACE_CString& port, CORBA::Object_ptr obj);
      ///  Registration for component object
      void bind (const ACE_CString& node, const ACE_CString& plan, const ACE_CString& component, CORBA::Object_ptr obj);

      void finish_binding (const ACE_CString& node, const ACE_CString& plan);
      // Removes all records for specified plan
      void unbind_context (const ACE_CString& node, const ACE_CString& plan);

      void add_node (const ACE_CString& node);

    protected:
      CosNaming::NamingContext_ptr resolve_context (const ACE_CString& name, const ACE_CString& kind, CosNaming::NamingContext_ptr naming);

      void clear_context (CosNaming::NamingContext_ptr& naming);

    private:
      CosNaming::NamingContext_var naming_;
      CosNaming::NamingContext_var domain_;

      struct SRecord
        {
          CosNaming::Name name;
          CORBA::Object_var obj;
        };

      typedef ACE_Vector<SRecord> TRecords;
      typedef ACE_Map_Manager<ACE_CString, TRecords*, ACE_Null_Mutex> TTransactions;
      TTransactions transactions_;
    };
}
#endif /*NAMESERVICEREDIRECTION_H_*/
