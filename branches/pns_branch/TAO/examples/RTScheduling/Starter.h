//$Id$
#ifndef STARTER_H
#define STARTER_H

#include "orbsvcs/Naming/Naming_Utils.h"
#include "SynchC.h"
#include "ace/SString.h"
#include "ace/Hash_Map_Manager.h"

class Starter
{
 public:

  Starter (CORBA::ORB_ptr orb);

  void init (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);

  void fire (void);

  typedef ACE_Hash_Map_Manager <ACE_CString,
    Synch_var,
    ACE_Null_Mutex> Synchs;


 private:
  void resolve_synch_objs (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);

  void add_to_synchs (CosNaming::BindingList &binding_list
		      ACE_ENV_ARG_DECL_WITH_DEFAULTS);


  /// A naming context.
  CosNaming::NamingContext_var synch_context_;
  TAO_Naming_Client naming_client_;

  Synchs synchs_;
};

#endif /*STARTER_H*/
