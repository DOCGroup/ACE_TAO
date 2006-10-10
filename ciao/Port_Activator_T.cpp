// $Id$

#ifndef CIAO_PORT_ACTIVATOR_T_CPP
#define CIAO_PORT_ACTIVATOR_T_CPP

#include "Port_Activator_T.h"
#include "tao/CORBA_String.h"

#include "ace/OS_NS_string.h"


namespace CIAO
{
  template <typename SERV,
            typename EXEC,
            typename CONTEXT,
            typename COMP_SERV>
  Port_Activator_T<SERV, EXEC, CONTEXT, COMP_SERV>::Port_Activator_T (
      const char *oid,
      const char *name,
      Port_Activator::Type t,
      EXEC *e,
      CONTEXT *c,
      COMP_SERV *cc)
    : Port_Activator (oid, name, t)
    , executor_ (e)
    , context_ (c)
    , comp_serv_ (cc)
  {
  }

  template <typename SERV,
            typename EXEC,
            typename CONTEXT,
            typename COMP_SERV>
  void
  Port_Activator_T<SERV, EXEC, CONTEXT, COMP_SERV>::deactivate (
      const PortableServer::Servant servant
      ACE_ENV_ARG_DECL)
  {
    SERVANT *s = dynamic_cast<SERVANT *> (servant);
    s->_remove_ref (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;
  }

  template <typename SERV,
            typename EXEC,
            typename CONTEXT,
            typename COMP_SERV>
  PortableServer::Servant
  Port_Activator_T<SERV, EXEC, CONTEXT, COMP_SERV>::activate (
      const PortableServer::ObjectId &oid
      ACE_ENV_ARG_DECL)
  {
    CORBA::String_var str =
      PortableServer::ObjectId_to_string (oid);

    // An additional check, may not be necessary. Being on the safe
    // side.
    if (ACE_OS::strcmp (this->oid_.in (),
                        str.in ()) == 0)
      {
        if (this->executor_ == 0 &&
            this->t_ == Port_Activator::Facet)
          {
            CORBA::Object_var tmp =
              this->comp_serv_->get_facet_executor (this->name_.in ());

            this->executor_ = EXEC::_narrow (tmp.in ());
          }

        SERVANT *s = 0;

        ACE_NEW_THROW_EX (s,
                          SERVANT (this->executor_,
                                   this->context_),
                          CORBA::NO_MEMORY ());
        return s;
      }

    ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (),
                      0);
  }
}

#endif /*SERVANT_ACTIVATOR_T_CPP*/
