// $Id$

#ifndef NT_CONNECTOR_T
#define NT_CONNECTOR_T

// connector template for Typed_NonTopic_Test_1
template <typename CCM_TYPE,
          typename T, bool FIXED_T,
          typename U, bool FIXED_U,
          typename V, bool FIXED_V,
          typename Tseq,
          typename Useq,
          typename VSeq>
class NT_Connector_T
 : public virtual CCM_TYPE::base_type
{
public:
    NT_Connector_T (void);

    virtual void set_session_context (Components::SessionContext_ptr ctx);

    virtual void configuration_complete (void);
    virtual void ccm_remove (void);
    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
};


// connector template for Typed_NonTopic_Test_2 and Typed_NonTopic_Test_3
template <typename CCM_TYPE,
          typename T, bool FIXED_T,
          typename U, bool FIXED_U,
          typename Tseq,
          typename Useq>
class NT_2_Connector_T
: public virtual CCM_TYPE::base_type
{
public:
   NT_2_Connector_T (void);

   virtual void set_session_context (Components::SessionContext_ptr ctx);

   virtual void configuration_complete (void);
   virtual void ccm_remove (void);
   virtual void ccm_activate (void);
   virtual void ccm_passivate (void);
};

#include "NonTopic_Connector_T.cpp"

#endif
