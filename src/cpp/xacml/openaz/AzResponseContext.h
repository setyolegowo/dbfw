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

#ifndef AZRESPONSERCONTEXT_H
#define AZRESPONSERCONTEXT_H

#include "AzResourceActionAssociation.h"
#include "AzResult.h"

class AzResult;

#pragma warning(disable : 4482)

INTERFACE AzResponseContext 
{
	public:

		//AzResult* getResult(AzResourceActionAssociation* azAssociation);

		vector<AzResult>* getResults() 
		{
			return &azResults;
		};

		vector<AzResult,allocator<AzResult>> azResults;
		vector<AzResult,allocator<AzResult>>::iterator azResultIterator;

		void next() 
		{
			advance(azResultIterator,1);
		};

		void remove() 
		{
			azResults.erase(azResultIterator);
		};

		bool hasNext() 
		{
			return azResultIterator != azResults.end();
		};

/*
		AzResult* getResult(AzResourceActionAssociation azResourceActionAssociation)
		{
			vector<AzResult,allocator<AzResult>>::iterator it = azResults.begin();
			while (it != azResults.end())
			{
				if (azResourceActionAssociation.equals(
						*(*it).getAzResourceActionAssociation()))
				{
					// found match
					return &*it;
				}
				it++;
			}
			
			return NULL;
		};
*/
		bool addResult(AzResult* azResult)
		{
			azResults.push_back(*azResult);
			azResultIterator = azResults.begin();
			if (azResults.empty())
				return false;
			return true;
		}
};

#endif  // AZRESPONSERCONTEXT_H
