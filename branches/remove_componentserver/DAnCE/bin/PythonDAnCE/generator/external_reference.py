from templet import stringfunction

@stringfunction
def template (location, provider, name, supportedtype) :
    """
    <externalReference>
      <location>${location}</location>
      <provider>${provider}</provider>
      <portName>${name}</portName>
      <supportedType>${supportedtype}</supportedType>
    <externalReference>
    """

