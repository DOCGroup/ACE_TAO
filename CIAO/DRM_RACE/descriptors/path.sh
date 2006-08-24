ulimit -c unlimited
export CIAO_DEBUG_LEVEL=11
export NameServiceIOR=corbaloc:iiop:blade35.isislab.vanderbilt.edu:45000/NameService
export MIDDLEWARE_ROOT=/opt/DRM/Middleware
export ACE_ROOT=$MIDDLEWARE_ROOT/ACE_wrappers
export TAO_ROOT=$ACE_ROOT/TAO
export CIAO_ROOT=$TAO_ROOT/CIAO
export RACE_ROOT=$CIAO_ROOT/RACE
export CIAO_DEBUG_LEVEL=10
export COSMIC_ROOT=$MIDDLEWARE_ROOT/CoSMIC
export CUTS_ROOT=$COSMIC_ROOT/CUTS
export XERCESCROOT=$MIDDLEWARE_ROOT/xerces
export PLAN_PATH=/opt/descriptors
export SERVER_NAME=blade35.isislab.vanderbilt.edu
export LD_LIBRARY_PATH=$ACE_ROOT/lib:$XERCESCROOT/lib:$COSMIC_ROOT/lib:$COSMIC_ROOT/PIM/PICML/examples/RT1H/:$COSMIC_ROOT/PIM/PICML/examples/Hog_String:/usr/local/lib/:/opt/xalanCpp/lib:.
export PATH=$ACE_ROOT/bin:$CIAO_ROOT/bin:$CIAO_ROOT/DAnCE/NodeManager:$CIAO_ROOT/DAnCE/NodeApplication:/opt/graphviz/graphviz-2.8/bin:/opt/xalanCpp/bin:$PATH:/opt/gnuplot/bin:/opt/gnuplot/bin
