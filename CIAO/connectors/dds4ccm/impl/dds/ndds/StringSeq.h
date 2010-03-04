/**
 * @author Marcel Smit <msmit@remedy.nl>
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 */

#ifndef DDS4CCM_STRINGSEQ_T_H
#define DDS4CCM_STRINGSEQ_T_H

#if defined (CIAO_DDS4CCM_OPENDDS) && (CIAO_DDS4CCM_OPENDDS==1)
typedef ::DDS::StringSeq DDS_StringSeq;
typedef long DDS_Long;
#endif

#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
inline void
operator<<= (::DDS::StringSeq & string_seq, const ::DDS_StringSeq & dds_string_seq)
{
  string_seq.length (dds_string_seq.length ());
  for (::DDS_Long i = 0 ; i < dds_string_seq.length(); ++i)
    {
      string_seq[i] = CORBA::string_dup (dds_string_seq[i]);
    }
}

inline void
operator<<= (::DDS_StringSeq & dds_string_seq, const ::DDS::StringSeq & string_seq)
{
    const char** parameterlist = new const char*[string_seq.length ()];

    for (CORBA::ULong i = 0; i < string_seq.length (); ++i)
      {
        parameterlist[i] = string_seq[i].in ();
      }
    dds_string_seq.length (string_seq.length ());
    dds_string_seq.from_array (parameterlist, string_seq.length ());
    delete [] parameterlist;
}
#endif

#endif /* DDS4CCM_STRINGSEQ_T_H */
