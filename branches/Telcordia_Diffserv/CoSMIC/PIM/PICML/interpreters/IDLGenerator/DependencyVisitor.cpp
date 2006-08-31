#include "stdafx.h"
#include "DependencyVisitor.h"

#define DEPENDS_DEBUG 1

#if (DEPENDS_DEBUG == 1)
#include "IDLStream.h"
#include <fstream>
#endif

namespace IDML
{
  void DependencyVisitor::visitOrderableImpl( const Orderable& object )
  {
    this->update_depends_lists (object);

    // If we're not a model, we're done.
    BON::Model model (object);
    if (!model) return;

    Component comp (object);

    if (comp
        || ComponentFactory (object)
        || Event (object))
      {
        this->set_include_components_idl (object);
      }

    std::set<BON::FCO>& children = model->getChildFCOs ();

    for (std::set<BON::FCO>::const_iterator it = children.begin ();
         it != children.end ();
         it++)
      {
        Orderable child (*it);
        if (!child) continue;
        object->ordered_children.push_back (child);
        this->visitOrderableImpl (child);
      }

    if (comp)
      {
        BON::FCOPtr spFCO;
        COMTHROW (comp->getFCOI ()->get_DerivedFrom (spFCO.Addr ()));

        if (spFCO)
          {
            Component base (BON::FCOImpl::attach (spFCO));
            if (!base) return;
            comp->base_component (base);

            // Just insert the derived component into the
            // depends_on_me container of the base. Since
            // this is an inheritance relationship, we won't
            // have any problems with incomplete types in
            // the IDL, so we can just call order_children()
            // as usual.
            base->depends_on_me.insert (comp);
          }
      }
  }

  void DependencyVisitor::set_include_components_idl( const BON::Model& mod )
  {
    File container =
      this->get_containing_file (mod->getParentModel ());
    container->include_components_idl (true);
  }

  void DependencyVisitor::set_include_orb_idl( const BON::Reference& ref )
  {
    File container =
      this->get_containing_file (ref->getParentModel ());
    container->include_orb_idl (true);
  }

  void DependencyVisitor::update_depends_lists( const Orderable& object )
  {
    BON::Model scope (object);
    if (!scope) return;
    std::set<BON::Reference> refs = scope->getChildReferences ();

    ComponentFactory cf (object);
    if (cf)
      {
        std::multiset<Manageable> manages = cf->getManagesComponentDsts ();
        std::multiset<Manageable>::iterator i = manages.begin ();
        Component comp (*i);

        if (comp)
          {
            comp->depends_on_me.insert (object);
            Orderable base = comp->base_component ();

            if (base)
              {
                // Maybe the base component is in another File.
                this->discover_include (comp, base);
              }
          }
        else
          {
            refs.insert (ComponentRef (*i));
          }
      }

#if (DEPENDS_DEBUG == 1)
    std::string filename (object->getName ());
    filename += "_Log_Pre";
    std::ofstream tmp (filename.c_str ());
    IDLStream dbg (tmp);
#endif

    for (std::set<BON::Reference>::iterator it = refs.begin ();
         it != refs.end ();
         it++)
      {
        BON::FCO orig_obj = (*it)->getReferred ();
        BON::FCO target_obj (orig_obj);
        if (!target_obj) continue;
        BON::Model target_scope = target_obj->getParentModel ();

        // This will happen if we are referencing a basic type, since
        // they are all in a folder in an attached library.
        if (!target_scope)
          {
            if (TypeEncoding (orig_obj) || TypeKind (orig_obj))
              this->set_include_orb_idl (*it);
            continue;
          }

        // To discover a necessary include that may not be specified
        // in the model.
        this->discover_include (object, orig_obj);

        // To discover if a Component is connected to a ComponentFactory
        // via a ComponentRef.
        this->discover_ref_manage (*it);

        // If the reference is to something that maps to IDL interface,
        // valuetype, eventtype or component, we can just forward declare,
        // unless the reference is used for a parent or a supported
        // interface (which usually maps to inheritance) in which case
        // we must know the full type.
        bool can_fwd_declare = ((Object (orig_obj)
                                || Component (orig_obj)
                                || ObjectByValue (orig_obj))
                                && !Inherits (*it)
                                && !Supports (*it));

        // If the BON::Reference *it is also an Orderable, then we're
        // trying to match *it, not its scope.
        BON::FCO try_obj;
        if (Orderable (*it)) try_obj = *it;
        else try_obj = (*it)->getParentModel ();
        BON::Model try_scope = try_obj->getParentModel ();

#if (DEPENDS_DEBUG == 1)
        dbg << "child_ref: " << (*it)->getName () << nl
            << "target_obj: " << orig_obj->getName () << nl
            << "target_scope: " << target_scope->getName () << nl
            << "try_obj: " << try_obj->getName () << nl
            << "try_scope: " << try_scope->getName () << nl << nl;
#endif

        // We are looking for the common scope of dependency and target,
        // since it is the order of the scope's members that is affected.
        // The common scope may actually be a container of the target scope,
        // so we try each containing scope, if we don't already have a match.
        int nbumps = 0;
        bool match = false;
        while (target_scope)
          {
            match = this->match_scopes (target_obj,
                                        target_scope,
                                        try_obj,
                                        try_scope);

            if (match)
              {
                if (can_fwd_declare)
                  {
                    std::pair<Orderable, int> data (Orderable (orig_obj),
                                                    nbumps);
                    Orderable (target_scope)->fwd_decl_children.insert (data);
#if (DEPENDS_DEBUG == 1)
                    dbg << orig_obj->getName () << " fwd declared"
                        << nl << nl;
#endif
                  }
                else
                  {
                    Orderable (target_obj)->depends_on_me.insert (try_obj);
#if (DEPENDS_DEBUG == 1)
                    dbg << try_obj->getName () << " depends on "
                        << target_obj->getName () << nl << nl;
#endif
                  }

                break;
              }

            // Passed by reference, so we must reset.
            try_obj = (*it)->getParentModel ();

            target_obj = target_scope;
            target_scope = target_scope->getParentModel ();
            ++nbumps;
          }
      }
  }

  bool DependencyVisitor::match_scopes( BON::FCO target_obj,
                                        BON::Model target_scope,
                                        BON::FCO& try_obj,
                                        BON::Model try_scope )
  {
    // Expand the scope until we get a match or can't expand further.
    while (try_scope && try_scope != target_scope)
      {
        try_obj = try_scope;
        try_scope = try_scope->getParentModel ();
      }

    // If we have not expanded the scope all the way out, we must
    // have a match.
    if (try_scope)
      {
        return true;
      }

    return false;
  }

  File DependencyVisitor::get_containing_file( const BON::FCO& fco )
  {
    BON::FCO ancestor (fco);
    File container = File (ancestor);

    while (!container)
      {
        ancestor = ancestor->getParentModel ();
        container = File (ancestor);
      }

    return container;
  }

  void DependencyVisitor::discover_include( const Orderable& object,
                                            const BON::FCO& ref )
  {
    Orderable ref_object (ref);
    if (!ref_object) return;
    File obj_file = this->get_containing_file (object);
    File ref_file = this->get_containing_file (ref_object);

    if (obj_file != ref_file)
      {
        obj_file->discovered_includes_.insert (ref_file);
      }
  }

  void DependencyVisitor::discover_ref_manage( const BON::Reference& ref )
  {
    ComponentRef cr (ref);
    if (!cr) return;
    std::multiset<ComponentFactory> cf = cr->getManagesComponentSrcs ();
    if (cf.size () > 0) cr->getComponent ()->ref_managed (true);
  }
} // namespace IDML

