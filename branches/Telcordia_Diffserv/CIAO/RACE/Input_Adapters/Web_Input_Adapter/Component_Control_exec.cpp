/**
 *  @file
 *  @ingroup Component_Control
 *  @brief  C++ implementation of the Component_Control facet
 *          provided by Web_Input_Adapter.

 *  @copydoc Component_Control
 *  $Id$
 */

#include "Component_Control_exec.h"
#include "Web_Input_Adapter_exec.h"
#include "tao/DynamicInterface/Request.h"
#include "tao/IFR_Client/IFR_BasicC.h"
#include "tao/IFR_Client/IFR_ComponentsC.h"
#include "ace/Bound_Ptr.h"
#include <map>
#include <string>
#include <vector>

namespace CIAO
{
  namespace RACE
  {
    namespace CIDL_Web_Input_Adapter_Impl
    {
      /// Pimpl helper class.
      /**
       *  Fulfills C++ "pimpl" idiom for certain functional aspects of
       *  main class Component_Control_exec_i.
       */
      class Component_Control_exec_i::Impl
      {
      private:
        Web_Input_Adapter_exec_i & component_;
        CORBA::Boolean is_initialized_;

        typedef std::map <std::string,
                          std::pair <Components::CCMObject_var,
                                     std::multimap <std::string,
                                                    std::string> > >
        COMPONENT_MAP;

        COMPONENT_MAP component_map_;
        std::string root_name_;

        static const int GET_METHOD_MAX_LENGTH = 1024;

      public:
        Impl (Web_Input_Adapter_exec_i & component)
          : component_ (component),
            is_initialized_ (0)
        {
          // constructor does nothing
        }

        ::CORBA::Boolean
        is_initialized ()
          throw (::CORBA::SystemException)
        {
          return is_initialized_;
        }

      private:

        char * make_component_name (::Components::CCMObject_ptr comp)
        {
          CORBA::String_var repo_id;
          CORBA::String_var name;

          repo_id = comp->_repository_id ();
          char * slash = ACE_OS::strrchr (repo_id.inout (), '/');
          name = CORBA::string_dup (++slash);
          *(ACE_OS::strrchr (name.inout (), ':')) = 0;
          return name._retn ();
        }

        void init_component_tree_impl (::Components::CCMObject_ptr root,
                                       std::string & root_name)
        {
          std::multimap <std::string, std::string> receptacle_map;
          Components::ReceptacleDescriptions_var receptacles =
            root->get_all_receptacles ();

          for (CORBA::ULong i = 0, l = receptacles->length (); i < l; ++i)
          {
            CORBA::String_var receptacle_name_var = receptacles[i]->name ();
            std::string receptacle_name (receptacle_name_var);
            Components::ConnectionDescriptions & connections = receptacles[i]->connections ();
            for (int j = 0, l = connections.length (); j < l; ++j)
            {
              CORBA::Object_ptr facet = connections[i]->objref ();

              CORBA::Object_var possible_child = facet->_get_component ();
              if (! CORBA::is_nil (possible_child.in ()))
              {
                Components::CCMObject_var child =
                  Components::CCMObject::_narrow (possible_child.in ());
                if (! CORBA::is_nil (child.in ()))
                {
                  CORBA::String_var child_name_var =
                    this->make_component_name (child.inout ());
                  std::string child_name (child_name_var.in ());
                  receptacle_map.insert
                    (std::make_pair (receptacle_name,
                                     child_name));
                  if (this->component_map_.find (child_name) ==
                      this->component_map_.end ())
                    this->init_component_tree_impl (child.inout (),
                                                    child_name);
                }
              }
            }
          }

          Components::CCMObject_var root_duplicate =
            Components::CCMObject::_duplicate (root);

          const COMPONENT_MAP::value_type & component_map_entry =
            std::make_pair (root_name,
                            std::make_pair (root_duplicate,
                                            receptacle_map));

          ACE_DEBUG ((LM_DEBUG,
                      "{\n"
                      "  name   = %s\n"
                      "  objref = %@\n"
                      "  child refs = {\n", root_name.c_str (), root_duplicate.ptr ()));
          for (std::multimap <std::string, std::string> ::iterator
                 i (receptacle_map.begin ()),
                 e (receptacle_map.end ());
               i != e;
               ++i)
          {
            ACE_DEBUG ((LM_DEBUG,
                        "    %s => %s\n",
                        i->first.c_str (),
                        i->second.c_str ()));
          }
          ACE_DEBUG ((LM_DEBUG,
                      "  }\n"
                      "}\n"));

          this->component_map_.insert (component_map_entry);
        }

      public:

        void
        init_component_tree (::Components::CCMObject_ptr root)
          throw (::CORBA::SystemException)
        {
          CORBA::String_var root_name_var = this->make_component_name (root);
          std::string root_name (root_name_var.in ());
          this->init_component_tree_impl (root,
                                          root_name);
          this->root_name_ = root_name;
          this->is_initialized_ = true;
        }

      private:

        void
        build_component_tree (RACE::Component_Tree_Node & parent_node,
                              const std::string & parent_name)
        {
          typedef
            std::vector <std::pair <std::string, std::string> >
            ACCUM_VEC;

          // Allocate a CCMObject_var to hold the parent component.
          Components::CCMObject_var parent_component;

          // Allocate a vector to hold the child components.  Children
          // are grouped according to which receptacle they're
          // reachable from.
          std::vector <ACCUM_VEC> connections;

          // Populate the above variables.
          {
            ACE_DEBUG ((LM_DEBUG, "Looking for %s...\n", parent_name.c_str ()));

            COMPONENT_MAP::iterator finder =
              this->component_map_.find (parent_name);

            ACE_ASSERT (finder != this->component_map_.end ());
            ACE_ASSERT (finder->second.first.in () != Components::CCMObject::_nil ());
            parent_component = finder->second.first;

            std::multimap <std::string, std::string> & receptacle_mappings =
              finder->second.second;

            for (std::multimap <std::string, std::string> ::iterator
                   i (receptacle_mappings.begin ()),
                   e (receptacle_mappings.end ());
                 i != e;)
            {
              std::pair
                <std::multimap <std::string, std::string>::iterator,
                 std::multimap <std::string, std::string>::iterator>
                bounds = receptacle_mappings.equal_range (i->first);

              ACCUM_VEC accum_vec;
              std::copy (bounds.first,
                         bounds.second,
                         std::back_inserter (accum_vec));

              connections.push_back (accum_vec);

              i = bounds.second;
            }
          }

          // Fill in component data (name and objref)
          RACE::Component_Data component_data;
          component_data.component_name = CORBA::string_dup (parent_name.c_str ());
          component_data.the_component = parent_component;

          // Plug component data into the union member of our tree node.
          parent_node.node_data.the_component_data (component_data);

          RACE::Component_Tree & receptacles = parent_node.children;
          receptacles.length (connections.size ());
          for (CORBA::ULong i = 0, l = receptacles.length ();
               i < l;
               ++i)
          {
            // Fill in connection data (receptacle name)
            RACE::Connection_Data connection_data;
            connection_data.receptacle_name =
              CORBA::string_dup (connections [i] [0].first.c_str ());

            // Plug connection data into the union member of a new tree node
            RACE::Component_Tree_Node_var connection_node (new RACE::Component_Tree_Node);
            connection_node->node_data.the_connection_data (connection_data);

            // Iterate over all the child components connected to this receptacle
            RACE::Component_Tree & child_nodes = connection_node->children;
            child_nodes.length (connections [i].size ());
            for (CORBA::ULong j = 0, l = child_nodes.length ();
                 j < l;
                 ++j)
            {
              // Obtain name of the child component.
              std::string & child_name =
                connections [i] [j].second;

              ACE_DEBUG ((LM_DEBUG,
                          "receptacle_name = %s; child_name = %s.\n",
                          connections [i] [j].first.c_str (),
                          connections [i] [j].second.c_str ()));

              // Recursively call build_component_tree on the child component.
              RACE::Component_Tree_Node_var child_node
                (new RACE::Component_Tree_Node);
              this->build_component_tree (child_node.inout (), child_name);

              // Add child name to the CORBA data structure.
              child_nodes [j] = child_node;
            }

            // Add connection node to the CORBA data structure.
            receptacles [i] = connection_node;
          }
        }

      public:

        RACE::Component_Tree_Node *
        get_component_tree ()
          throw (::CORBA::SystemException,
                 TreeNotInitialized)
        {
          if (this->is_initialized_ == false)
            throw TreeNotInitialized ();

          RACE::Component_Tree_Node_var root_node (new RACE::Component_Tree_Node);
          this->build_component_tree (root_node.inout (),
                                      this->root_name_);

          return root_node._retn ();
        }

        RACE::AttbuteSeq *
        get_component_attributes (const char* component_name)
        throw (::CORBA::SystemException,
               TreeNotInitialized,
               NoSuchComponent)
        {
          CORBA::Object_var obj =
            this->component_.orb_->resolve_initial_references ("InterfaceRepository"
                                                               ACE_ENV_ARG_PARAMETER);

          CORBA::Repository_var repo = CORBA::Repository::_narrow (obj.in ()
                                                                   ACE_ENV_ARG_PARAMETER);

          CORBA::ContainedSeq_var candidates =
            repo->lookup_name (component_name,
                               -1,            // Unlimited level recursion.
                               CORBA::dk_all, // Any type of contained object.
                               1              // Include parents of interfaces.
                               ACE_ENV_ARG_PARAMETER);

          ACE_DEBUG ((LM_DEBUG, "**** get_component_attributes (%s)\n",
                      component_name));

          // Add base components
          for (CORBA::ULong iii = 0;
               iii < candidates->length ();
               ++iii)
          {
            ACE_DEBUG ((LM_DEBUG, "****\tcandidates[%d]", iii));
            CORBA::ComponentIR::ComponentDef_var
              component_def (CORBA::ComponentIR::ComponentDef::_nil());

            component_def = CORBA::ComponentIR::ComponentDef::_narrow
              (candidates [CORBA::ULong (iii)]);
            if (CORBA::is_nil (component_def.in ()))
              continue;

            CORBA::String_var component_id = component_def->id ();

            ACE_DEBUG ((LM_DEBUG, "\t%s", component_id.in()));

            CORBA::RepositoryIdSeq bases (1);
            bases.length (0);
            std::vector <CORBA::ComponentIR::ComponentDef_var> base_components;

            CORBA::ComponentIR::ComponentDef_var sole_base_component
              (component_def->base_component ());
            
            if (! CORBA::is_nil(sole_base_component.in ())) {
              CORBA::String_var sole_base_id (sole_base_component->id ());
              if (sole_base_id.in ()) {
                bases.length (1);
                bases [0] = sole_base_id;
                base_components.push_back (sole_base_component);
              }
            }
              
            ACE_DEBUG ((LM_DEBUG, "\t #bases = %d", bases.length ()));

            for (CORBA::ULong ii = 0, ll = bases.length ();
                 ii < ll;
                 ++ii)
            {
              if (bases [ii] == 0)
                continue;
              
              CORBA::String_var base (bases [ii]);

              ACE_DEBUG ((LM_DEBUG, "\n****\t\t(base[%d] is %s", ii, base.in()));

              int found_duplicate = 0;
              for (CORBA::ULong i = 0, l = candidates->length ();
                   i < l;
                   ++i)
              {
                CORBA::String_var candidate_id = candidates [i] -> id ();
                if (candidate_id.in () == 0)
                  continue;

                if (ACE_OS::strcmp (candidate_id.in (), base.in ()) == 0)
                {
                  found_duplicate = 1;
                  continue;
                }
              }
              if (found_duplicate)
                continue;

              CORBA::Contained_var new_candidate
                (CORBA::Contained::_duplicate (base_components[ii].in ()));
              if (! CORBA::is_nil (new_candidate.in ()))
              {
                CORBA::ULong candidates_old_length = 
                  candidates->length ();
                candidates->length (candidates_old_length + 1);
                candidates [candidates_old_length] = new_candidate;
                ACE_DEBUG ((LM_DEBUG,
                            "\n********\tRetrieved and added %s!",
                            base.in ()));
              } else {
                ACE_DEBUG ((LM_DEBUG,
                            "\n********\tCouldn't find %s!",
                            base.in ()));
              }              
            }
            ACE_DEBUG ((LM_DEBUG, "\n"));
          }

          // Create a vector of user-friendly, attribute name-value
          // pairs.
          std::vector<RACE::Attbute_var> attbute_vec;
          
          for (CORBA::ULong ii = 0, ll = candidates->length ();
               ii < ll;
               ++ii)
          {
            CORBA::InterfaceDef_var interface (CORBA::InterfaceDef::_nil());
            ACE_Strong_Bound_Ptr <CORBA::InterfaceDef::FullInterfaceDescription,
                                 ACE_Null_Mutex >
                                 interface_helper (0);

            interface = CORBA::InterfaceDef::_narrow
              (candidates [CORBA::ULong (ii)]);
            if (CORBA::is_nil (interface.in ()))
              continue;

            interface_helper.reset (interface->describe_interface ());
            if (interface_helper.get () == 0)
              continue;

            CORBA::AttrDescriptionSeq & seq = interface_helper->attributes;

            // Check that the component tree is initialized.  If not,
            // then throw TreeNotInitialized.
            if (this->is_initialized_ == false)
              throw TreeNotInitialized ();

            // See if we know about a component with the given name.  If
            // not, then throw NoSuchComponent.
            COMPONENT_MAP::iterator finder =
              this->component_map_.find (component_name);
            if (finder == this->component_map_.end ())
              throw NoSuchComponent ();

            Components::CCMObject_var & component = finder->second.first;

            // For each attribute, determine its name from the
            // AttrDescriptionSeq, then fetch its value using CORBA DII.
            // Put name and value into the user-friendly AttbuteSeq.
            char get_method[GET_METHOD_MAX_LENGTH];

            for (CORBA::ULong i = 0, l = seq.length (); i < l; ++i)
            {
              RACE::Attbute_var attbute (new RACE::Attbute);
              {
                CORBA::AttributeDescription & _attr = seq[i];
                attbute->name = _attr.name;
                ACE_OS::snprintf (get_method,
                                  GET_METHOD_MAX_LENGTH,
                                  "_get_%s",
                                  attbute->name.in ());

                CORBA::Request_ptr req (component->_request (get_method));

                ACE_TRY
                  {
                    req->set_return_type (_attr.type.in ());
                    attbute->value = req->return_value ();
                    req->invoke ();
                  }
                ACE_CATCHANY
                  {
                    req->_decr_refcnt ();
                    ACE_RE_THROW;
                  }
                ACE_ENDTRY;

                req->_decr_refcnt ();
              }
              attbute_vec.push_back (attbute);
            }

          }

          // Copy the attribute name-value pairs from attbute_vec into
          // a CORBA sequence (AttbuteSeq).

          RACE::AttbuteSeq_var retval (new RACE::AttbuteSeq (attbute_vec.size ()));
          retval->length (attbute_vec.size ());
          for (CORBA::ULong i = 0, l = retval->length (); i < l; ++i)
          {
            retval [i] = attbute_vec [i];
          }

          return retval._retn ();
        }

        void
        set_component_attribute (const char *,
                                 const RACE::Attbute &)
          throw (::CORBA::SystemException,
                 TreeNotInitialized,
                 NoSuchComponent)
        {
          if (this->is_initialized_ == false)
            throw TreeNotInitialized ();

          // TODO implementation
        }
      };
    }
  }
}

namespace CIDL_Web_Input_Adapter_Impl = CIAO::RACE::CIDL_Web_Input_Adapter_Impl;

CIDL_Web_Input_Adapter_Impl::Component_Control_exec_i::Component_Control_exec_i
(Web_Input_Adapter_exec_i & component)
  : impl_ (new Component_Control_exec_i::Impl (component))
{
  // do nothing
}

CIDL_Web_Input_Adapter_Impl::Component_Control_exec_i::~Component_Control_exec_i ()
{
  delete this->impl_;
}

::CORBA::Boolean
CIDL_Web_Input_Adapter_Impl::Component_Control_exec_i::is_initialized
()
  throw (::CORBA::SystemException)
{
  return this->impl_->is_initialized ();
}

void
CIDL_Web_Input_Adapter_Impl::Component_Control_exec_i::init_component_tree
(::Components::CCMObject_ptr root)
  throw (::CORBA::SystemException)
{
  this->impl_->init_component_tree (root);
}

::CIAO::RACE::Component_Tree_Node *
CIDL_Web_Input_Adapter_Impl::Component_Control_exec_i::get_component_tree
()
  throw (::CORBA::SystemException,
         TreeNotInitialized)
{
  return this->impl_->get_component_tree ();
}

::CIAO::RACE::AttbuteSeq *
CIDL_Web_Input_Adapter_Impl::Component_Control_exec_i::get_component_attributes
(const char* attribute_name)
  throw (::CORBA::SystemException,
         TreeNotInitialized,
         NoSuchComponent)
{
  return this->impl_->get_component_attributes (attribute_name);
}

void
CIDL_Web_Input_Adapter_Impl::Component_Control_exec_i::set_component_attribute
(const char * component_name,
 const ::CIAO::RACE::Attbute & bute)
  throw (::CORBA::SystemException,
         TreeNotInitialized,
         NoSuchComponent)
{
  return this->impl_->set_component_attribute (component_name,
                                               bute);
}
