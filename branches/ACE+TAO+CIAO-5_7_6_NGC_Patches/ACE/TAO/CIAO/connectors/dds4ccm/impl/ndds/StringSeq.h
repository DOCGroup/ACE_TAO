/**
 * @author Marcel Smit <msmit@remedy.nl>
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 */

#ifndef CIAO_RTI_STRINGSEQ_T_H
#define CIAO_RTI_STRINGSEQ_T_H

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

#endif /* CIAO_RTI_STRINGSEQ_T_H */
