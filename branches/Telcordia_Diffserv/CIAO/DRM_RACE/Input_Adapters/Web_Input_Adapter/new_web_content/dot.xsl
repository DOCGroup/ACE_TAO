<!-- 
     This stylesheet operates on a flat component deployment descriptor (XML file) and 
     transforms it into DOT text for use by graphviz.  The DOT tools, such as neato and 
     fdp, can then be used to create a graphical view of the deployment.  This is intended 
     as a very simple way to visualize a deployment plan.
     
     The intended sequence is 
     
               PICML ==> Deployment Plan (XML) ==> XSLT ==> DOT ==> Image
     
     This sheet is used in conjunction with the xform and xform_html shell scripts.
     xform will pre-process this XSL file and then invoke saxon (XSLT parser) to do the 
     transformation.  The resulting DOT file is then passed to neato to generate an 
     image.  As a convenience, the xform_html shell script (which invokes xform), may be 
     used to generate a simple HTML that contains the image and a link to a detailed image.  
     The detailed image notates all connections (both event and facet) and label the 
     connections as appropriate.  Note that the layout engine of neato does not do a good 
     job with these graphs, so they are difficult to read.
     
     Tom Damiano    2/9/2006
     Lockheed Martin, Advanced Technology Laboratories

     With modifications by Jonathan Walsh
-->
     
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">

<!-- outputing .dot text -->
<xsl:output method="text"/>

<!-- Define a key to match the implementation tag to search for cpu usage-->
<!--<xsl:key name="impKey" match="implementation" use="@id"/>-->

<xsl:template match="/">

  digraph G 
  {
     label = "\n\nRACE Deployment";
     fontsize=20;
     node [fontname = "Arial",shape = "circle",width = "0.400000",height = "0.400000",color = "blue"];
     edge [color = "black",arrowsize=0.5];
     
     subgraph clusterUber
     {
     label="Deployment Domain";
   
     <xsl:variable name="posnodes" select="/deploymentPlan/instance"/>
       
     <xsl:for-each select="$posnodes">
     

        <xsl:if test="generate-id(.)=generate-id($posnodes[node=current()/node])">

          subgraph cluster_<xsl:value-of select="./node" />
          {
          fontsize=12;
          label="<xsl:value-of select="./node" />"; 

          <xsl:variable name="curnode" select="./node"/>
          
          <xsl:for-each select="/deploymentPlan/instance[node=$curnode]">
          
             <xsl:variable name="sname" select="./name" /> 

		<!--The call below trims the package names-->
		<xsl:variable name="sshortname">
		   <xsl:call-template name="substring-after-last">
		       <xsl:with-param name="string" select="$sname" />
    		       <xsl:with-param name="delimiter" select="'.'" />
		   </xsl:call-template>
		</xsl:variable>

             <xsl:variable name="slabel"> 
                <xsl:if test="$sshortname = ''"> <xsl:value-of select="$sname"/> </xsl:if>
                <xsl:if test="$sshortname != ''"> <xsl:value-of select="$sshortname"/> </xsl:if>
             </xsl:variable>

	     <xsl:variable name="impID" select="./implementation"/>

             node [shape = ellipse,fontsize = 8
<!-- Add tags to throw the necissary table tags in so that line breaks will work-->
<!-- historical note: used to always show (cpu%=VALUE) now the name is pulled from the actual xml-->
	label = #BEFORE#<xsl:value-of select='$slabel'/>
	<xsl:for-each select="/deploymentPlan/implementation">
	  <xsl:if test="@id=$impID">
		<xsl:for-each select="./execParameter">#MIDDLE#
		   (<xsl:value-of select="./name"/>=<xsl:value-of select="./value/value/double"/>)
		</xsl:for-each>
	  </xsl:if>
	</xsl:for-each>
		#AFTER#

             ]; <xsl:value-of select="./@id" />;
          </xsl:for-each>       
          }            
        </xsl:if>
     </xsl:for-each>

     <xsl:if test="false()">
        <xsl:for-each select="/deploymentPlan/connection[internalEndpoint/kind='SimplexReceptacle']"> 
           <xsl:variable name="pname" select="./internalEndpoint[kind='Facet']/portName" /> 
           edge [width = "0.500000",color = "brown",fontsize = 8,headlabel = "<xsl:value-of select='$pname'/>"];
           <xsl:value-of select="./internalEndpoint[kind='SimplexReceptacle' or kind='MultipleReceptacle']/instance"/>   #ARC#  <xsl:value-of select="./internalEndpoint[kind='Facet']/instance"/> ;
        </xsl:for-each>
   
        <xsl:for-each select="/deploymentPlan/connection[internalEndpoint/kind='EventPublisher']"> 
         
           <xsl:variable name="pname" select="./internalEndpoint[kind='EventConsumer']/portName" /> 
           edge [width = "0.500000",color = "green",fontsize = 8,headlabel = "<xsl:value-of select='$pname'/>"];
           <xsl:value-of select="./internalEndpoint[kind='EventPublisher']/instance"/>  #ARC#  <xsl:value-of select="./internalEndpoint[kind='EventConsumer']/instance"/> ;
        </xsl:for-each>


        <xsl:for-each select="/deploymentPlan/connection[internalEndpoint/kind='IOGR-Facet']">
           <xsl:variable name="pname" select="./internalEndpoint[kind='IOGR-SimplexReceptacle']/portName" /> 
           edge [width = "0.500000",color = "blue",fontsize = 8,headlabel = "<xsl:value-of select='$pname'/>"];
           <xsl:value-of select="./internalEndpoint[kind='IOGR-Facet']/instance"/>  #ARC#  <xsl:value-of select="./internalEndpoint[kind='IOGR-SimplexReceptacle']/instance"/> ;
        </xsl:for-each>


     </xsl:if>
     }
     <xsl:if test="false()">
        subgraph legend
        {
          ranksep=0;
          rank=samerank;
          node [shape = plaintext, fontsize = 8,fontcolor="green"];
          { "Publisher #TO# Consumer"; }
          node [shape = plaintext, fontsize = 8,fontcolor="brown"];
          { "Receptacle #TO# Facet"; }
          node [shape = plaintext, fontsize = 8,fontcolor="blue"];
          { "IOGRFacet #TO# IOGRReceptacale"; }
        }
     </xsl:if>
  }
</xsl:template>


<!-- should grab the string after a given substring -->
<xsl:template name="substring-after-last">
  <xsl:param name="string" />
  <xsl:param name="delimiter" />
  <xsl:choose>
    <xsl:when test="contains($string, $delimiter)">
      <xsl:call-template name="substring-after-last">
        <xsl:with-param name="string"
          select="substring-after($string, $delimiter)" />
        <xsl:with-param name="delimiter" select="$delimiter" />
      </xsl:call-template>
    </xsl:when>
    <xsl:otherwise><xsl:value-of select="$string" /></xsl:otherwise>
  </xsl:choose>
</xsl:template>



</xsl:stylesheet>

