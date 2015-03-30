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

#ifndef PEPREQUEST_H
#define PEPREQUEST_H

#include <vector>
#include <map>

#include "DecisionHandler.h"
#include "AzService.h"
#include "PepRequestQueryType.h"
#include "../OpenAzService.hpp"  // note:  use appropriate shim

using namespace stdext;

INTERFACE AbstractService;

class PepRequest  : DecisionHandler 
{
	public:
		PepRequest(AbstractService* pService);
		PepRequest(AzRequestContext* ctx, PepRequestOperation* operation);
		PepRequest();

		virtual ~PepRequest() {};

		virtual PepRequestOperation getOperation();
		virtual void setOperation(PepRequestOperation operation);

		virtual string getScope();

		virtual bool isQueryForAllowedResults();

		virtual AzObject getActionObject(AzResourceActionAssociation azRaa);

		virtual string getResourceObject(AzResourceActionAssociation azRaa);

		virtual void setResourceAction(AzObject resource, AzObject action);    // throw(PepException)
		virtual void setResourceAction(string resource, string action);        //  throw(PepException)

		virtual void setBulkResourceActions(std::vector<std::string>* resourceObjects, std::vector<std::string>* actionObjects) /* throw(PepException) */ ;

		virtual void setAccessSubject(AzObject subject);                       //  throw(PepException)
		virtual void setAccessSubject(std::map<std::string, std::string> subjectObj);    //   throw(PepException)

		virtual void setEnvironment(AzObject envObject);					   //  throw(PepException)
		virtual void setEnvironment(string envTag, string envValue);		   //   throw(PepException)
		virtual void setEnvironment(std::map<std::string, std::string> environmentObj);  //   throw(PepException)
		
		virtual void setScope(std::string inScope);

		virtual void setQueryReturnAllowed(bool inQueryReturnAllowed);

		AzRequestContext* getAzRequestContext();
		void setAzRequestContext(AzRequestContext* pAzReqCtx);


		// PepRequestFactory functions:

		PepRequest* newBulkPepRequest(
			std::map<std::string, std::string> subjectObj, 
			std::vector<std::string> actionObjects, 
			std::vector<std::string> resourceObjects, 
			std::map<std::string, std::string> environmentObj);
		PepRequest* newBulkPepRequest(
			std::map<std::string, std::string> subjectObj, 
			std::vector<std::string> actionResourceObjects, 
			std::map<std::string, std::string> environmentObj);
		PepRequest* newPepRequest(
			std::map<std::string, std::string>  subjectObj, 
			std::string actionResourceObject, 
			std::map<std::string, std::string> environmentObj);
		PepRequest* newPepRequest(
			std::map<std::string, std::string> subjectObj, 
			std::string actionObj, 
			std::string resourceObj, 
			std::map<std::string, std::string> environmentObj);
		PepRequest* newPepRequest(
			std::string subjectName, 
			std::string actionId, 
			std::string resourceId);
		PepRequest* newQueryPepRequest(
			std::map<std::string, std::string> subjectObj, 
			std::map<std::string, std::string> environmentObj, 
			std::string scope, 
			PepRequestQueryType queryType);
		
		std::map<std::string, std::string>*  getPreDecisionHandlers();
		// not implemented:  DecisionHandler* getDecisionHandler();
		std::map<std::string, std::string>*  getPostDecisionHandlers();
		AzService* getAzService();
		std::string getProviderClassName();
		std::string getContainerName();

		// implementation of abstract class:
		PepResponse* decide(PepRequest* request) {return NULL;}; // throw(PepException)	
		PepResponse* decide();  //throw(PepException)

		void setService(AbstractService* pService) 
		{
			m_pService = pService;
		};

	private:
		std::map<std::string, std::string> environment;
		std::map<std::string, std::string> subject;
		std::vector<std::string> actionList;
		std::vector<std::string> resourceList;
		std::vector<std::string> bulkResourceObjects;
		std::vector<std::string> bulkActionObjects;   
		std::string Resource;
		std::string Action;
		std::string defaultContainerName;
	    PepRequestOperation operation;
		std::string scope;
		bool queryReturnAllowed;

		AbstractService* m_pService;
		PepRequestOperation m_PepRequestOperation;
		AzRequestContext* m_pAzRequestContext;

};

#endif // PEPREQUEST_H
