# =============================================================================
#
# = DESCRIPTION
#     Awk script for converting C++ class header file to classinfo file.
#     Requires nawk or gawk.
#     comments like /*  ... */ with ... goes over multiple lines are not ok
#		and will be not printed as comments!
#     class xxx
#        : public yyyy   is not allowed but : class xxx : public yyyy
#
# = AUTHOR(S)
#     Graham Dumpleton
#     K. Dorn
# 
# = COPYRIGHT
#     Copyright 1991 OTC LIMITED
#
# =============================================================================

BEGIN {
  initialised = 0
  accpubl = "on"
  accprot = "on"
  accpriv = "on"
  accpubl = publ
  accprot = prot
  accpriv = priv
  accpubl = "on"
  accprot = "off"
  accpriv = "off"
}
{
  if ( initialised == 0 )
  {
    initialised = 1
    "date" | getline line
    printf( "CLASS2INFO\n%s\n%s\n\n", line, filename )
    anonenum = 1
  }
  doParse();
  next
}



{
  print "error" | "cat 1>&2"
  exit
}

#
# If a comment starts with '// ==', discard lines until the end of the
# comment or a line of the form '// = NAME' is found. This is special
# case to deal with comment headers in files. e.g:
#
# // ========== ...
# //
# // = LIBRARY
# //     ...
#
# lines up to '// = LIBRARY' would be discarded.
#
# /^[\t ]*\/\/[\t ]*==+[\t ]*$/ {
#   handleHeader()
# }

function handleHeader()
{
  getline
  while ( \
          ( $0 ~ "^[\t ]*//" ) \
	  && \
	  ( $0 !~ "^[\t ]*//[\t ]*= [a-zA-Z0-9]+[^<>]*[\t ]*$" ) \
	)
  {
    getline
  }
}

#
# Extract labelled comments. These are denoted by '// = NAME', where the word
# NAME is actually replaced by the some identifying label.
# A labelled section is terminated by the end of the comment block, another
# labelled comment, or a comment line of the form '// =='. e.g:
# Note: a label cannot contain the characters '<>', these are identified
# as being special commands and are simply passed through.
#
# // = LIBRARY
# //     C++
# //
# // = VERSION
# //     ...
# //
# // ========== ...
#
# the LIBRARY section would be terminated by '// = VERSION', and the VERSION
# section by '// =====...'.
#
# /^[\t ]*\/\/[\t ]*= [a-zA-Z0-9]+[^<>]*[\t ]*$/ {
#   handleInfo()
# }

function handleInfo(    label )
{
  sub( "^[\t ]*//", "", $0 )
  while ( $0 ~ "^[\t ]*= [a-zA-Z0-9]+[^<>]*[\t ]*$" )
  {
    match( $0, " [a-zA-Z0-9]+[^<>]*$" )
    label = substr( $0, RSTART, RLENGTH )
    sub( "^[\t ]*", "", label )
    if ( name ~ "^$" )
      printf( "INFO\nGLOBAL\n%s\n", label )
    else if ( hiding ~ "^$" )
      printf( "INFO\nHDR\n%s\n%s\n", label, name )
    else
      printf( "INFO\nBODY\n%s\n%s\n%s\n", label, name, hiding )
    $0 = outputComment()
  }
}

function SetCommentClrComment( line, comment)
{
  if ( match( line, "(\/\\*.*\\*\/)?$" ) > 0)
  {
  comment = substr( line, RSTART, RLENGTH )
  sub( "//.*$", "", line )
  }
  else 
  if ( match( line, "(//.*)?$" ) > 0)
  {
  comment = substr( line, RSTART, RLENGTH )
  sub( "\/\\*.*$", "", line )
  }
}


function check_print_first()
{
 if ( ((hiding  == "private") && (accpriv == "on") ) || ((hiding  == "public") && (accpubl == "on") ) || ((hiding  == "protected") && (accprot == "on") )  ) 
  pra = 1
 else 
 {
  pra = 0
  $0 = ""
 }
 return pra
}

function check_print()
{
  if ( ( type ~ "class" ) && ( hiding ~ "^$" )  )
  {
     if (accpriv == "on")
     {
	pra = 1
	return pra
     }
     else 
     {
        pra = 0
        $0 = ""
	return pra
     }
  }
  else
  {
     
     if ((accpubl == "on") || ( hiding ~ "^$" ))
     {
	pra = 1
	return pra
     }
     else 
     {
        pra = 0
        $0 = ""
	return pra
     }
  }
 
 if ( ((hiding  == "private") && (accpriv == "on") ) || ((hiding  == "public") && (accpubl == "on") ) || ((hiding  == "protected") && (accprot == "on") ) ) 
 {
  pra = 1
 }
 else 
 {
  pra = 0
  $0 = ""
 }
 return pra
}

#
# transfer a c comment /* ... */ to a C++ // ... comment
#   the comment comes in feld and goes back in feld
#

function Set_c_Comment_to_CPP_Comment( feld)
{
  if ( match( feld, "\/\\*.*\\*\/$" ) > 0)
  {
#        printf("\n----feldb=|%s|----\n",feld);
  	sub( "\/\\*", "\/\/", feld )
  	sub( "\\*\/", "", feld )
#        printf("\n----felda=|%s|----\n",feld);
  }
  return feld
}



#
# Gather up unlabelled comments.
#
# /^[\t ]*\/\/.*$/ {
#   handleComment()
# }

function handleComment()
{
  if ( name ~ "^$" )
    printf( "COMMENT\nGLOBAL\n" )
  else if ( hiding ~ "^$" )
    printf( "COMMENT\nHDR\n%s\n", name )
  else
    printf( "COMMENT\nBODY\n%s\n%s\n", name, hiding )
  $0 = outputComment( $0 )
}

#
# Skip past '#define's. Leave it up to programmers to document important
# definitions with a section.
#
# /^[\t ]*#[\t ]*define/ {
#   handleDefine()
# }

function handleDefine(    line )
{
  if ( $0 ~ ".*\\\\$" )
  {
    end = 0
    while ( end == 0 )
    {
      getline line
      if ( line !~ ".*\\\\$" )
        end = 1
    }
  }
  # next
  $0 = ""
}

#
# Record name of files which are included.
# Note: Since we do not process '#if's we will get all includes, even if
# some are particular to some systems etc.
#
# /^[\t ]*#[\t ]*include[\t ]*<.+>/ {
#    ()
# }

function handleInclude()
{
  match( $0, "<.+>" )
  $0 = substr( $0, RSTART, RLENGTH )
  printf( "INCLUDE\n%s\n\n", $0 )
  # next
  $0 = ""
}

# /^[\t ]*#[\t ]*include[\t ]*".+"/ {
#   handleLocalInclude()
# }

function handleLocalInclude()
{
  match( $0, "\".+\"" )
  $0 = substr( $0, RSTART, RLENGTH )
  printf( "INCLUDE\n%s\n\n", $0 )
  # next
  $0 = ""
}

#
# Skip any other pre-processor directives.
#
# /^[\t ]*#.*$/ {
#   handlePreprocessor()
# }

function handlePreprocessor_alt()
{
  # next
  $0 = ""
}

function handlePreprocessor()
{
  printf( "PREPROC\n%s\n\n", $0 )
  # next
  $0 = ""
}

#
# Trap typedefs.
#
# /^typedef[\t ]+/ {
#   handleTypedef()
# }

function handleTypedef(    comment, line )
{
  line = $0
  while ( line !~ ";[\t ]*(//.*)?$" )
  {
    line = uncomment( line )
    getline
    sub( "^[\t ]*", "", $0 )
    line = line " " $0
    sub( "\\) \\(", ")(", line )
  }
  match( line, "(//.*)?$" )
  comment = substr( line, RSTART, RLENGTH )
  sub( "//.*$", "", line )
  sub( "^[\t ]*typedef[\t ]*", "typedef ", line )
  if ( name ~ "^$" )
    printf( "TYPEDEF\n%s\n%s\n%s\n", "::", "public", line )
  else
    printf( "TYPEDEF\n%s\n%s\n%s\n", name, hiding, line )
  $0 = outputComment( comment )
}

#
# Trap externs.
#
# /^extern[\t ]+/ {
#   handleExtern()
# }

function handleExtern(    lang, comment )
{
  if ( $0 ~ "\"C\"[\t ]*$" || $0 ~ "\"C\"[\t ]*\{[\t ]*$" )
  {
    # Bracketed includes (??). Skip them for now.
    while ( $0 !~ "}[\t]*$" && $0 !~ "}[\t ]*;[\t]*$" )
      getline
    getline
  }
  else
  {
    if ( $0 !~ ";[\t ]*(//.*)?$" )
      $0 = handleArgs( $0 )
    match( $0, "(//.*)?$" )
    comment = substr( $0, RSTART, RLENGTH )
    sub( "//.*$", "", $0 )
    sub( "^extern[\t ]*", "", $0 )
    if ( match( $0, "\"[^\"]*\"" ) != 0 )
    {
      lang = substr( $0, RSTART, RLENGTH )
      sub( "^\"[\t ]*", "", lang )
      sub( "[\t ]*\"$", "", lang )
    }
    else
      lang = "C++"
    sub( "\"[^\"]*\"[\t ]*", "", $0 )
    printf( "EXTERN\n%s\n%s\n", lang , $0 )
    $0 = outputComment( comment )
  }
}

#
# Trap class, struct, union and template definitions.
# Note: handleADT() does the hard work of determining if the particular
# use is in a definition, declaration or other.
#
# /^[\t ]*class/ {
#   handleClass()
# }

function handleClass()
{
  class = 1
  sub( "^[\t ]*class[\t ]+(ACE_[.]*Export[\t ]+)?", "", $0 )
  if ( handleADT( $0 ) == 1 )
  {
    if ( topName ~ "^$" )
      printf( "CLASS\n%s\n", name )
    else
      printf( "CLASS\n%s::%s\n", topName, name )
    outputSuperClasses( bases )
    bases = ""
  }
  class = 0
  # next
  $0 = ""
}

# /^[\t ]*enum/ {
#   handleEnumCsa()
# }

function handleEnumCsa()
{
  enum = 1
  sub( "^[\t ]*enum[\t ]+", "", $0 )
  if ( handleADT( $0 ) == 1 )
  {
    if ( topName ~ "^$" )
      printf( "ENUM\n%s\n", name )
    else
      printf( "ENUM\n%s::%s\n", topName, name )
    bases = ""
  }
  enum = 0
  # next
  $0 = ""
}

# /^[\t ]*struct/ {
#   handleStruct()
# }

function handleStruct()
{
  struct = 1
  sub( "^[\t ]*struct[\t ]+", "", $0 )
  if ( handleADT( $0 ) == 1 )
  {
    if ( topName ~ "^$" )
      printf( "STRUCT\n%s\n", name )
    else
      printf( "STRUCT\n%s::%s\n", topName, name )
    outputSuperClasses( bases )
    bases = ""
  }
  struct = 0
  # next
  $0 = ""
}

# /^[\t ]*union/ {
#   handleUnion()
# }

function handleUnion()
{
  union = 1
  sub( "^[\t ]*union[\t ]+", "", $0 )
  if ( handleADT( $0 ) == 1 )
  {
    if ( topName ~ "^$" )
      printf( "UNION\n%s\n", name )
    else
      printf( "UNION\n%s::%s\n", topName, name )
    outputSuperClasses( bases )
    bases = ""
  }
  union = 0
  # next
  $0 = ""
}

# /^[\t ]*template[\t ]*<.+>[\t ]+class/ {
#   handleTemplate()
# }

function handleTemplate()
{
  template = 1

  match( $0, "^[\t ]*template[\t ]*<.+>[\t ]+(class|struct|union)[\t ]+" )
# nested template error!!! match( $0, "^template[\t ]*<.+>[\t ]+(class|struct|union)[\t ]+" )

  line = substr( $0, RSTART, RLENGTH )
  match( line, "[\t ]+(class|struct|union)[\t ]+$" )
  adttype = substr( line, RSTART, RLENGTH )
  sub( "^[\t ]*", "", adttype )
  sub( "[\t ]*$", "", adttype )
  match( line, "<.+>" )
  targs = substr( line, RSTART, RLENGTH )
  sub( "^[\t ]*template[\t ]*<.+>[\t ]+(class|struct|union)[\t ]+", "", $0 )
  if ( handleADT( $0 ) == 1 )
  {
#    printf( "TEMPLATE\n%s\n%s\n%s\n", adttype, name, targs )
    if ( topName !~ "^$" )
     printf( "TEMPLATE\n%s\n%s::%s\n%s\n", adttype, topName, name, targs )
    else
     printf( "TEMPLATE\n%s\n%s\n%s\n", adttype, name, targs )
    outputSuperClasses( bases )
    bases = ""
  }
  template = 0
  # next
  $0 = ""
}

#
# Trap enumerated types.
#

# This handles enums where there is potentially more than one entry on
# a line. We throw away all comments in this case.

function handleLineEnum()
{
  enum = 1
  sub( "^[\t ]*enum[\t ]*", "", $0 )
  line = $0
  sub( "[\t ]*\{.*", "", line )
  pushLevel()
  name = line
  if ( name != "" )
  {
    if ( topName ~ "^$" )
      printf( "ENUM\n%s\n\n", name )
    else
      printf( "ENUM\n%s::%s\n\n", topName, name )
  }
  else
  {
    name = anonenum
    anonenum++
    if ( topName ~ "^$" )
      printf( "ANONENUM\n%s\n\n", name )
    else
      printf( "ANONENUM\n%s::%s\n\n", topName, name )
  }
  line = $0
  sub( "^.*\{[\t ]*", "", line )
  sub( "[\t ]*//.*$", "", line )
  while ( line !~ "}.*" )
  {
    getline
    line = line $0
    sub( "[\t ]*//.*$", "", line )
  }
  # Forget about variables for now.
  sub( "[\t ]*}.*", "", line )
  gsub( "[\t ]+", "", line )
  num = split( line, item, "," )
  for ( i=1; i<=num; i++ )
  {
    if ( topName ~ "^$" )
      printf( "ENUMITEM\n%s\n%s\n\n", name, item[i] )
    else
      printf( "ENUMITEM\n%s::%s\n%s\n\n", topName, name, item[i] )
  }
  $0 = ""
  popLevel( $0 )
  enum = 0
}

# This handles enums which are formatted to preferred style.
# Can collect comments meaningfully in this format.

function handleEnum()
{
  enum = 1
  sub( "^[\t ]*enum[\t ]*", "", $0 )
  sub( "([\t ]*\{)?[\t ]*$", "", $0 )
  sub( "^[\t ]*", "", $0 )
  pushLevel()
  name = $0
  if ( name != "" )
  {
    if ( topName ~ "^$" )
      printf( "ENUM\n%s\n\n", name )
    else
      printf( "ENUM\n%s::%s\n\n", topName, name )
  }
  else
  {
    name = anonenum
    anonenum++
    if ( topName ~ "^$" )
      printf( "ANONENUM\n%s\n\n", name )
    else
      printf( "ANONENUM\n%s::%s\n\n", topName, name )
  }
  getline
  while ( $0 ~ /^[\t ]*\/\/[\t ]*=/ )
    handleInfo()
  while ( 1 )
  {
    while ( $0 ~ /^[\t ]*$/ || $0 ~ /^[\t ]*\{/ )
      getline

    if ( $0 ~ /,[^\/]*,/ )
    {
      line = $0
      sub( "[,\t ]*//.*", " ", line )
      gsub( "[\t ]+", "", line )
      num = split( line, item, "," )
      for ( i=1; i<=num; i++ )
      {
	if ( topName ~ "^$" )
	  printf( "ENUMITEM\n%s\n%s\n", name, item[i] )
	else
	  printf( "ENUMITEM\n%s::%s\n%s\n", topName, name, item[i] )
	if ( i != num )
	  printf( "\n" )
      }
      sub( "^.*//", "//", $0 )
      currentlevel = level
      $0 = outputComment( $0 )
#      if ( $0 ~ /^$/ )
      if ( currentlevel != level )
      {
	enum = 0
	return
      }
    }
    else if ( $0 ~ /^.*,[\t ]*(\/\/.*)?$/ )
    {
      match( $0, ".*,([\t ]*//)?" )
      enumval = substr( $0, RSTART, RLENGTH )
      sub( "[\t ]*,.*$", "", enumval )
      sub( "^[\t ]*", "", enumval )
      if ( topName ~ "^$" )
	printf( "ENUMITEM\n%s\n%s\n", name, enumval )
      else
	printf( "ENUMITEM\n%s::%s\n%s\n", topName, name, enumval )
      sub( "^.*//", "//", $0 )
      currentlevel = level
      $0 = outputComment( $0 )
#      if ( $0 ~ /^$/ )
      if ( currentlevel != level )
      {
	enum = 0
	return
      }
    }
    else if ( $0 ~ /^.*(\/\/.*)?$/ )
    {
      match( $0, ".*([\t ]*//)?" )
      enumval = substr( $0, RSTART, RLENGTH )
      sub( "[\t ]*([\t ]*//.*)?$", "", enumval )
      sub( "^[\t ]*", "", enumval )
      if ( topName ~ "^$" )
	printf( "ENUMITEM\n%s\n%s\n", name, enumval )
      else
	printf( "ENUMITEM\n%s::%s\n%s\n", topName, name, enumval )
      sub( "^.*//", "//", $0 )
      currentlevel = level
      $0 = outputComment( $0 )
#      if ( $0 ~ /^$/ )
      if ( currentlevel != level )
      {
	enum = 0
	return
      }
      $0 = ""
    }
    else if ( $0 ~ /^[\t ]*}[\t ]*;/ )
    {
      # Shouldn't happen, but just in case.
      popLevel( $0 )
      enum = 0
      return
    }
  }
  enum = 0
}

#
# Trap the end of a abstract data type or enumerated type.
# Note: arrays are going to cause a problem here. e.g:
#
# int foo[] =
# {
# };
#
# so this needs to be cleaned up a bit.
#
# /^[\t ]*}.*;[\t ]*/ {
#   handleEndADT()
# }

function handleEndADT()
{
  popLevel( $0 )
  # next
  $0 = ""
}

#
# Trap private, protected and public keywords in class, struct or union.
#
# ( name !~ "^$" ) &&
# ( type !~ "enum" ) &&
# /^[\t ]*private[\t ]*:[\t ]*(\/\/.*)?$/ {
#   handlePrivate()
# }

function handlePrivate()
{
#if (accpriv == "on" )
  printf( "ACCESS\n%s\nprivate\n\n", name )
  hiding = "private"
  # next
  $0 = ""
}

# ( name !~ "^$" ) &&
# ( type !~ "enum" ) &&
# /^[\t ]*protected[\t ]*:[\t ]*(\/\/.*)?$/ {
#   handleProtected()
# }

function handleProtected()
{
#if (accprot == "on" )
  printf( "ACCESS\n%s\nprotected\n\n", name )
  hiding = "protected"
  # next
  $0 = ""
}

# ( name !~ "^$" ) &&
# ( type !~ "enum" ) &&
# /^[\t ]*public[\t ]*:[\t ]*(\/\/.*)?$/ {
#   handlePublic()
# }

function handlePublic()
{
#if (accpubl == "on" )
  printf( "ACCESS\n%s\npublic\n\n", name )
  hiding = "public"
  # next
  $0 = ""
}

#
# Handle friend declaration.
#
function handleFriend(    comment, line )
{
  line = $0
  if ( $0 ~ /[,(][\t ]*(\/\/.*)?$/ )
  {
    line = uncomment( line )
    line = handleArgs( line )
  }
  if ( line !~ /;[\t ]*(\/\/.*)?$/ )
  {
    line = uncomment( line )
    comment = handleInline()
    line = line ";"
  }
  else
  {
    match( line, "//.*$" )
    comment = substr( line, RSTART, RLENGTH )
  }
  gsub( "\t", " ", line )
  gsub( " +", " ", line )
  sub( "^ *", "", line )
  if ( name ~ "^$" )
    printf( "FRIEND\n%s\n%s\n%s\n", "::", "public", line )
  else
  if ( hiding ~ "^$" )
    printf( "FRIEND\n%s\n%s\n%s\n", name, "public", line )
  else
    printf( "FRIEND\n%s\n%s\n%s\n", name, hiding, line )
  $0 = outputComment( comment )
}

#
# Trap inline constructors with an initialiser list.
#
# ( name !~ "^$" ) &&
# ( type !~ "enum" ) &&
# /\)[\t ]*:[\t ]*.*\(.*\)[\t ]*({(.*})?[\t ]*)?(\/\/.*)?$/ {
#   handleInlineCtor()
# }

function handleInlineCtor()
{
  sub( "[\t ]*//.*", "", $0 )
  match( $0, ".*\\)[\t ]*:[\t ]*" )
  prototype = substr( $0, RSTART, RLENGTH )
  sub( "[\t ]*:[\t ]*$", "", prototype )
  outputFunction( prototype )
  if ( $0 !~ "}[\t ]*$" )
  {
    $0 = handleInline()
    if ( $0 ~ "^[\t ]*//.*$" )
      $0 = outputComment( $0 )
  }
  else
    $0 = outputComment( "" )
}

#
# Trap any inline functions, including constructors/destructors, with
# a complete prototype on the first line.
#
# ( name !~ "^$" ) &&
# ( type !~ "enum" ) &&
# /\)[\t ]*(const[\t ]*)?({(.*})?[\t ]*)?(\/\/.*)?$/ {
#   handleInlineFunction()
# }

function handleInlineFunction()
{
  sub( "[\t ]*//.*$", "", $0 )

# XXXX v1  match( $0, ".*\\)[\t ]*(const[\t ]*)?([^{]*{)?[\t ]*" )
# XXXX v2  match( $0, ".*\\)[\t ]*(const)?([\t ]*\{)?[\t ]*" )
# XXXX v2  prototype = substr( $0, RSTART, RLENGTH )

  pos = index($0,"{")
  if ( pos != 0 )
    prototype = substr( $0, 1, pos-1 )
  else
    prototype = $0

  sub( "[\t ]*\{[\t ]*$", "", prototype )
  outputFunction( prototype )
  if ( $0 !~ "}[\t ]*$" )
  {
    savename = name
    $0 = handleInline()
    if ( $0 ~ "^[\t ]*//.*$" )
      $0 = outputComment( $0 )
    else
    {
      if ( savename == name )
	printf( "\n" )
    }
  }
  else
    $0 = outputComment()
}

#
# Trap normal Enum declaration.
#
# ( name !~ "^$" ) &&
# ( type !~ "enum" ) &&
# /\)[\t ]*(enum[\t ]*)?(=[\t ]*0[\t ]*)?,[\t ]*(\/\/.*)?$/ {
#   handleenum()
# }

function handleEnumFun()
{
  $0 = Set_c_Comment_to_CPP_Comment( $0)
  match( $0, "//.*" )
  comment = substr( $0, RSTART, RLENGTH )
  $0 = uncomment( $0 )
  outputEnum( $0 )
  $0 = outputComment( comment )
}

#
# Trap normal function declaration.
#
# ( name !~ "^$" ) &&
# ( type !~ "enum" ) &&
# /\)[\t ]*(const[\t ]*)?(=[\t ]*0[\t ]*)?;[\t ]*(\/\/.*)?$/ {
#   handleFunction()
# }

function handleFunction()
{
  $0 = Set_c_Comment_to_CPP_Comment( $0)
  match( $0, "//.*" )
  comment = substr( $0, RSTART, RLENGTH )
  $0 = uncomment( $0 )
  outputFunction( $0 )
  $0 = outputComment( comment )
}

#
# Trap member variables.
#
# ( name !~ "^$" ) &&
# ( type !~ "enum" ) &&
# /[\t ]*;[\t ]*(\/\/.*)?$/ {
#   handleMember()
# }

function handleMember()
{
  $0 = Set_c_Comment_to_CPP_Comment( $0)
  match( $0, "//.*" )
  comment = substr( $0, RSTART, RLENGTH )
  sub( "[\t ]*//.*", "", $0 )
  outputMember( $0 )
  $0 = outputComment( comment )
}

function handleMember_orig()
{
  match( $0, "//.*" )
  comment = substr( $0, RSTART, RLENGTH )
  sub( "[\t ]*//.*", "", $0 )
  outputMember( $0 )
  $0 = outputComment( comment )
}

function handleMember_neu()
{
  match( $0, "/\\*.*\\*/" )
  comment = substr( $0, RSTART, RLENGTH )
  if ( length(comment) == 0 )
  {
  match( $0, "//.*" )
  comment = substr( $0, RSTART, RLENGTH )
  }
#  else
#  sub( "[\t ]*//.*", "", $0 )
  $0 = uncomment( $0 )
  outputMember( $0 )
  $0 = outputComment( comment )
}

#
# Trap remainder of functions and constructors, where prototypes go
# over more than one line.
#
# ( name !~ "^$" ) &&
# ( type !~ "enum" ) &&
# /((\((.*,)?)|([_a-zA-Z0-9_]+))[\t ]*(\/\/.*)?$/ {
#   handleMultilineEnums()
# }

function handleMultilineEnums(    prototype )
{
  $0 = uncomment( $0 )
  prototype = handleEnumArgs( $0 )
  prototype = uncomment( prototype )
  sub( "\{.*}[\t ]*$", "", prototype )
  outputEnum( prototype )
  if ( prototype !~ ";[\t ]*" )
  {
    savename = name
    $0 = handleInline()
    if ( line ~ "^[\t ]*//.*$" )
      $0 = outputComment( $0 )
    else
    {
      if ( savename == name )
	printf( "\n" )
    }
  }
  else
    $0 = outputComment()
}

#
# Trap remainder of functions and constructors, where prototypes go
# over more than one line.
#
# ( name !~ "^$" ) &&
# ( type !~ "enum" ) &&
# /((\((.*,)?)|([_a-zA-Z0-9_]+))[\t ]*(\/\/.*)?$/ {
#   handleMultilineFunctions()
# }

function handleMultilineFunctions(    prototype )
{
  $0 = uncomment( $0 )
  prototype = handleArgs( $0 )
  prototype = uncomment( prototype )
  sub( "\{.*}[\t ]*$", "", prototype )
  outputFunction( prototype )
  if ( prototype !~ ";[\t ]*" )
  {
    savename = name
    $0 = handleInline()
    if ( line ~ "^[\t ]*//.*$" )
      $0 = outputComment( $0 )
    else
    {
      if ( savename == name )
	printf( "\n" )
    }
  }
  else
    $0 = outputComment()
}

#
# pushLevel() and popLevel() implement a stack for classes encountered.
# This is to handle class definitions local to classes.
# pushLevel() is invoked when entering a abstract data type, and
# popLevel() is executed when leaving.
#
function pushLevel()
{
  level++
  names[level] = name
  types[level] = type
  hidings[level] = hiding
  topName = name

# 3.11.95  evtl auskommentieren!
  hiding = ""

  if ( class == 1 )
  {
    type = "class"
#    hiding = "private"
  }
  else if ( struct == 1 )
  {
    type = "struct"
#    hiding = "public"
  }
  else if ( union == 1 )
  {
    type = "union"
#    hiding = "public"
  }
  else if ( enum == 1 )
    type = "enum"
}

function popLevel( line )
{
  if ( name !~ "^$" )
  {
    printf( "END\n%s\n\n", name )
    oldname = name
    name = names[level]
    type = types[level]
    hiding = hidings[level]
    level--
    topName = names[level]
    if ( line ~ "^[\t ]*}.*[_a-zA-Z0-9]+.*;[\t ]*(//.*)?$" )
    {
      line = uncomment( line )
      sub( "^[\t ]*}[\t ]*", "", line )
      if ( line ~ "^[_a-zA-Z0-9]+" )
	line = oldname " " line
      else
	line = oldname line
      outputMember( line )
      return outputComment()
    }
    return ""
  }
}

#
# Removes comments from a line.
#
function uncomment( line )
{
  sub( "/\\*.*\\*/", "", line )
  sub( "[\t ]*//.*$", "" , line )
  return line
}

#
# Accumulates comment blocks and outputs them, followed by a blank line.
#
function outputComment( line )
{
  if ( line !~ "^[\t ]*//" )
    getline line

  num = 0
  while ( line ~ "^[\t ]*//" )
  {
    sub( "^[\t ]*//", "", line )
    if ( line ~ "^[\t ]*=(( [a-zA-Z0-9]+[^<>]*)|(=+))[\t ]*$" )
      break
    else
      lines[num++] = line

    getline line
  }
  indent = -1
  for ( i=0; i<num; i++ )
  {
    if ( lines[i] !~ "^[\t ]*=.*" )
    {
      if ( match( lines[i], "^ *[^ ]" ) != 0 )
      {
	if ( indent == -1 )
	  indent = RLENGTH-1
	else
	{
	  if ( RLENGTH-1 < indent )
	    indent = RLENGTH-1
	}
      }
    }
  }
  blank = 0
  for ( i=0; i<num; i++ )
  {
    if ( lines[i] ~ "^[\t ]*$" )
      blank++
    else
    {
      for ( j=0; j<blank; j++ )
	printf( "//\n" )
      blank = 0
      if ( lines[i] !~ "^[\t ]*=( <.+>)?[\t ]*" )
      {
	printf( "// %s\n", substr( lines[i], indent+1 ) )
      }
      else
      {
	sub( "^[\t ]*", "", lines[i] )
	printf( "// %s\n", lines[i] )
      }
    }
  }
  printf( "\n" )
  if ( line ~ "^[\t ]*}.*;[\t ]*$" )
    line = popLevel( line )

  return line
}

#
# Checks occurences of ADT and determines if they are in fact a definition
# or a declaration. If they are definition, it will generate any superclasses
# for the ADT.
#
function handleADT( line )
{
  #
  # Check for class declaration:
  #    class Foo;
  #
  if ( line ~ "[_a-zA-Z][ <,>_0-9a-zA-Z]*[\t ]*;" )
  {
    # Do nothing, this is a declaration.
    return 0
  }
  #
  # Check for derived classes:
  #   class Foo : Foobar
  #
  else if ( line ~ "[^:]:[^:]*" )
  {
    pushLevel()
    match( line, ".*[^:]:[^:]?" )
    name = substr( line, RSTART, RLENGTH-1 )
    sub( ":.*", "", name )
    sub( "[\t ]*$", "", name )
    if ( template == 1)
    {
      if ( name ~ "<.+>" )
      {
        match( name, "<.+>" )
        args = substr( name, RSTART, RLENGTH )
	sub( "[\t ]*<.+>[\t ]*", "", name )
      }
    }
    match( line, ":.*" )
    bases = substr( line, RSTART, RLENGTH )
    sub( ":[\t ]*", "", bases )
    sub( "[\t ]*$", "", bases )
    if ( bases !~ ",$" && bases !~ "^$" )
    {
      sub( "[\t ]*\{[\t ]*$", "", bases )
    }
    else
    {
      while ( bases ~ ".*,[\t ]*$" || bases ~ "^$" )
      {
        getline contbases
        contbases = uncomment( contbases )
        while ( length( contbases ) == 0 )
        {
          getline contbases
          contbases = uncomment( contbases )
        }
        bases = bases " " contbases
      }
      sub( "[\t ]*\{[\t ]*$", "", bases )
    }
  }
  #
  # Check for non-derived classes:
  #   class Foo
  #
  # else if ( line ~ "[_a-zA-Z][ <>,_0-9a-zA-Z]*(<.+>)?[\t ]*\{*$" )
  else if ( line ~ "[_a-zA-Z][ <>,_0-9a-zA-Z]*(<.+>)?[\t ]*(\{[\t ]*)?$" )
  {
    pushLevel()
    if ( template == 1)
      match( line, "[_a-zA-Z][_0-9a-zA-Z]*" )
    else
      match( line, "[_a-zA-Z][ <>,_0-9a-zA-Z]*" )
    name = substr( line, RSTART, RLENGTH )
    sub( "[\t ]*$", "", name )
    if ( template == 1)
    {
      sub( "[_a-zA-Z][_0-9a-zA-Z]*", "", line )
      if ( line ~ "^<" )
      {
        match( line, "<.+>" )
        args = substr( line, RSTART, RLENGTH )
      }
    }
  }
  else
  {
    # Discard anything else.
    return 0
  }
  return 1
}

#
# Generates the actual list of superclasses.
#
function outputSuperClasses( line )
{
  if ( line ~ /^[\t ]*$/ )
    printf( "\n" )
  else
  {
    gsub( "(\t| )+", " ", line )
    narg = 0
    while ( match( line, "[^<>,# ]*<[^<>]*>" ) )
    {
      narg++
      arg = substr( line, RSTART, RLENGTH )
      sub( "[^<>,# ]*<[^<>]*>", "#" narg, line )
      bargs["#" narg] = arg
      # Need the following to stop resubstitution of the pattern matched
      # back into the string.
      gsub( "&", "\\\\&", bargs["#" narg] )
    }
    num = split( line, item, "," )
    i = 1
    while ( i<=num )
    {
      access = "private"
      inherit = ""

      if ( item[i] ~ "[\t ]*public[\t ]*" )
      {
	access = "public"
	sub( "[\t ]*public[\t ]*", "", item[i] )
      }
      else if ( item[i] ~ "[\t ]*protected[\t ]*" )
      {
	access = "protected"
	sub( "[\t ]*protected[\t ]*", "", item[i] )
      }
      sub( "[\t ]*private[\t ]*", "", item[i] )

      if ( item[i] ~ "[\t ]*virtual[\t ]*" )
      {
	inherit = " virtual"
	sub( "[\t ]*virtual[\t ]*", "", item[i] )
      }

      while ( match( item[i], "#[0-9]+" ) )
      {
	arg = substr( item[i], RSTART, RLENGTH )
	sub( arg, bargs[arg], item[i] )
      }
      sub( "^[\t ]*", "", item[i] )
      sub( "[\t ]*$", "", item[i] )

      printf( "%s%s %s\n", access, inherit, item[i] )

      ++i
    }
    printf( "\n" )
  }
}

#
# Outputs enum prototypes.
#
function outputEnum( line )
{
  if ( line !~ ";[\t ]*$" )
  {
    sub( "[\t ]*$", ";", line )
    if ( line !~ /^[\t ]*inline[\t ]+/ )
      sub( "^[\t ]*", "inline ", line )
  }
  if ( ( type ~ "class" ) && ( hiding ~ "^$" ) )
    hide = "private"
  else
    hide = "public"
  printf( "ENUM\n%s\n%s\n", name, hiding ~ "^$" ? hide : hiding ) 
  gsub( "\t", " ", line )
  gsub( " +", " ", line )
  sub( "^ *", "", line )
  sub( " *$", "", line )
  sub( "( ?;)*$", ";", line )
  printf( "%s\n", line )
}

#
# Outputs function prototypes.
#
function outputFunction( line )
{
  if ( line !~ ";[\t ]*$" )
  {
    sub( "[\t ]*$", ";", line )
    if ( line !~ /^[\t ]*inline[\t ]+/ )
      sub( "^[\t ]*", "inline ", line )
  }
  if ( ( type ~ "class" ) && ( hiding ~ "^$" ) )
    hide = "private"
  else
    hide = "public"
  printf( "FUNC\n%s\n%s\n", name, hiding ~ "^$" ? hide : hiding ) 
  gsub( "\t", " ", line )
  gsub( " +", " ", line )
  sub( "^ *", "", line )
  sub( " *$", "", line )
  sub( "( ?;)*$", ";", line )
  printf( "%s\n", line )
}

#
# Output member variables.
#
function outputMember( line )
{
  if ( ( type ~ "class" ) && ( hiding ~ "^$" ) )
    hide = "private"
  else
    hide = "public"
  printf( "MEMBER\n%s\n%s\n", name, hiding ~ "^$" ? hide : hiding ) 
  gsub( "\t", " ", line )
  gsub( " +", " ", line )
  sub( "^ *", "", line )
  sub( " *$", "", line )
  sub( "( ?;)*$", ";", line )
  printf( "%s\n", line )
}

#
# Output member variables.
#
function outputMemberEnum( line )
{
  if ( ( type ~ "class" ) && ( hiding ~ "^$" ) )
    hide = "private"
  else
    hide = "public"
  printf( "ENUM\n%s\n%s\n", name, hiding ~ "^$" ? hide : hiding ) 
  gsub( "\t", " ", line )
  gsub( " +", " ", line )
  sub( "^ *", "", line )
  sub( " *$", "", line )
  sub( "( ?;)*$", ";", line )
  printf( "%s\n", line )
}

#
# Gathers up argument lists which cover more than one line.
#
function handleArgs( prototype )
{
  getline line
  line = uncomment( line )
  sub( "^[\t ]*", "", line )
  sub( "[\t ]*$", "", line )

  # 3.11.95 supress preproc in fuctions args
  sub( "#.*$", "", line )

  if ( ( prototype ~ "\\($" || line ~ "^\\(" || line ~ "^\\)" ) && \
   prototype !~ ",$" )
    prototype = prototype line
  else
    prototype = prototype " " line
  while ( \
   ( prototype !~ "\\)[\t ]*(const[\t ]*)?((\{.*)|(//.*))?$" ) \
   && \
   ( prototype !~ "\\)[\t ]*(const[\t ]*)?(=[\t ]*0[\t ]*)?;[\t ]*(//.*)?$" ) \
   && \
   ( prototype !~ "\\)[\t ]*:.*$" ) \
  )
  {
    getline line
    line = uncomment( line )
    sub( "^[\t ]*", "", line )
    sub( "[\t ]*$", "", line )

  # 3.11.95 supress preproc in fuctions args
  sub( "#.*$", "", line )

    if ( ( prototype ~ "\\($" || line ~ "^\\(" || line ~ "^\\)" ) && \
     prototype !~ ",$" )
      prototype = prototype line
    else
      prototype = prototype " " line
  }
  if ( prototype ~ "\\)[\t ]*:.*$" )
    sub( "\\)[\t ]*:.*$", ")", prototype )
  else if ( prototype ~ "\\)[\t ]*\{.*$" )
    sub( "\\)[\t ]*\{.*$", ")", prototype )
  return prototype
}

#
# Gathers up enum argument lists which cover more than one line.
#
function handleEnumArgs( prototype )
{
  getline line
  line = uncomment( line )
  sub( "^[\t ]*", "", line )
  sub( "[\t ]*$", "", line )
  if ( ( prototype ~ "\\{$" || line ~ "^\\{" || line ~ "^\\}" ) && \
   prototype !~ ",$" )
    prototype = prototype line
  else
    prototype = prototype " " line
  while ( \
   ( prototype !~ "\\}[\t ]*(const[\t ]*)?((\{.*)|(//.*))?$" ) \
   && \
   ( prototype !~ "\\}[\t ]*(const[\t ]*)?(=[\t ]*0[\t ]*)?;[\t ]*(//.*)?$" ) \
   && \
   ( prototype !~ "\\}[\t ]*:.*$" ) \
  )
  {
    getline line
    line = uncomment( line )
    sub( "^[\t ]*", "", line )
    sub( "[\t ]*$", "", line )
    if ( ( prototype ~ "\\($" || line ~ "^\\(" || line ~ "^\\)" ) && \
     prototype !~ ",$" )
      prototype = prototype line
    else
      prototype = prototype " " line
  }
  if ( prototype ~ "\\}[\t ]*:.*$" )
    sub( "\\}[\t ]*:.*$", "}", prototype )
  else if ( prototype ~ "\\}[\t ]*\{.*$" )
    sub( "\\}[\t ]*\{.*$", "}", prototype )
  return prototype
}

#
# Skips inline code. End of such code is determined when either a blank line,
# comment, or end of ADT is encountered.
#
function handleInline()
{
  getline line
  while (line !~ "^[\t ]*(}.*;[\t ]*)?(//.*)?$" )
    getline line

  if ( line ~ "[\t ]*}[\t ]*[_a-zA-Z0-9]*[\t ]*;[\t ]*$" )
  {
    printf( "\n" )
    popLevel( line )
  }

  if ( line ~ "^[\t ]*//.*" )
    return line
  else
    return ""
}

#
# The main parsing loop.
# This implements a recursive descent parser of sorts.
#
function doParse()
{
  while ( $0 !~ "^$" )
  {
    if ( $0 ~ /^[\t ]*template[\t ]*<[^:]+>[\t ]*$/ )
    {
      getline line
      $0 = $0 " " line
    }

    if ( $0 ~ /^[\t ]*\/\/[\t ]*==+[\t ]*$/ )
    {
      handleHeader()
    }
    else if ( $0 ~ /^[\t ]*\/\/[\t ]*= [a-zA-Z0-9]+[^<>]*[\t ]*$/ )
    {
      handleInfo()
    }
    else if ( $0 ~ /^[\t ]*\/\/.*$/ )
    {
      handleComment()
    }
    else if ( $0 ~ /^[\t ]*#[\t ]*define/ )
    {
      handleDefine()
    }
    else if ( $0 ~ /^[\t ]*#[\t ]*include[\t ]*<.+>/ )
    {
      handleInclude()
    }
    else if ( $0 ~ /^[\t ]*#[\t ]*include[\t ]*".+"/ )
    {
      handleLocalInclude()
    }
    else if ( $0 ~ /^[\t ]*#.*$/ )
    {
      handlePreprocessor()
    }
    else if ( $0 ~ /^[\t ]*typedef[\t ]+/ )
    {
      handleTypedef()
    }
    else if ( $0 ~ /^extern[\t ]+/ )
    {
      handleExtern()
    }
    else if ( $0 ~ /^[\t ]*class[\t ]/ )
    {
      handleClass()
    }
    else if ( $0 ~ /^[\t ]*struct[\t ]/ )
    {
      handleStruct()
    }
    else if ( $0 ~ /^[\t ]*union[\t ]/ )
    {
      handleUnion()
    }
    else if ( $0 ~ /^[\t ]*template[\t ]*<.+>[\t ]+class[\t ]/ )
    {
      handleTemplate()
    }
    else if ( $0 ~ /^[\t ]*enum[\t ]*.*;[\t ]*(\/\/.*)?$/ )
    {
#printf("\ntype=%s-------single------\n",type);
      handleEnumFun()
    }
    else if ( ($0 ~ /((\((.*,)?)|([_a-zA-Z0-9_]+))[\t ]*(\/\/.*)?$/) && ( name !~ "^$" ) && ( $0 ~ /^[\t ]*enum/ ) )
    {
#printf("\ntype=%s-------multi------\n",type);
             handleMultilineEnums()
    }
    else if ( $0 ~ /^[\t ]*}.*;[\t ]*/ )
    {
      handleEndADT()
    }
    else if ( ( name !~ "^$" ) && ( type !~ "enum" ) )
    {
      if ( $0 ~ /^[\t ]*private[\t ]*:[\t ]*(\/\/.*)?$/ )
      {
	handlePrivate()
      }
      else if ( $0 ~ /^[\t ]*protected[\t ]*:[\t ]*(\/\/.*)?$/ )
      {
        handleProtected()
      }
      else if ( $0 ~ /^[\t ]*public[\t ]*:[\t ]*(\/\/.*)?$/ )
      {
	handlePublic()
      }
      else if ( $0 ~ /^[\t ]*friend[\t ]+/ )
      {
	handleFriend()
      }
      else if ( $0 ~ /\)[\t ]*:[\t ]*.*\(.*\)[\t ]*(\{(.*})?[\t ]*)(\/\/.*)?$/ )
      {
	handleInlineCtor()
      }
      else if ( $0 ~ /\)[\t ]*(const[\t ]*)?(\{(.*})?[\t ]*)?(\/\/.*)?$/ &&
       $0 !~ /^.*operator[\t ]*\(\)[\t ]*(\/\/.*)?$/ )
      {
	handleInlineFunction()
      }
      else if ( $0 ~ /\)[\t ]*(const[\t ]*)?(=[\t ]*0[\t ]*)?;?[\t ]*(\/\/.*)?$/ &&
       $0 !~ /^.*operator[\t ]*\(\)[\t ]*(\/\/.*)?$/ )
      {
        handleFunction()
      }
      else if ( $0 ~ /[\t ]*;[\t ]*(\/\/.*)?(\/\\*.*\\*\/)?$/ )
      {
	handleMember()
      }
      else if ( $0 ~ /((\((.*,)?)|([_a-zA-Z0-9_>)]+[*&]?))[\t ]*(\/\/.*)?$/ )
      {
	handleMultilineFunctions()
      }
      else
	$0 = ""
    }
    else
       $0 = ""
  }
}
