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

#ifndef AZSERVICE_H
#define AZSERVICE_H

#include "Types.h"
#include "AzResponseContext.h"
#include "AzRequestContext.h"
#include "AzResourceActionAssociation.h"
#include "AzAttributeFinder.h"

static string DEFAULT_PROVIDER_NAME = "AZ_SERVICE";

class AzRequestContext;

class AzService  
{
	public :
		AzResponseContext* decide(AzRequestContext* azRequestContext);

		vector<AzResourceActionAssociation> query(string scope, 
												  AzRequestContext* azRequestContext,
												  bool allowedNotAllowed);
	
		AzResponseContext queryVerbose(string scope, AzRequestContext* azRequestContext);
	
		void registerAzAttributeFinder(AzAttributeFinder<class T>* azAttributeFinder);

		void registerProvider(string name, AzService* azService)
		{
			typedef pair <string,AzService> AzService_Pair;
			providers.insert(AzService_Pair(name,*azService));
		};

	private:
		static stdext::hash_map<string, AzService> providers;

};
 

#endif // AZSERVICE_H
