/
// Copyright 2011 NextLabs, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License"); 
// you may not use this file except in compliance with the License. 
// You may obtain a copy of the License at 
//   http://www.apache.org/licenses/LICENSE-2.0 
// Unless required by applicable law or agreed to in writing, software 
// distributed under the License is distributed on an "AS IS" BASIS, 
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
// See the License for the specific language governing permissions and 
// limitations under the License.
//
// Translation of original OpenAz java source code
// from Josh Bregman, Rich Levinson, Prateek Mishra
// Into a C++ PEP layer interpretation
// By NextLabs Inc. San Mateo CA. 
// Corbin Miller
//
// <-------------------------------------------------------------------->
#include "Pep.h"

PepRequest::PepRequest(AbstractService* pService)
{
	setService(pService);
	m_pAzRequestContext = NULL;
	resourceList.clear();
	bulkActionObjects.clear();
	queryReturnAllowed = true;
}

PepRequest::PepRequest()
{
	setService(NULL);
	m_pAzRequestContext = NULL;
	resourceList.clear();
	bulkActionObjects.clear();
	queryReturnAllowed = true;
}

PepRequest::PepRequest(AzRequestContext* ctx, PepRequestOperation* operation)
{
	setService(NULL);
	m_pAzRequestContext = ctx;
	resourceList.clear();
	bulkActionObjects.clear();
	
    operation = operation;
	queryReturnAllowed = true;
}

PepResponse* PepRequest::decide()  throw(PepException)
{
	PepResponse* pReturnValue = new PepResponse();

	// Get handle to AzService
    SimpleConcreteDummyService* azHandle = ((SimpleConcreteDummyService*)m_pService);
    AzResponseContext* azRspCtx = NULL;
       
	if ((getOperation()==PepRequestOperation::DECIDE) ||
		(getOperation()==PepRequestOperation::BULK_DECIDE)) 
	{
		// Note:  replace with your implementation if a different shim
        azRspCtx = azHandle->decide(getAzRequestContext());
	} 
	else if (getOperation()==PepRequestOperation::QUERY_VERBOSE) 
	{
        azRspCtx = azHandle->queryVerbose(getScope(), getAzRequestContext());
    } 
	else 
	{
        vector<AzResourceActionAssociation>*  actionResourceAssociations =
            azHandle->query(getScope(), getAzRequestContext(), isQueryForAllowedResults());
        
        return pReturnValue->createPepResponse(actionResourceAssociations,
				        					  this,
        									  isQueryForAllowedResults());            
    }
    
	PepResponse* pReturnPepResp = pReturnValue->createPepResponse(azRspCtx, this, getOperation());

	// note: this is a Dummy value for testing
	Obligation* obligation = new Obligation();
	pReturnPepResp->addObligation("Get your Hair Done", *obligation);


    return pReturnPepResp;
}

AzRequestContext* PepRequest::getAzRequestContext()
{
    return m_pAzRequestContext;
}

void PepRequest::setAzRequestContext(AzRequestContext* pAzReqCtx)
{
	m_pAzRequestContext = pAzReqCtx;
}

PepRequestOperation PepRequest::getOperation()
{
	return m_PepRequestOperation;
}

void PepRequest::setOperation(PepRequestOperation operation)
{
	m_PepRequestOperation = operation;
}

string PepRequest::getScope()
{
    return scope;
}

bool PepRequest::isQueryForAllowedResults()
{
	return queryReturnAllowed;
}

AzObject PepRequest::getActionObject(AzResourceActionAssociation azRaa)
{
	/*
    	if ( ! ( actionList == null) ) {
	    	// Get Iterator of the Action object ids
	    	Iterator it = actionList.iterator();
	    	// Get Iterator of original Action objects
	    	ListIterator itBulk = bulkActionObjects.listIterator();
	    	// Iterate thru original objects
	    	while (itBulk.hasNext()){
	    		// Get next resource id of corresponding Resource Object
	    		Object actionId = it.next();
	    		// Get next original action object
	    		Object obj = itBulk.next();
	    		// Does the requested action object match 
	    		//  current original action object?
	    		Object azRaaActionId = azRaa.getAzAction().getId();
	    		if (azRaaActionId == actionId){
	    			return obj;
	    		}
	    	} // end while(itBulk.hasNext())
	    	return null;
    	} else {
    		// must be a query
    		return azRaa.getAzResourceActionAssociationId().getActionId();
    	}
	*/
	return AzObject();
}

string PepRequest::getResourceObject(AzResourceActionAssociation azRaa)
{	
	if (resourceList.size() == 0)
	{
		vector<string>::iterator it = resourceList.begin();
		// Get Iterator of original resource objects
		vector<string>::iterator itBulk = bulkResourceObjects.begin();

		while (itBulk != bulkResourceObjects.end())
		{
			string resourceId = *it;
			string obj = *itBulk;
			string azRaaResourceId = azRaa.getAzResource()->getId();
			if (azRaaResourceId == resourceId)
			{
				return obj;
			}

			itBulk++;
		}

	}

	return azRaa.getAzResourceActionAssociationId()->getResourceId();
}

void PepRequest::setResourceAction(AzObject resource, AzObject action)  throw(PepException)
{
	// ToDo - implement AzObject
	Resource = "";
	Action = "";
}

void PepRequest::setResourceAction(string resource, string action)  throw(PepException)
{
	Resource = resource;
	Action = action;
}

void PepRequest::setBulkResourceActions(vector<string>* resourceObjects, vector<string>* actionObjects)   throw(PepException)
{
	
    bulkActionObjects = *actionObjects; // save the original objects
    bulkResourceObjects = *resourceObjects; // save the original objects
/*    
    if (bulkActionObjects.size()!=bulkResourceObjects.size()) 
	{
        throw new PepException("Actions "+bulkActionObjects+
        		" size("+bulkActionObjects.size()+
        		") does not match Resources "+bulkResourceObjects+
        		" size("+bulkResourceObjects.size()+")");
    }

    actionList = new ArrayList(); // use to correlate to input actions
    resourceList = new ArrayList(); // use to correlate to input resources
    for (int i=0; i<bulkResourceObjects.size(); i++) {
        Resource resource = createResource(bulkResourceObjects.get(i));
        resourceList.add(resource.getAzEntity().getId());
        Action action = createAction(bulkActionObjects.get(i));
        actionList.add(action.getAzEntity().getId());
        Set<Action> actions = new HashSet<Action>();
        actions.add(action);
        addResourceActionAssociation(resource,actions);
    }    	
*/
}

void PepRequest::setAccessSubject(AzObject subjectObject)   throw(PepException)
{
}

void PepRequest::setAccessSubject(hash_map<string, string> subjectObj)   throw(PepException)
{
	typedef pair <string, string> String_Pair;
	subject = subjectObj;
}

void PepRequest::setEnvironment(AzObject envObject)   throw(PepException)
{
	// ToDo - implement AzObject
	typedef pair <string, string> String_Pair;
	environment.insert(String_Pair("urn:oasis:names:tc:xacml:1.0:environment:...", "value"));
}

void PepRequest::setEnvironment(string envTag, string envValue)   throw(PepException)
{
	typedef pair <string, string> String_Pair;
	environment.insert(String_Pair(envTag, envValue));
}

void PepRequest::setEnvironment(hash_map<string, string> environmentObj)   throw(PepException)
{
	environment = environmentObj;
}

void PepRequest::setScope(string inScope)
{
	scope = inScope;
}

void PepRequest::setQueryReturnAllowed(bool inQueryReturnAllowed)
{
	queryReturnAllowed = inQueryReturnAllowed;
}

PepRequest* PepRequest::newBulkPepRequest(hash_map<string, string> subjectObj, 
	    										vector<string>  actionObjects, 
	    										vector<string>  resourceObjects, 
	    										hash_map<string, string> environmentObj) throw(PepException)
{	
	PepRequest* request = newPepRequest( subjectObj, *actionObjects.begin(), *resourceObjects.begin(), environmentObj);
	
	request->setAccessSubject(subjectObj);
    
    // This sets up the correlations
    request->setBulkResourceActions(&resourceObjects, &actionObjects);

    request->setEnvironment(environmentObj);
    
    return request;  
	
}

PepRequest* PepRequest::newBulkPepRequest(hash_map<string, string> subjectObj, 
    											vector<string> actionResourceObjects, 
    											hash_map<string, string> environmentObj) throw(PepException)
{
        
	return newBulkPepRequest(subjectObj,
      						 actionResourceObjects,
      						 actionResourceObjects,
       						 environmentObj);
}

PepRequest* PepRequest::newPepRequest(hash_map<string, string> subjectObj, 
    									   string actionResourceObject, 
										   hash_map<string, string> environmentObj)  throw(PepException)
{
	PepRequest* request = new PepRequest();

	request->setOperation(PepRequestOperation::DECIDE);

	request->setEnvironment(environmentObj);
        
    request->setAccessSubject(subjectObj);
        
    request->setResourceAction("", "");
        
	return request;
}

PepRequest* PepRequest::newPepRequest(
    						hash_map<string, string> subjectObj, 
    						string  actionObj, 
    						string  resourceObj, 
    						hash_map<string, string> environmentObj) throw(PepException)
{
	PepRequest* request = new PepRequest();

	request->setOperation(PepRequestOperation::DECIDE);

	request->setEnvironment(environmentObj);
        
    request->setAccessSubject(subjectObj);
        
    request->setResourceAction(resourceObj, actionObj);
        
	return request;
}

PepRequest* PepRequest::newPepRequest(
    					string subjectName, 
    					string actionId, 
    					string resourceId) throw(PepException)
{
	return NULL;  // temporary
}

PepRequest* PepRequest::newQueryPepRequest(
    						hash_map<string, string> subjectObj, 
    						hash_map<string, string> environmentObj, 
    						string scope, 
    						PepRequestQueryType queryType) throw(PepException)
{
	return NULL;  // temporary
}

      
hash_map<string, string>* PepRequest::getPreDecisionHandlers()
{
	return 0;
}

/* not implemented, call decide() directly
DecisionHandler* PepRequestFactory::getDecisionHandler()
{
	DecisionHandler* pHandler = new DecisionHandler();
	return pHandler;
}
*/

hash_map<string, string>* PepRequest::getPostDecisionHandlers()
{
	return 0;
}
 
AzService* PepRequest::getAzService()
{
	AzService* pService = new AzService();
	return pService;
}
    
string PepRequest::getProviderClassName()
{
	return "";
}

string PepRequest::getContainerName()
{
	return defaultContainerName;
}


/*
    protected void addResourceActionAssociation(Resource azResource, 
											Set<Action> actions) 
		{
        Set<AzEntity<AzCategoryIdAction>> newSet = 
        	new HashSet<AzEntity<AzCategoryIdAction>>();
        Iterator<Action> it = actions.iterator();        
        while (it.hasNext()) {
            Action a = it.next();
            newSet.add(a.getAzEntity());
        }
        Set<AzResourceActionAssociation> setRA = 
        	this.azRequestContext.addResourceActionAssociation(
        			azResource.getAzEntity(), newSet);
        Iterator<AzResourceActionAssociation> itRA = setRA.iterator();
        while (itRA.hasNext()) 
		{
        	AzResourceActionAssociation azRA = itRA.next();
        }
    }
    
    /*
    protected void createAndAddResourceActionAssociation(
    		Resource azResource, Action action) 
	{
        this.azRequestContext.createAndAddResourceActionAssociation(
        			azResource.getAzEntity(), action.getAzEntity());
    }
	   
*/