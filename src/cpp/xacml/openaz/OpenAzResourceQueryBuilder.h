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

#ifndef OPENAZRESOURCEQUERYBUILDER_H
#define OPENAZRESOURCEQUERYBUILDER_H

#include "OpenAzResourceDirectory.h"
#include "AzService.h"

class OpenAzResourceQueryBuilder 
{
	public:

	static AzRequestContext* getScopedAzRequestContext(string scope, AzRequestContext* azRequestContext)
	{
		
    	// TODO: this is placeholder for now; when multi component
		// query string support is added in a more substantive
		// manner, then it will need to be configurable,
		// probably by resource type.
		string scopeSeparatorChar = ",";

		// Get the configured AzResourceDirectory
//ToDo 	OpenAzResourceDirectory azResourceDirectory = AzService.azResourceDirectory;

    	// Create a list to collect the resource-action pairs
    	// to be obtained from the AzResourceDirectory
    	vector<vector<string>> azResourceActionList;
/*
    	// Parse the scope into parts based on separator char.
    	// for now this should only be one part
    	string[] scopeArray = scope.split(scopeSeparatorChar);
    	List<String> scopeParts = new ArrayList<String>();
    	
    	// It is assumed that the first "part" of the scope
    	// will be the resource type, and if there is only
    	// one part, then that is the resource type and is
    	// equivalent to a query for all resources of that type.
    	String scopeResourceType = null;
    	if ( !(scopeArray == null) ) {
    		// get rid of whitespace at front and end of the "part"
    		// but a "part" may have enclosed white space
        	for (String scopePart : scopeArray){
        		scopeParts.add(scopePart.trim());
        	}
        	// if any parts were found
        	if ( (scopeArray.length > 0 ) ) {
        		scopeResourceType = scopeParts.get(0);
        		// if scope is only the resource type
        		if ( (scopeArray.length==1) ) {
        			// get list of all resources of that type from
        			// the AzResourceDirectory
        			azResourceActionList = 
        				azResourceDirectory.getListByType(
        					scopeResourceType);
    			/*
    			// TODO: decide how to handle more detailed scope
    			// than just get all resources of specific resource=type
        		} else if (scopeArray.length==2) {
        			azResourceActionList =
        				azResourceDirectory.getListByInstance(
        					scopeParts.get(0),
        					new Integer(
        						scopeParts.get(1)).intValue());
				/
        		} else {
        			return null;
        		}
        		
        	}
    		
    	} else {
    		return null;
    	}
  	
    	// Create variables for Resource and Action AzEntity's
    	AzEntity<AzCategoryIdResource> azResource = null;
    	AzEntity<AzCategoryIdAction> azAction = null;
    	for ( List<String> azResourceActionPair : azResourceActionList ) {
    		// create an AzEntity<AzCategoryIdResource> with a
    		// resource-id AzAttribute and a
    		// resource-type AzAttribute
    		azResource = azRequestContext.createAzEntity(
    				AzCategoryIdResource.AZ_CATEGORY_ID_RESOURCE);
    		azResource.createAzAttribute(
				"org.openliberty.openaz.pdp.resources.TestResourceCollection",
				AzXacmlStrings.X_ATTR_RESOURCE_ID,
				azResource.createAzAttributeValue(
    				AzDataTypeIdString.AZ_DATATYPE_ID_STRING, 
    				azResourceActionPair.get(0)));
    		azResource.createAzAttribute(
				"org.openliberty.openaz.pdp.resources.TestResourceCollection",
				OPENAZ_ATTR_RESOURCE_TYPE, 
				azResource.createAzAttributeValue(
    				AzDataTypeIdString.AZ_DATATYPE_ID_STRING, 
    				scopeResourceType));
    		// create an AzEntity<AzCategoryIdAction> with an
    		// action-id AzAttribute
    		azAction = azRequestContext.createAzEntity(
    				AzCategoryIdAction.AZ_CATEGORY_ID_ACTION);
    		azAction.createAzAttribute(
				"org.openliberty.openaz.pdp.resources.TestResourceCollection",
				AzXacmlStrings.X_ATTR_ACTION_ID,
				azAction.createAzAttributeValue(
					AzDataTypeIdString.AZ_DATATYPE_ID_STRING, 
					azResourceActionPair.get(1)));
    		// add this resource action association to context
    		azRequestContext.createAndAddResourceActionAssociation(
    				azResource, azAction);
    	}
*/
		return azRequestContext;
	}

};

#endif OPENAZRESOURCEQUERYBUILDER_H
