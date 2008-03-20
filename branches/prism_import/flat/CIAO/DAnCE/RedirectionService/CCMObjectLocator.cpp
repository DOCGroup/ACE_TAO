
#include "CCMObjectLocator.h"
#include "ace/String_Base.h"
#include "tao/IORTable/IORTable.h"
#include "DAnCE/Logger/Log_Macros.h"

namespace DAnCE
  {

  CCMObjectLocator::CCMObjectLocator (CORBA::ORB_ptr orb, PortableServer::POA_ptr parent_poa, const char * poa_name)
      : orb_ (CORBA::ORB::_duplicate (orb))
  {
    DANCE_DEBUG ( (LM_DEBUG, "[%M] CCMObjectLocator::CCMObjectLocator - started\n"));
    CORBA::PolicyList policies (4);
    policies.length (4);
    policies[0] = parent_poa->create_id_assignment_policy (PortableServer::USER_ID);
    policies[1] = parent_poa->create_request_processing_policy (PortableServer::USE_SERVANT_MANAGER);
    policies[2] = parent_poa->create_servant_retention_policy (PortableServer::NON_RETAIN);
    policies[3] = parent_poa->create_lifespan_policy (PortableServer::PERSISTENT);
    PortableServer::POAManager_var mgr = parent_poa->the_POAManager ();
    DANCE_DEBUG ( (LM_DEBUG, "[%M] CCMObjectLocator::CCMObjectLocator - before create_POA\n"));
    this->myPOA_ = parent_poa->create_POA (poa_name
                                           , mgr.in()
                                           , policies);
    DANCE_DEBUG ( (LM_DEBUG, "[%M] CCMObjectLocator::CCMObjectLocator - after create_POA\n"));
    for (size_t i = 0; i < policies.length(); ++i)
      {
        policies[i]->destroy();
      }

    DANCE_DEBUG ( (LM_DEBUG, "[%M] CCMObjectLocator::CCMObjectLocator - before set_servant_manager\n"));
    this->myPOA_->set_servant_manager (this);
    DANCE_DEBUG ( (LM_DEBUG, "[%M] CCMObjectLocator::CCMObjectLocator - CCMObjectLocator started on POA \"%s\"\n"
                 ,  poa_name));
  }

  CCMObjectLocator::~CCMObjectLocator()
  {}

  ::PortableServer::Servant
  CCMObjectLocator::preinvoke (
    const ::PortableServer::ObjectId & oid,
    ::PortableServer::POA_ptr ,
    const char * ,
    ::PortableServer::ServantLocator::Cookie &
  )
  {
    CORBA::String_var s = PortableServer::ObjectId_to_string (oid);
    ACE_CString path = s.in();
    CORBA::Object_var res = this->db_.getValue (path.c_str());
    if (!CORBA::is_nil (res))
      {
        throw ::PortableServer::ForwardRequest (res._retn());
      }
    else
      {
        int level = this->db_.missingNodeLevel (path.c_str());
        if (0 > level        // all levels (app(1), inst(2), port(3) exist but object is nil yet
            || 1 == level)  // application is not registered
          {
            throw CORBA::TRANSIENT();
          }
        else
          {
            throw CORBA::OBJECT_NOT_EXIST();
          }
      }
  }

  void
  CCMObjectLocator::start_register (const ACE_CString& plan)
  {
    TreeNode* tree = 0;
    if (0 != this->transactions_.find (plan, tree))
      {
        tree = new TreeNode();
      }
    else
      {
        DANCE_ERROR ( (LM_ERROR, "[%M] CCMObjectLocator::start_register - transaction for plan \"%s\" alreday started!\n"
                     , plan.c_str()));
        ///TODO Deside correcet processing of this error - probably finish_register should be called
        delete tree;
        tree = new TreeNode();
      }
    this->transactions_.rebind (plan, tree);
  }


  CORBA::Object_ptr
  CCMObjectLocator::register_object (const ACE_CString & plan
                                     , const ACE_CString & inst
                                     , const ACE_CString & port
                                     , CORBA::Object_ptr obj)
  {
    TreeNode* transaction = 0;
    ACE_CString s;
    s = plan + TreeNode::delimiter_ + inst;
    if (0 < port.length())
      {
        s += TreeNode::delimiter_ + port;
      }

    if (0 == this->transactions_.find (plan, transaction))
      {
        ACE_CString path = inst;
        if (0 < port.length())
          {
            path += TreeNode::delimiter_ + port;
          }
        transaction->addChild (path.c_str(), obj);
      }
    else
      {
        DANCE_DEBUG ( (LM_DEBUG, "[%M] CCMObjectLocator::register_object - transaction record for \"%s\" doesn't exist.\n"
                     , plan.c_str()));

        DANCE_DEBUG ( (LM_DEBUG, "[%M] CCMObjectLocator::register_object - registering \"%s\"\n"
                     , s.c_str()));
        this->db_.addChild (s.c_str(), obj);
      }

    PortableServer::ObjectId_var oid = PortableServer::string_to_ObjectId (s.c_str());
    DANCE_DEBUG ( (LM_DEBUG, "[%M] CCMObjectLocator::register_object obj id : \"%s\"\n", PortableServer::ObjectId_to_string (oid.in())));
    CORBA::Object_var o = this->myPOA_->create_reference_with_id (oid.in(), "IDL:omg.org/CORBA/Object:1.0");
    DANCE_DEBUG ( (LM_DEBUG, "[%M] CCMObjectLocator::register_object url : \"%s\"\n", this->orb_->object_to_string (o)));

    //--------------- Temporal workaround
//    CORBA::Object_var table_object =
//        this->orb_->resolve_initial_references ("IORTable" );
//
//    IORTable::Table_var adapter =
//        IORTable::Table::_narrow (table_object.in () );
//
//    adapter->bind(s.c_str(), this->orb_->object_to_string(o));
    //---------------
    return o._retn();
  }

  CORBA::Object_ptr
  CCMObjectLocator::register_object (const ACE_CString & app
                                     , const ACE_CString & inst
                                     , CORBA::Object_ptr obj)
  {
    return this->register_object (app, inst, "", obj);
  }

  void
  CCMObjectLocator::register_objects (const char * name, TreeNode & node)
  {
    this->db_.addChild (name, node);
  }

  void
  CCMObjectLocator::finish_register (const ACE_CString& plan)
  {
    TreeNode* tree = 0;
    if (0 != this->transactions_.find (plan, tree))
      {
        DANCE_DEBUG ( (LM_DEBUG, "[%M] CCMObjectLocator::finish_register - can't find \"%s\" record for register\n", plan.c_str()));
        return;
      }
    this->register_objects (plan.c_str(), *tree);
    this->transactions_.unbind (plan);
    delete tree;

  }

  void
  CCMObjectLocator::unregister_object (const ACE_CString & app
                                       , const ACE_CString & inst
                                       , const ACE_CString & port)
  {
    ACE_CString s = app + TreeNode::delimiter_ + inst;
    if (0 < inst.length())
      {
        s += TreeNode::delimiter_ + inst;
        if (0 < port.length())
          {
            s += TreeNode::delimiter_ + port;
          }
      }
    this->db_.removeChild (s.c_str());
    DANCE_DEBUG ( (LM_DEBUG, "[%M] CCMObjectLocator::unregister_object - unregistering \"%s\"\n"
                 , s.c_str()));
  }

  ACE_CString
  CCMObjectLocator::TreeNode::mergePath (const char * parent, const char * child)
  {
    return ACE_CString (parent) + delimiter_ + child;
  }

  ACE_CString
  CCMObjectLocator::TreeNode::splitPath (ACE_CString & path, bool first)
  {
    size_t pos = first
                  ? path.find (CCMObjectLocator::TreeNode::delimiter_)
                  : path.rfind (CCMObjectLocator::TreeNode::delimiter_);
    /*    if ( 0 == pos )
        {
            path = path.substring(1); Cuts off a leading delimiter
        }*/
    ACE_CString res;
    if (ACE_CString::npos == pos)
      {
        res = path;
        path = "";
      }
    else
      {
        res = path.substring (0, pos);
        path = path.substring (pos + 1);
      }
    return res;
  }

  CCMObjectLocator::TreeNode::TreeNode (const TreeNode & src)
      : obj_ (src.obj_)
  {
    TNodes::const_iterator it (src.children_);
    for (;!it.done(); ++it)
      {
        this->children_.bind ( (*it).ext_id_, new TreeNode (* (*it).int_id_));
      }
  }

  CCMObjectLocator::TreeNode::~TreeNode()
  {
    for (TNodes::iterator it = this->children_.begin(); !it.done(); ++it)
      {
        TreeNode * p = (*it).int_id_;
        (*it).int_id_ = 0;
        delete p;
      }
    this->children_.unbind_all();
  }

  CORBA::Object_ptr
  CCMObjectLocator::TreeNode::getValue (const char * path) const
    {
      const TreeNode * p = this->getChild (path);
      if (0 != p)
        {
          return p->getValue();
        }
      else
        {
          return CORBA::Object::_nil();
        }
    }

  CCMObjectLocator::TreeNode *
  CCMObjectLocator::TreeNode::getChild (const char * path) const
    {
      ACE_CString path_loc = path;
      ACE_CString s = splitPath (path_loc);

      TreeNode * res = 0;
      if (0 == this->children_.find (s, res))
        {
          return 0 == path_loc.length() ? res : res->getChild (path_loc.c_str());
        }
      else
        {
          DANCE_DEBUG ( (LM_DEBUG, "[%M] CCMObjectLocator::TreeNode::getChild - Node \"%s\" not found.\n"
                       , s.c_str()));
          return 0;
        }
    }

  void
  CCMObjectLocator::TreeNode::addChild (const char * path, CORBA::Object_ptr obj)
  {
    ACE_CString path_loc = path;
    ACE_CString child = splitPath (path_loc);

    TreeNode * p = 0;
//    DANCE_DEBUG((LM_DEBUG, "[%M] CCMObjectLocator::TreeNode::addChild looking for %C in map with %i.\n"
//            , child.c_str()
//            , this->children_.current_size()));

    if (0 != this->children_.find (child, p))
      {
        DANCE_DEBUG ( (LM_DEBUG, "[%M] CCMObjectLocator::TreeNode::addChild - create new Node.\n"));
        p = new TreeNode;
        this->children_.bind (child, p);
      }
//    DANCE_DEBUG((LM_DEBUG, "[%M] CCMObjectLocator::TreeNode::addChild - Check for path."));
    if (0 == path_loc.length())   // i.e. empty
      {
        p->setValue (obj);
      }
    else
      {
        p->addChild (path_loc.c_str(), obj);
      }
  }

  void
  CCMObjectLocator::TreeNode::removeChild (const char * path)
  {
    ACE_CString path_loc = path;
    ACE_CString child = splitPath (path_loc);

    TreeNode * p = 0;
    if (0 != this->children_.find (child, p))
      {
        DANCE_ERROR ( (LM_ERROR, "[%M] CCMObjectLocator::TreeNode::removeChild failed. Node \"%s\" is missing.\n"
                     , child.c_str()));
      }
    if (0 == path_loc.length())   // i.e. empty
      {
        delete p;
      }
    else
      {
        p->removeChild (path_loc.c_str());
      }
  }

  int
  CCMObjectLocator::TreeNode::missingNodeLevel (const char * path)
  {
    ACE_CString path_loc = path;
    ACE_CString child = splitPath (path_loc);

    TreeNode * p = 0;
    if (0 != this->children_.find (child, p))
      {
        return 1;
      }
    else
      {
        if (0 == path_loc.length())
          {
            return -1;
          }
        else
          {
            int i = p->missingNodeLevel (path_loc.c_str());
            return 0 > i ? -1 : i + 1;
          }
      }
  }

  void
  CCMObjectLocator::TreeNode::addChild (const char * path, TreeNode & node)
  {
    ACE_CString path_loc = path;
    ACE_CString child = splitPath (path_loc);
    if (0 == path_loc.length())
      {
        TreeNode * p = 0;
        if (0 == this->children_.find (child, p))
          {
            DANCE_DEBUG ( (LM_DEBUG, "[%M] CCMObjectLocator::TreeNode::register_objects - "
                         "node \"%s\" already exists. Replacing.\n"
                         , child.c_str()));
            this->children_.unbind (child);
            delete p;
            p = 0;
          }
        this->children_.bind (child, new TreeNode (node));
      }
    else
      {
        TreeNode * p = 0;
        if (0 != this->children_.find (child, p))
          {
            p = new TreeNode();
          }
        p->addChild (path_loc.c_str(), node);
      }
  }

} // DAnCE

