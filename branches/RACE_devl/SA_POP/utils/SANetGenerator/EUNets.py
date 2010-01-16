#Daniel L.C. Mack
#EUNets.py
#Runs the Net_Complete Script with a set of params for a certain number of runs.
#Nnets takes the number of networks to make, the rest of the params are from
#The Net Complete Script

import os
import sys

Nnets = int(sys.argv[1])

Ntasks = sys.argv[2]
Nconds = sys.argv[3]
inDegree = sys.argv[4]
outDegree = sys.argv[5]
rGen = sys.argv[6]
outname = sys.argv[7]

for each in range(0,Nnets):
	nname = outname + "_"+Ntasks+"_"+Nconds+"_"+inDegree+"_"+outDegree+"_"+str(each)
	os.system("python Net_Complete.py " + Ntasks + " " + Nconds + " " + inDegree + " " + outDegree + " " + rGen + " " + nname)
