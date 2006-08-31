<?xml version="1.0" encoding="UTF-8"?>

<!-- XSL Translator for converting UDM generated CAD file to confirm the
     CCM CAD DTD. This transformation is made to help to solve two problems
     brought in by using UDM.
     1. The fixed alphabical order among elements contained in any element.
     2. No PCDATA is allowed in UDM, they are all replaced by attribute with
        name "content"
  -->

<!-- Version 1 (tao.lu@vanderbilt.edu) only problem 1 listed above is fixed -->

<xsl:stylesheet
     version="1.0"
     xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

  <xsl:output
       method="xml"
       encoding="UTF-8"
       doctype-system="CCM_CAD_TEMP.dtd"
       indent="yes"/>
  <xsl:strip-space elements="*"/>


  <!--Here copy all the attributs of the elements in which order of
      the children elements needs to be changed-->
  <xsl:template name="copier" match="@* | text()">
    <xsl:copy-of select="@*"/>
  </xsl:template>


  <!-- Here we specify the sorting rules to change the order of elements -->

  <xsl:template match="hostcollocation">
     <xsl:element name="hostcollocation">
       <xsl:call-template name="copier"/>
       <xsl:apply-templates select="homeplacement"/>
       <xsl:apply-templates select="processcollocation"/>
       <xsl:apply-templates select="destination"/>
     </xsl:element>
  </xsl:template>


  <xsl:template match="processcollocation">
     <xsl:element name="processcollocation">
       <xsl:call-template name="copier"/>
       <xsl:apply-templates select="homeplacement"/>
       <xsl:apply-templates select="destination"/>
     </xsl:element>
  </xsl:template>


  <xsl:template match="homeplacement">
     <xsl:element name="homeplacement">
       <xsl:call-template name="copier"/>
       <xsl:apply-templates select="componentfileref"/>
       <xsl:apply-templates select="componentinstantiation"/>
       <xsl:apply-templates select="destination"/>
     </xsl:element>
  </xsl:template>

  <xsl:template match="providesport">
     <xsl:element name="providesport">
       <xsl:call-template name="copier"/>
       <xsl:apply-templates select="providesidentifier"/>
       <xsl:apply-templates select="componentinstantiationref"/>
     </xsl:element>
  </xsl:template>


  <xsl:template match="connections">
     <xsl:element name="connections">
        <xsl:call-template name="copier"/>
     	<xsl:apply-templates select="connectevent"/>
     	<xsl:apply-templates select="connectinterface"/>
     </xsl:element>
  </xsl:template>


  <xsl:template match="connectevent">
     <xsl:element name="connectevent">
       <xsl:call-template name="copier"/>
       <xsl:apply-templates select="consumesport"/>
       <xsl:apply-templates select="emitsport"/>
       <xsl:apply-templates select="publishesport"/>
     </xsl:element>
  </xsl:template>


  <xsl:template match="componentassembly">
     <xsl:element name="componentassembly">
       <xsl:call-template name="copier"/>
       <xsl:apply-templates select="description"/>
       <xsl:apply-templates select="componentfiles"/>
       <xsl:apply-templates select="partitioning"/>
       <xsl:apply-templates select="connections"/>
     </xsl:element>
  </xsl:template>


  <xsl:template match="consumesport">
     <xsl:element name="consumesport">
       <xsl:call-template name="copier"/>
       <xsl:apply-templates select="consumesidentifier"/>
       <xsl:apply-templates select="componentinstantiationref"/>
     </xsl:element>
  </xsl:template>

  <xsl:template match="partitioning">
     <xsl:element name="partitioning">
        <xsl:call-template name="copier"/>
     	<xsl:apply-templates select="homeplacement"/>
     	<xsl:apply-templates select="hostcollocation"/>
     	<xsl:apply-templates select="processcollocation"/>
     </xsl:element>
  </xsl:template>


  <xsl:template match="connectinterface">
     <xsl:element name="connectinterface">
       <xsl:call-template name="copier"/>
       <xsl:apply-templates select="usesport"/>
       <xsl:apply-templates select="providesport"/>
     </xsl:element>
  </xsl:template>


  <xsl:template match="emitsport">
     <xsl:element name="emitsport">
       <xsl:call-template name="copier"/>
       <xsl:apply-templates select="emitsidentifier"/>
       <xsl:apply-templates select="componentinstantiationref"/>
     </xsl:element>
  </xsl:template>


  <xsl:template match="publishesport">
     <xsl:element name="publishesport">
       <xsl:call-template name="copier"/>
       <xsl:apply-templates select="publishesidentifier"/>
       <xsl:apply-templates select="componentinstantiationref"/>
     </xsl:element>
  </xsl:template>


  <xsl:template match="usesport">
     <xsl:element name="usesport">
       <xsl:call-template name="copier"/>
       <xsl:apply-templates select="usesidentifier"/>
       <xsl:apply-templates select="componentinstantiationref"/>
     </xsl:element>
  </xsl:template>



  <!--Here copy all the attributs back-->
  <xsl:template match="*|@* | text()">
    <xsl:copy>
      <xsl:apply-templates select="*|@*"/>
    </xsl:copy>
  </xsl:template>

<!--Here we need to change all content attribut back to #PCDATA-->
  <xsl:template match="*[@content]">
    <xsl:copy>
      <xsl:value-of select="@content"/>
      <xsl:apply-templates/>
    </xsl:copy>
  </xsl:template>






</xsl:stylesheet>
