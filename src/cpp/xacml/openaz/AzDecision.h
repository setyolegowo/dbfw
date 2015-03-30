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

#ifndef AZDECISION_H
#define AZDECISION_H

const static string AZ_DEC_EMPTYSTR		 = "";
const static string AZ_PERMITSTR		 = "Permit";
const static string AZ_DENYSTR			 = "Deny";
const static string AZ_INDETERMINATESTR  = "Indeterminate";
const static string AZ_NOTAPPLICABLESTR  = "NotApplicable";

enum AzDecision {

	AZ_DEC_EMPTY = 0,
	
	/** "Permit": the requested access is permitted. */
	AZ_PERMIT, //("Permit")
	
	/** "Deny": the requested access is denied. */
	AZ_DENY, //("Deny")
	
	/** "Indeterminate": the PDP is unable to evaluate the requested access. Reasons for such inability include: missing attributes, network errors while retrieving policies, division by zero during policy evaluation, syntax errors in the decision request or in the policy, etc. The 
	 * most "interesting" of these is MissingAttributes, which returns information that the caller can take action on to submit another request if that is appropriate or desired. */
	AZ_INDETERMINATE, //("Indeterminate")
	
	/** "NotApplicable": the PDP does not have any policy that applies to this decision request. In general, Xacml Policies and Rules have a Target
	 * element that is used to determine if the Policy or Rule should be applied to the Request. If not, then the Target result causes the Policy
	 * or Rule to evaluate to NotApplicable. After all Policy evaluation is complete, if result is NotApplicable, then that is Decision value that is returned. */
	AZ_NOTAPPLICABLE //("NotApplicable")
};

#endif // AZDECISION_H
