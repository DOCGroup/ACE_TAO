/* -*- c++ -*- */
// $Id$

#include "ifr_adding_visitor.h"
#include "ifr_adding_visitor_operation.h"
#include "ifr_adding_visitor_structure.h"
#include "ifr_adding_visitor_exception.h"
#include "ifr_adding_visitor_union.h"

ACE_RCSID(IFR_Service, ifr_adding_visitor, "$Id$")

ifr_adding_visitor::ifr_adding_visitor (void)
{
}

ifr_adding_visitor::~ifr_adding_visitor (void)
{
}

int 
ifr_adding_visitor::visit_scope (UTL_Scope *node)
{
  // Proceed if the number of members in our scope is greater than 0.
  if (node->nmembers () > 0)
    {
      // Initialize an iterator to iterate over our scope.
      UTL_ScopeActiveIterator si (node,
                                  UTL_Scope::IK_decls);

      AST_Decl *d = 0;

      // Continue until each element is visited.
      while (!si.is_done ())
        {
          d = si.item ();

          if (d == 0)
            {
              ACE_ERROR_RETURN ((
                  LM_ERROR,
                  ACE_TEXT ("(%N:%l) ifr_adding_visitor::visit_scope -")
                  ACE_TEXT (" bad node in this scope\n")
                ), 
                -1
              );
            }

          // These are created at startup in the repository, and
          // need not be dealt with here.
          if (d->node_type () == AST_Decl::NT_pre_defined)
            {
              si.next ();
              continue;
            }

          if (d->ast_accept (this) == -1)
            {
              ACE_ERROR_RETURN ((
                  LM_ERROR,
                  ACE_TEXT ("(%N:%l) ifr_adding_visitor::visit_scope -")
                  ACE_TEXT (" failed to accept visitor\n")
                ),  
                -1
              );
            }

          si.next ();
        }
    }

  return 0;
}

int 
ifr_adding_visitor::visit_predefined_type (AST_PredefinedType *node)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {  
      this->ir_current_ = 
      be_global->repository ()->get_primitive (
                                    this->predefined_type_to_pkind (node),
                                    ACE_TRY_ENV
                                  );
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           ACE_TEXT ("visit_predefined_type"));
    
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

int 
ifr_adding_visitor::visit_module (AST_Module *node)
{
  CORBA_Container_var new_def;

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // If this module been opened before, it will already be in
      // the repository.
      CORBA_Contained_var prev_def = 
        be_global->repository ()->lookup_id (node->repoID (),
                                             ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (prev_def.in ()))
        {
          // New module.
          CORBA_Container_ptr container = CORBA_Container::_nil ();

          if (be_global->ifr_scopes ().top (container) == 0)
            {
              new_def = container->create_module (
                                       node->repoID (),
                                       node->local_name ()->get_string (),
                                       this->gen_version (node),
                                       ACE_TRY_ENV
                                     );
              ACE_TRY_CHECK;
            }
          else
            {
              ACE_ERROR_RETURN ((
                  LM_ERROR,
                  ACE_TEXT ("(%N:%l) ifr_adding_visitor::visit_module -")
                  ACE_TEXT (" scope stack is empty\n")
                ),  
                -1
              );
            }
        }
      else
        {
          // Reopened module.
          new_def = CORBA_ModuleDef::_narrow (prev_def.in (),
                                              ACE_TRY_ENV);
          ACE_TRY_CHECK;

          // Nothing prevents this modules's repo id from already being
          // in the repository as another type, if it came from another
          // IDL file whose generated code is not linked to the generated
          // code from this IDL file. So we check here before we make a
          // call on new_def.
          if (CORBA::is_nil (new_def.in ()))
            {
              ACE_ERROR_RETURN ((
                  LM_ERROR,
                  ACE_TEXT ("(%N:%l) ifr_adding_visitor::visit_module -")
                  ACE_TEXT (" module %s, in IDL file %s,  already entered")
                  ACE_TEXT (" in repository as a different type\n"),
                  node->full_name (),
                  be_global->filename ()
                ),
                -1
              );
            }
        }

      if (be_global->ifr_scopes ().push (new_def.in ()) != 0)
        {
          ACE_ERROR_RETURN ((
              LM_ERROR,
              ACE_TEXT ("(%N:%l) ifr_adding_visitor::visit_module -")
              ACE_TEXT (" scope push failed\n")
            ),  
            -1
          );
        }

      if (this->visit_scope (node) == -1)
        {
          ACE_ERROR_RETURN ((
              LM_ERROR,
              ACE_TEXT ("(%N:%l) ifr_adding_visitor::visit_module -")
              ACE_TEXT (" visit_scope failed\n")
            ),  
            -1
          );
        }

      CORBA_Container_ptr tmp = CORBA_Container::_nil ();

      if (be_global->ifr_scopes ().pop (tmp) != 0)
        {
          ACE_ERROR_RETURN ((
              LM_ERROR,
              ACE_TEXT ("(%N:%l) ifr_adding_visitor::visit_module -")
              ACE_TEXT (" scope pop failed\n")
            ),  
            -1
          );
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           ACE_TEXT ("visit_module"));

      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

int 
ifr_adding_visitor::visit_interface (AST_Interface *node)
{
  // If the node is really a valuetype, go there instead.
  if (node->is_valuetype ())
    {
      return this->visit_valuetype (node);
    }

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // Is this interface already in the respository?
      CORBA_Contained_var prev_def =
        be_global->repository ()->lookup_id (node->repoID (),
                                             ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // If not, create a new entry.
      if (CORBA::is_nil (prev_def.in ()))
        {
          CORBA::ULong n_parents = ACE_static_cast (CORBA::ULong, 
                                                    node->n_inherits ());

          CORBA_InterfaceDefSeq bases (n_parents);
          bases.length (n_parents);
          CORBA_Contained_var result;

          AST_Interface **parents = node->inherits ();

          // Construct a list of the parents.
          for (CORBA::ULong i = 0; i < n_parents; ++i)
            {
              result = 
                be_global->repository ()->lookup_id (parents[i]->repoID (),
                                                     ACE_TRY_ENV);
              ACE_TRY_CHECK

              bases[i] = CORBA_InterfaceDef::_narrow (result.in (),
                                                      ACE_TRY_ENV);
              ACE_TRY_CHECK;

              if (CORBA::is_nil (bases[i].in ()))
                {
                  ACE_ERROR_RETURN ((
                      LM_ERROR,
                      ACE_TEXT ("(%N:%l) ifr_adding_visitor::")
                      ACE_TEXT ("visit_interface -")
                      ACE_TEXT (" CORBA_InterfaceDef::_narrow failed\n")
                    ),  
                    -1
                  );
                }
            }

          CORBA_Container_ptr current_scope = CORBA_Container::_nil ();
          
          if (be_global->ifr_scopes ().top (current_scope) == 0)
            {
              CORBA_InterfaceDef_var new_def;

              if (node->is_local ())
                {
                  new_def =
                    current_scope->create_local_interface (
                                       node->repoID (),
                                       node->local_name ()->get_string (),
                                       this->gen_version (node),
                                       bases,
                                       ACE_TRY_ENV
                                     );
                }
              else
                {
                  new_def =
                    current_scope->create_interface (
                                       node->repoID (),
                                       node->local_name ()->get_string (),
                                       this->gen_version (node),
                                       bases,
                                       ACE_TRY_ENV
                                     );
                }

              ACE_TRY_CHECK;

              node->ifr_added_ = 1;

              // Push the new IR object onto the scope stack.
              CORBA_Container_var new_scope =
                CORBA_Container::_narrow (new_def.in (),
                                          ACE_TRY_ENV);
              ACE_TRY_CHECK;

              if (be_global->ifr_scopes ().push (new_scope.in ()) != 0)
                {
                  ACE_ERROR_RETURN ((
                      LM_ERROR,
                      ACE_TEXT ("(%N:%l) ifr_adding_visitor::")
                      ACE_TEXT ("visit_interface -")
                      ACE_TEXT (" scope push failed\n")
                    ),  
                    -1
                  );
                }

              // Visit the members, if any.
              if (this->visit_scope (node) == -1)
                {
                  ACE_ERROR_RETURN ((
                      LM_ERROR,
                      ACE_TEXT ("(%N:%l) ifr_adding_visitor::")
                      ACE_TEXT ("visit_interface -")
                      ACE_TEXT (" visit_scope failed\n")
                    ),  
                    -1
                  );
                }

              // This spot in the AST doesn't necessarily have to be the
              // interface definition - it could be any reference to it.
              // The front end will already have fully defined it, so all
              // the info in available anywhere. So it's a good idea to
              // update the current IR object holder now. This will
              // consume the objref pointer.
              this->ir_current_ = 
                CORBA_IDLType::_duplicate (new_def.in ());

              CORBA_Container_ptr used_scope = CORBA_Container::_nil ();

              // Pop the new IR object back off the scope stack.
              if (be_global->ifr_scopes ().pop (used_scope) != 0)
                {
                  ACE_ERROR_RETURN ((
                      LM_ERROR,
                      ACE_TEXT ("(%N:%l) ifr_adding_visitor::")
                      ACE_TEXT ("visit_interface -")
                      ACE_TEXT (" scope pop failed\n")
                    ),  
                    -1
                  );
                }
            }
          else
            {
              ACE_ERROR_RETURN ((
                  LM_ERROR,
                  ACE_TEXT ("(%N:%l) ifr_adding_visitor::visit_interface -")
                  ACE_TEXT (" scope stack is empty\n")
                ),  
                -1
              );
            }
        }
      else
        {
          // There is already an entry in the repository. If the interface is
          // defined and has not already been populated, we do so 
          // now. If it is not yet defined or the full definition has already
          // been added to the repository, we just update the current IR object 
          // holder.
          if (node->is_defined () && node->ifr_added_ == 0)
            {
              CORBA_InterfaceDef_var extant_def =
                CORBA_InterfaceDef::_narrow (prev_def.in (),
                                             ACE_TRY_ENV);
              ACE_TRY_CHECK;

              // Nothing prevents this interface's repo id from already being
              // in the repository as another type, if it came from another
              // IDL file whose generated code is not linked to the generated
              // code from this IDL file. So we check here before we make a
              // call on extant_def.
              if (CORBA::is_nil (extant_def.in ()))
                {
                  ACE_ERROR_RETURN ((
                      LM_ERROR,
                      ACE_TEXT ("(%N:%l) ifr_adding_visitor::visit_interface -")
                      ACE_TEXT (" interface %s, in IDL file %s,  already entered")
                      ACE_TEXT (" in repository as a different type\n"),
                      node->full_name (),
                      be_global->filename ()
                    ),
                    -1
                  );
                }
              else if (node->ifr_fwd_added_ == 0)
                {
                  // No way to tell if this is just the definition of a forward
                  // declaration from another file, or an error. This warning
                  // will let the user decide, and roll back the processing of
                  // this IDL file if necessary.
                  this->redef_warning (node);

                  return -1;
                }

              CORBA::ULong n_parents = ACE_static_cast (CORBA::ULong, 
                                                        node->n_inherits ());

              CORBA_InterfaceDefSeq bases (n_parents);
              bases.length (n_parents);
              CORBA_Contained_var result;

              AST_Interface **parents = node->inherits ();

              // Construct a list of the parents.
              for (CORBA::ULong i = 0; i < n_parents; ++i)
                {
                  result = 
                    be_global->repository ()->lookup_id (parents[i]->repoID (),
                                                         ACE_TRY_ENV);
                  ACE_TRY_CHECK;

                  // If one of our interface's parents is not in the repository,
                  // that means that it has not yet been seen (even as a
                  // forward declaration) in the IDL file, and we will have to
                  // postpone the populating of our interface until they are all 
                  // added.
                  if (CORBA::is_nil (result.in ()))
                    {
                      this->ir_current_ =
                        CORBA_IDLType::_narrow (prev_def.in (),
                                                ACE_TRY_ENV);
                      ACE_TRY_CHECK

                      return 0;
                    }

                  bases[i] = CORBA_InterfaceDef::_narrow (result.in (),
                                                          ACE_TRY_ENV);
                  ACE_TRY_CHECK;

                  if (CORBA::is_nil (bases[i].in ()))
                    {
                      ACE_ERROR_RETURN ((
                          LM_ERROR,
                          ACE_TEXT ("(%N:%l) ifr_adding_visitor::")
                          ACE_TEXT ("visit_interface -")
                          ACE_TEXT (" CORBA_InterfaceDef::_narrow failed\n")
                        ),  
                        -1
                      );
                    }
                }

              extant_def->base_interfaces (bases,
                                           ACE_TRY_ENV);
              ACE_TRY_CHECK

              node->ifr_added_ = 1;

              CORBA_Container_var new_scope =
                CORBA_Container::_narrow (extant_def.in (),
                                          ACE_TRY_ENV);
              ACE_TRY_CHECK;

              // Push the new IR object onto the scope stack.
              if (be_global->ifr_scopes ().push (new_scope.in ()) != 0)
                {
                  ACE_ERROR_RETURN ((
                      LM_ERROR,
                      ACE_TEXT ("(%N:%l) ifr_adding_visitor::")
                      ACE_TEXT ("visit_interface -")
                      ACE_TEXT (" scope push failed\n")
                    ),  
                    -1
                  );
                }

              // Visit the members, if any.
              if (this->visit_scope (node) == -1)
                {
                  ACE_ERROR_RETURN ((
                      LM_ERROR,
                      ACE_TEXT ("(%N:%l) ifr_adding_visitor::")
                      ACE_TEXT ("visit_interface -")
                      ACE_TEXT (" visit_scope failed\n")
                    ),  
                    -1
                  );
                }

              // This spot in the AST doesn't necessarily have to be the
              // interface definition - it could be any reference to it.
              // The front end will already have fully defined it, so all
              // the info is available anywhere. So it's a good idea to
              // update the current IR object holder now.
              this->ir_current_ = 
                CORBA_IDLType::_duplicate (extant_def.in ());

              CORBA_Container_ptr used_scope = CORBA_Container::_nil ();

              // Pop the new IR object back off the scope stack.
              if (be_global->ifr_scopes ().pop (used_scope) != 0)
                {
                  ACE_ERROR_RETURN ((
                      LM_ERROR,
                      ACE_TEXT ("(%N:%l) ifr_adding_visitor::")
                      ACE_TEXT ("visit_interface -")
                      ACE_TEXT (" scope pop failed\n")
                    ),  
                    -1
                  );
                }
            }
          else
            {
              this->ir_current_ = 
                CORBA_InterfaceDef::_narrow (prev_def.in (),
                                             ACE_TRY_ENV);
              ACE_TRY_CHECK;

              if (CORBA::is_nil (this->ir_current_.in ()))
                {
                  ACE_ERROR_RETURN ((
                      LM_ERROR,
                      ACE_TEXT ("(%N:%l) ifr_adding_visitor::")
                      ACE_TEXT ("visit_interface -")
                      ACE_TEXT (" CORBA_InterfaceDef::_narrow failed\n")
                    ),  
                    -1
                  );
                }
            }
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           ACE_TEXT ("visit_interface"));

      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

int 
ifr_adding_visitor::visit_interface_fwd (AST_InterfaceFwd *node)
{
  AST_Interface *i = node->full_definition ();

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // Is this interface already in the respository?
      CORBA_Contained_var prev_def =
        be_global->repository ()->lookup_id (i->repoID (),
                                             ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (prev_def.in ()))
        {
          // The forward declared interface is not defined anywhere
          // in this IDL file, so we just create an empty entry to
          // be populated by some other IDL file.
          CORBA_InterfaceDefSeq bases (0);
          bases.length (0);

          CORBA_Container_ptr current_scope = CORBA_Container::_nil ();
      
          if (be_global->ifr_scopes ().top (current_scope) == 0)
            {
              if (i->is_local ())
                {
                  this->ir_current_ =
                    current_scope->create_local_interface (
                                       i->repoID (),
                                       i->local_name ()->get_string (),
                                       this->gen_version (i),
                                       bases,
                                       ACE_TRY_ENV
                                     );
                }
              else
                {
                  this->ir_current_ =
                    current_scope->create_interface (
                                       i->repoID (),
                                       i->local_name ()->get_string (),
                                       this->gen_version (i),
                                       bases,
                                       ACE_TRY_ENV
                                     );
                }

              ACE_TRY_CHECK;
            }
          else
            {
              ACE_ERROR_RETURN ((
                  LM_ERROR,
                  ACE_TEXT ("(%N:%l) ifr_adding_visitor::")
                  ACE_TEXT ("visit_interface_fwd -")
                  ACE_TEXT (" scope stack is empty\n")
                ),  
                -1
              );
            }
        }
      else
        {
          // There is already an entry in the repository, so just update
          // the current IR object holder.
          this->ir_current_ = 
            CORBA_InterfaceDef::_narrow (prev_def.in (),
                                         ACE_TRY_ENV);
          ACE_TRY_CHECK;

          // Nothing prevents this interface's repo id from already being
          // in the repository as another type, if it came from another
          // IDL file whose generated code is not linked to the generated
          // code from this IDL file. So we check here before we make a
          // call on ir_current_.
          if (CORBA::is_nil (this->ir_current_.in ()))
            {
              ACE_ERROR_RETURN ((
                  LM_ERROR,
                  ACE_TEXT ("(%N:%l) ifr_adding_visitor::")
                  ACE_TEXT ("visit_interface_fwd -")
                  ACE_TEXT (" interface %s, in IDL file %s,  already entered")
                  ACE_TEXT (" in repository as a different type\n"),
                  node->full_name (),
                  be_global->filename ()
                ),
                -1
              );
            }
        }

      i->ifr_fwd_added_ = 1;
   }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           ACE_TEXT ("visit_interface_fwd"));

      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

int 
ifr_adding_visitor::visit_valuetype (AST_Interface *)
{
  return 0;
}

int 
ifr_adding_visitor::visit_valuetype_fwd (AST_InterfaceFwd *)
{
  return 0;
}

int 
ifr_adding_visitor::visit_structure (AST_Structure *node)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      CORBA_Contained_var prev_def = 
        be_global->repository ()->lookup_id (node->repoID (),
                                             ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (prev_def.in ()))
        {
          ifr_adding_visitor_structure visitor (0);

          int retval = visitor.visit_structure (node);

          if (retval == 0)
            {
              this->ir_current_ =
                CORBA_IDLType::_duplicate (visitor.ir_current ());
            }

          return retval;
        }
      else
        {
          this->ir_current_ =
            CORBA_IDLType::_narrow (prev_def.in (),
                                    ACE_TRY_ENV);
          ACE_TRY_CHECK;

          // Nothing prevents this struct's repo id from already being
          // in the repository as another type, if it came from another
          // IDL file whose generated code is not linked to the generated
          // code from this IDL file. So we check here before we make a
          // call on ir_current_.
          if (CORBA::is_nil (this->ir_current_.in ()))
            {
              ACE_ERROR_RETURN ((
                  LM_ERROR,
                  ACE_TEXT ("(%N:%l) ifr_adding_visitor::visit_structure -")
                  ACE_TEXT (" struct %s, in IDL file %s,  already entered")
                  ACE_TEXT (" in repository as a different type\n"),
                  node->full_name (),
                  be_global->filename ()
                ),
                -1
              );
            }
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           ACE_TEXT ("ifr_adding_visitor::visit_structure"));

      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

int 
ifr_adding_visitor::visit_exception (AST_Exception *node)
{
  ifr_adding_visitor_exception visitor;

  // No point in updating ir_current_ here because
  // ExceptionDef is not an IDLType.

  return visitor.visit_exception (node);
}

int 
ifr_adding_visitor::visit_enum (AST_Enum *node)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // Is this enum already in the respository?
      CORBA_Contained_var prev_def =
        be_global->repository ()->lookup_id (node->repoID (),
                                             ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // If not, create a new entry.
      if (CORBA::is_nil (prev_def.in ()))
        {
          CORBA::ULong member_count = 
            ACE_static_cast (CORBA::ULong, 
                             node->member_count ());

          CORBA_EnumMemberSeq members (member_count);
          members.length (member_count);

          UTL_ScopedName *member_name = 0;

          // Get a list of the member names.
          for (CORBA::ULong i = 0; i < member_count; ++i)
            {
              member_name = node->value_to_name (i);

              members[i] = 
                CORBA::string_dup (
                    member_name->last_component ()->get_string ()
                  );
            }

          CORBA_Container_ptr current_scope = CORBA_Container::_nil ();

          if (be_global->ifr_scopes ().top (current_scope) == 0)
            {
              this->ir_current_ =
                current_scope->create_enum (
                    node->repoID (),
                    node->local_name ()->get_string (),
                    this->gen_version (node),
                    members,
                    ACE_TRY_ENV
                  );
              ACE_TRY_CHECK;
            }
          else
            {
              ACE_ERROR_RETURN ((
                  LM_ERROR,
                  ACE_TEXT ("(%N:%l) ifr_adding_visitor::visit_enum -")
                  ACE_TEXT (" scope stack is empty\n")
                ),  
                -1
              );
            }
        }
      else
        {
          // There is already an entry in the repository, so just update
          // the current IR object holder.
          this->ir_current_ = 
            CORBA_EnumDef::_narrow (prev_def.in (),
                                    ACE_TRY_ENV);
          ACE_TRY_CHECK;

          // Nothing prevents this enum's repo id from already being
          // in the repository as another type, if it came from another
          // IDL file whose generated code is not linked to the generated
          // code from this IDL file. So we check here before we make a
          // call on ir_current_.
          if (CORBA::is_nil (this->ir_current_.in ()))
            {
              ACE_ERROR_RETURN ((
                  LM_ERROR,
                  ACE_TEXT ("(%N:%l) ifr_adding_visitor::visit_enum -")
                  ACE_TEXT (" enum %s, in IDL file %s,  already entered")
                  ACE_TEXT (" in repository as a different type\n"),
                  node->full_name (),
                  be_global->filename ()
                ),
                -1
              );
            }
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           ACE_TEXT ("ifr_adding_visitor::visit_enum"));

      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

int 
ifr_adding_visitor::visit_operation (AST_Operation *node)
{
  ifr_adding_visitor_operation visitor;

  return visitor.visit_operation (node);
}

int 
ifr_adding_visitor::visit_field (AST_Field *node)
{
  AST_Type *ft = AST_Type::narrow_from_decl (node->field_type ());

  if (ft == 0)
    {
      ACE_ERROR_RETURN ((
          LM_ERROR,
          ACE_TEXT ("(%N:%l) ifr_adding_visitor::")
          ACE_TEXT ("visit_field - ")
          ACE_TEXT ("Bad field type\n")
        ), 
        -1
      );
    }

  if (ft->ast_accept (this) == -1)
    {
      ACE_ERROR_RETURN ((
          LM_ERROR,
          ACE_TEXT ("(%N:%l) ifr_adding_visitor::")
          ACE_TEXT ("visit_field - ")
          ACE_TEXT ("failed to accept visitor\n")
        ), 
        -1
      );
    }

  return 0;
}

int 
ifr_adding_visitor::visit_attribute (AST_Attribute *node)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      CORBA_Contained_var prev_def =
        be_global->repository ()->lookup_id (node->repoID (),
                                             ACE_TRY_ENV);
      ACE_TRY_CHECK;

      CORBA_AttributeDef_var new_def;

      if (CORBA::is_nil (prev_def.in ()))
        {
          CORBA::AttributeMode mode = 
            node->readonly () ? CORBA::ATTR_READONLY : CORBA::ATTR_NORMAL;

          AST_Type *type = node->field_type ();

          // If this attribute is itself an interface, we just update
          // the current IR object holder. The forward declaration
          // (if any) will create a repository entry, and the full
          // definition will take care of the interface's scope. Trying 
          // to take care of the interface's scope at this point could
          // cause problems, if the types of all its members have not yet
          // been declared.
          if (type->node_type () == AST_Decl::NT_interface)
            {
              CORBA_Contained_var prev_type_def =
                be_global->repository ()->lookup_id (type->repoID (),
                                                     ACE_TRY_ENV);
              ACE_TRY_CHECK;

              this->ir_current_ = 
                CORBA_IDLType::_narrow (prev_type_def.in (),
                                        ACE_TRY_ENV);
              ACE_TRY_CHECK;
            }
          else
            {
              // Since the type of this attribute has already been seen
              // in the IDL file (or is a primitive type), the following
              // call will simply look up the repository entry and place
              // it in the current IR object holder.
              if (type->ast_accept (this) == -1)
                {
                  ACE_ERROR_RETURN ((
                      LM_ERROR,
                      ACE_TEXT ("(%N:%l) ifr_adding_visitor::")
                      ACE_TEXT ("visit_attribute - ")
                      ACE_TEXT ("failed to accept visitor\n")
                    ), 
                    -1
                  );
                }
            }

          CORBA_Container_ptr current_scope = CORBA_Container::_nil ();

          if (be_global->ifr_scopes ().top (current_scope) == 0)
            {
              CORBA_InterfaceDef_var iface = 
                CORBA_InterfaceDef::_narrow (current_scope,
                                             ACE_TRY_ENV);
              ACE_TRY_CHECK;

              new_def =
                iface->create_attribute (node->repoID (),
                                         node->local_name ()->get_string (),
                                         this->gen_version (node),
                                         this->ir_current_.in (),
                                         mode,
                                         ACE_TRY_ENV);
              ACE_TRY_CHECK;
            }
          else
            {
              ACE_ERROR_RETURN ((
                  LM_ERROR,
                  ACE_TEXT ("(%N:%l) ifr_adding_visitor::visit_attribute -")
                  ACE_TEXT (" scope stack is empty\n")
                ),  
                -1
              );
            }
        }
      else
        {
          new_def = 
            CORBA_AttributeDef::_narrow (prev_def.in (),
                                         ACE_TRY_ENV);
          ACE_TRY_CHECK;

          // Nothing prevents this attribute's repo id from already being
          // in the repository as another type, if it came from another
          // IDL file whose generated code is not linked to the generated
          // code from this IDL file. So we check here before we make a
          // call on new_def.
          if (CORBA::is_nil (new_def.in ()))
            {
              ACE_ERROR_RETURN ((
                  LM_ERROR,
                  ACE_TEXT ("(%N:%l) ifr_adding_visitor::visit_attribute -")
                  ACE_TEXT (" attribute %s, in IDL file %s,  already entered")
                  ACE_TEXT (" in repository as a different type\n"),
                  node->full_name (),
                  be_global->filename ()
                ),
                -1
              );
            }
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           ACE_TEXT ("visit_attribute"));

      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

int 
ifr_adding_visitor::visit_union (AST_Union *node)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      CORBA_Contained_var prev_def = 
        be_global->repository ()->lookup_id (node->repoID (),
                                             ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (prev_def.in ()))
        {
          ifr_adding_visitor_union visitor (0);

          int retval = visitor.visit_union (node);

          if (retval == 0)
            {
              this->ir_current_ =
                CORBA_IDLType::_duplicate (visitor.ir_current ());
            }

          return retval;
        }
      else
        {
          this->ir_current_ =
            CORBA_UnionDef::_narrow (prev_def.in (),
                                     ACE_TRY_ENV);
          ACE_TRY_CHECK;

          // Nothing prevents this union's repo id from already being
          // in the repository as another type, if it came from another
          // IDL file whose generated code is not linked to the generated
          // code from this IDL file. So we check here before we make a
          // call on ir_current_.
          if (CORBA::is_nil (this->ir_current_.in ()))
            {
              ACE_ERROR_RETURN ((
                  LM_ERROR,
                  ACE_TEXT ("(%N:%l) ifr_adding_visitor::visit_union -")
                  ACE_TEXT (" union %s, in IDL file %s,  already entered")
                  ACE_TEXT (" in repository as a different type\n"),
                  node->full_name (),
                  be_global->filename ()
                ),
                -1
              );
            }
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           ACE_TEXT ("ifr_adding_visitor::visit_union"));

      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

int 
ifr_adding_visitor::visit_union_branch (AST_UnionBranch *node)
{
  AST_Type *ft = AST_Type::narrow_from_decl (node->field_type ());

  if (ft == 0)
    {
      ACE_ERROR_RETURN ((
          LM_ERROR,
          ACE_TEXT ("(%N:%l) ifr_adding_visitor::")
          ACE_TEXT ("visit_union_branch - ")
          ACE_TEXT ("Bad field type\n")
        ), 
        -1
      );
    }

  if (ft->ast_accept (this) == -1)
    {
      ACE_ERROR_RETURN ((
          LM_ERROR,
          ACE_TEXT ("(%N:%l) ifr_adding_visitor::")
          ACE_TEXT ("visit_union_branch - ")
          ACE_TEXT ("failed to accept visitor\n")
        ), 
        -1
      );
    }

  return 0;
}

int 
ifr_adding_visitor::visit_constant (AST_Constant *node)
{
  const char *id = node->repoID ();

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      CORBA_Contained_var prev_def =
        be_global->repository ()->lookup_id (id,
                                             ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Nothing prevents this constant's repo id from already being
      // in the repository as another type, if it came from another
      // IDL file whose generated code is not linked to the generated
      // code from this IDL file. So we check here before we make a
      // call on ir_current_.
      if (!CORBA::is_nil (prev_def.in ()))
        {      
          CORBA_ConstantDef_var const_def =
            CORBA_ConstantDef::_narrow (prev_def.in (),
                                        ACE_TRY_ENV);
          ACE_TRY_CHECK;

          if (CORBA::is_nil (const_def.in ()))
            {
              ACE_ERROR_RETURN ((
                  LM_ERROR,
                  ACE_TEXT ("(%N:%l) ifr_adding_visitor::visit_constant -")
                  ACE_TEXT (" constant %s, in IDL file %s,  already entered")
                  ACE_TEXT (" in repository as a different type\n"),
                  node->full_name (),
                  be_global->filename ()
                ),
                -1
              );
            }

          // If everything's ok, just return.
          return 0;
        }

      AST_Expression::AST_ExprValue *ev = node->constant_value ()->ev ();

      CORBA::PrimitiveKind pkind = this->expr_type_to_pkind (ev->et);

      CORBA_IDLType_var idl_type = 
        be_global->repository ()->get_primitive (pkind,
                                                 ACE_TRY_ENV);
      ACE_TRY_CHECK;

      CORBA::Any any;

      this->load_any (ev,
                      any);

      CORBA_Container_ptr current_scope = CORBA_Container::_nil ();

      if (be_global->ifr_scopes ().top (current_scope) == 0)
        {
          CORBA_ConstantDef_var new_def =
            current_scope->create_constant (
                id,
                node->local_name ()->get_string (),
                this->gen_version (node),
                idl_type.in (),
                any,
                ACE_TRY_ENV
              );
          ACE_TRY_CHECK;
        }
      else
        {
          ACE_ERROR_RETURN ((
              LM_ERROR,
              ACE_TEXT ("(%N:%l) ifr_adding_visitor::visit_constant -")
              ACE_TEXT (" scope stack is empty\n")
            ),  
            -1
          );
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           ACE_TEXT ("visit_constant"));

      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

int 
ifr_adding_visitor::visit_array (AST_Array *node)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      this->element_type (node->base_type (),
                          ACE_TRY_ENV);
      ACE_TRY_CHECK;

      AST_Expression **dims = node->dims ();

      for (unsigned long i = node->n_dims (); i > 0; --i)
        {
          this->ir_current_ = 
            be_global->repository ()->create_array (
                dims[i - 1]->ev ()->u.ulval,
                this->ir_current_.in (),
                ACE_TRY_ENV
              );
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           ACE_TEXT ("visit_array"));

      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

int 
ifr_adding_visitor::visit_sequence (AST_Sequence *node)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      this->element_type (node->base_type (),
                          ACE_TRY_ENV);
      ACE_TRY_CHECK;

      this->ir_current_ =
        be_global->repository ()->create_sequence (
                                      node->max_size ()->ev ()->u.ulval,
                                      this->ir_current_.in (),
                                      ACE_TRY_ENV
                                    );
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           ACE_TEXT ("visit_sequence"));

      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

int 
ifr_adding_visitor::visit_string (AST_String *node)
{
  AST_Expression *ex = node->max_size ();

  AST_Expression::AST_ExprValue *ev = ex->ev ();

  CORBA::ULong bound = ACE_static_cast (CORBA::ULong, ev->u.ulval);

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      if (node->node_type () == AST_Decl::NT_string)
        {
          this->ir_current_ =
            be_global->repository ()->create_string (bound,
                                                     ACE_TRY_ENV);
        }
      else
        {
          this->ir_current_ =
            be_global->repository ()->create_wstring (bound,
                                                      ACE_TRY_ENV);
        }

      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           ACE_TEXT ("visit_string"));

      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

int 
ifr_adding_visitor::visit_typedef (AST_Typedef *node)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // Is this typedef already in the respository?
      CORBA_Contained_var prev_def =
        be_global->repository ()->lookup_id (node->repoID (),
                                             ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // If not, create a new entry.
      if (CORBA::is_nil (prev_def.in ()))
        {
          this->element_type (node->base_type (),
                              ACE_TRY_ENV);
          ACE_TRY_CHECK;

          CORBA_Container_ptr current_scope = CORBA_Container::_nil ();

          if (be_global->ifr_scopes ().top (current_scope) == 0)
            {
              this->ir_current_ = 
                current_scope->create_alias (
                   node->repoID (),
                   node->local_name ()->get_string (),
                   this->gen_version (node),
                   this->ir_current_.in (),
                   ACE_TRY_ENV
                 );
              ACE_TRY_CHECK;
            }
          else
            {
              ACE_ERROR_RETURN ((
                  LM_ERROR,
                  ACE_TEXT ("(%N:%l) ifr_adding_visitor::visit_typedef -")
                  ACE_TEXT (" scope stack is empty\n")
                ),  
                -1
              );
            }
        }
      else
        {
          // There is already an entry in the repository, so just update
          // the current IR object holder.
          this->ir_current_ = 
            CORBA_TypedefDef::_narrow (prev_def.in (),
                                       ACE_TRY_ENV);
          ACE_TRY_CHECK;

          // Nothing prevents this typedef's repo id from already being
          // in the repository as another type, if it came from another
          // IDL file whose generated code is not linked to the generated
          // code from this IDL file. So we check here before we make a
          // call on ir_current_.
          if (CORBA::is_nil (this->ir_current_.in ()))
            {
              ACE_ERROR_RETURN ((
                  LM_ERROR,
                  ACE_TEXT ("(%N:%l) ifr_adding_visitor::visit_typedef -")
                  ACE_TEXT (" typedef %s, in IDL file %s,  already entered")
                  ACE_TEXT (" in repository as a different type\n"),
                  node->full_name (),
                  be_global->filename ()
                ),
                -1
              );
            }
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           ACE_TEXT ("visit_typedef"));

      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

int 
ifr_adding_visitor::visit_root (AST_Root *node)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      CORBA_Container_var new_scope = 
        CORBA_Container::_narrow (be_global->repository (),
                                  ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (be_global->ifr_scopes ().push (new_scope.in ()) != 0)
        {
          ACE_ERROR_RETURN ((
              LM_ERROR,
              ACE_TEXT ("(%N:%l) ifr_adding_visitor::visit_root -")
              ACE_TEXT (" scope push failed\n")
            ),  
            -1
          );
        }

      if (this->visit_scope (node) == -1)
        {
          ACE_ERROR_RETURN ((
              LM_ERROR,
              ACE_TEXT ("(%N:%l) ifr_adding_visitor::visit_root -")
              ACE_TEXT (" visit_scope failed\n")
            ),  
            -1
          );
        }

      CORBA_Container_ptr tmp = CORBA_Container::_nil ();

      if (be_global->ifr_scopes ().pop (tmp) != 0)
        {
          ACE_ERROR_RETURN ((
              LM_ERROR,
              ACE_TEXT ("(%N:%l) ifr_adding_visitor::visit_root -")
              ACE_TEXT (" scope pop failed\n")
            ),  
            -1
          );
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           ACE_TEXT ("visit_root"));

      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

int 
ifr_adding_visitor::visit_native (AST_Native *node)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      CORBA_Contained_var prev_def =
        be_global->repository ()->lookup_id (node->repoID (),
                                             ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (prev_def.in ()))
        {
          CORBA_Container_ptr current_scope = CORBA_Container::_nil ();

          if (be_global->ifr_scopes ().top (current_scope) == 0)
            {
              this->ir_current_ =
                current_scope->create_native (
                    node->repoID (),
                    node->local_name ()->get_string (),
                    this->gen_version (node),
                    ACE_TRY_ENV
                  );
              ACE_TRY_CHECK;
            }
          else
            {
              ACE_ERROR_RETURN ((
                  LM_ERROR,
                  ACE_TEXT ("(%N:%l) ifr_adding_visitor::visit_native -")
                  ACE_TEXT (" scope stack is empty\n")
                ),  
                -1
              );
            }
        }
      else
        {
          this->ir_current_ =
            CORBA_NativeDef::_narrow (prev_def.in (),
                                      ACE_TRY_ENV);
          ACE_TRY_CHECK;

          // Nothing prevents this native type's repo id from already being
          // in the repository as another type, if it came from another
          // IDL file whose generated code is not linked to the generated
          // code from this IDL file. So we check here before we make a
          // call on ir_current_.
          if (CORBA::is_nil (this->ir_current_.in ()))
            {
              ACE_ERROR_RETURN ((
                  LM_ERROR,
                  ACE_TEXT ("(%N:%l) ifr_adding_visitor::visit_native -")
                  ACE_TEXT (" native type %s, in IDL file %s,  ")
                  ACE_TEXT ("already entered")
                  ACE_TEXT (" in repository as a different type\n"),
                  node->full_name (),
                  be_global->filename ()
                ),
                -1
              );
            }
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           ACE_TEXT ("visit_native"));

      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

CORBA::PrimitiveKind 
ifr_adding_visitor::expr_type_to_pkind (AST_Expression::ExprType et)
{
  switch (et)
    {
    case AST_Expression::EV_short:
      return CORBA::pk_short;
    case AST_Expression::EV_ushort:
      return CORBA::pk_ushort;
    case AST_Expression::EV_long:
      return CORBA::pk_long;
    case AST_Expression::EV_ulong:
      return CORBA::pk_ulong;
    case AST_Expression::EV_longlong:
      return CORBA::pk_longlong;
    case AST_Expression::EV_ulonglong:
      return CORBA::pk_ulonglong;
    case AST_Expression::EV_float:
      return CORBA::pk_float;
    case AST_Expression::EV_double:
      return CORBA::pk_double;
    case AST_Expression::EV_longdouble:
      return CORBA::pk_longdouble;
    case AST_Expression::EV_char:
      return CORBA::pk_char;
    case AST_Expression::EV_wchar:
      return CORBA::pk_wchar;
    case AST_Expression::EV_octet:
      return CORBA::pk_octet;
    case AST_Expression::EV_bool:
      return CORBA::pk_boolean;
    case AST_Expression::EV_string:
      return CORBA::pk_string;
    case AST_Expression::EV_wstring:
      return CORBA::pk_wstring;
    case AST_Expression::EV_any:
      return CORBA::pk_any;
    case AST_Expression::EV_void:
      return CORBA::pk_void;
    case AST_Expression::EV_none:
      return CORBA::pk_null;
    default:
      return CORBA::pk_null;
    }
}

CORBA::PrimitiveKind 
ifr_adding_visitor::predefined_type_to_pkind (AST_PredefinedType *node)
{
  switch (node->pt ())
    {
    case AST_PredefinedType::PT_short:
      return CORBA::pk_short;
    case AST_PredefinedType::PT_ushort:
      return CORBA::pk_ushort;
    case AST_PredefinedType::PT_long:
      return CORBA::pk_long;
    case AST_PredefinedType::PT_ulong:
      return CORBA::pk_ulong;
    case AST_PredefinedType::PT_longlong:
      return CORBA::pk_longlong;
    case AST_PredefinedType::PT_ulonglong:
      return CORBA::pk_ulonglong;
    case AST_PredefinedType::PT_float:
      return CORBA::pk_float;
    case AST_PredefinedType::PT_double:
      return CORBA::pk_double;
    case AST_PredefinedType::PT_longdouble:
      return CORBA::pk_longdouble;
    case AST_PredefinedType::PT_char:
      return CORBA::pk_char;
    case AST_PredefinedType::PT_wchar:
      return CORBA::pk_wchar;
    case AST_PredefinedType::PT_octet:
      return CORBA::pk_octet;
    case AST_PredefinedType::PT_boolean:
      return CORBA::pk_boolean;
    case AST_PredefinedType::PT_any:
      return CORBA::pk_any;
    case AST_PredefinedType::PT_void:
      return CORBA::pk_void;
    case AST_PredefinedType::PT_pseudo:
      {
        const char *local_name = node->local_name ()->get_string ();

        if (!ACE_OS::strcmp (local_name, "Object"))
          {
            return CORBA::pk_objref;
          }
        else if (!ACE_OS::strcmp (local_name, "Principal"))
          {
            return CORBA::pk_Principal;
          }
        else
          {
            return CORBA::pk_TypeCode;
          }
      }
    default:
      return CORBA::pk_null;
    }
}

void 
ifr_adding_visitor::load_any (AST_Expression::AST_ExprValue *ev,
                              CORBA::Any &any)
{
  switch (ev->et)
    {
    case AST_Expression::EV_short:
      any <<= ev->u.sval;
      break;
    case AST_Expression::EV_ushort:
      any <<= ev->u.usval;
      break;
    case AST_Expression::EV_long:
      any <<= ACE_static_cast (CORBA::Long, ev->u.lval);
      break;
    case AST_Expression::EV_ulong:
      any <<= ACE_static_cast (CORBA::ULong, ev->u.ulval);
      break;
#if !defined (ACE_LACKS_LONGLONG_T)
    case AST_Expression::EV_longlong:
      any <<= ev->u.llval;
      break;
    case AST_Expression::EV_ulonglong:
      any <<= ev->u.ullval;
      break;
#endif /* !defined (ACE_LACKS_LONGLONG_T) */
    case AST_Expression::EV_float:
      any <<= ev->u.fval;
      break;
    case AST_Expression::EV_double:
      any <<= ev->u.dval;
      break;
#if 0
    case AST_Expression::EV_longdouble:
    // Not implemented in IDL compiler.
#endif      
    case AST_Expression::EV_char:
      any <<= CORBA::Any::from_char (ev->u.cval);
      break;
    case AST_Expression::EV_wchar:
      any <<= CORBA::Any::from_wchar (ev->u.wcval);
      break;
    case AST_Expression::EV_octet:
      any <<= CORBA::Any::from_octet (ev->u.oval);
      break;
    case AST_Expression::EV_bool:
      any <<= CORBA::Any::from_boolean ((CORBA::Boolean) ev->u.bval);
      break;
    case AST_Expression::EV_string:
      any <<= ev->u.strval->get_string ();
      break;
    case AST_Expression::EV_wstring:
      {
        char *str = ev->u.wstrval;
        int len = ACE_OS::strlen (str);
        CORBA::WChar *wstr = 0;
        ACE_NEW (wstr,
                 CORBA::WChar[len + 1]);

        for (int i = 0; i < len; ++i)
          {
            wstr[i] = ACE_static_cast (CORBA::WChar, str[i]);
          }

        wstr[len] = 0;
        any <<= wstr;
        delete wstr;
        break;
      }
    default:
      break;
    }
}

// This may get more elaborate someday.
const char *
ifr_adding_visitor::gen_version (AST_Decl *)
{
  return "1.0";
}

void 
ifr_adding_visitor::element_type (AST_Type *base_type,
                                  CORBA::Environment &ACE_TRY_ENV)
{
  if (base_type->anonymous ())
    {
      if (base_type->ast_accept (this) == -1)
        {
          ACE_ERROR ((
              LM_ERROR,
              ACE_TEXT ("(%N:%l) ifr_adding_visitor::element_type -")
              ACE_TEXT (" failed to accept visitor\n")
            ));
        }
    }
  else
    {
      CORBA_Contained_var contained = 
        be_global->repository ()->lookup_id (base_type->repoID (),
                                             ACE_TRY_ENV);
      ACE_CHECK;

      if (CORBA::is_nil (contained.in ()))
        {
          ACE_ERROR ((
              LM_ERROR,
              ACE_TEXT ("(%N:%l) ifr_adding_visitor::element_type -")
              ACE_TEXT (" lookup_id failed\n")
            ));
        }

      this->ir_current_ = CORBA_IDLType::_narrow (contained.in (),
                                                  ACE_TRY_ENV);
      ACE_CHECK;
    }
}

void
ifr_adding_visitor::redef_warning (AST_Decl *node)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Warning: there is already an entry in the")
              ACE_TEXT (" repository with this type and name %s. If this")
              ACE_TEXT (" is because of a forward declaration in another")
              ACE_TEXT (" IDL file, ignore this warning. Otherwise,remove")
              ACE_TEXT (" this IDL file's contents from the repository")
              ACE_TEXT (" with 'tao_ifr -r %s'\n"),
              node->full_name (),
              be_global->filename ()));
}

