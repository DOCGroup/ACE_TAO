// Get_Opt.cpp
// $Id$

#define ACE_BUILD_DLL
#include "ace/Log_Msg.h"
#include "ace/Get_Opt.h"

#if !defined (__ACE_INLINE__)
#include "ace/Get_Opt.i"
#endif /* __ACE_INLINE__ */

ACE_ALLOC_HOOK_DEFINE(ACE_Get_Opt)

ACE_Get_Opt::ACE_Get_Opt (int argc, 
		  char **argv,
		  char *optstring, 
		  int skip, 
		  int report_errors)
  : optarg (0), 
    optind (skip),
    opterr (report_errors), 
    nargc (argc), 
    nargv (argv),
    noptstring (optstring), 
    nextchar (0), 
    first_nonopt (skip), 
    last_nonopt (skip)
{
  ACE_TRACE ("ACE_Get_Opt::ACE_Get_Opt");
}

void
ACE_Get_Opt::dump (void) const
{
  ACE_TRACE ("ACE_Get_Opt::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, "\n"));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

int
ACE_Get_Opt::operator () (void)
{
  ACE_TRACE ("ACE_Get_Opt::operator");
  if (this->nextchar == 0 || *this->nextchar == 0)
    {
      // Special ARGV-element `--' means premature end of options.
      // Skip it like a null option, then exchange with previous
      // non-options as if it were an option, then skip everything
      // else like a non-option.

      if (this->optind != this->nargc && !::strcmp (this->nargv[this->optind], "--"))
        {
          this->optind++;

          if (this->first_nonopt == this->last_nonopt)
            this->first_nonopt = this->optind;
          this->last_nonopt = this->nargc;

          this->optind = this->nargc;
        }

      // If we have done all the ARGV-elements, stop the scan and back
      // over any non-options that we skipped and permuted.

      if (this->optind == this->nargc)
        {
          // Set the next-arg-index to point at the non-options
	  // that we previously skipped, so the caller will digest them. 
          if (this->first_nonopt != this->last_nonopt)
            this->optind = this->first_nonopt;
          return EOF;
        }
	 
      // If we have come to a non-option and did not permute it,
      // either stop the scan or describe it to the caller and pass it
      // by.

      if (this->nargv[this->optind][0] != '-' || this->nargv[this->optind][1] == 0)
	return EOF;

      // We have found another option-ARGV-element.  Start decoding
      // its characters.

      this->nextchar = this->nargv[this->optind] + 1;
    }

  // Look at and handle the next option-character. 

  {
    char c = *this->nextchar++;
    char *temp = (char *) strchr (this->noptstring, c);

    // Increment `optind' when we start to process its last character.
    if (*this->nextchar == 0)
      this->optind++;

    if (temp == 0 || c == ':')
      {
        if (this->opterr != 0)
          {
            if (c < 040 || c >= 0177)
              ACE_ERROR ((LM_ERROR, "%s: unrecognized option, character code 0%o\n",
			  this->nargv[0], c));
            else
              ACE_ERROR ((LM_ERROR, "%s: unrecognized option `-%c'\n",
			  this->nargv[0], c));
          }
        return '?';
      }
    if (temp[1] == ':')
      {
        if (temp[2] == ':')
          {
            // This is an option that accepts an argument optionally.
            if (*this->nextchar != 0)
              {
                this->optarg = this->nextchar;
                this->optind++;
              }
            else
              this->optarg = 0;
            this->nextchar = 0;
          }
        else
          {
            // This is an option that requires an argument.
            if (*this->nextchar != 0)
              {
                this->optarg = this->nextchar;
                // If we end this ARGV-element by taking the rest as
                // an arg, we must advance to the next element now.
                this->optind++;
              }
            else if (this->optind == this->nargc)
              {
                if (this->opterr != 0)
                  ACE_ERROR ((LM_ERROR, "%s: no argument for `-%c' option\n",
			      this->nargv[0], c));
                c = '?';
              }
            else
              // We already incremented `optind' once; increment it
              // again when taking next ARGV-elt as argument.
              this->optarg = this->nargv[this->optind++];
            this->nextchar = 0;
          }
      }
    return c;
  }
}
