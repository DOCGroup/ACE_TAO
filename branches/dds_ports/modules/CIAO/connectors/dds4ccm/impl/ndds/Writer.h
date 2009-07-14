/**
 * @file Writer.h
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 *
 * $Id$
 *
 * Writer template for RTI NDDS dds4ccm support.
 */

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    namespace RTI
    {
      template <typename NDDS_TYPE, typename BASE>
      class Writer_T
      {
      public:
        // Constructor 
        Writer_T (::DDS::DataWriter_ptr dw);
        
        // Destructor 
        virtual ~Writer_T (void);
        
        virtual void write (typename NDDS_TYPE::value_type an_instance);
        
      private:
        typename NDDS_TYPE::data_writer *writer_;
      };
    }
  }
}

#include "dds4ccm/impl/ndds/Writer.tpp"
