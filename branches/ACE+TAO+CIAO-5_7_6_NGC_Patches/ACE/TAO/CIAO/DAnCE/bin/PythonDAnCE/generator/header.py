from templet import stringfunction

@stringfunction
def template (uuid):
    """
<Deployment:DeploymentPlan
  xmlns:Deployment="http://www.omg.org/Deployment"
  xmlns:xmi="http://www.omg.org/XMI"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://www.omg.org/Deployment Deployment.xsd">
  <label>${uuid}</label>
  <UUID>${uuid}</UUID>

"""
