// $Id$

#ifndef CIAO_UPGRADEABLE_CONTEXT_IMPL_T_C
#define CIAO_UPGRADEABLE_CONTEXT_IMPL_T_C

#include "Upgradeable_Context_Impl_T.h"

#include "ciao/Servant_Activator.h"

namespace CIAO
{
  template <typename BASE_CTX,
            typename SVNT,
            typename COMP>
  Upgradeable_Context_Impl<BASE_CTX, SVNT, COMP>::Upgradeable_Context_Impl (
    Components::CCMHome_ptr the_home,
    Session_Container *c,
    SVNT *sv)
    : Context_Impl<BASE_CTX, SVNT, COMP> (the_home, c, sv)
  {
  }

  template <typename BASE_CTX,
            typename SVNT,
            typename COMP>
  Upgradeable_Context_Impl<BASE_CTX, SVNT, COMP>::~Upgradeable_Context_Impl (void)
  {
  }

  template <typename BASE_CTX,
            typename SVNT,
            typename COMP>
  void
  Upgradeable_Context_Impl<BASE_CTX, SVNT, COMP>::deactivate_facet (
      const PortableServer::ObjectId &oid)
  {
    this->container_->deactivate_facet (oid);
  }

  template <typename BASE_CTX,
            typename SVNT,
            typename COMP>
  void
  Upgradeable_Context_Impl<BASE_CTX, SVNT, COMP>::remove_facet (
      CORBA::Object_ptr reference)
  {
    PortableServer::ObjectId_var oid =
      this->container_->the_facet_cons_POA ()->reference_to_id (reference);

    this->update_port_activator (oid.in ());

    this->deactivate_facet (oid.in ());
  }

  template <typename BASE_CTX,
            typename SVNT,
            typename COMP>
  void
  Upgradeable_Context_Impl<BASE_CTX, SVNT, COMP>::update_port_activator (
      const PortableServer::ObjectId &oid)
  {
    CIAO::Servant_Activator *sa =
      this->container_->ports_servant_activator ();
    sa->update_port_activator (oid);
  }

}

#endif /* CIAO_UPGRADEABLE_CONTEXT_IMPL_T_C */
