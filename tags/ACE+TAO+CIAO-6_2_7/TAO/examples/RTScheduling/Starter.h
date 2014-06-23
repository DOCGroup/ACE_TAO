//$Id$
#ifndef STARTER_H
#define STARTER_H

#include "SynchC.h"

#include "orbsvcs/Naming/Naming_Client.h"
#include "orbsvcs/Naming/Naming_Server.h"

#include "ace/SString.h"
#include "ace/Hash_Map_Manager.h"

class Starter
{
 public:

  Starter (CORBA::ORB_ptr orb);

  void init (void);

  void fire (void);

  typedef ACE_Hash_Map_Manager <ACE_CString,
                                Synch_var,
                                ACE_Null_Mutex> Synchs;

 private:
  void resolve_synch_objs (void);

  void add_to_synchs (CosNaming::BindingList &binding_list);

  /// A naming context.
  CosNaming::NamingContext_var synch_context_;
  TAO_Naming_Client naming_client_;

  Synchs synchs_;
};

#endif /*STARTER_H*/
