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

#ifndef AZRESOURCEACTIONASSOCIATION_H
#define AZRESOURCEACTIONASSOCIATION_H

#include "AzResourceActionAssociationId.h"

#define INTERFACE class  // no "interface" in C++

class AzEntity;

class AzResourceActionAssociation 
{
	public:
	
		AzEntity* getAzAction()      { return NULL; };
		AzEntity* getAzResource()    { return NULL; };
		string getAzActionString()   { return action;   };
		string getAzResourceString() { return resource; };
	
		AzResourceActionAssociationId* getAzResourceActionAssociationId() { return NULL; };
	
		bool equals(AzResourceActionAssociation azResourceActionAssociation);
	
		int getCorrelationId();

		string action;
		string resource;
};

#endif   // AZRESOURCEACTIONASSOCIATION_H
