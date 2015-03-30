//
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

#include "OpenAzService.hpp"
#include "openaz/OpenAzResourceQueryBuilder.h"
#include "openaz/AzStatusMessages.h"
#include "openaz/AzDataTypeId.h"

OpenAzService::OpenAzService() : AbstractService()
{

}

AzResponseContext* OpenAzService::decide(AzRequestContext* azReqCtx)
{
	if (azReqCtx == NULL)
		return NULL;  

    // Create a Response Context to store the results
    AzResponseContext* rspCtx = new AzResponseContext();
        
    // Create the first AzResult which must be prepared for
    // return in case there are no AzResourceActionAssociations
    // to process.
    AzResult* azResult = new AzResult();  
    
    AzResourceActionAssociation* azAssoc = NULL;
    AzResourceActionAssociationId* azAssocId = NULL;
	std::vector<AzResourceActionAssociation>* pAzRAAvector = azReqCtx->getAssociations();
	std::vector<AzResourceActionAssociation,std::allocator<AzResourceActionAssociation>> \
		::iterator it = pAzRAAvector->begin();

    if (it != pAzRAAvector->end())
	{
        // Set the null AssociationId case to be processing error
        azResult->setAzDecision(AZ_INDETERMINATE);
        azResult->setAzStatusCode(AZ_SYNTAX_ERROR);
		azResult->setStatusMessage(NO_RESOURCE_ACTION_ASSOCIATION);
        //rspCtx->addResult((AzResult*) azResult);

		while (it != pAzRAAvector->end())
		{
			azAssoc = &*it++;
			azAssocId = azAssoc->getAzResourceActionAssociationId();
			azResult = new AzResult();
			azResult->setAzResourceActionAssociation(azAssoc);
			azResult->setAzDecision(AZ_PERMIT);  // TODO:  these are dummy vals
			azResult->setAzStatusCode(AZ_OK);
			azResult->setStatusMessage(AZMSG_PERMIT);
			switch (azResult->getAzDecision()) {
				case AZ_PERMIT:
					{
					AzObligations* azObligations = new AzObligations();
					AzEntity* azObligation = azReqCtx->createNewAzEntity(AzCategoryIdObligation);

					azObligation->setAzEntityId("This.is.an.ObligationId");
					azObligation->createAzAttribute(
									"PDP-Issuer",            // issuer
									"Obl-attr-id-test-obl",  // attr-id
									*((AzAttributeValue*) azObligation->createAzAttributeValue(
									AZ_DATATYPE_ID_STRING,
									"PEP is obligated to roll out the red carpet")));

					azObligation->createAzAttribute(
									"PDP-Issuer",              // issuer
									"Obl-attr-id-test-obl-2",  // attr-id
									*((AzAttributeValue*) azObligation->createAzAttributeValue(
									AZ_DATATYPE_ID_STRING,
									"PEP is obligated to roll out the green carpet, too")));

					azObligations->addAzObligation(azObligation);
					azResult->setAzObligations((AzObligations *)azObligations);

					std::vector<AzEntity,std::allocator<AzEntity>>::iterator it = \
					  (azResult->getAzObligations())->azObligationList.begin();
					}
					break;
				case AZ_DENY:
					//TBD: put a test case for Deny here.
					break;
				case AZ_NOTAPPLICABLE:
					//TBD: put a test case for NotApplicable here.
					break;
				case AZ_INDETERMINATE:
					switch (azResult->getAzStatusCode())
					{
						case AZ_SYNTAX_ERROR:
							// TODO: put a test case for 
                    		// Indeterminate.SyntaxError here.
							break;
						case AZ_PROCESSING_ERROR:
							//TODO: put a test case for 
                    		// Indeterminate.ProcessingError here.
							break;
						case AZ_MISSING_ATTRIBUTE:
							AzEntity* azStatusDetail = (AzEntity*) azReqCtx->createNewAzEntity(
								AzCategoryIdStatusDetail);
							azStatusDetail->createAzAttribute(
							  "PDP-issuer",
							  "MissingAttribute-01-AttributeId",
							  *((AzAttributeValue*) azStatusDetail->createAzAttributeValue(
							  AZ_DATATYPE_ID_STRING,
								"This is the value needed for this missing attribute")));
							azResult->setAzStatusDetail((AzCategoryId*) azStatusDetail);
							break;
					} // end inner switch                                       

			} // end outer switch                       
		   rspCtx->addResult((AzResult*) azResult);
		}
	}

	delete azResult;

	return (AzResponseContext*) rspCtx;
}


vector<AzResourceActionAssociation>* OpenAzService::query(string scope, 
											AzRequestContext* azRequestContext,
											bool allowedNotAllowed)
{
	OpenAzResourceQueryBuilder* qb = new OpenAzResourceQueryBuilder();
  	azRequestContext = qb->getScopedAzRequestContext(scope, azRequestContext);
	
	// Submit the request and process the results 
	AzResponseContext* azResponseContext = decide(azRequestContext);

	// Pull the ResourceAction associations out of the response
	vector<AzResourceActionAssociation>* azResActAssoc = new vector<AzResourceActionAssociation>;
	vector<AzResult>* azResults;
	if (azResponseContext != NULL)
	{
		azResults = azResponseContext->getResults();
		if ((*azResults).size() > 0)
		{
			azResActAssoc = new vector<AzResourceActionAssociation>();
			for (unsigned int x = 0; x < (*azResults).size(); x++)
 			{
				// if want only permits only add permits to set
				if (allowedNotAllowed) 
				{
					if ((*azResults)[x].getAzDecision() == AZ_PERMIT) 
					{
						azResActAssoc->push_back(*((*azResults)[x].getAzResourceActionAssociation()));	    					
    				}
    			// otherwise only add denies to the set
				}
				else 
				{
					if ((*azResults)[x].getAzDecision() == AZ_DENY) 
					{
						azResActAssoc->push_back(*((*azResults)[x].getAzResourceActionAssociation()));
					}
				}
			}

		}
	}

	delete qb;
    return azResActAssoc; 

}

AzResponseContext* OpenAzService::queryVerbose(string scope, AzRequestContext* azRequestContext)
{
	OpenAzResourceQueryBuilder* qb = new OpenAzResourceQueryBuilder();

	azRequestContext = qb->getScopedAzRequestContext(scope, azRequestContext);
	
	// Submit the request and process the results
	AzResponseContext* azResponseContext = decide(azRequestContext);

	delete qb;
    return azResponseContext;
}   

AzRequestContext* OpenAzService::createAzRequestContext()
{
	AzRequestContext* pReqCtx = new AzRequestContext();
/*
		subjects = new HashSet<AzEntity<? extends AzCategoryId>>();
		resources = new HashSet<AzEntity<? extends AzCategoryId>>();
		actions = new HashSet<AzEntity<? extends AzCategoryId>>();
		resourceActionAssociations = 
			new HashSet<AzResourceActionAssociation>();
		azRequestContextId = "AzRequestContext-" +
			new Integer(idAzRequestContextCounter++).toString();
*/
	AzResourceActionAssociation* aRAA = new AzResourceActionAssociation();
	aRAA->action = "read";
	aRAA->resource = "file\\\\toplevel";

	pReqCtx->resourceActionAssociations.push_back(*aRAA);

	return pReqCtx;

}