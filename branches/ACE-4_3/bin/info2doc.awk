# =============================================================================
#
# = DESCRIPTION
#     Awk script which performs the actual conversion from classinfo file
#     to various types of documentation.
#
# = AUTHOR(S)
#     Graham Dumpleton
#     K.Dorn
# 
# = COPYRIGHT
#     Copyright 1991 OTC LIMITED
#     Copyright 1994 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
#
# =============================================================================

BEGIN {
  #
  # Set some defaults.
  #
  #device = "text"
#  access["public"] = "on"
#  access["protected"] = "on"
#  access["private"] = "on"
  access["public"] = publ
  access["protected"] = prot
  access["private"] = priv

  lower["CLASS"] = "class"
  lower["STRUCT"] = "struct"
  lower["UNION"] = "union"
  lower["class"] = "class"
  lower["struct"] = "struct"
  lower["union"] = "union"
  upper["public"] = "PUBLIC"
  upper["protected"] = "PROTECTED"
  upper["private"] = "PRIVATE"

  classfunc = ""
  rettyp = ""
  classname=""
  classtitle=""
  filedate=""
  filename=""
  filename1=""
  origfile=""
  author=""
  classdescription=""
  firsttime = "first"
  nesting_level = 0

#  csaprintheader=1

  #
  # Check that info file, class have been specified.
  #
  # if ( infile ~ /^$/ || class ~ /^$/ )
  #   exit

  #
  # Read in info file.
  #
  # FS="\n"; RS=""
  # while ( getline < infile > 0 )
  #   lines[++num] = $0

  #
  # Prepare for format file.
  #
  FS=" "; RS="\n"

  #
  # Find some defaults in info file.
  #
  # findLibrary()
  # findFilename()
}
# end of Begin section!!!!!


#
#
#
function csa_print_compilation_header_alt()
{
   if (csaprintheader == "on")
   {       
 	if ( length( firsttime ) > 3 )
	{
           
  csa_compilation_header=sprintf("\n/*[ Compilation unit "\
	"----------------------------------------------------------\n"\
	"\n"\
	"   Component\ \ \ \ \ \ \ : CSA - OSC\n"\
	"\n"\
	"   Name\ \ \ \ \ \ \ \ \ \ \ \ : %s.h\n"\
	"\n"\
	"   Author          : %s\n"\
	"\n"\
	"   Language        : C++\n"\
	"\n"\
	"   Creation Date   : %s\n"\
	"\n"\
	"   Test State	   : %%Q%%\n"\
	"\n"\
 	"   Description     : %s\n"\
 	"                     %s\n"\
	"\n"\
	"\n"\
	"               Copyright (C) Siemens AG 1995 All Rights Reserved\n"\
	"\n"\
	"--------------------------------------"\
	"---------------------------------------*/\n"\
	"/*] END */\n"\
	"#pragma ident \"%%Z%% %%M%% %%I%% (%%G%%), %%Y%% %%Q%%:"\
	" implementation file for class \n"\
	"%s\"\n"\
	"\n"\
	"#undef __STDC__\n"\
	"#undef __GNUG__\n"\
	"\n",filename1,author,filedate,classtitle,classdescription,classname);
	printf("%s",csa_compilation_header);
	firsttime = ""
	}
   }
}
#
#
#
function csa_print_compilation_header()
{
   if ((csaprintheader == "on" && merge != "on") || (merge == "on" && loop == "1"))
   {       
 	if ( length( firsttime ) > 3 )
	{
  for ( i=0; i<=num; i++ )
  {
    n = split( lines[i], line, "\n" )
    if (line[1] == "INCLUDE" && line[2] !~ /^.*\.\i/)
	printf( "#include %s\n", line[2])
  }

           
  csa_compilation_header=sprintf(vcohxx,filename1,author,filedate,classtitle,classdescription,classname);
	printf("%s",csa_compilation_header);
	firsttime = ""
	}
   }
}

#
#
#
function csa_print_class_header_alt()
{
 if (csaprintheader == "on")
 {       
  csa_class_header=sprintf("\n/*[ Class ---------------------"\
   "-------------------------------------------\n"\
   "\n"\
   "  Name           : %s\n"\
   "\n"\
   "  Description    : %s\n"\
   "                   %s\n"\
   "\n"\
    "--------------------------------------------"\
   "------------------------------*/\n\n",class,classtitle,classdescription)

  if ( device != "text" )
    print( ".nf\n" )
  printf("%s",csa_class_header);
 }
} 
#
#
#
function csa_print_class_header()
{
 if (csaprintheader == "on")
 {       
  csa_class_header=sprintf(vclhxx,class,classtitle,classdescription)

  if ( device != "text" )
    print( ".nf\n" )
  printf("%s",csa_class_header);
 }
} 

#
#
#
function csa_print_class_end()
{
 if (csaprintheader == "on")
 {       
  csa_class_end=sprintf("\n/*] END Class */\n\n")

  if ( device != "text" )
    print( ".nf\n" )
  printf("%s",csa_class_end);
 }
} 

#
#
#
function csa_collect_info()
{
  classdescription = ""
  classtitle = ""
  found = 0
  for ( i=0; i<=num; i++ )
  {

    n = split( lines[i], line, "\n" )

    csa_get_filedate_and_name()
    csa_get_class_name_and_title(n)
    csa_get_library()
    csa_get_author()



    if ( line[1] == "INFO" &&
	 line[2] == "HDR" &&
	 line[3] == "DESCRIPTION" &&
	 line[4] == class )
    {
      found = 1
      break
    }

  }
  if ( found == 1 )
  {
      for ( i=5; i<=n; i++ )
      {
        len = length(line[i])
        comm = substr(line[i],4,len)
	classdescription = classdescription comm "\n                     " 
      }
  }

  found = 0

  csa_get_origfile_name()
}

#
# 
#
function csa_get_origfile_name()
{
  len=length(infile)
  origfile=substr(infile,1,len-2)
  origfile = origfile "h"
}

#
# 
#
function csa_get_author()
{
    if ( line[1] == "INFO" &&
	 line[2] == "GLOBAL" &&
	 line[3] == "AUTHOR" )
    {
 	 len=length(line[4])
  	 author=substr(line[4],4,len)
    }
}

#
# 
#
function csa_get_library()
{
    if ( line[1] == "INFO" &&
	 line[2] == "GLOBAL" &&
	 line[3] == "LIBRARY" )
    {
  	#library=line[4]
    }
}

#
# 
#
function csa_get_class_name_and_title(n)
{
    if ( line[1] == "INFO" &&
	 line[2] == "HDR" &&
	 line[3] == "TITLE" &&
	 line[4] == class )
    {
      classname=line[4]
#      classtitle=line[5]
#      len = length(line[5])
#      classtitle = substr(line[5],4,len)
      for ( j=5; j<=n; j++ )
      {
        len = length(line[j])
        comm = substr(line[j],4,len)
	classtitle = classtitle comm "\n                     " 
      }
    }
}

#
# 
#
function csa_get_filedate_and_name()
{
    if ( line[1] == "CLASS2INFO" )
    {
  	filedate=line[2]
 	filename=line[3]
        len=length(line[3])
 	filename1=substr(line[3],1,(len-2))
    }
}

#
# 
#
function csa_get_()
{
}

#
# 
#
function read_ci_file()
{
    while ( getline < infile > 0 )
      lines[++num] = $0
}

function read_fvclhxx_file()
{
    vclhxx1 = "\n"
    while ( getline < fvclhxx > 0 )
      vclhxx1 = vclhxx1 $0 "\n"
    vclhxx =  vclhxx1  
}

function read_fvcohxx_file()
{
    vcohxx1 = "\n"
    while ( getline < fvcohxx > 0 )
      vcohxx1 = vcohxx1 $0 "\n"
    vcohxx = vcohxx1 
}

#
# currently not used because not necessary (in recursion properly handled!)
#
function read_ci_file_and_modify()
{
    while ( getline < infile > 0 )
    {
      if ($1 ~ "^(CLASS|STRUCT|UNION)$" && $2 ~ "(::|<)")
      {
#printf("\n----old=|%s|-----\n",$2)
        n = split( $0, line, "\n" )
	len = length($2)
    	match( $2, "(::|<).*$")
    	$2 = substr( $2,RSTART+2,RLENGTH)
#printf("\n----new=|%s|-----\n",$2)
   
	line[1] = $1 "\n"
	line[2] = $2
        new = ""
        for (i=1; i<=(n+1) ;i++)
          new = new line[i]
        lines[++num] = new "\n"
      }
      else
        lines[++num] = $0 "\n"

    }

#    for (i=1; i<=num ;i++)
#       print lines[i]
}

#
# Start next pass.
#
$1 == "start" {

  if ( pass == 0)
  {
    #
    # Check that info file, class have been specified.
    #
    if ( infile ~ /^$/ || class ~ /^$/ )
      exit

    #
    # Read in info file.
    #
    FS="\n"; RS=""

    read_ci_file()
    FS=" "; RS="\n"
    if (csaprintheader == "on")
    {   
     read_fvclhxx_file()
     read_fvcohxx_file()
    }
    #
    # Prepare for format file.
    #
    FS=" "; RS="\n"

    #
    # Find some defaults in info file.
    #
    findLibrary()
    findFilename()
  }

  pass++
}

#
# Sets the output device to use.
#
# $1 == "set" && $2 == "device" {
#   device = $3
# }

#
# Prevents a section from being printed.
#
$1 == "hide" && $2 == "section" {
  section = $3
  for ( i=4; i<=NF; i++ )
   section = " " section
  sections[section] = ""
}

#
# Prevents a contract from being printed.
#
# Note: a contract will not be displayed if it occurs in a part of the class
# which has been disabled. For instance, if anything occuring in the private
# section of a class is being hidden, the no contracts from that section will
# be displayed either.
#
$1 == "hide" && $2 == "contract" {
  contract = $3
  for ( i=4; i<=NF; i++ )
   contract = " " contract
  contracts[contract] = ""
}

#
# Specification of whether a part of a class is hidden or not.
#
$1 == "set" && $2 in access {
  access[$2] = $3
}

#
# Print a prelude. This should produce any stuff necessary to initialise
# the document.
#
pass == "2" && $1 == "print" && $2 == "prelude" {
  "date" | getline date
  split ( date, d, " " )
  date = d[2] " " d[3] ", " d[6]
  if ( device == "text" )
  {
  }
  else if ( device == "troff" )
  {
    print( ".\\\" troff -man %" )
    print( ".\\\"" )
    print( ".\\\" DO NOT EDIT" )
    print( ".\\\"" )
    print( ".\\\" This manual page is automatically generated by class2man." )
    print( ".\\\"" )
    print( ".ds sL " library )
    print( ".ds sV " date )
    print( ".ds sC " class )
    print( ".ds sS 3" )
    print( ".TH \"\\*(sC\" \"\\*(sS\" \"\\*(sV\" \"\\*(sL Library\" \"\\*(sL Reference Manual\"" )
  }
  else if ( device == "mml" )
  {
    print( "<MML>" )
    print( "<Comment *** This file is automatically generated by class2mml.>" )
    print( "<!DefineTag Body>" )
    print( "<!DefineTag Heading1>" )
    print( "<!DefineTag Heading2>" )
    print( "" )
  }
}

#
# Print out a title.
# The title should be generated from the class name and any information
# in the section labelled 'TITLE" which occurs in the hdr of a class.
# On pass 1 we simply note that we intend using the 'TITLE' section so
# that it doesn't get printed out as a seperate section.
#
pass == "1" && $1 == "print" && $2 == "title" {
  sections["TITLE"] = ""
}

#
# On pass 2 we actually print the title.
# Note: even if a 'TITLE' section is not found for the class, then the class
# name at least should be output.
#
pass == "2" && $1 == "print" && $2 == "title" {
  found = 0
  for ( i=0; i<=num; i++ )
  {
    n = split( lines[i], line, "\n" )
    if ( line[1] == "INFO" &&
	 line[2] == "HDR" &&
	 line[3] == "TITLE" &&
	 line[4] == class )
    {
      found = 1
      break
    }
  }
  if ( device == "text" )
  {
    if ( found == 1 )
    {
      print( class " " )
      for ( i=5; i<=n; i++ )
	outputLine( line[i] )
    }
    else
      print( class )
  }
  else if ( device == "troff" )
  {
    print( ".PP" )
    print( ".SH \"NAME\"" )
    if ( found == 1 )
    {
      print( class " \\- " )
      for ( i=5; i<=n; i++ )
	outputLine( line[i] )
    }
    else
      print( class )
  }
  else if ( device == "mml" )
  {
    print( "<Heading1>" )
    print( class )
    if ( found == 1 )
    {
      print( "<Body>" )
      for ( i=5; i<=n; i++ )
	outputLine( line[i] )
    }
  }
}

#
# On pass 1 we actually store the class description for csa.
#
pass == "1" {
	if (csaprintheader == "on")
		csa_collect_info()
}

#
# Print a synopsis.
# This should include a line indicating which header file needs to be included
# and a description of the members variables and functions in the class.
# A section of the class should not be output if access["section"] == "off".
# e.g.: Don't print private stuff if access["private"] == "off".
#
#
#  das ist die standard ausgabe! (nur hier!)
#
pass == 2 && $1 == "print" && $2 == "synopsis" {


  if (csaprintheader == "on")
    csa_print_compilation_header()
  
  if ( device == "text" )
  {
    print( "\ "SYNOPSIS"\ " )
    print( "\n" )
  }
  else if ( device == "troff" )
  {
    print( ".PP" )
    print( ".SH \"SYNOPSIS\"" )
    print( ".nf\n" )
  }
  else if ( device == "mml" )
  {
    print( "" )
    print( "<Heading2>")
    print( "SYNOPSIS" )
    printf( "<Body>" )
    printf( "<Plain>" )
    printf( "<LeftIndent 0.1in>" )
    printf( "<FirstIndent 0.1in>" )
    printf( "<Alignment l>" )
  }

  # 
  # check for includes
  # 
  if ( filename != "" && library != "C++" )
  {
    if ( device == "text" )
    {
      if ( merge != "on")
      {
#      printf( "\n#include <%s/%s>\n", library,filename)
#       printf( "\n#include <%s/%s>\n", library,origfile)
#       print( "\n" )
      }
      else
      {
       if ( loop == "1")
       {
#       printf( "\n#include <%s/%s>\n", library,filename)
#        printf( "\n#include <%s/%s>\n", library,origfile)
#        print( "\n" )
       }
      }
    }
    else if ( device == "troff" )
    {
      print( "\\f(CO#include <" library "/" filename ">" )
      print( "" )
    }
    else if ( device == "mml" )
    {
      print( "<Family Courier>#include \\<" library "/" filename "\\>" )
      print( "<HardReturn>" )
    }

    if (csaprintheader == "on")
    	csa_print_class_header()

  }

  bases = ""
  baseindex = 0
  numfields = 0

  #
  # start searching for loop for output 1 ADTs
  #
  for ( i=0; i<=num; i++ )
  {
    numfields = split( lines[i], line, "\n" )
    if ( line[1] ~ /(CLASS|UNION|STRUCT)/ && line[2] == class )
    {
      type = lower[line[1]]
      baseindex = 3
      #
      #
      # break heisst, jetzt ist richtige klasse gefunden, die von aussen mitgegeben wurde!!!!!!
      #

      break
    }
    else if ( \
     line[1] == "TEMPLATE" && \
     line[2] ~ /(class|union|struct)/ && \
     line[3] == class \
    )
    {
      baseindex = 5
      type = "template"
      break
    }
  }

  #
  # end of searching for loop for output 1 ADTs
  # ab hier function eingesetzt!
  #

#printf("\nvor parse_and_print_adt: class=%s nesting_level=%s",class,nesting_level);
  class = parse_and_print_adt(class,class,class,baseindex,type)
#printf("\nafter parse_and_print_adt: class=%s nesting_level=%s",class,nesting_level);


#
# bis hier geht parse function
#

#till end  csa_print_class_end()

  if (csaprintheader == "on") 
	csa_print_class_end()

}

#
# Print copyright holders. This must break the line before each copyright
# holder.
#
# wird im moment nicht gerufen
#
pass == "2" && $1 == "print" && $2 == "copyright" {
  for ( i=0; i<=num; i++ )
  {
    n = split( lines[i], line, "\n" )
    if ( line[1] == "INFO" &&
	 line[2] == "GLOBAL" &&
	 line[3] == "COPYRIGHT" )
    {
      if ( device == "text" )
      {
	print( "COPYRIGHT" )
      }
      else if ( device == "troff" )
      {
	print( ".PP" )
	print( ".SH COPYRIGHT" )
      }
      else if ( device == "mml" )
      {
	print( "" )
	print( "<Heading2>" )
	print( "COPYRIGHT" )
	printf( "<Body>" )
	printf( "<Plain>" )
	printf( "<LeftIndent 0in>" )
	printf( "<FirstIndent 0in>" )
	printf( "<SpaceAfter 8pt>" )
      }
      start = 4
      for ( k=start; k<=n; k++ )
      {
	if ( k != start && line[k] ~ "[\t ]*Copyright" )
	{
	  if ( device == "text" )
	    print( "\n" )
	  else if ( device == "troff" )
	    print( ".br" )
	  else if ( device == "mml" )
	    printf( "<HardReturn>" )
	}
	outputLine( line[k] )
      }
      break
    }
  }
}

#
# Print out any sections which have not been previously printed out and
# which are not hidden.
#
pass == "1" && $1 == "print" && $2 == "section" && $3 == "*" {
  next
}

pass == "2" && $1 == "print" && $2 == "section" && $3 == "*" {
  for ( i=0; i<=num; i++ )
  {
    n = split( lines[i], line, "\n" )
    if ( line[1] == "INFO" &&
	 ( line[2] == "GLOBAL" ||
	   ( line[2] == "HDR" && line[4] == class ) ) )
    {
      if ( line[3] in sections )
	continue
      else
      {
	section = line[3]
	sections[section] = ""
#printf("\noutputSection() - 1 - called!!!!!!!!!!!!");
	outputSection()
      }
    }
  }
}

#
# Print out a section.
# On pass 1 only record the fact that this section will be printed.
#
pass == "1" && $1 == "print" && $2 == "section" {
  section = $3
  for ( i=4; i<=NF; i++ )
    section = section " " $i
  sections[section] = ""
}

#
# Print out a section.
# On pass 2 actually print it out.
#
pass == "2" && $1 == "print" && $2 == "section" {
  section = $3
  for ( i=4; i<=NF; i++ )
    section = section " " $i
  found = 0
  for ( i=0; i<=num; i++ )
  {
    n = split( lines[i], line, "\n" )
    if ( line[1] == "INFO" &&
	 ( line[2] == "GLOBAL" ||
	   ( line[2] == "HDR" && line[4] == class ) ) &&
	 line[3] == section )
    {
      found = 1
      break
    }
  }
  if ( found == 1 )
  {
#printf("\noutputSection() - 2 - called!!!!!!!!!!!!");
    outputSection()
  }
}

#
# Print out any contracts which have not previously been printed, and
# which are not hidden.
#
# If there are no contracts in the class, then print out sections for the
# private, protected and public members; thats if they are not being hidden.
#
pass == "1" && $1 == "print" && $2 == "contract" && $3 == "\*" {
  next
}

pass == "2" && $1 == "print" && $2 == "contract" && $3 == "\*" {
  for ( i=0; i<=num; i++ )
  {
    n = split( lines[i], line, "\n" )

    if ( line[1] == "INFO" &&
	 line[2] == "BODY" &&
	 line[4] == class )
    {
      if ( line[3] in contracts )
	continue
      else if ( access[line[5]] == "on" )
      {
	contract = line[3]
	contracts[contract] = ""

#printf("\noutputContract() called!!!!!");
	outputContract()
      }
    }
  }
  i = 0
  for ( j in contracts )
    i++
  if ( i == 0 )
  {
#printf("\noutputClassSection() -2- called!!!!!!!\n");
    if ( access["public"] == "on" )
      outputClassSection( "public" )
    if ( access["protected"] == "on" )
      outputClassSection( "protected" )
    if ( access["private"] == "on" )
      outputClassSection( "private" )
  }
}

#
# Print out a particular contract, regardless of whether it is hidden or
# not.
# On pass 1 we only note that the contract will be printed.
#
pass == "1" && $1 == "print" && $2 == "contract" {
  contract = $3
  for ( i=4; i<=NF; i++ )
    contract = contract " " $i
  contracts[contract] = ""
}

pass == "2" && $1 == "print" && $2 == "contract" {
  contract = $3
  for ( i=4; i<=NF; i++ )
    contract = contract " " $i
  found = 0
  for ( i=0; i<=num; i++ )
  {
    n = split( lines[i], line, "\n" )
    if ( line[1] == "INFO" &&
	 line[2] == "BODY" &&
	 line[4] == class &&
	 line[3] == section )
    {
      found = 1
      break
    }
  }
  if ( found == 1 )
  {
#printf("\noutputContract() called!!!!!");
    outputContract()
  }
}

#
# Output a particular section of a class.
# This ignores whether that part is hidden.
#
pass == "2" && $1 == "print" && $3 in access {
#printf("\noutputClassSection() -1- called!!!!!!!\n");
  outputClassSection( $3 )
}

#
# Actually prints out a particular section of a class.
#
#
# wird nie gerufen
#
function outputClassSection( part )
{
  start = 0
  for ( i=0; i<=num; i++ )
  {
    n = split( lines[i], line, "\n" )
    if ( ( line[1] == "FUNC" ||
	   line[1] == "MEMBER" ||
	   line[1] == "TYPEDEF" ||
           line[1] == "FRIEND" ) &&
	 line[2] == class &&
	 line[3] == part )
    {
      if ( start == 0 )
      {
	start = 1
	if ( device == "text" )
	{
	  printf( "\n %s  MEMBERS\n" ,upper[line[3]])
	}
	else if ( device == "troff" )
	{
	  print( ".PP" )
	  print( ".SH \"" upper[line[3]] " MEMBERS\"" )
	}
	else if ( device == "mml" )
	{
	  print( "" )
	  print( "<Heading2>" )
	  print( upper[line[3]] " MEMBERS" )
	  printf( "<Body>" )
	  printf( "<Plain>" )
	  printf( "<LeftIndent 0in>" )
	  printf( "<FirstIndent 0in>" )
	  printf( "<SpaceAfter 8pt>" )
	}
      }
      if ( device == "text" )
      {
	print( "\n" )
      }
      else if ( device == "troff" )
      {
	print( ".nf" )
      }
      else if ( device == "mml" )
      {
	printf( "<Alignment l>" )
      }
      outputPrototype( line[4] )
      if ( device == "text" )
      {
      }
      else if ( device == "troff" )
      {
	print( ".fi" )
	print( ".RS 0.25i" )
      }
      else if ( device == "mml" )
      {
	printf( "<Alignment lr>" )
	printf( "<LeftIndent 0.1in>" )
	printf( "<FirstIndent 0.1in>" )
      }
      for ( l=5; l<=n; l++ )
	outputLine( line[l] )
      if ( device == "text" )
      {
	print( "" )
      }
      else if ( device == "troff" )
      {
	print( ".RE" )
	print( "" )
      }
      else if ( device == "mml" )
      {
	printf( "<LeftIndent 0in>" )
	printf( "<FirstIndent 0in>" )
      }
    }
  }
}

#
# Once a contract has been found, this prints it out.
#
#
# wird nie gerufen
#
function outputContract() {
  if ( device == "text" )
  {
    print( "\ " contract "\ " )
  }
  else if ( device == "troff" )
  {
    print( ".PP" )
    print( ".SH \"" contract "\"" )
  }
  else if ( device == "mml" )
  {
    print( "" )
    print( "<Heading2>" )
    print( contract )
    printf( "<Body>" )
    printf( "<Plain>" )
    printf( "<LeftIndent 0in>" )
    printf( "<FirstIndent 0in>" )
    printf( "<SpaceAfter 8pt>" )
  }

  for ( k=6; k<=n; k++ )
    outputLine( line[k] )

  # print( "" )
  for ( k=i+1; k<=num; k++ )
  {
    n = split( lines[k], line, "\n" )
    if ( ( line[1] == "INFO" &&
	   line[2] == "BODY" &&
	   line[4] == class ) ||
	 ( line[1] == "END" &&
	   line[2] == class ) ||
	 ( line[1] == "ACCESS" &&
	   line[2] == class ) )
    {
      break
    }
    else 
         if ( ( line[1] == "FUNC" ||
		line[1] == "MEMBER" ||
		line[1] == "TYPEDEF" ||
		line[1] == "FRIEND" ) &&
		line[2] == class )
    {
      if ( device == "text" )
      {
        print( "\n" )
      }
      else if ( device == "troff" )
      {
        print( ".nf" )
      }
      else if ( device == "mml" )
      {
        printf( "<Alignment l>" )
      }
      outputPrototype( line[4] )
      if ( device == "text" )
      {
      }
      else if ( device == "troff" )
      {
        print( ".fi" )
        print( ".RS 0.25i" )
      }
      else if ( device == "mml" )
      {
        printf( "<Alignment lr>" )
        printf( "<LeftIndent 0.1in>" )
        printf( "<FirstIndent 0.1in>" )
      }
      for ( l=5; l<=n; l++ )
	outputLine( line[l] )
      if ( device == "text" )
      {
        print( "" )
      }
      else if ( device == "troff" )
      {
        print( ".RE" )
        print( "" )
      }
      else if ( device == "mml" )
      {
        printf( "<LeftIndent 0in>" )
        printf( "<FirstIndent 0in>" )
      }
    }
    else if ( line[1] == "COMMENT" &&
		line[2] == "BODY" &&
		line[3] == class )
    {
      for ( l=5; l<=n; l++ )
	outputLine( line[l] )
      # print( "" )
    }
  }
}

#
# Once a section has been found, this prints it out.
#
#
# wird nie gerufen
#
function outputSection()
{

  if ( line[2] == "GLOBAL" )
    start = 4
  else
    start = 5

  if ( device == "text" )
  {
    print( "\ " section "\ " )
  }
  else if ( device == "troff" )
  {
    print( ".PP" )
    print( ".SH \"" section "\"" )
  }
  else if ( device == "mml" )
  {
    print( "" )
    print( "<Heading2>" )
    print( section )
    if ( line[start] != "" )
    {
      printf( "<Body>" )
      printf( "<Plain>" )
      printf( "<LeftIndent 0in>" )
      printf( "<FirstIndent 0in>" )
      printf( "<Alignment lr>" )
      printf( "<SpaceAfter 8pt>" )
      printf( "<Family Times>" )
    }
  }

  blankLine = 0
  for ( k=start; k<=n; k++ )
    outputLine( line[k] )
}

#
# Removes comment delimiter from start of line.
#
function removeComment( line )
{
  sub( "^// ?", "", line )
  return line
}

#
# Output a line. This checks for various formatting requests and will
# appropriately expand them.
#
function outputLine( line )
{
  line = removeComment( line )
  if ( inComment == 0 )
  {
    if ( line ~ "^= BEGIN<CODE>" )
    {
      inCode = 1
      if ( device == "text" )
      {
	print( "\n" )
      }
      else if ( device == "troff" )
      {
	print( ".RS 0.25i" )
	print( ".nf\n\\f(CO" )
      }
      else if ( device == "mml" )
      {
	printf( "<Plain>" )
	printf( "<LeftIndent 0.2in>" )
	printf( "<FirstIndent 0.2in>" )
	printf( "<Alignment l>" )
	printf( "<Family Courier>" )
      }
    }
    else if ( line ~ "^= END<CODE>" )
    {
      inCode = -1
      if ( device == "text" )
      {
	print( "\n" )
      }
      else if ( device == "troff" )
      {
	print( "\\fP\n.fi" )
	print( ".RE" )
      }
      else if ( device == "mml" )
      {
	printf( "<LeftIndent 0in>" )
	printf( "<FirstIndent 0in>" )
	printf( "<Alignment lr>" )
	printf( "<Family Times>")
      }
    }
    else if ( line ~ "^= BEGIN<COMMENT>" )
      inComment = 1
    else if ( line ~ "^= BEGIN<INDENT>" )
    {
      if ( device == "text" )
      {
      }
      else if ( device == "troff" )
	print( ".RS 0.25i" )
      else if ( device == "mml" )
      {
	printf( "<LeftIndent 0.2in>" )
	printf( "<FirstIndent 0.2in>" )
      }
    }
    else if ( line ~ "^= END<INDENT>" )
    {
      if ( device == "text" )
      {
      }
      else if ( device == "troff" )
	print( ".RE" )
      else if ( device == "mml" )
      {
	printf( "<LeftIndent 0in>" )
	printf( "<FirstIndent 0in>" )
      }
    }
    else if ( line ~ "^= BEGIN<NOFILL>" )
    {
      inNoFill = 1
      if ( device == "text" )
      {
      }
      else if ( device == "troff" )
	print( ".nf" )
      else if ( device == "mml" )
	printf( "<Alignment l>" )
    }
    else if ( line ~ "^= END<NOFILL>" )
    {
      inNoFill = 0
      if ( device == "text" )
      {
      }
      else if ( device == "troff" )
	print( ".fi" )
      else if ( device == "mml" )
	printf( "<Alignment lr>" )
    }
    else
    {
      if ( inCode > 0 )
      {
	if ( device == "mml" )
	{
	  gsub( " ", "\\ ", line )
	  gsub( "<", "\\<", line )
	  gsub( ">", "\\>", line )
	  if ( inCode > 1 )
	    line = "<HardReturn>" line
	}
	print( line )
	inCode = inCode + 1
      }
      else 
      {
	gsub( "\\\\]>", "]\\fP", line )
	gsub( "\\\\}>", "]\\fP", line )
	gsub( "([^\\\\]|^)<\\[", "&<\\fB", line )
	gsub( "[^\\\\]\\]>", "&>\\fP", line )
	gsub( "<\\[<", "", line )
	gsub( "\\]>>", "", line )
	gsub( "([^\\\\]|^)<\{", "&<\\fI", line )
	gsub( "[^\\\\]}>", "&>\\fP", line )
	gsub( "<\{<", "", line )
	gsub( "}>>", "", line )
	gsub( "([^\\\\]|^)<", "&<\\f(CO", line )
	gsub( "[^\\\\]>", "&>\\fP", line )
	gsub( "<<", "", line )
	gsub( ">>", "", line )
	gsub( "\\\\<", "<", line )
	gsub( "\\\\>", ">", line )
	gsub( "\\\\", "\\\\", line )
	gsub( "\\\\fB", "fB", line )
	gsub( "\\\\fI", "fI", line )
	gsub( "\\\\fP", "fP", line )
	gsub( "\\\\f\\(CO", "f(CO", line )

	if ( device == "mml" )
	{
	  if ( inNoFill > 0 )
	    gsub( " ", "\\ ", line )

	  gsub( "<", "\\<", line )
	  gsub( ">", "\\>", line )
	  gsub( "\\\\fP", "<Family Times><Plain>", line )
	  gsub( "\\\\fB", "<Bold>", line )
	  gsub( "\\\\fI", "<Italic>", line )
	  gsub( "\\\\f\\(CO", "<Family Courier>", line )

	  if ( inNoFill > 0 )
	  {
	    if ( inNoFill > 1 )
	      line = "<HardReturn>" line
	    inNoFill = inNoFill + 1
	  }
	}

	if ( line == "" )
	{
	  blankLine = 1
	}
	else
	{
	  if ( blankLine == 1 )
	  {
	    if ( inCode == 0 )
	      print( "" )
	    blankLine = 0
	    if ( inCode == -1 )
	      inCode = 0
	  }
	  print( line )
	}
      }
    }
  }
  else
  {
    if ( line ~ "^= END<COMMENT>" )
      inComment = 0
  }
}

#
# Searches for the 'LIBRARY' section and sets 'library' to the value found.
#
function findLibrary()
{
  for ( i=0; i<=num; i++ )
  {
    split( lines[i], line, "\n" )
    if ( line[1] == "INFO" &&
	 line[2] == "GLOBAL" &&
	 line[3] == "LIBRARY" )
    {
      len=length(line[4])
      libr = substr(line[4],4,len)
      library = removeComment( libr )
      break
    }
  }
  if ( library == "" )
    library = "C++"
}

#
# Searches for the 'FILENAME' section and sets 'filename' to the value found.
#
function findFilename()
{
  for ( i=0; i<=num; i++ )
  {
    split( lines[i], line, "\n" )
    if ( line[1] == "INFO" &&
	 line[2] == "GLOBAL" &&
	 ( line[3] == "FILENAME" ||
	   line[3] == "RCSID" ||
	   line[3] == "SCCSID" ) )
    {
      filename = removeComment( line[4] )
      if ( line[3] == "RCSID" )
      {
	sub( "^.*\\$Id:[\t ]*", "", filename )
	sub( ",v.*$", "", filename )
      }
      else if ( line[3] == "SCCSID" )
      {
	sub( "^.*@\\(#\\)", "", filename )
	sub( "[\t ]+.*$", "", filename )
      }
      break
    }
  }
}

#
# Outputs a prototype, wrapping it if required.
#
function outputPrototype( prototype )
{
  sub( "[\t ]*$", "", prototype )
  if ( prototype !~ "^.*;$" )
    prototype = prototype ";"

  if ( device == "text" )
    width = 70
  else if ( device == "troff" )
    width = 70
  else if ( device  == "mml" )
    width = 60

  if ( length(prototype) > width )
  {
    # multiline enums,functions have no spaces at the beginning, insert it!
    if ( device == "mml" )
    {
      gsub( ">", "\\>", prototype )
      gsub( "<", "\\<", prototype )
    }
    if ( ( funcptr = match( prototype, "^[^(]*\\([^)]*\\)\\(" ) ) !~ "0" )
      match( prototype, "^[^(]*\\([^)]*\\)" )
    else
      match( prototype, "^[^(]*\\(" )
    funcname = substr( prototype, RSTART, RLENGTH )
    if ( prototype ~ "^[^(]*\\(\\)[\t ]*\\(" )
    {
      funcname = funcname ")("
      sub( "^[^(]*\\(\\)[\t ]*\\(", "", prototype )
    }
    else
    {
      if ( funcptr ~ "0" )
	sub( "^[^(]*\\(", "", prototype )
      else
	sub( "^[^(]*\\([^)]*\\)\\(", "", prototype )
    }
    match( prototype, "\\)([\t ]*)?(=[\t ]*0|const[\t ]*(=[\t ]*0)?)?;[\t ]*" )
    functail = substr( prototype, RSTART, RLENGTH )
    sub( "\\)([\t ]*)?(=[\t ]*0|const[\t ]*(=[\t ]*0)?)?;.*$", "", prototype )
    sub( "\\($", "", funcname )
    match( funcname, "^[\t ]*" )
    space = substr( funcname, RSTART, RLENGTH )
    if (funcname != "")
    {
     # multiline enums or long typedefs have no funcname, so do not print it!
     if ( device == "text" )
     {
      printf( funcname )
      printf( "(\n" )
#printf("\n---funcname=|%s|----\n",funcname);
     }
     else if ( device == "troff" )
     {
      printf( "\\f(CO" funcname )
      printf( "(\n" )
     }
     else if ( device = "mml" )
     {
      gsub( " ", "\\ ", space )
      gsub( " ", "\\ ", funcname )
      printf( "<Family Courier>%s(\n<HardReturn>", funcname )
     }
    }
    narg = 0
    while ( match( prototype, "[^<>,#]*<[^<>]*>" ) )
    {
      # collect the multiple line arguments
      narg++
      arg = substr( prototype, RSTART, RLENGTH )
      sub( "[^<>,#]*<[^<>]*>", "#" narg, prototype )
      fargs["#" narg] = arg
      # Need the following to stop resubstitution of the pattern matched
      # back into the string.
      gsub( "&", "\\\\&", fargs["#" narg] )
    }
    numargs = split( prototype, args, "," )
#------
    space = ""
    for (z=1; z<=(nesting_level+1); z++)
	space = space "    "
#    printf( "%s",outspace)
#------

    for ( m=1; m<=numargs; m++ )
    {
      while ( match( args[m], "#[0-9]+" ) )
      {
	arg = substr( args[m], RSTART, RLENGTH )
	sub( arg, fargs[arg], args[m] )
      }
      sub( "[\t ]*", "", args[m] )

      # print one argument

#      printf( "%s  %s", space, args[m] )
      printf( "%s%s", space, args[m] )
      if ( m == numargs )
      {
        # print after the last argument

	if ( device == "text" )
	  print( "" )
	else if ( device == "troff" )
	  print( "" )
	else if ( device == "mml" )
	  printf( "\n<HardReturn>" )
      }
      else
      {
        # print after each argument of a line

	if ( device == "text" )
	  print( "," )
	else if ( device == "troff" )
	  print( "," )
	else if ( device == "mml" )
	  printf( ",\n<HardReturn>" )
      }
    }
    # print after all arguments of a multiline are printed

    if ( device == "text" )
      print( space functail "\ " )
    else if ( device == "troff" )
      print( space functail "\\fP" )
    else if ( device == "mml" )
      print( space functail "<Family Times>" )

  }
  else	# prototype < width
  {
    # not multiline enums,functions have spaces at the beginning already!
    if ( device == "mml" )
    {
      gsub( ">", "\\>", prototype )
      gsub( "<", "\\<", prototype )
    }
    if ( device == "text" )
      print( prototype "\ " )
    else if ( device == "troff" )
      print( "\\f(CO" prototype "\\fP" )
    else if ( device == "mml" )
    {
      gsub( " ", "\\ ", prototype )
      print( "<Family Courier>" prototype "<Family Times>" )
    }
  }
}

#=============================================================
#  neue rkursive parse function fuer nested ADTs
#=============================================================
function parse_and_print_adt (baseandsubclass, class, oldclass, baseindex, type)
{
  # 3.11.95
  bases = "" 

  nesting_level++
  printf( "\n")
#  printf("\nclass=%s oldclass=%s nesting_level=%s\n",class,oldclass,nesting_level);
  if ( baseindex != 0 )
  {
    for ( i=baseindex; i<=numfields; i++ )
    {
      if ( bases == "" )
	bases = line[i]
      else
	bases = bases ", " line[i]
    }
  }

  outspace = ""
  for (z=1; z<nesting_level; z++)
	outspace = outspace "    "
  printf( "%s",outspace)

  if ( type == "template" )
  {
  #
  #  ADT == template
  #
    for ( i=0; i<=num; i++ )
    {
      split( lines[i], line, "\n" )

#      if ( line[1] == "TEMPLATE" && line[3] == class )
      if ( line[1] == "TEMPLATE" && (line[3] == class || line[3] == baseandsubclass) )
	break
    }
    if ( device == "text" )
      printf( "template%s\n",line[4] )
    else if ( device == "troff" )
      printf( "template%s\n",line[4] )
    else if ( device == "mml" )
    {
      sub( "<", "\\<", line[4] )
      sub( ">", "\\>", line[4] )
      printf( "<HardReturn>template%s\n",line[4] )
    }
#    typestat = line[2] " " line[3]
    typestat = line[2] " " class

    outspace = ""
    for (z=1; z<nesting_level; z++)
	outspace = outspace "    "
    printf( "%s",outspace)

    if ( device == "text" )
      printf( "%s", typestat )
    else if ( device == "troff" )
      printf( "%s", typestat )
    else if ( device == "mml" )
      printf( "<HardReturn>%s", typestat )
  }
  else
  {
  #
  #  ADT != template
  #
    typestat = type " " class
    if ( device == "mml" )
      printf( "<HardReturn>" )
    printf( "%s", typestat )
  }

  if ( bases != "" )
  {
  #
  #  analyse if ADT has base classes
  #
    typestat = typestat " : "
    printf( " : " )
    if ( length(typestat) + length(bases) > 70 )
    {
      print( "" )
      if ( device == "mml" )
	printf( "<HardReturn>" )
      narg = 0
      while ( match( bases, "[^<>,# ]*<[^<>]*>" ) )
      {
	narg++
	arg = substr( bases, RSTART, RLENGTH )
	sub( "[^<>,# ]*<[^<>]*>", "#" narg, bases )
	bargs["#" narg] = arg
	# Need the following to stop resubstitution of the pattern matched
	# back into the string.
	gsub( "&", "\\\\&", bargs["#" narg] )
      }
      numbases = split( bases, base, "," )
      for ( m=1; m<=numbases; m++ )
      {
	while ( match( base[m], "#[0-9]+" ) )
	{
	  arg = substr( base[m], RSTART, RLENGTH )
	  sub( arg, bargs[arg], base[m] )
	}
	sub( "^[\t ]*", "", base[m] )
	if ( device == "mml" )
	{
	  gsub( "<", "\\<", base[m] )
	  gsub( ">", "\\>", base[m] )
	}
	if ( device == "text" )
	  printf( "  %s", base[m] )
	else if ( device == "troff" )
	  printf( "  %s", base[m] )
	else
	  printf( "\\ \\ %s", base[m] )
	if ( m == numbases )
	  print( "" )
	else
	{
	  print( "," )
	  if ( device == "mml" )
	    printf( "<HardReturn>" )
	}
      }
    }
    else
    {
      if ( device == "mml" )
      {
	gsub( "<", "\\<", bases )
	gsub( ">", "\\>", bases )
      }
      print( bases )
    }
  }
  else
  {
  #
  #  ADT has no base classes
  #
    printf( "\n" )
  }

#till start  if (csaprintheader == "on") csa_print_class_end()

  outspace = ""
  for (z=1; z<nesting_level; z++)
	outspace = outspace "    "
  if ( device == "text" )
    print( outspace "{\ " )
  else if ( device == "troff" )
    print( outspace "{\\fP" )
  else if ( device == "mml" )
    print( outspace "<HardReturn>{<Family Times>")
  if ( access["public"] == "on" )
  {
  #
  #  collecting loop over the public lines of 1 ADT
  #
    start = 0
    for ( i=0; i<=num; i++ )
    {
      split( lines[i], line, "\n" )

      #
      # 3.11.95 check outermost access
      #
      if ( line[1] == "ACCESS") 
      {
	outeraccess = line[3]
        #printf("\n----- outeraccess = |%s| --------\n",outeraccess)
      }

      #
      # start for nesting ADT
      #
      if ( line[1] == "CLASS" || line[1] == "STRUCT" || line[1] == "UNION" )
      {	
	#
	# check for a normal nested ADT
	#
	if ( line[2] ~ "(::|<)" && lastaccess == "public" && outeraccess == "public")
        {	
	   if (line[2] ~ "::")
	   {		
		len = length(line[2])
    		match( line[2], "(::|<).*$")
    		newclass  = substr( line[2],RSTART+2,RLENGTH)
    		thisclass = substr( line[2],1,RSTART-1)
		if (class == thisclass)
		{
		  #
		  # hier is a nested class, struct, or union found!!!!!!!
		  #

		  basesubclass = line[2]
		  #printf("\n----basesubclass=|%s|-----\n",basesubclass)
		  #printf("\n----thisclass=|%s|---------newclass=|%s|-----\n",thisclass,newclass)
		  #printf("\n----RECURSION--class=|%s|--line[1]=|%s|--line[2]=|%s|---\n", \
			#class,line[1],line[2]);

		  nesttype = lower[line[1]]
                  nestbaseindex = 3
		  #printf("\nvor parse_and_print_adt: class=%s nesting_level=%s",class,nesting_level);
		  #printf("\ni=%s num=%s line[1]=|%s| line[2]=|%s| line[3]=|%s|\n", \
				#i,num,line[1],line[2],line[3]);
		  save_i = i
		  class = parse_and_print_adt(basesubclass,newclass,thisclass,nestbaseindex,nesttype)
		  i = save_i
		  #printf("\nafter parse_and_print_adt: class=%s nesting_level=%s",class,nesting_level);
		  #printf("\ni=%s num=%s line[1]=|%s| line[2]=|%s| line[3]=|%s|\n", \
				#i,num,line[1],line[2],line[3]);
	          start = 1
		  continue
		}
	    }
        }
      }
      else 
      {
	#
	# check for a TEMPLATE nested ADT
	#
	if ( line[1] == "TEMPLATE" && line[2] ~ /(class|union|struct)/  && lastaccess == "public" && outeraccess == "public")
	{
		len = length(line[3])
    		match( line[3], "(::|<).*$")
    		newclass  = substr( line[3],RSTART+2,RLENGTH)
    		thisclass = substr( line[3],1,RSTART-1)
		if (class == thisclass)
		{
		  #
		  # hier is a nested class, struct, or union found!!!!!!!
		  #

		  basesubclass = line[3]
		  #printf("\n----basesubclass=|%s|-----\n",basesubclass)
		  #printf("\n----thisclass=|%s|---------newclass=|%s|-----\n",thisclass,newclass)
		  #printf("\n---RECURSION--class=|%s|--line[1]=|%s|--line[2]=|%s|--line[3]=|%s|---\n", \
			#class,line[1],line[2],line[3]);

		  nesttype = "template"
                  nestbaseindex = 5
		  #printf("\nvor parse_and_print_adt: class=%s nesting_level=%s",class,nesting_level);
		  #printf("\ni=%s num=%s line[1]=|%s| line[2]=|%s| line[3]=|%s|\n", \
				#i,num,line[1],line[2],line[3]);
		  save_i = i
		  class = parse_and_print_adt(basesubclass,newclass,thisclass,nestbaseindex,nesttype)
		  i = save_i
		  #printf("\nafter parse_and_print_adt: class=%s nesting_level=%s",class,nesting_level);
		  #printf("\ni=%s num=%s line[1]=|%s| line[2]=|%s| line[3]=|%s|\n", \
				#i,num,line[1],line[2],line[3]);
	          start = 1
		  continue
		}
       }
      }
      #
      # end for nesting ADT
      #
      #
      # now print the protoypes of the public ADT
      #
      if ( ( line[1] == "MEMBER" ||
	     line[1] == "FUNC" ||
	     line[1] == "TYPEDEF" ||
	     line[1] == "ENUM" ||
	     line[1] == "FRIEND" ) &&
	   line[2] == class  &&
	   line[3] == "public" )
      {
	outspace = ""
	for (z=1; z<nesting_level; z++)
		outspace = outspace "    "
	lastaccess = "public"
	if ( start == 0 )
	{
	  start = 1
	  if ( device == "text" )
	    print( outspace "\ \n" outspace "public:\n")
	  else if ( device == "troff" )
	    print( outspace "  \\f(COpublic:\\fP")
	  else if ( device == "mml" )
	    print( outspace "<HardReturn><Family Courier>\\ \\ public:<Family Times>" )
	}
	outspace = ""
	for (z=1; z<=nesting_level; z++)
		outspace = outspace "    "
	if ( device == "text" )
        {
	  outputPrototype( outspace line[4] )
        }
	else if ( device == "troff" )
	  outputPrototype( outspace line[4] )
	else
	{
	  printf( "<HardReturn>" )
	  outputPrototype( outspace line[4] )
	}
      }
    }
  }
  if ( access["protected"] == "on" )
  {
  #
  #  collecting loop over the protected lines of 1 ADT
  #
    start = 0
    for ( i=0; i<=num; i++ )
    {
      split( lines[i], line, "\n" )

      #
      # 3.11.95 check outermost access
      #
      if ( line[1] == "ACCESS") 
      {
	outeraccess = line[3]
        #printf("\n----- outeraccess = |%s| --------\n",outeraccess)
      }

      #
      # start for nesting ADT
      #
      if ( line[1] == "CLASS" || line[1] == "STRUCT" || line[1] == "UNION" )
      {	
	#
	# check for a normal nested ADT
	#
	if ( line[2] ~ "(::|<)"  && lastaccess == "protected" && outeraccess == "protected")
        {	
	   if (line[2] ~ "::")
	   {		
		len = length(line[2])
    		match( line[2], "(::|<).*$")
    		newclass  = substr( line[2],RSTART+2,RLENGTH)
    		thisclass = substr( line[2],1,RSTART-1)
		if (class == thisclass)
		{
		  #
		  # hier is a nested class, struct, or union found!!!!!!!
		  #

		  basesubclass = line[2]
		  #printf("\n----basesubclass=|%s|-----\n",basesubclass)
		  #printf("\n----thisclass=|%s|---------newclass=|%s|-----\n",thisclass,newclass)
		  #printf("\n----RECURSION--class=|%s|--line[1]=|%s|--line[2]=|%s|---\n", \
			#class,line[1],line[2]);

		  nesttype = lower[line[1]]
                  nestbaseindex = 3
		  #printf("\nvor parse_and_print_adt: class=%s nesting_level=%s",class,nesting_level);
		  #printf("\ni=%s num=%s line[1]=|%s| line[2]=|%s| line[3]=|%s|\n", \
				#i,num,line[1],line[2],line[3]);
		  save_i = i
		  class = parse_and_print_adt(basesubclass,newclass,thisclass,nestbaseindex,nesttype)
		  i = save_i
		  #printf("\nafter parse_and_print_adt: class=%s nesting_level=%s",class,nesting_level);
		  #printf("\ni=%s num=%s line[1]=|%s| line[2]=|%s| line[3]=|%s|\n", \
				#i,num,line[1],line[2],line[3]);
	          start = 1
		  continue
		}
	    }
        }
      }
      else 
      {
	#
	# check for a TEMPLATE nested ADT
	#
	if ( line[1] == "TEMPLATE" && line[2] ~ /(class|union|struct)/  && lastaccess == "protected" && outeraccess == "protected")
	{
		len = length(line[3])
    		match( line[3], "(::|<).*$")
    		newclass  = substr( line[3],RSTART+2,RLENGTH)
    		thisclass = substr( line[3],1,RSTART-1)
		if (class == thisclass)
		{
		  #
		  # hier is a nested class, struct, or union found!!!!!!!
		  #

		  basesubclass = line[3]
		 #printf("\n----basesubclass=|%s|-----\n",basesubclass)
		 #printf("\n----thisclass=|%s|---------newclass=|%s|-----\n",thisclass,newclass)
		 #printf("\n---RECURSION--class=|%s|--line[1]=|%s|--line[2]=|%s|--line[3]=|%s|---\n", \
			#class,line[1],line[2],line[3]);

		  nesttype = "template"
                  nestbaseindex = 5
		  #printf("\nvor parse_and_print_adt: class=%s nesting_level=%s",class,nesting_level);
		  #printf("\ni=%s num=%s line[1]=|%s| line[2]=|%s| line[3]=|%s|\n", \
				#i,num,line[1],line[2],line[3]);
		  save_i = i
		  class = parse_and_print_adt(basesubclass,newclass,thisclass,nestbaseindex,nesttype)
		  i = save_i
		  #printf("\nafter parse_and_print_adt: class=%s nesting_level=%s",class,nesting_level);
		  #printf("\ni=%s num=%s line[1]=|%s| line[2]=|%s| line[3]=|%s|\n", \
				#i,num,line[1],line[2],line[3]);
	          start = 1
		  continue
		}
       }
      }
      #
      # end for nesting ADT
      #
      #
      # now print the protoypes of the public ADT
      #
      if ( ( line[1] == "MEMBER" ||
	     line[1] == "FUNC" ||
	     line[1] == "TYPEDEF" ||
	     line[1] == "ENUM" ||
	     line[1] == "FRIEND" ) &&
	   line[2] == class &&
	   line[3] == "protected" )
      {
	outspace = ""
	for (z=1; z<nesting_level; z++)
		outspace = outspace "    "
	lastaccess = "protected"
	if ( start == 0 )
	{
	  start = 1
	  if ( device == "text" )
	    print( outspace "\ \n" outspace "protected:\n")
	  else if ( device == "troff" )
	    print( outspace "  \\f(COprotected:\\fP")
	  else if ( device == "mml" )
	    print(outspace "<HardReturn><Family Courier>\\ \\ protected:<Family Times>" )
	}
	outspace = ""
	for (z=1; z<=nesting_level; z++)
		outspace = outspace "    "
	if ( device == "text" )
	  outputPrototype( outspace line[4] )
	else if ( device == "troff" )
	  outputPrototype( outspace line[4] )
	else
	{
	  printf( "<HardReturn>" )
	  outputPrototype( outspace line[4] )
	}
      }
    }
  }
  if ( access["private"] == "on" )
  {
  #
  #  collecting loop over the private lines of 1 ADT
  #
    start = 0
    for ( i=0; i<=num; i++ )
    {
      split( lines[i], line, "\n" )

      #
      # 3.11.95 check outermost access
      #
      if ( line[1] == "ACCESS") 
      {
	outeraccess = line[3]
        #printf("\n----- outeraccess = |%s| --------\n",outeraccess)
      }

      #
      # start for nesting ADT
      #
      if ( line[1] == "CLASS" || line[1] == "STRUCT" || line[1] == "UNION" )
      {	
	#
	# check for a normal nested ADT
	#
	if ( line[2] ~ "(::|<)"  && lastaccess == "private"  && outeraccess == "private")
        {	
	   if (line[2] ~ "::")
	   {		
		len = length(line[2])
    		match( line[2], "(::|<).*$")
    		newclass  = substr( line[2],RSTART+2,RLENGTH)
    		thisclass = substr( line[2],1,RSTART-1)
		if (class == thisclass)
		{
		  #
		  # hier is a nested class, struct, or union found!!!!!!!
		  #

		  basesubclass = line[2]
		  #printf("\n----basesubclass=|%s|-----\n",basesubclass)
		  #printf("\n----thisclass=|%s|---------newclass=|%s|-----\n",thisclass,newclass)
		  #printf("\n----RECURSION--class=|%s|--line[1]=|%s|--line[2]=|%s|---\n", \
			#class,line[1],line[2]);

		  nesttype = lower[line[1]]
                  nestbaseindex = 3
		  #printf("\nvor parse_and_print_adt: class=%s nesting_level=%s",class,nesting_level);
		  #printf("\ni=%s num=%s line[1]=|%s| line[2]=|%s| line[3]=|%s|\n", \
				#i,num,line[1],line[2],line[3]);
		  save_i = i
		  class = parse_and_print_adt(basesubclass,newclass,thisclass,nestbaseindex,nesttype)
		  i = save_i
		  #printf("\nafter parse_and_print_adt: class=%s nesting_level=%s",class,nesting_level);
		  #printf("\ni=%s num=%s line[1]=|%s| line[2]=|%s| line[3]=|%s|\n", \
				#i,num,line[1],line[2],line[3]);
	          start = 1
		  continue
		}
	    }
        }
      }
      else 
      {
	#
	# check for a TEMPLATE nested ADT
	#
	if ( line[1] == "TEMPLATE" && line[2] ~ /(class|union|struct)/  && lastaccess == "private" && outeraccess == "private")
	{
		len = length(line[3])
    		match( line[3], "(::|<).*$")
    		newclass  = substr( line[3],RSTART+2,RLENGTH)
    		thisclass = substr( line[3],1,RSTART-1)
		if (class == thisclass)
		{
		  #
		  # hier is a nested class, struct, or union found!!!!!!!
		  #

		  basesubclass = line[3]
		  #printf("\n----basesubclass=|%s|-----\n",basesubclass)
		  #printf("\n----thisclass=|%s|---------newclass=|%s|-----\n",thisclass,newclass)
		  #printf("\n---RECURSION--class=|%s|--line[1]=|%s|--line[2]=|%s|--line[3]=|%s|---\n", \
			#class,line[1],line[2],line[3]);

		  nesttype = "template"
                  nestbaseindex = 5
		  #printf("\nvor parse_and_print_adt: class=%s nesting_level=%s",class,nesting_level);
		  #printf("\ni=%s num=%s line[1]=|%s| line[2]=|%s| line[3]=|%s|\n", \
				#i,num,line[1],line[2],line[3]);
		  save_i = i
		  class = parse_and_print_adt(basesubclass,newclass,thisclass,nestbaseindex,nesttype)
		  i = save_i
		  #printf("\nafter parse_and_print_adt: class=%s nesting_level=%s",class,nesting_level);
		  #printf("\ni=%s num=%s line[1]=|%s| line[2]=|%s| line[3]=|%s|\n", \
				#i,num,line[1],line[2],line[3]);
	          start = 1
		  continue
		}
       }
      }
      #
      # end for nesting ADT
      #
      #
      # now print the protoypes of the public ADT
      #
      if ( ( line[1] == "MEMBER" ||
	     line[1] == "FUNC" ||
	     line[1] == "TYPEDEF" ||
	     line[1] == "ENUM" ||
	     line[1] == "FRIEND" ) &&
	   line[2] == class &&
	   line[3] == "private" )
      {
	outspace = ""
	for (z=1; z<nesting_level; z++)
		outspace = outspace "    "
	lastaccess = "private"
	if ( start == 0 )
	{
	  start = 1
	  if ( device == "text" )
	    print( outspace "\ \n" outspace "private:\n")
	  else if ( device == "troff" )
	    print( outspace "  \\f(COprivate:\\fP")
	  else if ( device == "mml" )
	    print(outspace "<HardReturn><Family Courier>\\ \\ private:<Family Times>" )
	}
	outspace = ""
	for (z=1; z<=nesting_level; z++)
		outspace = outspace "    "
	if ( device == "text" )
	  outputPrototype( outspace line[4] )
	else if ( device == "troff" )
	  outputPrototype( outspace line[4] )
	else
	{
	  printf( "<HardReturn>" )
	  outputPrototype( outspace line[4] )
	}
      }
    }
  }
  outspace = ""
  for (z=1; z<nesting_level; z++)
	outspace = outspace "    "
  if ( device == "text" )
  {
    print( outspace "};" )
    print( "\n" )
  }
  else if ( device == "troff" )
  {
    print( outspace "\\f(CO};" )
    print( "\\fP\n.fi" )
  }
  else if ( device == "mml" )
  {
    print( outspace "<HardReturn><Family Courier>};<Family Times>" )
  }
  nesting_level--
  return oldclass
}
