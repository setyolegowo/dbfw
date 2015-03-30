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

#ifndef OPENAZSERVICE_H
#define OPENAZSERVICE_H

#include "AbstractService.h"
#include "openaz/AzAttributeFinder.h"
#include "openaz/AzResult.h"

const static string OPENAZ_ATTR_RESOURCE_TYPE = "urn:openaz:names:xacml:1.0:resource:resource-type";

extern "C" void __stdcall GetService();

class OpenAzService : AbstractService 
{
	public:
		OpenAzService();
		 
		AzCategoryId registerAzAttributeFinder(AzAttributeFinder<AzCategoryId> azAttributeFinder){}; //TODO: AzCategoryId?

		AzResponseContext* decide(AzRequestContext* azReqCtx);
	        
		vector<AzResourceActionAssociation>* query(string scope, 
												    AzRequestContext* azRequestContext,
												    bool allowedNotAllowed);
		
		AzResponseContext* queryVerbose(string scope, AzRequestContext* azRequestContext);

		AzRequestContext* createAzRequestContext();


};

#endif // OPENAZSERVICE_H
