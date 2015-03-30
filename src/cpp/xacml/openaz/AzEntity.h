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

#ifndef AZENTITY_H
#define AZENTITY_H

#include "AzCategoryId.h"
#include "AzAttribute.h"

typedef stdext::hash_map<string,AzAttribute>  attrSetType;

INTERFACE AzEntity  
{
	public:

		AzCategoryId azCategoryId;

		AzCategoryId getAzCategoryId();

		string azEntityId;
		
		void setAzEntityId(string azObligationId);

		AzAttribute* AzEntity::createAzAttribute(string issuer, 
												string attributeId,  
												AzAttributeValue attributeValue);

		AzAttributeValue* createAzAttributeValue(string u, string v);

		string getAzEntityId();

		string getId();

		AzAttribute* getAttributeByAttribId(string attributeId);
	
		attrSetType* getAzAttributeSetByAttribId(string attributeId);

		attrSetType* getAttributeByAttribId(string attributeId, attrSetType* attributeSet);

		attrSetType* getAzAttributeSet();

		attrSetType* getAzAttributeMixedSet();

		bool addAzAttribute(AzAttribute* azAttribute);

		bool addAzAttribute(AzCategoryId w,
							string issuer,
							string attributeId, 
							AzAttributeValue* attributeValue);

		// TODO : implement more data types, as needed:
/*
		AzAttributeValueAnyURI createAzAttributeValue(AzDataTypeIdAnyURI u, URI v);
		
		AzAttributeValueBase64Binary createAzAttributeValue(AzDataTypeIdBase64Binary u, AzDataByteArray v);

		AzAttributeValueBoolean createAzAttributeValue(AzDataTypeIdBoolean u, bool v);

		AzAttributeValueDate createAzAttributeValue(AzDataTypeIdDate u, AzDataDateTime v);
		
		AzAttributeValueDateTime createAzAttributeValue(AzDataTypeIdDateTime u, AzDataDateTime v);
		
		AzAttributeValueDayTimeDuration createAzAttributeValue(AzDataTypeIdDayTimeDuration u, AzDataDayTimeDuration v);
		
		AzAttributeValueDnsName createAzAttributeValue(AzDataTypeIdDnsName u, string v);
		
		AzAttributeValueDouble createAzAttributeValue(AzDataTypeIdDouble u, double v);

		AzAttributeValueHexBinary createAzAttributeValue(AzDataTypeIdHexBinary u, AzDataByteArray v);

		AzAttributeValueInteger createAzAttributeValue(AzDataTypeIdInteger u, long v);

		AzAttributeValueIpAddress createAzAttributeValue(AzDataTypeIdIpAddress u, string v);

		AzAttributeValueRfc822Name createAzAttributeValue(AzDataTypeIdRfc822Name u, string v);

		AzAttributeValueString createAzAttributeValue(AzDataTypeIdString u, string v);

		AzAttributeValueTime createAzAttributeValue(AzDataTypeIdTime u, AzDataDateTime v);

		AzAttributeValueX500Name createAzAttributeValue(AzDataTypeIdX500Name u, X500Principal v);

		AzAttributeValueYearMonthDuration createAzAttributeValue(AzDataTypeIdYearMonthDuration u, 
																 AzDataYearMonthDuration v);

		AzDataDateTime createAzDataDateTime(Date date, 
											int actualTimeZone, 
											int intendedTimeZone, 
											int nanoSeconds);
		
		AzDataDayTimeDuration createAzDataDayTimeDuration(bool isNegativeDuration,
														  long days,
														  long hours,
														  long minutes,
														  long seconds,
														  int nanoSeconds);

		AzDataYearMonthDuration createAzDataYearMonthDuration(bool isNegativeDuration,
															  long years,
															  long months);

		AzDataByteArray createAzDataByteArray(byte[] byteArray);

*/	

		int idEntityCounter;	// caller-specified identifier:
		string id;				// string value of idEntityCounter
		
		attrSetType attributes;	// use attributes for action, resource, subject, env
		
		attrSetType mixedAttributes; // use mixedAttributes for obligations and status detail

/*
	AzEntityImpl(T t, int idEntityCounter) 
	{
		this.t = t;
		this.idEntityCounter = idEntityCounter;
		// create a String identifier that may be used to reference
		// this instance of the object specifying that this is an
		// AzEntity, of a specific AzCategoryId, with a counter to
		// diffentiate the identifier
		this.id = "AzEntity(" + 
				   t.getClass().getSimpleName() + 
				  ")-" + 
				  new Integer(idEntityCounter).toString();
		if (t.equals(AzCategoryIdObligation.AZ_CATEGORY_ID_OBLIGATION) || 
			t.equals(AzCategoryIdStatusDetail.AZ_CATEGORY_ID_STATUSDETAIL) ) {
			mixedAttributes = new HashSet<AzAttribute<?>>();
		}
		else {
			attributes = new HashSet<AzAttribute<T>>();
		}
	}
	
*/	
	void setId(string StrId);

	int getIdEntityCounter();

};

#endif  // AZENTITY_H