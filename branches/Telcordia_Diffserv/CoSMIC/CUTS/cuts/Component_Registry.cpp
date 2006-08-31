// $Id$

#include "cuts/Component_Registry.h"

#if !defined (__CUTS_INLINE__)
#include "cuts/Component_Registry.inl"
#endif

#include "ace/Guard_T.h"
#include "ace/Auto_Ptr.h"

//=============================================================================
//
// CUTS_Component_Registry_Node
//
//=============================================================================

//
// CUTS_Component_Registry_Node
//
CUTS_Component_Registry_Node::CUTS_Component_Registry_Node (void)
{

}

//
// ~CUTS_Component_Registry_Node
//
CUTS_Component_Registry_Node::~CUTS_Component_Registry_Node (void)
{

}

//=============================================================================
//
// CUTS_Component_Registry
//
//=============================================================================

//
// CUTS_Component_Registry
//
CUTS_Component_Registry::CUTS_Component_Registry (void)
{

}

//
// ~CUTS_Component_Registry
//
CUTS_Component_Registry::~CUTS_Component_Registry (void)
{
  CUTS_Component_Registry_Map::iterator iter;

  // Delete all the <CUTS_Component_Registry_Node> objects.
  for (iter = this->registry_.begin ();
       iter != this->registry_.end ();
       iter ++)
  {
    delete iter->second;
  }
}

//
// registry_size
//
size_t CUTS_Component_Registry::registry_size (void)
{
  return this->registry_.size ();
}

//
// register_component
//
long CUTS_Component_Registry::register_component (const char * uuid)
{
  // Allocate a new <CUTS_Component_Registry_Node> and store it
  // in a <ACE_Auto_Ptr>. In case anything bad happens and
  // return before we insert the allocated memory in the <registry_>
  // it will be destroyed on exit.
  CUTS_Component_Registry_Node * node = new CUTS_Component_Registry_Node;
  ACE_Auto_Ptr <CUTS_Component_Registry_Node> auto_clean (node);
  long regid = 0;

  do
  {
    ACE_WRITE_GUARD_RETURN (ACE_RW_Thread_Mutex,
                            guard,
                            this->lock_,
                            regid);

    // Insert the <node> into the <registry_> and "release" the
    // reference stored in the <auto_clean> variable.
    std::pair <CUTS_Component_Registry_Map::iterator, bool> result =
      this->registry_.insert (
      CUTS_Component_Registry_Map::value_type (uuid, node));

    if (result.second)
    {
      regid = reinterpret_cast <long> (node);
      auto_clean.release ();
    }
    else
    {
      regid = reinterpret_cast <long> (result.first->second);
    }
  } while (0);

  return regid;
}

//
// unregister_component
//
void CUTS_Component_Registry::unregister_component (const char *)
{
  // This is a summy operation until we have a method for
  // mapping UUID strings to integer values that will remain
  // persistant as long as the registry is active.
}

//
// get_registration
//
long CUTS_Component_Registry::get_registration (const char * uuid,
                                                bool auto_register)
{
  long regid = 0;

  CUTS_Component_Registry_Map::const_iterator iter;

  do
  {
    ACE_READ_GUARD_RETURN (ACE_RW_Thread_Mutex,
                           guard,
                           this->lock_,
                           regid);

    iter = this->registry_.find (uuid);
  } while (0);

  if (iter != this->registry_.end ())
  {
    regid = reinterpret_cast <long> (iter->second);
  }
  else if (auto_register)
  {
    regid = this->register_component (uuid);
  }

  return regid;
}

//
// get_component_by_id
//
std::string CUTS_Component_Registry::get_component_by_id (long regid) const
{
  CUTS_Component_Registry_Map::const_iterator iter;

  for (iter = this->registry_.begin ();
       iter != this->registry_.end ();
       iter ++)
  {
    if (reinterpret_cast <long> (iter->second) == regid)
    {
      return iter->first;
    }
  }

  return "";
}
