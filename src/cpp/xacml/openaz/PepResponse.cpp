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

#include "Pep.h"

enum AzDecision;

PepResponse::PepResponse() 
{
	currentResult = NULL;
}

PepResponse::~PepResponse() {}


bool PepResponse::allowed() throw(PepException)
{     
	if (currentResult == NULL)
		throw new PepException("current Result is empty");

	AzDecision dec = currentResult->getAzDecision();
   
	switch (dec)
	{
	   case AZ_PERMIT: 
		   return true;
	   case AZ_DENY:
		   return false;
	   case AZ_NOTAPPLICABLE:
		   return enforceBehavior(getNotApplicableBehavior()); 
	   case AZ_INDETERMINATE:
		   switch (currentResult->getAzStatusCode())
		   {
			   case AZ_SYNTAX_ERROR:
    			   return enforceBehavior(getSyntaxErrorBehavior());
			   case AZ_PROCESSING_ERROR:
    			   return enforceBehavior(getProcessingErrorBehavior());
			   case AZ_MISSING_ATTRIBUTE:
    			   return enforceBehavior(getMissingAttributeBehavior());
		   };
	}; 

	throw new PepException("AzResult.getAzDecision did not match any of the known values");

	return true;
}

bool PepResponse::next()  throw(PepException)
{
	return true;
}

AzObject PepResponse::getAction()  throw(PepException)
{
   return getPepRequest()->getActionObject(*getAzResourceActionAssociation());
}

AzObject PepResponse::getResource() throw(PepException)
{
	return AzObject();
}

hash_map<string,Obligation>* PepResponse::getObligations()  throw(PepException)
{
	return &obligations;
}

void PepResponse::addObligation(string descr, Obligation obligation)  throw(PepException)
{
	typedef std::pair <std::string, Obligation> Obligations_Pair;
	obligations.insert(Obligations_Pair(descr, obligation));
}

AzResponseContext PepResponse::getAzResponseContext()  throw(PepException)
{
	return AzResponseContext();
}


PepResponseBehavior PepResponse::getSyntaxErrorBehavior() 
{
	return syntaxErrorBehavior;
}

PepResponseBehavior PepResponse::getProcessingErrorBehavior() 
{
    return processingErrorBehavior;
}

PepResponseBehavior PepResponse::getMissingAttributeBehavior()
{
	return missingAttributeBehavior;
}

AzResourceActionAssociation* PepResponse::getAzResourceActionAssociation() throw(PepException)
{    
	assertValidResult();
	return currentResult->getAzResourceActionAssociation();
}

void PepResponse::assertValidResult()  throw(PepException)
{
	if (currentResult == NULL) 
	{
		throw new PepException("The current AzResult in this PepResponse is null.");
    }
}

bool PepResponse::enforceBehavior(PepResponseBehavior behavior)   throw(PepException)
{
	if (behavior == RETURN_YES) {
        return true;    
	} else if (behavior == RETURN_NO) {
        return false;
    } else {
        throw new PepException(
    		"This Excecption is being thrown based on configured \
    		PepResponseBehavior for AzResult containing AzDecision \
    		with value of NotApplicable or Indeterminate");
    }
}

PepRequest* PepResponse::getPepRequest()
{
   	return pepRequest;
}
    
PepResponseBehavior PepResponse::getNotApplicableBehavior() 
{
   return notApplicableBehavior;
}

