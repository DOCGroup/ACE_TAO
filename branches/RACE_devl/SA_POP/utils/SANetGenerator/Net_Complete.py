#Daniel L.C. Mack
#Build Random Graphs for SA_POP with simple parameter assignment
#Note, the random values can cause a dead process, am working to find out why.

import networkx as nx
import re
import sys
import random as rand
import Rand_Skeletons as RS
import Rand_Params as RP

Ntasks = int(sys.argv[1]) 
Nconds = int(sys.argv[2])
inDegree = float(sys.argv[3])
outDegree = float(sys.argv[4])
rGen = int(sys.argv[5])
outname = sys.argv[6]

output = outname + ".dot"
outSAN = outname + ".xml"
outTM = outname + ".tm" + ".xml"

ofile = open(output, 'w')
tfile = open(outTM, 'w')
ofile.write("strict digraph testGraph {\n")

SANet = nx.DiGraph()
tasks = []
conds = []
ttonum = {}
ctonum = {}

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
print "Generate Skeleton"
if rGen == 1:
	SANet = RS.erdosGen(SANet, tasks, conds, inDegree, outDegree)
	
print "Generate Parameters"	
#Choose a method for placing probabilities on the skeleton
ParamInfo = RP.flatGen(SANet, tasks, conds)			

#Print out the graph currently into something resembling an SANet, for GraphViz to render			
for each in tasks:
	ofile.write("\"" + each + "\" [shape = box, style=filled, color = grey];\n"  )

for each in conds:
	ofile.write("\"" + each +"\" [ style=filled, color = grey];\n")
	
for edge in SANet.edges():
	ofile.write("\"" + edge[0] + "\" -> \"" + edge[1] + "\";\n")
ofile.write("}\n")
ofile.close()

#Print out a SAN XML
print "Print SAN XML"
nfile = open(outSAN, 'w')
nfile.write("<?xml version=\"1.0\" encoding=\"utf-8\" ?>\n<SANet:network\n\txmlns:SANet=\"http://www.vanderbilt.edu/SANet\"\n")
nfile.write("\txmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"\n\txsi:schemaLocation=\"http://www.vanderbilt.edu/SANet SANet_Network.xsd\"\n\txmlns=\"http://www.vanderbilt.edu/SANet\">")
nfile.write("\n\n\t")
nfile.write("<defaultAttenFactor />\n\t<defaultTaskCost />\n\t<defaultCondUtil />\n\t<defaultCondProbTrue />\n\t<linkThresh />\n")

tnode = 1001
for each in tasks:
	ttonum[each] = tnode 
	nfile.write("\n\t<taskNode>\n\t\t<nodeID>" + str(tnode) + "</nodeID>\n\t\t<name>" + each + "</name>\n\t\t<priorProb>" + str(ParamInfo[1][each]) + "</priorProb>\n\t\t<attenFactor />\n\t\t<cost />\n\t</taskNode>\n" )
	tnode += 1

cnode = 2001
for each in conds:
	ctonum[each] = cnode
	nfile.write("\n\t<condNode>\n\t\t<nodeID>" + str(cnode) + "</nodeID>\n\t\t<name>" + each + "</name>\n\t\t<probTrue>" + str(0) + "</probTrue>\n\t\t<utility>0</utility>\n\t\t<kind />\n\t\t<attenFactor />\n\t</condNode>\n" )
	cnode += 1	

for each in conds:
	if each in ParamInfo[2]:
		plinks = ParamInfo[2][each]
		for link in plinks:
			nfile.write("\n\t<precondLink>\n\t\t<condID>" + str(ctonum[each]) + "</condID>\n\t\t<taskID>" + str(ttonum[link[0]]) + "</taskID>\n\t\t<portID>Port</portID>\n\t\t<trueProb>" + str(link[1]) + "</trueProb>\n\t\t<falseProb>0</falseProb>\n\t\t</precondLink>\n")

for each in tasks:
	if each in ParamInfo[3]:
		elinks = ParamInfo[3][each]
		for link in elinks:
			nfile.write("\n\t<effectLink>\n\t\t<taskID>" + str(ttonum[each]) + "</taskID>\n\t\t<condID>" + str(ctonum[link[0]]) + "</condID>\n\t\t<portID>Port</portID>\n\t\t<weight>" + str(link[1]) + "</weight>\n\t</effectLink>\n")
	
nfile.write("</SANet:network>")	
nfile.close()			

tfile.write("<?xml version=\"1.0\" encoding=\"utf-8\" ?>\n<SA-POP:taskMap\n\txmlns:SA-POP=\"http://www.vanderbilt.edu/SA-POP\"\n")
tfile.write("\txmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"\n\txsi:schemaLocation=\"http://www.vanderbilt.edu/SA-POP Task_Map.xsd\"\n\txmlns=\"http://www.vanderbilt.edu/SA-POP\">")
tfile.write("\n\n\t")	
		
for each in tasks:
	tfile.write("\n\t<taskImpl>\n\t\t<implID>" + each + "_1" + "</implID>\n\t\t<param>\n\t\t\t<paramID></paramID>\n\t\t\t<kind></kind>\n\t\t\t<value></value>\n\t\t</param>\n\t</taskImpl>\n" )
	
tfile.write("\n\t<resource>\n\t\t<resourceID>Dack</resourceID>\n\t\t<kind>DISCRETE</kind>\n\t\t<capacity>1</capacity>\n\t</resource>\n")	

for each in tasks:
	tfile.write("\n\t<taskToImpl>\n\t\t<taskID>" + str(ttonum[each]) + "</taskID>\n\t\t<implID>" + each + "_1" + "</implID>\n\t\t<duration>1</duration>\n\t</taskToImpl>\n")	

for each in tasks:
	tfile.write("\n\t<implToResource>\n\t\t<implID>" + each + "_1</implID>\n\t\t<resourceID>Dack</resourceID>\n\t\t<utilization>1</utilization>\n\t</implToResource>")	

tfile.write("</SA-POP:taskMap>")
tfile.close()	