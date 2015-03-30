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

#ifndef PEPREQUESTQUERYTYPE_H
#define PEPREQUESTQUERYTYPE_H

enum PepRequestQueryType 
{
	/** The PepResponse returned for this query type will contain
	 * all the full results for each resource action association
	 * requested
	 */
	VERBOSE, 
	/** The PepResponse returned for this query type will contain
	 * only the list of resource action associations that are
	 * allowed.
	 */
	RETURN_ONLY_ALLOWED_RESULTS, 
	/**
	 * The PepResponse returned for this query type will contain
	 * only the list of resource action associations that are
	 * denied.
	 */
	RETURN_ONLY_DENIED_RESULTS
};

#endif  // PEPREQUESTQUERYTYPE_H
