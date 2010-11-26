// $Id$

// connector template for Typed_NonTopic_Test_1
template <typename CCM_TYPE, typename T, bool FIXED_T,
                                typename U, bool FIXED_U,
                                typename V, bool FIXED_V,
                                typename Tseq, bool FIXED_Ts,
                                typename Useq, bool FIXED_Ys,
                                typename VSeq, bool FIXED_Vs>
NT_Connector_T<CCM_TYPE, T, FIXED_T,U, FIXED_U,V,  FIXED_V,
               Tseq, FIXED_Ts, Useq,  FIXED_Ys,
               VSeq,  FIXED_Vs>::NT_Connector_T (void)
{
}

template <typename CCM_TYPE, typename T, bool FIXED_T,
                                typename U, bool FIXED_U,
                                typename V, bool FIXED_V,
                                typename Tseq, bool FIXED_Ts,
                                typename Useq, bool FIXED_Ys,
                                typename VSeq, bool FIXED_Vs>
void
NT_Connector_T < CCM_TYPE, T,  FIXED_T,U, FIXED_U,V, FIXED_V,
                  Tseq, FIXED_Ts,Useq, FIXED_Ys,
                  VSeq,FIXED_Vs>::set_session_context (
                      ::Components::SessionContext_ptr /* ctx*/)
{
}

template <typename CCM_TYPE, typename T, bool FIXED_T,
                                typename U, bool FIXED_U,
                                typename V, bool FIXED_V,
                                typename Tseq, bool FIXED_Ts,
                                typename Useq, bool FIXED_Ys,
                                typename VSeq, bool FIXED_Vs>
void
NT_Connector_T < CCM_TYPE, T,  FIXED_T, U, FIXED_U, V, FIXED_V,
                 Tseq, FIXED_Ts, Useq, FIXED_Ys,
                VSeq, FIXED_Vs>::configuration_complete (void)
{
}

template <typename CCM_TYPE, typename T, bool FIXED_T,
                                typename U, bool FIXED_U,
                                typename V, bool FIXED_V,
                                typename Tseq, bool FIXED_Ts,
                                typename Useq, bool FIXED_Ys,
                                typename VSeq, bool FIXED_Vs>
void
NT_Connector_T < CCM_TYPE, T,  FIXED_T, U, FIXED_U,   V, FIXED_V,
                 Tseq, FIXED_Ts,Useq, FIXED_Ys,
                 VSeq, FIXED_Vs>::ccm_remove (void)
{
}

template <typename CCM_TYPE, typename T, bool FIXED_T,
                                typename U, bool FIXED_U,
                                typename V, bool FIXED_V,
                                typename Tseq, bool FIXED_Ts,
                                typename Useq, bool FIXED_Ys,
                                typename VSeq, bool FIXED_Vs>
void
NT_Connector_T < CCM_TYPE, T,  FIXED_T, U, FIXED_U, V, FIXED_V,
Tseq, FIXED_Ts, Useq, FIXED_Ys, VSeq, FIXED_Vs>::ccm_activate (void)
{
}

template <typename CCM_TYPE, typename T, bool FIXED_T,
                                typename U, bool FIXED_U,
                                typename V, bool FIXED_V,
                                typename Tseq, bool FIXED_Ts,
                                typename Useq, bool FIXED_Ys,
                                typename VSeq, bool FIXED_Vs>
void
NT_Connector_T < CCM_TYPE, T,  FIXED_T, U, FIXED_U, V, FIXED_V,
Tseq, FIXED_Ts, Useq, FIXED_Ys, VSeq, FIXED_Vs>::ccm_passivate (void)
{
}

// connector template for Typed_NonTopic_Test_2
template <typename CCM_TYPE,typename  T, bool FIXED_T,
                              typename  U, bool FIXED_U,
                              typename  Tseq,bool FIXED_Ts,
                              typename  Useq,bool FIXED_Us>
NT_2_Connector_T<CCM_TYPE, T, FIXED_T, U, FIXED_U,
 Tseq, FIXED_Ts, Useq, FIXED_Us>::NT_2_Connector_T(void)
{
}

template <typename CCM_TYPE,typename  T, bool FIXED_T,
                              typename  U, bool FIXED_U,
                              typename  Tseq,bool FIXED_Ts,
                              typename  Useq,bool FIXED_Us>
void
NT_2_Connector_T<CCM_TYPE, T, FIXED_T,U, FIXED_U,
Tseq, FIXED_Ts,Useq, FIXED_Us>::set_session_context (
    ::Components::SessionContext_ptr /* ctx*/)
{
}

template <typename CCM_TYPE,typename  T, bool FIXED_T,
                              typename  U, bool FIXED_U,
                              typename  Tseq,bool FIXED_Ts,
                              typename  Useq,bool FIXED_Us>
void
NT_2_Connector_T<CCM_TYPE, T, FIXED_T,
U, FIXED_U,Tseq, FIXED_Ts,Useq, FIXED_Us>::configuration_complete (void)
{
}

template <typename CCM_TYPE,typename  T, bool FIXED_T,
                              typename  U, bool FIXED_U,
                              typename  Tseq,bool FIXED_Ts,
                              typename  Useq,bool FIXED_Us>
void
NT_2_Connector_T<CCM_TYPE, T, FIXED_T,U, FIXED_U,
Tseq, FIXED_Ts,Useq, FIXED_Us>::ccm_remove (void)
{
}

template <typename CCM_TYPE,typename  T, bool FIXED_T,
                              typename  U, bool FIXED_U,
                              typename  Tseq,bool FIXED_Ts,
                              typename  Useq,bool FIXED_Us>
void
NT_2_Connector_T<CCM_TYPE, T, FIXED_T,U, FIXED_U,
Tseq, FIXED_Ts,Useq, FIXED_Us>::ccm_activate (void)
{
}

template <typename CCM_TYPE,typename  T, bool FIXED_T,
                              typename  U, bool FIXED_U,
                              typename  Tseq,bool FIXED_Ts,
                              typename  Useq,bool FIXED_Us>
void
NT_2_Connector_T<CCM_TYPE, T, FIXED_T,U, FIXED_U,
Tseq, FIXED_Ts,Useq, FIXED_Us>::ccm_passivate (void)
{
}
