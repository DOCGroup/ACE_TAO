// $Id$

#include "Convert_Plan_Impl.h"
#include "ace/OS_NS_stdio.h"
#include "tao/CDR.h"
#include "Config_Handlers/XML_File_Intf.h"
#include "Logger/Log_Macros.h"

namespace DAnCE
{
  Deployment::DeploymentPlan * 
  Convert_Plan::read_xml_plan (const char *filename)
  {
    if (!filename)
      {
        DANCE_ERROR (2, (LM_WARNING, DLINFO ACE_TEXT ("Convert_Plan::read_xml_plan - ")
                         ACE_TEXT ("Passed a null pointer, returning.\n")));
      }

    CIAO::Config_Handlers::XML_File_Intf xml_intf (filename);
    xml_intf.add_search_path ("ACE_ROOT", "/docs/schema/");
    xml_intf.add_search_path ("TAO_ROOT", "/docs/schema/");
    xml_intf.add_search_path ("CIAO_ROOT", "/docs/schema/");
    xml_intf.add_search_path ("DANCE_ROOT", "/docs/schema/");
  
    return xml_intf.release_plan ();  
  }

  Deployment::DeploymentPlan * 
  Convert_Plan::read_cdr_plan (const char *filename)
  {
    if (!filename)
      {
        DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT ("Convert_Plan::read_cdr_plan - ")
                         ACE_TEXT ("Passed a null pointer, returning.\n")));
        return 0;
      }
    
    FILE *file = ACE_OS::fopen (filename, "r");
    
    if (!file)
      {
        DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT ("Convert_Plan::read_cdr_plan - ")
                         ACE_TEXT ("Error: Unable to open file for reasding %C\n"),
                         filename));
        return 0;
      }

    char byte_order;
    size_t n = ACE_OS::fread (&byte_order, sizeof (byte_order), 1, file);
    
    if (n == 0)
      {
        DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT ("Convert_Plan::read_cdr_plan - ")
                         ACE_TEXT ("Byte order not written to file\n")));
        ACE_OS::fclose (file);
        return 0;
      }

    ACE_UINT32 bufsize (0);
    n = ACE_OS::fread (&bufsize, sizeof (bufsize), 1, file);
    
    if (n == 0)
      {
        DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT ("Convert_Plan::read_cdr_plan - ")
                         ACE_TEXT ("Plan size not written to file\n")));
        ACE_OS::fclose (file);
        return 0;
      }
    
    if (byte_order != ACE_CDR_BYTE_ORDER)
      {
        DANCE_DEBUG (6, (LM_DEBUG, DLINFO ACE_TEXT ("Convert_Plan::read_cdr_plan - ")
                         ACE_TEXT ("Plan appears encoded in a different endian.\n")));
        ACE_CDR::swap_4 (reinterpret_cast<char *> (&bufsize),
                         reinterpret_cast<char *> (&bufsize));
      }
    
    
    char *buffer = 0;
    ACE_NEW_RETURN (buffer,
                    char[bufsize],
                    0);
  
#if defined (ACE_INITIALIZE_MEMORY_BEFORE_USE)
    ACE_OS::memset(buffer, 0, sizeof (buffer));
#endif /* ACE_INITIALIZE_MEMORY_BEFORE_USE */
    
    // Make sure <buffer> is released automagically.
    ACE_Auto_Basic_Array_Ptr<char> b (buffer);
  
    n = ACE_OS::fread (buffer, 1, bufsize, file);
    ACE_OS::fclose (file);

    if (n != bufsize)
      {
        DANCE_ERROR (1, (LM_ERROR, DLINFO "Convert_Plan_Impl::read_cdr_plan - read %u bytes "
                         "instead of %u bytes\n", n, bufsize));
        return 0;
      }
  
    ACE_Message_Block mb (ACE_CDR::MAX_ALIGNMENT + bufsize);
    ACE_CDR::mb_align (&mb);
    mb.copy (buffer, bufsize);
  
    TAO_InputCDR input_cdr (&mb);
    input_cdr.reset_byte_order ((int) byte_order);
  
    DANCE_DEBUG (9, (LM_TRACE, DLINFO "Convert_Plan_Impl::read_cdr_plan - "
                     "Reading file %C in %s endian format with size %u\n",
                     filename, 
                     ACE_CDR_BYTE_ORDER ? ACE_TEXT("little") : ACE_TEXT("big"),
                     bufsize));
  
  
    Deployment::DeploymentPlan *retval;
    
    ACE_NEW_RETURN (retval, Deployment::DeploymentPlan (), 0);
  
    if (!(input_cdr >> *retval))
      {
        DANCE_ERROR (1, (LM_ERROR, ACE_TEXT ("Convert_Plan_Impl::read_cdr_plan - ")
                         ACE_TEXT ("Failed to demarshal plan\n")));
        delete retval;
        return 0;
      }
  
  
    return retval;
  }

  bool
  Convert_Plan::write_cdr_plan (const char *output_filename, Deployment::DeploymentPlan &plan)
  {
    if (!output_filename)
      {
        DANCE_ERROR (1, (LM_ERROR, DLINFO "Convert_Plan_Impl::write_cdr_plan - passed nil file name\n"));
        return false;
      }    
  
    TAO_OutputCDR output_cdr;
  
    if (output_cdr << plan)
      {
        FILE *file = ACE_OS::fopen (output_filename, "w");
        
        if (file == 0)
          {
            DANCE_ERROR (1, (LM_ERROR, DLINFO "Convert_Plan_Impl::write_cdr_plan - failed to open file %C\n",
                             output_filename));
            return false;
          }
        
        // First write the byte order
        char byte_order = ACE_CDR_BYTE_ORDER;
        size_t n = ACE_OS::fwrite (&byte_order, 1, sizeof(byte_order), file);
        
        // Next write the length
        ACE_UINT32 cdr_length (output_cdr.total_length ());
        n += ACE_OS::fwrite (&cdr_length, 1, sizeof (cdr_length), file);

        DANCE_DEBUG (9, (LM_TRACE, DLINFO  "Convert_Plan_Impl::write_cdr_plan - "
                         ACE_TEXT ("Writing plan to file %s in %s endian format and length %u\n"),
                         output_filename,
                         ACE_CDR_BYTE_ORDER ? ACE_TEXT("little") : ACE_TEXT("big"),
                         cdr_length));
        
        // Now write the IDL structure.
        for (const ACE_Message_Block *output_mb = output_cdr.begin ();
             output_mb != 0;
             output_mb = output_mb->cont ())
          {
            n += ACE_OS::fwrite (output_mb->rd_ptr (),
                                 1,
                                 output_mb->length (),
                                 file);
          }
          
        ACE_OS::fclose (file);
        
        size_t total_size = sizeof (byte_order) + sizeof (cdr_length) + cdr_length;
        if (n != total_size)
          {
            DANCE_ERROR (1, (LM_ERROR, ACE_TEXT ("Convert_Plan::write_cdr_plan - ")
                             ACE_TEXT ("Error: Unexpected number of bytes written: %u instead of %u\n"),
                             n, total_size));
          }
      }
    else
      {
        DANCE_ERROR (1, (LM_ERROR, ACE_TEXT ("Convert_Plan::write_cdr_plan - ")
                         ACE_TEXT ("Failed to mashal deployment plan\n")));
        return false;
      }
  
    return true;    
  }
}
