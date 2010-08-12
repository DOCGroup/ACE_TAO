svn co https://svn.dre.vanderbilt.edu/DOC/Middleware/branches/FLARE_F6 .
svn co https://svn.dre.vanderbilt.edu/DOC/MPC/trunk MPC
svn co https://svn.dre.vanderbilt.edu/DOC/CUTS/trunk/CUTS CUTS
scp wolff1@thulcandra.dre.vanderbilt.edu:/home/wolff1/tmp/OpenSpliceDDSV3.4.2-x86.linux2.6-HDE-installer.bin .
chmod +x OpenSpliceDDSV3.4.2-x86.linux2.6-HDE-installer.bin
./OpenSpliceDDSV3.4.2-x86.linux2.6-HDE-installer.bin
echo "#\!/bin/bash" > ~/setup.sh
echo "export ACE_ROOT=`pwd`/ACE" >> ~/setup.sh
echo "export TAO_ROOT=`pwd`/TAO" >> ~/setup.sh
echo "export MPC_ROOT=`pwd`/MPC" >> ~/setup.sh
echo "export EXP_ROOT=/proj/F6/exp/FLARE-F6/test" >> ~/setup.sh
echo "export PATH=$PATH:$ACE_ROOT/bin:$MPC_ROOT" >> ~/setup.sh
echo "export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$ACE_ROOT/lib" >> ~/setup.sh
echo "source `pwd`/OpenSpliceDDS/V3.4.2/HDE/x86.linux2.6/release.com" >> ~/setup.sh
echo "cd $EXP_ROOT"
