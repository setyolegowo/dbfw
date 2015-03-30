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

#include "AzResult.h"

AzResult::AzResult() 
{
	azDecision = AZ_DEC_EMPTY; 
	azStatusCode = AZ_STAT_EMPTY;
	statusMessage = "";
};

AzDecision AzResult::getAzDecision()
{ 
	azDecision = AZ_PERMIT; 
	return azDecision; 
};

string AzResult::getResourceId()
{
	return "";
}

AzResourceActionAssociation*  AzResult::getAzResourceActionAssociation()
{
	return NULL;
};

void AzResult::setAzDecision(AzDecision newAzDecision)
{
	azDecision = newAzDecision; 
};

AzObligations* AzResult::getAzObligations()
{		
	return azObligations;
};

void AzResult::setAzObligations(AzObligations* ptrAzObligations)
{
	azObligations = ptrAzObligations;
};

AzCategoryId AzResult::getAzStatusDetail()
{
	return azStatusDetail;
};

void AzResult::setAzStatusDetail(AzCategoryId* AzStatusDetail)
{
	azStatusDetail = *AzStatusDetail;
};

string AzResult::getStatusMessage() 
{
	return statusMessage;
};

AzResourceActionAssociationId* AzResult::getAzResourceActionAssociationId()
{
	if (azResourceActionAssociation == NULL) 
		return NULL;
	return azResourceActionAssociation->getAzResourceActionAssociationId();
};

AzEntity* AzResult::getAzAction()
{
	if (azResourceActionAssociation == NULL) 
		return NULL;
	return azResourceActionAssociation->getAzAction();
};

AzEntity* AzResult::getAzResource()
{
	if (azResourceActionAssociation == NULL) 
		return NULL;
	return azResourceActionAssociation->getAzResource();
};

void AzResult::setStatusMessage(string statusMessage)
{
	statusMessage = statusMessage; 
};

AzStatusCode AzResult::getAzStatusCode()
{
	return azStatusCode;
};

void AzResult::setAzStatusCode(AzStatusCode azStatusCode) 
{
	azStatusCode = azStatusCode; 
};

void AzResult::setAzResourceActionAssociation(AzResourceActionAssociation* azResourceActionAssociation) 
{
	azResourceActionAssociation = azResourceActionAssociation;
};
