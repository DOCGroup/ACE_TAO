// ============================================================================
//
// = LIBRARY
//    cos
// 
// = FILENAME
//    NS_CosNaming.C
//
// = AUTHOR
//    Marina Spivak <marina@cs.wustl.edu>
// 
// ============================================================================

#include "NS_CosNaming.h"

NS_IntId::NS_IntId (void)
  : type_ (CosNaming::nobject)
{
  ref_ = CORBA::Object::_nil ();
}

NS_IntId::NS_IntId (CORBA::Object_ptr obj,
		    CosNaming::BindingType type)
  : type_ (type)
{
  ref_ = obj->_duplicate (obj);
}

NS_IntId::NS_IntId (const NS_IntId &rhs)
{
  type_ = rhs.type_;
  ref_ =  rhs.ref_->_duplicate (rhs.ref_);
}

NS_IntId::~NS_IntId (void)
{
  CORBA::release (ref_);
}

void 
NS_IntId::operator= (const NS_IntId &rhs)
{
  // check for self assignment.
  if (&rhs == this)
    return;

  type_ = rhs.type_;
  
  CORBA::release (ref_);
  ref_ =  rhs.ref_->_duplicate (rhs.ref_);
}

NS_ExtId::NS_ExtId (void)
  : kind_ (),
    id_ ()
{
}

NS_ExtId::NS_ExtId (const char *id, 
		    const char *kind)
  : id_ (id),
    kind_ (kind)
{
}

NS_ExtId::NS_ExtId (const NS_ExtId &rhs)
{
  id_ = rhs.id_;
  kind_ = rhs.kind_; 
}

NS_ExtId::~NS_ExtId (void)
{
}

void 
NS_ExtId::operator= (const NS_ExtId &rhs)
{
  // Check for self assignment.
  if (&rhs == this)
    return;

  id_ = rhs.id_;
  kind_ = rhs.kind_; 
}
 
int 
NS_ExtId::operator== (const NS_ExtId &rhs) const
{
  return id_ == rhs.id_ && kind_ == rhs.kind_;
}

int 
NS_ExtId::operator!= (const NS_ExtId &rhs) const
{
  return id_ != rhs.id_ || kind_ != rhs.kind_;
}

u_long
NS_ExtId::hash (void) const
{
  ACE_CString temp (id_);
  temp += kind_;

  return temp.hash ();
}

NS_NamingContext::NS_NamingContext (void)
{
  if (context_.open (NS_MAP_SIZE) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "NS_NamingContext"));
  // deal with fault
}

void
NS_NamingContext::initialize (CosNaming::NamingContext_ptr tie_ref)
{
  tie_ref_ = tie_ref;
}

NS_NamingContext::~NS_NamingContext (void)
{
}

CosNaming::NamingContext_ptr 
NS_NamingContext::get_context (const CosNaming::Name &name)
  throw (CORBA::SystemException, 
	 CosNaming::NamingContext::NotFound, 
	 CosNaming::NamingContext::CannotProceed, 
	 CosNaming::NamingContext::AlreadyBound)
{
  // create compound name to be resolved
  // (<name> - last component)
  CORBA::ULong len = name.length ();
  CosNaming::Name comp_name (name);
  comp_name.length (len - 1);
  
  // resolve
  CORBA::Object_var cont_ref;

  try 
    {
      cont_ref = resolve (comp_name);
    }
  // Deal with exceptions in resolve: basicly, add the last component
  // of the name to <rest_of_name> and rethrow.
  catch (CosNaming::NamingContext::NotFound &nf)
    {      
      CORBA::ULong l = nf.rest_of_name.length ();
      nf.rest_of_name.length (l + 1);
      nf.rest_of_name[l] = name[len - 1]; 
      throw CosNaming::NamingContext::NotFound (nf);
    }
  catch (CosNaming::NamingContext::CannotProceed &cp)
    {
      CORBA::ULong l = cp.rest_of_name.length ();
      cp.rest_of_name.length (l + 1);
      cp.rest_of_name[l] = name[len - 1];
      throw CosNaming::NamingContext::CannotProceed (cp);
    }
  
  // Reference to a context from <resolve> cannot be nil because
  // cannot <bind> to a nil object.
   
  // Try narrowing object reference to a context type.
  CosNaming::NamingContext_ptr c;
  try
    {
      c = CosNaming::NamingContext::_narrow (cont_ref);
    }
  catch (CORBA::SystemException)
    {
      CosNaming::Name rest;
      rest.length (2);
      rest[0] = name[len - 2];
      rest[1] = name[len - 1];
      throw CosNaming::NamingContext::NotFound (CosNaming::NamingContext::not_context, rest);
    }

  if (CORBA::is_nil (c))
    {
      CosNaming::Name rest;
      rest.length (2);
      rest[0] = name[len - 2];
      rest[1] = name[len - 1];
      throw CosNaming::NamingContext::NotFound (CosNaming::NamingContext::not_context, rest);
    }
  return c;
}

void 
NS_NamingContext::bind (const CosNaming::Name& n, 
			CORBA::Object_ptr obj, 
			CORBA::Environment &IT_env) 
  throw (CORBA::SystemException, 
	 CosNaming::NamingContext::NotFound, 
	 CosNaming::NamingContext::CannotProceed, 
	 CosNaming::NamingContext::InvalidName, 
	 CosNaming::NamingContext::AlreadyBound)
{
  // get the length of the name
  CORBA::ULong len = n.length ();

  // Check for invalid name.
  if (len == 0)
    throw CosNaming::NamingContext::InvalidName ();

  // If we received compound name, resolve it to get the context in
  // which the binding should take place, then perform the binding on
  // target context.
  if (len > 1) 
    {
      CosNaming::NamingContext_var cont = get_context (n);
      CosNaming::Name simple_name;
      simple_name.length (1);
      simple_name[0] = n[len - 1];
      cont->bind (simple_name, obj);
    }

  // If we received a simple name, we need to bind it in this context.
  else 
    {
      NS_IntId entry (obj);
      NS_ExtId name (n[0].id, n[0].kind);

      // Try binding the name.
      if (context_.bind (name, entry) == -1)
	throw CosNaming::NamingContext::AlreadyBound ();
      // May need to add case dealing with -1. (Maybe throw cannot
      // proceed).
    }
}

void 
NS_NamingContext::rebind (const CosNaming::Name& n, 
			  CORBA::Object_ptr obj, 
			  CORBA::Environment &IT_env)    
  throw (CORBA::SystemException, 
	 CosNaming::NamingContext::NotFound, 
	 CosNaming::NamingContext::CannotProceed, 
	 CosNaming::NamingContext::InvalidName) 
{ 
  // get the length of the name
  CORBA::ULong len = n.length ();

  // check for invalid name.
  if (len == 0)
    throw CosNaming::NamingContext::InvalidName ();

  // If we received compound name, resolve it to get the context in
  // which the rebinding should take place, then perform the rebinding
  // on target context.
  if (len > 1) 
    {
      CosNaming::NamingContext_var cont = get_context (n);
      CosNaming::Name simple_name;
      simple_name.length (1);
      simple_name[0] = n[len - 1];
      cont->rebind (simple_name, obj);
    }

  // If we received a simple name, we need to rebind it in this context.
  else 
    {
      NS_IntId entry (obj);
      NS_ExtId name (n[0].id, n[0].kind);
      NS_IntId oldentry;
      NS_ExtId oldname;

      // Try rebinding the name.
      if (context_.rebind (name, entry, oldname, oldentry) == -1)
	;
	// Deal with consequences.
    }
}

void
NS_NamingContext::bind_context (const CosNaming::Name &n, 
				CosNaming::NamingContext_ptr nc, 
				CORBA::Environment &IT_env) 
  throw (CORBA::SystemException, 
	 CosNaming::NamingContext::NotFound, 
	 CosNaming::NamingContext::CannotProceed, 
	 CosNaming::NamingContext::InvalidName, 
	 CosNaming::NamingContext::AlreadyBound)
{
  // Get the length of the name.
  CORBA::ULong len = n.length ();

  // Check for invalid name.
  if (len == 0)
    throw CosNaming::NamingContext::InvalidName ();

  // If we received compound name, resolve it to get the context in
  // which the binding should take place, then perform the binding on
  // target context.
  if (len > 1) 
    {
      CosNaming::NamingContext_var cont = get_context (n);
      CosNaming::Name simple_name;
      simple_name.length (1);
      simple_name[0] = n[len - 1];
      cont->bind_context (simple_name, nc);
    }

  // If we received a simple name, we need to bind it in this context.
  else 
    {
      CosNaming::NameComponent comp = n[0];
      NS_IntId entry (nc, CosNaming::ncontext);
      NS_ExtId name (n[0].id, n[0].kind);

      // Try binding the name.
      if (context_.bind (name, entry) == 1)
	throw CosNaming::NamingContext::AlreadyBound ();
      // May need to add case dealing with -1.
    }
}

void 
NS_NamingContext::rebind_context (const CosNaming::Name &n, 
				  CosNaming::NamingContext_ptr nc, 
				  CORBA::Environment &IT_env) 
  throw (CORBA::SystemException, 
	 CosNaming::NamingContext::NotFound, 
	 CosNaming::NamingContext::CannotProceed, 
	 CosNaming::NamingContext::InvalidName)
{
  // Get the length of the name.
  CORBA::ULong len = n.length ();

  // Check for invalid name.
  if (len == 0)
    throw CosNaming::NamingContext::InvalidName ();

  // If we received compound name, resolve it to get the context in
  // which the rebinding should take place, then perform the rebinding
  // on target context.
  if (len > 1) 
    {
      CosNaming::NamingContext_var cont = get_context (n);
      CosNaming::Name simple_name;
      simple_name.length (1);
      simple_name[0] = n[len - 1];
      cont->rebind_context (simple_name, nc);
    }

  // if we received a simple name, we need to rebind it in this context.
  else 
    {
      CosNaming::NameComponent comp = n[0];
      NS_IntId entry (nc, CosNaming::ncontext);
      NS_ExtId name (n[0].id, n[0].kind);
      NS_IntId oldentry;
      NS_ExtId oldname;
      
      // try rebinding the name.
      if (context_.rebind (name, entry, oldname, oldentry) < 0);
	// deal with consequences
    }
}
    
CORBA::Object_ptr 
NS_NamingContext::resolve (const CosNaming::Name& n, 
				       CORBA::Environment &IT_env) 
  throw (CORBA::SystemException, 
	 CosNaming::NamingContext::NotFound, 
	 CosNaming::NamingContext::CannotProceed, 
	 CosNaming::NamingContext::InvalidName)
{
  // get the length of the name
  CORBA::ULong len = n.length ();

  // check for invalid name.
  if (len == 0) {
    throw CosNaming::NamingContext::InvalidName ();
  }

  // resolve the first component of the name
  NS_ExtId name (n[0].id, n[0].kind);
  NS_IntId entry;
  if (context_.find (name, entry) == -1) {
    throw CosNaming::NamingContext::NotFound (CosNaming::NamingContext::not_object, n);
  }
  CORBA::Object_ptr item = entry.ref_;

  // if the name we have to resolve is a compound name
  // we need to recursively resolve it.
  if (len > 1)
    {
      CosNaming::NamingContext_var cont;
      if (entry.type_ == CosNaming::ncontext)
	cont = CosNaming::NamingContext::_narrow (item);
      else
	throw CosNaming::NamingContext::NotFound (CosNaming::NamingContext::not_context, n);
      
      CosNaming::Name rest_of_name;
      rest_of_name.length (len - 1);
      for (CORBA::ULong i = 1; i < len; i++)
	rest_of_name[i-1] = n[i];

      return (cont->resolve (rest_of_name));
    }

  // if the name we had to resolve was simple, we just need
  // to return the result.
  return (item->_duplicate (item));
}

void 
NS_NamingContext::unbind (const CosNaming::Name& n, 
			  CORBA::Environment &IT_env) 
  throw (CORBA::SystemException, 
	 CosNaming::NamingContext::NotFound, 
	 CosNaming::NamingContext::CannotProceed, 
	 CosNaming::NamingContext::InvalidName)
{
  // if (do_operation (n, CORBA::_nil (), NS_NamingContext::unbind) == 0)

  // get the length of the name
  CORBA::ULong len = n.length ();

  // check for invalid name.
  if (len == 0)
    throw CosNaming::NamingContext::InvalidName ();

  // If we received compound name, resolve it to get the context in
  // which the unbinding should take place, then perform the unbinding
  // on target context.
  if (len > 1) 
    {
      CosNaming::NamingContext_var cont = get_context (n);
      CosNaming::Name simple_name;
      simple_name.length (1);
      simple_name[0] = n[len - 1];
      cont->unbind (simple_name);
    }

  // If we received a simple name, we need to unbind it in this
  // context.
    {
      NS_ExtId name (n[0].id, n[0].kind);
      // try unbinding the name.
      if (context_.unbind (name) == -1)
	throw CosNaming::NamingContext::NotFound (CosNaming::NamingContext::not_object, n);
    }
}
    
CosNaming::NamingContext_ptr 
NS_NamingContext::new_context (CORBA::Environment &IT_env) 
  throw (CORBA::SystemException)
{
  NS_NamingContext *c = new NS_NamingContext;
  CosNaming::NamingContext_ptr cont 
    = new TIE_CosNaming_NamingContext (NS_NamingContext) (c);

  c->initialize (cont);

  return cont->_duplicate (cont);
}
    
CosNaming::NamingContext_ptr 
NS_NamingContext::bind_new_context (const CosNaming::Name& n, 
						CORBA::Environment &IT_env) 
  throw (CORBA::SystemException, 
	 CosNaming::NamingContext::NotFound, 
	 CosNaming::NamingContext::AlreadyBound, 
	 CosNaming::NamingContext::CannotProceed, 
	 CosNaming::NamingContext::InvalidName)
{
  NS_NamingContext *c = new NS_NamingContext;
  CosNaming::NamingContext_ptr cont = 
    new TIE_CosNaming_NamingContext (NS_NamingContext) (c);
  
  c->initialize (cont);

  try
    {
      bind_context (n, cont);
    }
  catch (CosNaming::NamingContext::NotFound &nf)
    {
      CORBA::release (cont);
      throw CosNaming::NamingContext::NotFound (nf);
    }
  catch (CosNaming::NamingContext::CannotProceed &cp)
    {
      CORBA::release (cont);
      throw CosNaming::NamingContext::CannotProceed (cp);
    }
  catch (CosNaming::NamingContext::InvalidName)
    {
      CORBA::release (cont);
      throw CosNaming::NamingContext::InvalidName ();
    }
  catch (CosNaming::NamingContext::AlreadyBound)
    {
      CORBA::release (cont);
      throw CosNaming::NamingContext::AlreadyBound ();
    }

  return cont->_duplicate (cont);
}
    
void 
NS_NamingContext::destroy (CORBA::Environment &IT_env) 
  throw (CORBA::SystemException, 
	 CosNaming::NamingContext::NotEmpty)
{
  if (context_.current_size () != 0)
    throw CosNaming::NamingContext::NotEmpty ();

  // destroy context
  CORBA::release (tie_ref_);
}
    
void 
NS_NamingContext::list (CORBA::ULong how_many, 
			CosNaming::BindingList *&bl, 
			CosNaming::BindingIterator_ptr &bi, 
			CORBA::Environment &IT_env) 
  throw (CORBA::SystemException)
{
  // Dynamically allocate hash map iterator.
  NS_NamingContext::HASH_MAP::ITERATOR *hash_iter = 
    new NS_NamingContext::HASH_MAP::ITERATOR (context_);

  // Number of bindings that will go into the BindingList.
  CORBA::ULong n;

  if (context_.current_size () > how_many)
    // number of bindings in the context is > <how_many>
    // so need to return a BindingIterator.
     {
       NS_BindingIterator *bind_iter = new NS_BindingIterator (hash_iter);
	 
       bind_iter->initialize (bi); 
       bi->_duplicate (bi);
       
       n = how_many;
    }
  else
    {
      // Number of bindings in the context is <= <how_many>,
      // so do not need to return a BindingIterator.
      bi = CosNaming::BindingIterator::_nil (); 
      n = context_.current_size ();
    }     

  // use hash iterator to populate a BindingList with
  // bindings.
  CosNaming::BindingList bindings;
  bindings.length (n); 
  NS_NamingContext::HASH_MAP::ENTRY *hash_entry;

  for (CORBA::ULong i = 0; i < n; i++) 
    {
      hash_iter->next (hash_entry);
      hash_iter->advance ();
      
      bindings[i].binding_type = hash_entry->int_id_.type_;
      
      bindings[i].binding_name.length (1); 
      bindings[i].binding_name[0].id =
	CORBA::string_dup (hash_entry->ext_id_.id_.fast_rep ());
      bindings[i].binding_name[0].kind =
	CORBA::string_dup (hash_entry->ext_id_.kind_.fast_rep ());
    }
  // Marina, please add check for memory failure.
  bl = new CosNaming::BindingList (bindings);

  // If did not allocate BindingIterator, deallocate hash map
  // iterator.
  if (context_.current_size () <= how_many)
    delete hash_iter;
}
  
NS_BindingIterator::NS_BindingIterator (NS_NamingContext::HASH_MAP::ITERATOR *hash_iter)
{
  hash_iter_ = hash_iter;
}
 
NS_BindingIterator::~NS_BindingIterator (void)
{
  delete hash_iter_;
}
 
void
NS_BindingIterator::initialize (CosNaming::BindingIterator_ptr tie_ref)
{
  tie_ref_ = tie_ref;
}
  
CORBA::Boolean 
NS_BindingIterator::next_one (CosNaming::Binding*& b, 
			      CORBA::Environment &IT_env) 
  throw (CORBA::SystemException)
{
  if (hash_iter_->done ()) {
    b = new CosNaming::Binding;
    return 0;
  }
  else
    {
      b = new CosNaming::Binding;

      NS_NamingContext::HASH_MAP::ENTRY *hash_entry;
      hash_iter_->next (hash_entry);
      hash_iter_->advance ();
      
      b->binding_type = hash_entry->int_id_.type_;

      b->binding_name.length (1); 
      b->binding_name[0].id =
	CORBA::string_dup (hash_entry->ext_id_.id_.fast_rep ());
      b->binding_name[0].kind =
	CORBA::string_dup (hash_entry->ext_id_.kind_.fast_rep ());      

      return 1;
    }
}
    
CORBA::Boolean 
NS_BindingIterator::next_n (CORBA::ULong how_many, 
			    CosNaming::BindingList *&bl,
			    CORBA::Environment &IT_env) 
  throw (CORBA::SystemException)
{
  if (hash_iter_->done ()) {
    bl = new CosNaming::BindingList;
    return 0;
  }
  else
    {
      // Statically allocate a BindingList.
      CosNaming::BindingList bindings;

      // Initially assume that iterator has the requested number of
      // bindings.
      bindings.length (how_many);

      // Iterate and populate the BindingList.
      NS_NamingContext::HASH_MAP::ENTRY *hash_entry;
      for (CORBA::ULong i = 0; i < how_many; i++) 
	{
	  hash_iter_->next (hash_entry);
	  
	  bindings[i].binding_type = hash_entry->int_id_.type_;
	  
	  bindings[i].binding_name.length (1); 
	  bindings[i].binding_name[0].id =
	    CORBA::string_dup (hash_entry->ext_id_.id_.fast_rep ());
	  bindings[i].binding_name[0].kind =
	    CORBA::string_dup (hash_entry->ext_id_.kind_.fast_rep ());
	  
	  if (hash_iter_->advance () == 0)
	    {
	      // If no more bindings left, reset length to the actual
	      // number of bindings populated and get out of the loop.
	      bindings.length (i + 1);
	      break;	    
	    }
	}

      bl = new CosNaming::BindingList (bindings);
      // Marina, please check for failed memory allocation.
      return 1;
    }
}
    
void 
NS_BindingIterator::destroy (CORBA::Environment &IT_env) 
  throw (CORBA::SystemException)
{
  CORBA::release (tie_ref_);
}


