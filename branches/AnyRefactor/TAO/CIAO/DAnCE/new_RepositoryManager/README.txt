This is the README file for the Repository Manager (RM).

In order to be able to compile the RM you need to have ZZIP lib because the RM 
uses it to read the contents of CCM packages. ZZIP itself is a C++ wrapper
around ZLIB, thus you will also need ZLIB. In order to compile the RM successfully
you will need to do the following:
 
1. Download ZLIB and ZZIP-LIB
2. Compile the multi-threaded versions of these libraries
3. create a separate directory for each library and create an include and a lib 
subdirectory within them. Then copy the libraries files in the library subdirectory.
Copy zlib.h in the zlib_path/include and copy zziplib.h, zzip-conf.h and zzip-msvc.h 
(for Windows platforms; Linux might have its own file!!!). This step might be easier if
you find a binary package of the libraries and just install it.
4. Set $ZLIB_ROOT and $ZZIP_ROOT to point to the directories where you placed the 
libraries and the include files.
5. Turn on zzip and zlib in default.features for MPC.
6. MPC will handle the rest.


Things to watch out for: 
On Windows make sure that you are linking the right ZIP libraries 
together with the rest of the libraries, i.e. if you are building the debug version of ACE, 
TAO and CIAO, then use the debug version of the ZIP libraries. Otherwise the RM will compile
but it will not work properly. 

to compile the idl with the tao_idl compiler:

tao_idl -GI new_RepositoryManager.idl -I%TAO_ROOT% -I%TAO_ROOT%\tao -I%TAO_ROOT%\orbsvcs -I%CIAO_ROOT%\DAnCE\ciao



