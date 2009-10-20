#Daniel L.C. Mack
#Build Random Skeletons for SA_POP

import networkx as nx
import re
import sys
import random as rand
import Rand_Skeletons as RS

Ntasks = int(sys.argv[1]) 
Nconds = int(sys.argv[2])
inDegree = float(sys.argv[3])
outDegree = float(sys.argv[4])
rGen = int(sys.argv[5])
output = sys.argv[6]

ofile = open(output, 'w')
ofile.write("strict digraph testGraph {\n")

SANet = nx.DiGraph()
tasks = []
conds = []

#Build and record different types of nodes
for each in range(0,Ntasks+1):
	tname = "task" + str(each)
	SANet.add_node(tname)
	tasks.append(tname)
	
for each in range(0,Nconds+1):
	cname = "cond" + str(each)
	SANet.add_node(cname)
	conds.append(cname)	

#Choose function to generate from
if rGen == 1:
	SANet = RS.erdosGen(SANet, tasks, conds, inDegree, outDegree)		

#Print out the graph currently into something resembling an SANet, for GraphViz to render			
for each in tasks:
	ofile.write("\"" + each + "\" [shape = box, style=filled, color = grey];\n"  )

for each in conds:
	ofile.write("\"" + each +"\" [ style=filled, color = grey];\n")
	
for edge in SANet.edges():
	ofile.write("\"" + edge[0] + "\" -> \"" + edge[1] + "\";\n")
ofile.write("}\n")
ofile.close()			
			
		
		

	
	
	