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

#ifndef PEPRESPONSE_H
#define PEPRESPONSE_H

#include <vector>
#include <map>

#include "Obligation.h"
#include "AzResponseContext.h"
#include "PepResponseBehavior.h"

const static PepResponseBehavior notApplicableBehavior = RETURN_NO;
const static PepResponseBehavior missingAttributeBehavior = RETURN_NO;
const static PepResponseBehavior syntaxErrorBehavior = THROW_EXCEPTION;
const static PepResponseBehavior processingErrorBehavior = THROW_EXCEPTION;

class PepRequest;

class PepResponse
{
    public:
		PepResponse();
        virtual ~PepResponse();
   
		bool allowed();
		bool next(); 
		AzObject getAction();
		AzObject getResource();
		std::map<std::string, Obligation>* getObligations();
		AzResponseContext getAzResponseContext();
		PepRequest* getPepRequest();
		PepResponse* createPepResponse(AzResponseContext* inResponseContext,
										PepRequest* inPepRequest,
										PepRequestOperation inOperation) 
		{	
			PepResponse* pResponse = new PepResponse();
			pResponse->responseContext = inResponseContext;
			pResponse->pepRequest = inPepRequest;
			std::vector<AzResult>* results = inResponseContext->getResults(); 
			pResponse->azResultIterator = results->begin();
			pResponse->operation = inOperation;
            pResponse->currentResult = &(*results)[0];

			return pResponse; 
		}

		PepResponse* createPepResponse(std::vector<AzResourceActionAssociation>* actionResourceAssociations,
										PepRequest* inPepRequest,
										bool queryAllowed) 
		{	
			PepResponse* pResponse = new PepResponse();
			pResponse->operation = PepRequestOperation::QUERY;
			pResponse->pepRequest = inPepRequest;
			pResponse->azRAAIterator = actionResourceAssociations->begin();
	        pResponse->queryAllowed = queryAllowed;
			return pResponse; 
		}

		void addObligation(string descr, Obligation obligation); //  throw(PepException)
  
		AzResourceActionAssociation* getAzResourceActionAssociation(); // throw(PepException);

		bool enforceBehavior(PepResponseBehavior behavior); // throw(PepException);
	    
		void assertValidResult();

		PepResponseBehavior getNotApplicableBehavior(); 

		PepResponseBehavior getProcessingErrorBehavior();

		PepResponseBehavior getMissingAttributeBehavior();

		std::vector<AzResult,allocator<AzResult>>::iterator azResultIterator;
		std::vector<AzResourceActionAssociation,allocator<AzResourceActionAssociation>>::iterator azRAAIterator;

	   	AzResult* currentResult;

	private:
		std::map<std::string, Obligation> obligations;
		PepRequest* pepRequest;
		bool queryAllowed;

		AzResponseContext* responseContext;
		PepRequestOperation operation;

		PepResponseBehavior getSyntaxErrorBehavior(); 
};

#endif // PEPRESPONSE_H
