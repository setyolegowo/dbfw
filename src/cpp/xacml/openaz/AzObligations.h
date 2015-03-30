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

#ifndef AZOBLIGATIONS_H
#define AZOBLIGATIONS_H

#include <map>

class AzEntity;

typedef std::map<string,AzEntity> ObligationSetType;

INTERFACE AzObligations {

	public:

	std::vector<AzEntity> azObligationList;

	void addAzObligation(AzEntity azObligation);

	ObligationSetType* azObligations;
	
	AzObligations()
	{
		azObligations = new ObligationSetType();
	};

	void addAzObligation(AzEntity* azObligation)
	{
		azObligationList.push_back(*azObligation);
	};

	std::vector<AzEntity,std::allocator<AzEntity>>::iterator azResultIterator()
	{
		return azObligationList.begin();;
	};
	
	int size()
	{
		return azObligationList.size();
	}
	
};

#endif // AZOBLIGATIONS_H
