# =============================================================================
#
# = DESCRIPTION
#     Awk script to convert classinfo description file to src stubs.
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
#  separator = "/* ------------------------------"\
#   "------------------------------------------- */"
  separator = ""
  csa_method_trailer=sprintf("\n/*] END Method */\n\n")
  FS="\n"; RS=""
  firsttime = "first"
  templ=""
  blank=" "
  classfunc = ""
  rettyp = ""
  classname=""
  classtitle=""
  filedate=""
  filename=""
  filename1=""
  author=""
  classdescription=""
  methoddescription=""

#   csaprintheader=1

#  accpubl = "off"
#  accprot = "off"
#  accpriv = "off"
}
#
# variable von awk sind nicht in begin zuweisbar!
#
{
   accmergecxxhxx = merge
   accloop = loop
   accpubl = publ
   accprot = prot
   accpriv = priv
#printf("\nmerge=%s loop=%s csaprintheader=%s firsttime=%s\n",merge,loop,csaprintheader,firsttime);
}

function csa_print_compilation_header_alt()
{
 	if ( length( firsttime ) > 3 )
	{
           
  csa_compilation_header=sprintf("\n/*[ Compilation unit "\
	"----------------------------------------------------------\n"\
	"\n"\
	"   Component	    : CSA - OSC\n"\
	"\n"\
	"   Name            : %s.[Ci]\n"\
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
	"\n",filename1,author,filedate,classdescription,names[$2]);
	printf("%s",csa_compilation_header);
	firsttime = ""
	}
}

function csa_print_compilation_header()
{
   if ((csaprintheader == "on" && merge != "on") || (merge == "on" && loop == "1"))
   {        	
        if ( length( firsttime ) > 3 )
	{
  FS=" "; RS="\n"
		read_fvmecxx_file()
		read_fvcocxx_file()
  FS="\n"; RS=""
           
  		#csa_compilation_header=sprintf(vcocxx,filename1,author,filedate,classdescription,names[$2]);
csa_compilation_header=sprintf(vcocxx,filename1,author,filedate,classdescription,classname);
		printf("%s",csa_compilation_header);
		firsttime = ""
	}
   }
   else if (csaprintheader == "on" && merge == "on" && loop != "1" && length( firsttime ) > 3)
   {
  FS=" "; RS="\n"
		read_fvmecxx_file()
		read_fvcocxx_file()
  FS="\n"; RS=""
	firsttime = ""
   }
}

function csa_print_method_header_alt()
{
  prrettyp = ""
  if (rettyp != "") 
	prrettyp=sprintf("type = %s",rettyp);
  else
	prrettyp=sprintf("-");

  csa_method_header=sprintf("\n/*[ Method ---------------------"\
   "-------------------------------------------\n"\
   "\n"\
   "  Name             : %s\n"\
   "\n"\
   "  Description      : %s\n"\
   "\n"\
   "  Return           : %s\n"\
   "\n"\
    "--------------------------------------------"\
   "------------------------------*/\n\n",classfunc,methoddescription,prrettyp)
   printf("%s",csa_method_header);
} 

function csa_print_method_header()
{
 if (csaprintheader == "on")
 {      
  prrettyp = ""
  if (rettyp != "") 
	prrettyp=sprintf("type = %s",rettyp);
  else
	prrettyp=sprintf("-");

  csa_method_header=sprintf(vmecxx,classfunc,methoddescription,prrettyp)
  printf("%s",csa_method_header);
 }
} 

function csa_print_method_trailer()
{
 if (csaprintheader == "on")
 {     
  csa_method_trailer=sprintf("/*] END Method */\n")
  printf("\n%s\n",csa_method_trailer);
 }
} 

function csa_get_method_description()
{
  n = split( $0, line, "\n" )
  methoddescription=""
  for ( i=5; i<=n; i++ )
  {
        len = length(line[i])
        comm = substr(line[i],4,len)
	methoddescription = methoddescription comm "\n                     " 
  }
} 


function read_fvmecxx_file()
{
    vmecxx1 = "\n"
    while ( getline < fvmecxx > 0 )
      vmecxx1 = vmecxx1 $0 "\n"
    vmecxx =  vmecxx1  
}

function read_fvcocxx_file()
{
    vcocxx1 = "\n"
    while ( getline < fvcocxx > 0 )
      vcocxx1 = vcocxx1 $0 "\n"
    vcocxx = vcocxx1 
}


#
#  hier laueft er durch mit allen zeilen
#

$1 ~ /CLASS2INFO/ {
  filedate=$2
  filename=$3
  len=length($3)
  filename1=substr($3,1,(len-2))
}

$1 ~ /INFO/ && $2 ~ /GLOBAL/ && $3 ~ /LIBRARY/ {
  library=$4
  len=length(library)
  library=substr(library,4,len)
  if ( merge != "on")
   printf( "#include <%s/%s>\n", library, filename )
  else
   if ( loop == "1")
    printf( "#include <%s/%s>\n", library, filename )
}

$1 ~ /INFO/ && $2 ~ /GLOBAL/ && $3 ~ /AUTHOR/ {
  l=length($4)
  author=substr($4,4,l)
}

$1 ~ /INFO/ && $2 ~ /HDR/ && $3 ~ /TITLE/ {
  classname=$4
  classtitle=$5
}

$1 ~ /INFO/ && $2 ~ /HDR/ && $3 ~ /DESCRIPTION/ {
  n = split( $0, line, "\n" )
  classdescription=""
  for ( i=5; i<=n; i++ )
  {
        len = length(line[i])
        comm = substr(line[i],4,len)
	classdescription = classdescription comm "\n                     " 
  }
#  if (csaprintheader == "on")
#     csa_print_compilation_header()
}

$1 ~ /INCLUDE/ {
  if ( merge != "on")
    printf( "#include %s\n", $2)
#  else
#   if ( loop == "1")
#    printf( "#include %s\n", $2)
}

#
# neu mit nested classes
#
$1 ~ /(CLASS|STRUCT|UNION)/ {
  nestedclass = ""
  nested = ""
  templnested = ""
  containerclass = ""
  if ( $2 ~ "(::|<)" )
  {
  	len = length($2)
  	match( $2, "(::|<).*$")
  	newclass  = substr( $2,RSTART+2,RLENGTH)
  	thisclass = substr( $2,1,RSTART-1)
  	if (thisclass == classext)
	{
		  nestedclass = newclass
  		  nested = "on"
  		  containerclass = thisclass
  		  class = newclass $3  # 23.10.95
	}
        else
		  nestedclass = ""
  }
  else
  {
  	names[$2] = $2
  }
}

#
# neu mit nested classes
#
$1 ~ /TEMPLATE/ {
  templnestedclass = ""
  nested = ""
  templnested = ""
  if ( $2 ~ /(class|union|struct)/ && $3 ~ "(::|<)" )
  {
  	len = length($3)
  	match( $3, "(::|<).*$")
  	templnewclass  = substr( $3,RSTART+2,RLENGTH)
  	templthisclass = substr( $3,1,RSTART-1)
  	if (templthisclass == classext)
	{
		  templnestedclass = templnewclass
		  nestedtempl = $4
#printf("\n------nestedtempl=%s----\n",nestedtempl);
  		  class = templnewclass $4
  		  nestedtemplclass = templnewclass $4
  		  nested = "on"
  		  templnested = "on"
  		  containerclass = templthisclass
	}
        else
		  templnestedclass = ""
  }
  else
  {
  	class = $3 $4
  	templclass = $3 $4
  	args[$3] = $4
  	templ=$4
  	# This needs extra work.
  	#
  	# macht aus: <class hans,class otto> folgendes:
  	#           <hans,otto>

  	sub( "<[\t ]*(class)[\t ]+", "<", class )
  	sub( ",[\t ]*(class)[\t ]+", ",", class )
  
  	names[$3] = class
  }
}

#
# neu nested classes, achtung: nur level1 moeglich, da keine rekursion!
#
$1 ~ /END/ && $2 == class{
 nested= ""
 templnested= ""
}

#
# alt ohne nested classes
#
#$1 ~ /(CLASS|STRUCT|UNION)/ {
#  names[$2] = $2
#}

#
# alt ohne nested classes
#
#$1 ~ /TEMPLATE/ {
#  class = $3 $4
#  templclass = $3 $4
#  args[$3] = $4
#  templ=$4
#  # This needs extra work.
#
# macht aus: <class hans,class otto> folgendes:
#           <hans,otto>
#
#  sub( "<[\t ]*(class)[\t ]+", "<", class )
#  sub( ",[\t ]*(class)[\t ]+", ",", class )
  
#  names[$3] = class
#}


$1 ~ /FUNC/ {
 prototype = $4
 class = $2
 hiding = $3
 if ( (((hiding  == "private") && (accpriv == "on") ) || ((hiding  == "public") && (accpubl == "on") ) || ((hiding  == "protected") && (accprot == "on") )) && ((mode != "single")  || ((mode == "single") && ((classext == class) || (class == nestedclass) || (class == templnestedclass))) ))
 {
#  printf("\nclassext=%s\n",classext);
#  printf("\nclass   =%s\n",class);
#  printf("\nhiding=%s\n",hiding);
  csa_print_compilation_header()
  # Filter out inline functions.
  flinline=0
  if ( prototype ~ /^[\t ]*inline[\t ]+/ )
  {
     flinline=1
#printf("\n######inline####\n");
#     next
      sub( "^[\t ]*inline[\t ]+", "", prototype )
  }
  # Filter out pure virtual functions: wenn prototype "= 0 ;" enthaelt!
  flpurevirt=0
  if ( prototype ~ /[\t ]*=[\t ]*0[\t ]*;[\t ]*$/ )
  {
    flpurevirt=1
    next
  }
  # Strip out unwanted bits "static oder virtual".
  flstatic=0
  if ( prototype ~ /^[\t ]*static[\t ]+/ )
  {
    flstatic=1
  }
  flvirtual=0
  if ( prototype ~ /^[\t ]*virtual[\t ]+/ )
  {
    flvirtual=1
  }
  sub( "^[\t ]*(static|virtual)[\t ]+", "", prototype )
#            suchstring              ersetzen  src/zielstring
  				# Strip out unwanted bits "; 
				# blank und tab" am ende der methode.
  sub( ";[\t ]*$", "", prototype )
  # Strip out unwanted bits : 1. default werte der methode aus class def und comma.
#  gsub( "[\t ]*\\(=[^(=][^,=]*,", ",", prototype )
#  gsub( "[\t ]*\\(=[^(=][^,=]*\\)", ")", prototype )

#neu
  if ( prototype !~ /[\t \&\*]*operator[^a-zA-Z0-9]+/ )
  {
#neu
  # Strip out unwanted bits : 1. default werte der methode aus class def und comma, aber nicht bei
  # operator funs , wegen deren "=" !
    gsub( "[\t ]*=[^(=][^,=]*,", ",", prototype )
    gsub( "[\t ]*=[^(=][^,=]*\\)", ")", prototype )
#neu
  }
#neu

#printf("\nprototype=|%s|\n",prototype);
  
  # Put in class name.
  floperator=0
  if ( prototype ~ /[\t \&\*]*operator[^a-zA-Z0-9]+/ )
  {
    #
    # operator funcs
    #
    floperator=1
#printf("\noperatorfunc|%s|\n",prototype);
    match( prototype, "[^\t \&\*]*operator" )
    typoper = substr( prototype, 1,RSTART-1)
#printf("\ntypoper=|%s|\n",typoper);
    wholelen=length(prototype)
    restoper = substr( prototype, RSTART, wholelen)
#printf("\nrestoper=|%s|\n",restoper);
    len=length(restoper)
    restoper1 = substr( restoper, 9, len )
    match( restoper1, "[^\t ].*$" )    # trimstring fuer anf des feldes
    restoper2 = substr( restoper1, RSTART, RLENGTH )
#printf("\nrestoper2=|%s|\n",restoper2);
    len=length(restoper2)	       # pvar++
    restoper3 = substr( restoper2,2, len )
#printf("\nrestoper3=|%s|\n",restoper3);
    match( restoper3, "\\(.*$" )       # gebe naechsten string der 
				       # mit ( anfaengt bis zum ende
    decl = substr( restoper3, RSTART, RLENGTH )
#printf("\nparas=|%s| s=%d l=%d\n",decl,RSTART,RLENGTH);
    parstart=index(restoper,decl)
    restoperfun=substr(restoper,1,parstart-1)
#printf("\nrestoperfun=|%s|\n",restoperfun);

#neu
  # Strip out unwanted bits : 1. default werte der methode aus class def und comma.
    gsub( "[\t ]*=[^(=][^,=]*,", ",", decl )
    gsub( "[\t ]*=[^(=][^,=]*\\)", ")", decl )
    restoper=restoperfun decl
#printf("\nneu restoper=|%s|\n",restoper);
#neu


if (nested == "on" )
    prototype=typoper containerclass "::" class "::" restoper
else
    prototype=typoper class "::" restoper
#printf("\nprototype=|%s|\n",prototype);

if (nested == "on" )
    classfunc = containerclass "::" class "::" restoperfun
else
    classfunc = class "::" restoperfun
#printf("\nclassfunc=|%s|\n",classfunc);
    rettyp = typoper
#printf("\nrettyp=|%s|\n",rettyp);
     funcname = classfunc
#printf("\n7|%s|\n",funcname);
  }
  else
  {
    #
    # other functions
    #
    match( prototype, "[^\t ]*\\(.*$" )
    decl = substr( prototype, RSTART, RLENGTH )

#-----neu start, damit 0-n blanks zwischen "func-name" und "(" stehen koennen!
    match( decl, "\\(.*$" )
    decl = substr( decl, RSTART, RLENGTH )
    parstart=index(prototype,decl)
    typfuncn=substr(prototype,1,parstart-1)
    sub( "[\t ]*$", "", typfuncn )
    prototype = ""
    prototype = typfuncn " " decl
#printf("\nprototypeneu=|%s|\n",prototype);
#printf("\ntypfuncnneu=|%s|\n",typfuncn);
#printf("\ndeclneu=|%s|\n",decl);


    match( prototype, "[^\t ]*\\(.*$" )
    decl = substr( prototype, RSTART, RLENGTH )
#-----neu end

#printf("\ndecl=|%s|\n",decl);
    parstart=index(prototype,decl)
    typfuncn=substr(prototype,1,parstart-1)

    nitems=split(typfuncn,typfuncitems,"[\t \&\*]")
    sub( "[\t ]*$", "", typfuncn )
    funcn = typfuncitems[nitems-1]
    funstart=index(typfuncn,funcn)
    typ=substr(typfuncn,1,funstart-1)
#printf("\ntypfuncn=|%s|\n",typfuncn);
#printf("\nfuncn=|%s|\n",funcn);
#printf("\ntyp=|%s|\n",typ);
if (nested == "on" )
    sub( "^", typ containerclass "::" class "::" funcn, decl )
else
    sub( "^", typ class "::" funcn, decl )
    prototype=decl
#printf("\nprototype=|%s|\n",prototype);
    classfunc = ""
if (nested == "on" )
    sub( "^", containerclass "::" class "::" funcn, classfunc )
else
    sub( "^", class "::" funcn, classfunc )
#printf("\nclassfunc=|%s|\n",classfunc);
    rettyp = ""
    sub( "^", typ, rettyp )
#printf("\nrettyp=|%s|\n",rettyp);
#printf("\n6|%s|\n",prototype);
    match( prototype, "^[^(]*\\(" )
    funcname = substr( prototype, RSTART, RLENGTH )
#printf("\n7|%s|\n",funcname);
  }

  # Output it.
  
  if (firsttime == "")
  {
  csa_get_method_description()
#  if (csaprintheader == "on")
      csa_print_method_header()

  if ( length(prototype) > 75 )
  {
    match( prototype, "^[^(]*\\(" )
    funcname = substr( prototype, RSTART, RLENGTH )
    if ( prototype ~ "^[^(]*\\(\\)[\t ]*\\(" )
    {
      funcname = funcname ")("
      sub( "^[^(]*\\(\\)[\t ]*\\(", "", prototype )
    }
    else
      sub( "^[^(]*\\(", "", prototype )

    if ( templclass ~ "<"  && templnested == "" && nested == "")
    {
      printf( "template%s\n", templ)
    }
    if ( templnested == "on")
    {
      printf( "template%s\n", nestedtempl )
    }
    printf( "%s\n", funcname )

    match( prototype, "\\)([\t ]*const)?[\t ]*$" )
    functail = substr( prototype, RSTART, RLENGTH )
    sub( "\\)([\t ]* const)?[\t ]*$", "", prototype )
    narg = 0
    while ( match( prototype, "[^<>,#]*<[^<>]*>" ) )
    {
      narg++
      arg = substr( prototype, RSTART, RLENGTH )
      sub( "[^<>,#]*<[^<>]*>", "#" narg, prototype )
      fargs["#" narg] = arg
      # Need the following to stop resubstitution of the pattern matched
      # back into the string.
      gsub( "&", "\\\\&", fargs["#" narg] )
    }
    numargs = split( prototype, args, "," )
    for ( m=1; m<=numargs; m++ )
    {
      while ( match( args[m], "#[0-9]+" ) )
      {
	arg = substr( args[m], RSTART, RLENGTH )
	sub( arg, fargs[arg], args[m] )
      }
      sub( "[\t ]*", "", args[m] )
      printf( " %s", args[m] )
      if ( m == numargs )
      {
	print( "" )
      }
      else
	print( "," )
    }
    if (flinline == 1)
	printf("/* inline */\n");
#    printf( "%s\n%s{\n}\n\n%s\n", functail,csa_method_trailer, separator )
    printf( "%s\n", functail)
#    csa_print_method_trailer()
#    printf( "{\n}\n\n%s\n",  separator )
  }
  else
  {
    if (flinline == 1)
	printf("/* inline */\n");

    if ( templclass ~ "<" && templnested == "" && nested == "")
    {
      printf( "template%s\n", templ )
    }
    if ( templnested == "on")
    {
      printf( "template%s\n", nestedtempl )
    }
#    printf( "%s\n%s{\n}\n\n%s\n", prototype,csa_method_trailer, separator )
    printf( "%s\n", prototype)
#    csa_print_method_trailer()
#    printf( "{\n}\n\n%s\n",  separator )
  }
  csa_print_method_trailer()
  if (rettyp != "" && rettyp != "void " && rettyp != "void") 
    printf( "{\n\treturn((%s)0);\n}\n\n%s\n",rettyp,  separator )
  else
    printf( "{\n}\n\n%s\n",  separator )

  }
  # end of firsttime
 }
 # end of if
}
#end of FUNC
