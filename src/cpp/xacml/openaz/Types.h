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

#ifndef TYPES_H
#define TYPES_H

#define INTERFACE class  // no "interface" in C++

enum PepRequestOperation {
	/** The {@link PepRequest} will return only a single decision 
	 * for a single resource action association 
	 * {@link AzResourceActionAssociation}
	 */
	DECIDE,			// For single decisions
	/** The {@link PepRequest} will return one or more decisions on
	 * a set of resource action associations 
	 */
	BULK_DECIDE,	// For multiple decisions
	/** The {@link PepRequest} will return one of more decisions
	 * for a set of allowed resource action associations or
	 * a set of denied resource action associations depending
	 * on the value of the (@link PepRequestQueryType} submitted
	 * with the {@link PepRequestFactory#newQueryPepRequest}
	 */
	QUERY,
	/** The {@link PepRequest} will return a {@link PepResponse} 
	 * with a full result (including all status and obligations)
	 * for each resource action association 
	 * {@link AzResourceActionAssociation} requested.
	 */
	QUERY_VERBOSE
};

class AzObject
{
    public:
        virtual ~AzObject() {}
};


class Action
{
    public:
        virtual ~Action() {}
};

class Resource
{
    public:
        virtual ~Resource() {}
};

class Subject
{
    public:
        virtual ~Subject() {}
};

class Environment
{
    public:
        virtual ~Environment() {}
};


class ResourceActionAssociation
{
    public:
        virtual ~ResourceActionAssociation() {}
};

#endif // TYPES_H
