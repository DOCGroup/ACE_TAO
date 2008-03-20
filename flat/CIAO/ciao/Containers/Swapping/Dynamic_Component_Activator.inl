// $Id$

namespace CIAO
{
  ACE_INLINE void
  Dynamic_Component_Activator::add_servant_to_map(
    PortableServer::ObjectId &oid,
    Dynamic_Component_Servant_Base* servant)
  {
    this->servant_map_.bind (oid, servant);
  }

  ACE_INLINE void
  Dynamic_Component_Activator::delete_servant_from_map(
    PortableServer::ObjectId &oid)
  {
    Dynamic_Component_Servant_Base* servant = 0;
    
    if (this->servant_map_.unbind (oid, servant) != 0)
      {
        CIAO_DEBUG ((LM_DEBUG, "Invalid object reference\n"));
      }
  }
}
