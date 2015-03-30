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

#ifndef AZREQUESTCONTEXT_H
#define AZREQUESTCONTEXT_H

#include "AzEntity.h"

//* The AzResourceActionAssociation is a pairing of an AzResource 
//* and an AzAction 
//typedef stdext::hash_map<string,string>  AzResourceActionAssociation;

INTERFACE AzRequestContext 
{
	public:

	template <class T>
	AzEntity* createNewAzEntity (T a) 
	{
	  AzEntity* result = new AzEntity();
	  return result;
	};

/*	
	public <T extends Enum<T> & AzCategoryId> 
			AzEntity<T>  createAzEntity(T t);
		
	public <T extends Enum<T> & AzCategoryId> 
		boolean addAzEntity(AzEntity<T> azEntity);

	public <T extends Enum<T> & AzCategoryId> 
		AzEntity<T> getAzEntity(T t);
		
	public <T extends Enum<T> & AzCategoryId> 
		Set<AzEntity<? extends AzCategoryId>> 
			getAzEntitySet(T t);

	public <T extends Enum<T> & AzCategoryId> 
		boolean removeSubject(T azSubjectCategoryId);
	
*/
	vector<AzResourceActionAssociation>* getAssociations()
	{
		return &resourceActionAssociations;
	};

	AzResourceActionAssociation createAndAddResourceActionAssociation(
			AzEntity azResource, 
			AzEntity azAction);

	vector<AzResourceActionAssociation> addResourceActionAssociation(
			vector<AzEntity> azResourceSet, 
			vector<AzEntity> azActionSet);

	vector<AzResourceActionAssociation> addResourceActionAssociation(
			AzEntity resource, 
			vector<AzEntity> actions);
	
	vector<AzResourceActionAssociation> addResourceActionAssociation(
			vector<AzEntity> resources, 
			AzEntity action);
	
	void removeResourceActionAssociation(AzEntity resource, AzEntity action);
	
	void removeResourceActionAssociations( vector<AzEntity> resources, vector<AzEntity> actions);
	
	void removeAllResourceActionAssociations();

	string getId();

	vector<AzResourceActionAssociation> resourceActionAssociations;

};

#endif // AZREQUESTCONTEXT_H
