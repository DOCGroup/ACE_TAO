// $Id$

#include "ECM_Data.h"
#include "tao/CDR.h"



CORBA::Boolean
operator<< (TAO_OutputCDR& cdr, ECM_Data& x)
{
  // We are a little careless about error checking in this routine,
  // because one the CDR gets the error bit on it is never disabled.
  CORBA::ULong count = x.inventory.current_size ();
  if (cdr << x.description.in ()
      && cdr << count )
    {
      for (ECM_Data::Inventory::ITERATOR i = x.inventory.begin ();
           i != x.inventory.end () && cdr.good_bit ();
           ++i)
        {
          const ECM_Data::Inventory::ENTRY& v = *i;
          cdr << v.ext_id_;
          cdr << v.int_id_;
        }
    }
  return cdr.good_bit ();
}

CORBA::Boolean
operator>> (TAO_InputCDR& cdr, ECM_Data& x)
{
  // We are a little careless about error checking in this routine,
  // because one the CDR gets the error bit on it is never disabled.
  CORBA::ULong count;
  if (cdr >> x.description.out ()
      && cdr >> count)
    {
      // ACE_DEBUG ((LM_DEBUG, "Decoding <%d> elements\n", count));

      for (CORBA::ULong i = 0; i < count && cdr.good_bit (); ++i)
        {
          CORBA::ULong ext_id;
          CORBA::Double int_id;
          cdr >> ext_id;
          cdr >> int_id;
          if (cdr.good_bit ())
            x.inventory.bind (ext_id, int_id);

          // ACE_DEBUG ((LM_DEBUG, "Boung <%d,%f>\n", ext_id, int_id));
        }
    }
  return cdr.good_bit ();
}
