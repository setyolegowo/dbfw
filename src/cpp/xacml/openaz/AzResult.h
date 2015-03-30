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

#ifndef AZRESULT_H
#define AZRESULT_H

#include "Pep.h"
#include "AzObligations.h"
#include "AzStatusCode.h"
#include "AzDecision.h"

enum AzCategoryId;
class AzEntity;

INTERFACE AzResult 
{
	public:

		AzResult();

		AzDecision getAzDecision();
	
		string getResourceId();

		AzResourceActionAssociation* getAzResourceActionAssociation();

		AzCategoryId azStatusDetail;
		
		void setAzDecision(AzDecision newAzDecision);
	
		AzObligations* getAzObligations();

		void setAzObligations(AzObligations* ptrAzObligations);

		AzCategoryId getAzStatusDetail();

		void setAzStatusDetail(AzCategoryId* AzStatusDetail);
	
		string getStatusMessage();

		void setStatusMessage(string statusMessage);
		
		AzStatusCode getAzStatusCode();

		void setAzStatusCode(AzStatusCode azStatusCode);

		void setAzResourceActionAssociation(AzResourceActionAssociation* azResourceActionAssociation);

		AzResourceActionAssociationId* getAzResourceActionAssociationId();

		AzEntity* getAzAction();

		AzEntity* getAzResource();

		AzDecision azDecision;
		AzStatusCode azStatusCode;
		string statusMessage;
		AzResourceActionAssociation* azResourceActionAssociation;
		AzObligations* azObligations;
};

#endif //  AZRESULT_H
