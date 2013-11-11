// $Id$

// connector template for Typed_NonTopic_Test_1
template <typename CCM_TYPE,
          typename T, bool FIXED_T,
          typename U, bool FIXED_U,
          typename V, bool FIXED_V,
          typename Tseq,
          typename Useq,
          typename VSeq>
NT_Connector_T<CCM_TYPE, T, FIXED_T, U, FIXED_U, V, FIXED_V, Tseq, Useq, VSeq>::NT_Connector_T (void)
{
}

template <typename CCM_TYPE,
          typename T, bool FIXED_T,
          typename U, bool FIXED_U,
          typename V, bool FIXED_V,
          typename Tseq,
          typename Useq,
          typename VSeq>
void
NT_Connector_T<CCM_TYPE, T, FIXED_T, U, FIXED_U, V, FIXED_V, Tseq, Useq, VSeq>::set_session_context (
  ::Components::SessionContext_ptr /* ctx*/)
{
}

template <typename CCM_TYPE,
          typename T, bool FIXED_T,
          typename U, bool FIXED_U,
          typename V, bool FIXED_V,
          typename Tseq,
          typename Useq,
          typename VSeq>
void
NT_Connector_T<CCM_TYPE, T, FIXED_T, U, FIXED_U, V, FIXED_V, Tseq, Useq, VSeq>::configuration_complete (void)
{
}

template <typename CCM_TYPE,
          typename T, bool FIXED_T,
          typename U, bool FIXED_U,
          typename V, bool FIXED_V,
          typename Tseq,
          typename Useq,
          typename VSeq>
void
NT_Connector_T<CCM_TYPE, T, FIXED_T, U, FIXED_U, V, FIXED_V, Tseq, Useq, VSeq>::ccm_remove (void)
{
}

template <typename CCM_TYPE,
          typename T, bool FIXED_T,
          typename U, bool FIXED_U,
          typename V, bool FIXED_V,
          typename Tseq,
          typename Useq,
          typename VSeq>
void
NT_Connector_T<CCM_TYPE, T, FIXED_T, U, FIXED_U, V, FIXED_V, Tseq, Useq, VSeq>::ccm_activate (void)
{
}

template <typename CCM_TYPE,
          typename T, bool FIXED_T,
          typename U, bool FIXED_U,
          typename V, bool FIXED_V,
          typename Tseq,
          typename Useq,
          typename VSeq>
void
NT_Connector_T<CCM_TYPE, T, FIXED_T, U, FIXED_U, V, FIXED_V, Tseq, Useq, VSeq>::ccm_passivate (void)
{
}

// connector template for Typed_NonTopic_Test_2
template <typename CCM_TYPE,
          typename T, bool FIXED_T,
          typename U, bool FIXED_U,
          typename Tseq,
          typename Useq>
NT_2_Connector_T<CCM_TYPE, T, FIXED_T, U, FIXED_U, Tseq, Useq>::NT_2_Connector_T(void)
{
}

template <typename CCM_TYPE,
          typename T, bool FIXED_T,
          typename U, bool FIXED_U,
          typename Tseq,
          typename Useq>
void
NT_2_Connector_T<CCM_TYPE, T, FIXED_T,U, FIXED_U, Tseq,Useq>::set_session_context (
  ::Components::SessionContext_ptr /* ctx*/)
{
}

template <typename CCM_TYPE,
          typename T, bool FIXED_T,
          typename U, bool FIXED_U,
          typename Tseq,
          typename Useq>
void
NT_2_Connector_T<CCM_TYPE, T, FIXED_T, U, FIXED_U, Tseq, Useq>::configuration_complete (void)
{
}

template <typename CCM_TYPE,
          typename T, bool FIXED_T,
          typename U, bool FIXED_U,
          typename Tseq,
          typename Useq>
void
NT_2_Connector_T<CCM_TYPE, T, FIXED_T,U, FIXED_U, Tseq,Useq>::ccm_remove (void)
{
}

template <typename CCM_TYPE,
          typename T, bool FIXED_T,
          typename U, bool FIXED_U,
          typename Tseq,
          typename Useq>
void
NT_2_Connector_T<CCM_TYPE, T, FIXED_T,U, FIXED_U, Tseq,Useq>::ccm_activate (void)
{
}

template <typename CCM_TYPE,
          typename T, bool FIXED_T,
          typename U, bool FIXED_U,
          typename Tseq,
          typename Useq>
void
NT_2_Connector_T<CCM_TYPE, T, FIXED_T,U, FIXED_U, Tseq,Useq>::ccm_passivate (void)
{
}
