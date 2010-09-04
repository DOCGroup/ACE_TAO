// $Id$

#include "XML_Typedefs.h"
#include "SatisfierProperty_Handler.h"
#include "DD_Handler.h"
#include "Any_Handler.h"
#include "Deployment.hpp"
#include "DAnCE/Deployment/Deployment_TargetDataC.h"
#include "DAnCE/Logger/Log_Macros.h"

using namespace std;

namespace CIAO
{
  namespace Config_Handlers
  {
    /*
    DD_Handler::DD_Handler (const ACE_TCHAR *file) :
      idl_domain_(0),
      domain_ptr_ (0),
      retval_ (false)
    {
      DANCE_TRACE("DP_PCD_Handler::constructor");

      XERCES_CPP_NAMESPACE::DOMDocument *dom =
        XML_Helper::XML_HELPER.create_dom (file);

      if (!dom)
        throw DD_Handler::NoDomain ();

      Domain *dm =
        new Domain (reader::domain (dom));

      //      Domain d = domain (dom);

      this->domain_ptr_.reset (dm);

      if (!this->build_domain ())
        throw NoDomain ();
    }
    */

    DD_Handler::DD_Handler (Domain *dmn):
      idl_domain_(0),
      domain_ptr_ (dmn),
      domain_ (*dmn),
      retval_(false)
    {
      DANCE_TRACE("DP_PCD_Handler::constructor - Domain");
      if(!this->build_domain ())
        throw NoDomain ();
    }

    DD_Handler::DD_Handler (Domain &dmn):
      idl_domain_(0),
      domain_ (dmn),
      retval_(false)
    {
      DANCE_TRACE("DP_PCD_Handler::constructor - Domain");
      if(!this->build_domain ())
        throw NoDomain ();
    }
    
    /*
    DD_Handler::DD_Handler (::Deployment::Domain *dmn):
      idl_domain_(dmn),
      domain_ptr_ (dmn),
      retval_(false)
    {
      if(!this->build_xsc())
        throw NoDomain ();
    }
    */

    DD_Handler::~DD_Handler (void)
    {
    }

    bool
    DD_Handler::build_domain ()
    {
      DANCE_TRACE("DP_PCD_Handler::build_domain");
      this->idl_domain_.reset ( new ::Deployment::Domain );

      // Read in the name
      // Check if the label is there or not
      if (domain_.label_p ())
        this->idl_domain_->label =
          CORBA::string_dup (ACE_TEXT_ALWAYS_CHAR (domain_.label ().c_str ()));

      if (domain_.UUID_p ())
        this->idl_domain_->UUID =
          CORBA::string_dup (ACE_TEXT_ALWAYS_CHAR (domain_.UUID ().c_str ()));

      CORBA::ULong len = domain_.count_node ();
      this->idl_domain_->node.length (len);

      //Resource _resource;
      int i =0;
      for (Domain::node_const_iterator iter = domain_.begin_node ();
           iter != domain_.end_node ();
           ++iter,++i
           )
        {
          // Node* thisNode = (Node*)iter;
          this->idl_domain_->node[i].name =
            CORBA::string_dup (ACE_TEXT_ALWAYS_CHAR ((*iter)->name ().c_str ()));

          if ((*iter)->label_p ())
            this->idl_domain_->node[i].label =
              CORBA::string_dup (ACE_TEXT_ALWAYS_CHAR ((*iter)->label ().c_str ()));

          // length is hard-coded for now ...
          // here iterate over the resources ...
          CORBA::ULong res_len = (*iter)->count_resource ();
          this->idl_domain_->node[i].resource.length (res_len);

          int res_id=0;
          // FOR EACH RESOURCE ....
          for (Node::resource_const_iterator res_iter = (*iter)->begin_resource ();
               res_iter != (*iter)->end_resource ();
               res_iter++, res_id++)
            {

              this->idl_domain_->node[i].resource[res_id].name =
                CORBA::string_dup (ACE_TEXT_ALWAYS_CHAR ((*res_iter)->name ().c_str ()));

              CORBA::ULong resource_type_len = (*res_iter)->count_resourceType ();
              this->idl_domain_->node[i].resource[res_id].resourceType.length (resource_type_len);

              int res_type_id =0;

              // FOR EACH RESOURCE TYPE
              for (Resource::resourceType_const_iterator res_type_iter =
                     (*res_iter)->begin_resourceType ();
                   res_type_iter != (*res_iter)->end_resourceType ();
                   res_type_iter++, res_type_id++)
                {
                  this->idl_domain_->node[i].resource[res_id].resourceType[res_type_id] =
                    CORBA::string_dup (ACE_TEXT_ALWAYS_CHAR ((*res_type_iter)->c_str ()));
                }

              CORBA::ULong property_len = (*res_iter)->count_property ();
              this->idl_domain_->node[i].resource[res_id].property.length (property_len);

              //              int property_id =0;

              std::for_each ((*res_iter)->begin_property (),
                             (*res_iter)->end_property (),
                             SatisfierProperty_Functor (this->idl_domain_->node[i].resource[res_id].property));

              /*
              //FOR EACH PROPERTY
              for (Resource::property_const_iterator property_iter =
              (*res_iter)->begin_property ();
              property_iter != (*res_iter)->end_property ();
              property_iter++, property_id++)
              {
              this->idl_domain_->node[i].resource[res_id].property[property_id ].name =
              CORBA::string_dup (ACE_TEXT_ALWAYS_CHAR (property_iter->name ().c_str ()));


              this->idl_domain_->node[i].resource[res_id].property[property_id].kind =
              ::Deployment::Quantity;

              Any the_any = property_iter->value ();
              CORBA::Any a_corba_any;
              Any_Handler::extract_into_any (the_any , a_corba_any);
              this->idl_domain_->node[i].resource[res_id].property[property_id].value
              = a_corba_any;
              }
              */
            }
        }

      return true;

    }

    bool
    DD_Handler::build_xsc ()
    {
      return true;
    }

    ::Deployment::Domain const *
    DD_Handler::domain_idl () const
    {
      if(!this->idl_domain_.get())
        throw NoDomain ();

      //else
      return this->idl_domain_.get();
    }

    ::Deployment::Domain *
    DD_Handler::domain_idl ()
    {
      if(!this->idl_domain_.get())
        throw NoDomain();

      //else
      return this->idl_domain_.release();
    }

    Domain const *
    DD_Handler::domain_xsc () const
    {
      if(!this->domain_ptr_.get())
        throw NoDomain ();

      //else
      return this->domain_ptr_.get();
    }

    Domain *
    DD_Handler::domain_xsc ()
    {
      if(!this->domain_ptr_.get())
        throw NoDomain ();

      //else
      return this->domain_ptr_.release();
    }
  }
}

