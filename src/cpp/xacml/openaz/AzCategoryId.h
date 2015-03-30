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

#ifndef AZCATEGORYID_H
#define AZCATEGORYID_H

#include "Pep.h"

const static string AZ_CATEGORY_ID_ACTIONSTR			= "urn:oasis:names:tc:xacml:3.0:attribute-category:action";
const static string AZ_CATEGORY_ID_ENVIRONMENTSTR		= "urn:oasis:names:tc:xacml:3.0:attribute-category:environment";
const static string AZ_CATEGORY_ID_RESOURCESTR			= "urn:oasis:names:tc:xacml:3.0:attribute-category:resource";
const static string AZ_CATEGORY_ID_SUBJECT_ACCESSSTR	= "urn:oasis:names:tc:xacml:1.0:subject-category:access-subject";
const static string AZ_CATEGORY_ID_SUBJECT_CODEBASESTR	= "urn:oasis:names:tc:xacml:1.0:subject-category:codebase";
const static string AZ_CATEGORY_ID_SUBJECT_INTERMEDIARYSTR			= "urn:oasis:names:tc:xacml:1.0:subject-category:intermediary-subject";
const static string AZ_CATEGORY_ID_SUBJECT_RECIPIENTSTR				= "urn:oasis:names:tc:xacml:1.0:subject-category:recipient-subject";
const static string AZ_CATEGORY_ID_SUBJECT_REQUESTING_MACHINESTR	= "urn:oasis:names:tc:xacml:1.0:subject-category:requesting-machine";
const static string AZ_CATEGORY_ID_STATUSDETAILSTR		= "urn:oasis:names:tc:xacml:3.0:attribute-category:status-detail";
const static string AZ_CATEGORY_ID_OBLIGATIONSTR		= "urn:oasis:names:tc:xacml:3.0:attribute-category:obligation";

enum AzCategoryId 
{
    AzCategoryIdAction,
    AzCategoryIdEnvironment,
	AzCategoryIdResource,
	AzCategoryIdSubjectAccess,
	AzCategoryIdSubjectCodebase,
	AzCategoryIdSubjectIntermediary,
	AzCategoryIdSubjectRecipient,
	AzCategoryIdSubjectRequestingMachine,
	AzCategoryIdStatusDetail,
	AzCategoryIdObligation
};

extern string getAzCategoryIdString(AzCategoryId catID);


#endif // AZCATEGORYID_H