// $Id$

#include "CB_Handler.h"
#include "CIAOServerResources.hpp"
#include "ace/Basic_Types.h"

namespace CIAO
{
  namespace Config_Handlers
  {
    //    IDREF_Base CB_Handler::IDREF;

    bool
    CB_Handler::connection_band (const ConnectionBandsDef &src,
                                 ::CIAO::DAnCE::ORS_ConnectionBands &dest)
    {
      // Set the number of bands
      CORBA::ULong num (dest.bands.length ());
      dest.bands.length (num + src.count_band ());

      for (ConnectionBandsDef::band_const_iterator i = src.begin_band ();
           i != src.end_band ();
           ++i)
        {
          dest.bands[num].low = static_cast <ACE_INT32> ((i->low ()));
          dest.bands[num].high = static_cast < ACE_INT32 > (i->high ());
          num++;
        }

      if (src.id_p ())
        {
          /*
          ACE_CString cstr (src.id ().c_str ());
          if (!IDD_Handler::IDREF.bind_ref (cstr, dest))
            return false;
          */
          dest.Id = CORBA::string_dup (src.id ().c_str ());
        }

      return true;
    }


    ConnectionBandsDef
    CB_Handler::connection_band (const ::CIAO::DAnCE::ORS_ConnectionBands &src)
    {
      ConnectionBandsDef cb;

      for (CORBA::ULong i = 0;
           i < src.bands.length ();
           ++i)
        {
          cb.add_band (PriorityBandDef (src.bands[i].low,
                                        src.bands[i].high));
        }

      /*
      ACE_CString cstr;
      if (CB_Handler::IDREF.find_ref (src, cstr))
        cb->id (cstr.c_str ());
      */

      if (src.Id.in () != 0)
        cb.id (src.Id.in ());

      return cb;
    }

  }
}
