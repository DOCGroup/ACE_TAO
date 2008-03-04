#ifndef CCMOBJECTLOCATOR_H_
#define CCMOBJECTLOCATOR_H_

#include "tao/corba.h"
#include "tao/PortableServer/PortableServer.h"
#include "tao/PortableServer/ServantLocatorC.h"
#include "ace/Map_Manager.h"
#include "ace/Null_Mutex.h"
#include "ace/SStringfwd.h"
#include "ace/String_Base.h"

#include "RedirectionService_Export.h"

namespace DAnCE
  {

  class RedirectionService_Export CCMObjectLocator
        : public PortableServer::ServantLocator
        , public CORBA::LocalObject
    {
    public:
      class TreeNode;
      typedef ACE_Map_Manager<ACE_CString, TreeNode*, ACE_Null_Mutex> TNodes;
      class TreeNode
        {
        public:
          static const char delimiter_ = '/';
          TreeNode() {};
          TreeNode (const TreeNode & src);
          ~TreeNode();
          CORBA::Object_ptr getValue() const
            {
              return CORBA::Object::_duplicate (this->obj_.in());
            };
          CORBA::Object_ptr getValue (const char * path) const;
          void setValue (CORBA::Object_ptr obj)
          {
            this->obj_ = CORBA::Object::_duplicate (obj);
          };
          void addChild (const char * path, CORBA::Object_ptr obj = CORBA::Object::_nil());
          void addChild (const char * path, TreeNode & node);
          void removeChild (const char * path);
          int missingNodeLevel (const char * path);
          static ACE_CString mergePath (const char * parent, const char * child);
        private:
          CORBA::Object_var obj_;
          TNodes children_;
          TreeNode * getChild (const char * path) const;
          static ACE_CString splitPath (ACE_CString & path, bool first = true);
        };

      CCMObjectLocator (CORBA::ORB_ptr orb, PortableServer::POA_ptr parent_poa, const char * poa_name);


      ~CCMObjectLocator();

      virtual ::PortableServer::Servant preinvoke (
        const ::PortableServer::ObjectId & oid,
        ::PortableServer::POA_ptr adapter,
        const char * operation,
        ::PortableServer::ServantLocator::Cookie & the_cookie
      )
      ACE_THROW_SPEC ( (
                         CORBA::SystemException,
                         ::PortableServer::ForwardRequest
                       ));

      virtual void postinvoke (
        const ::PortableServer::ObjectId & /*oid*/,
        ::PortableServer::POA_ptr /*adapter*/,
        const char * /*operation*/,
        ::PortableServer::ServantLocator::Cookie /*the_cookie*/,
        ::PortableServer::Servant /*the_servant*/
      )
      ACE_THROW_SPEC ( (
                         CORBA::SystemException
                       )) {};

      void start_register (const ACE_CString& plan);

      CORBA::Object_ptr register_object (const ACE_CString & plan
                                         , const ACE_CString & inst
                                         , const ACE_CString & port
                                         , CORBA::Object_ptr obj);

      CORBA::Object_ptr register_object (const ACE_CString & plan
                                         , const ACE_CString & inst
                                         , CORBA::Object_ptr obj);

      void unregister_object (const ACE_CString & plan
                              , const ACE_CString & inst
                              , const ACE_CString & port);

      void finish_register (const ACE_CString& plan);

      void register_objects (const char * name, TreeNode & node);

    private:
      TreeNode db_;
      CORBA::ORB_var orb_;
      PortableServer::POA_var myPOA_;

      TNodes transactions_;
      CCMObjectLocator() {};
// to ban the usage
    }; // CCMObjectLocator

} //DAnCE

#endif /*CCMOBJECTLOCATOR_H_*/
