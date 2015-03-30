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

#include "AzEntity.h"

string getAzCategoryIdString(AzCategoryId catID)
{
	switch (catID)
	{
		case AzCategoryIdAction :
			return AZ_CATEGORY_ID_ACTIONSTR;
		case AzCategoryIdEnvironment :
			return AZ_CATEGORY_ID_ENVIRONMENTSTR;
		case AzCategoryIdResource :
			return AZ_CATEGORY_ID_RESOURCESTR;
		case AzCategoryIdSubjectAccess :
			return AZ_CATEGORY_ID_SUBJECT_ACCESSSTR;
		case AzCategoryIdSubjectCodebase :
			return AZ_CATEGORY_ID_SUBJECT_CODEBASESTR;
		case AzCategoryIdSubjectIntermediary :
			return AZ_CATEGORY_ID_SUBJECT_INTERMEDIARYSTR;
		case AzCategoryIdSubjectRecipient :
			return AZ_CATEGORY_ID_SUBJECT_RECIPIENTSTR;
		case AzCategoryIdSubjectRequestingMachine :
			return AZ_CATEGORY_ID_SUBJECT_REQUESTING_MACHINESTR;
		case AzCategoryIdStatusDetail :
			return AZ_CATEGORY_ID_STATUSDETAILSTR;
		case AzCategoryIdObligation :
			return AZ_CATEGORY_ID_OBLIGATIONSTR;
	}
	return "";
};

AzCategoryId AzEntity::getAzCategoryId() 
{
	return AzCategoryId();
}

string AzEntity::getAzEntityId()
{
	return azEntityId;
}

void AzEntity::setAzEntityId(string azObligationId)
{
	if (((int) getAzCategoryId()) == AzCategoryIdObligation)
	{
		azEntityId = azObligationId;
	}
}

void AzEntity::setId(string strId)
{
	id = strId;
}

string AzEntity::getId()
{
	return id;
}

int AzEntity::getIdEntityCounter()
{
	return idEntityCounter;
}

attrSetType* AzEntity::getAzAttributeSet()
{
	return &attributes;
}

attrSetType* AzEntity::getAzAttributeMixedSet()
{
	return &mixedAttributes;
}
	
AzAttribute* AzEntity::createAzAttribute(string issuer, 
										 string attributeId,  
										 AzAttributeValue attributeValue)
{		
	AzAttribute* testAzAttribute = new AzAttribute();

	testAzAttribute->attributeValue = attributeValue;
	attributes.insert(AzAttribute_Pair(attributeId,*testAzAttribute));

	return testAzAttribute;
}

AzAttributeValue* AzEntity::createAzAttributeValue(string u, string v)
{
	AzAttributeValue* azAttrVal = new AzAttributeValue();
	return azAttrVal;
}

/*	AzAttributeValueBase64Binary* AzEntity::createAzAttributeValue(AzDataTypeIdBase64Binary u, 
																   AzDataByteArray v)
	{
		AzAttributeValueBase64Binary azAttrValBase64Binary = 
			new AzAttributeValueBase64BinaryImpl(v);
		return azAttrValBase64Binary;
	}
	
	public AzAttributeValueBoolean createAzAttributeValue(
		AzDataTypeIdBoolean u, Boolean v){
		AzAttributeValueBoolean azAttrValBoolean = 
			new AzAttributeValueBooleanImpl(v);
		return azAttrValBoolean;
	}
	
	public AzAttributeValueDate createAzAttributeValue(
		AzDataTypeIdDate u, AzDataDateTime v){
		AzAttributeValueDate azAttrValDate = 
			new AzAttributeValueDateImpl(v);
		return azAttrValDate;
	}
	
	public AzAttributeValueDateTime createAzAttributeValue(
		AzDataTypeIdDateTime u, AzDataDateTime v){
		AzAttributeValueDateTime azAttrValDateTime = 
			new AzAttributeValueDateTimeImpl(v);
		return azAttrValDateTime;
	}
	
	public AzAttributeValueDayTimeDuration createAzAttributeValue(
		AzDataTypeIdDayTimeDuration u, AzDataDayTimeDuration v){
		AzAttributeValueDayTimeDuration azAttrValDayTimeDuration = 
			new AzAttributeValueDayTimeDurationImpl(v);
		return azAttrValDayTimeDuration;
	}
	
	public AzAttributeValueDnsName createAzAttributeValue(
		AzDataTypeIdDnsName u, String v){
		AzAttributeValueDnsName azAttrValDnsName = 
			new AzAttributeValueDnsNameImpl(v);
		return azAttrValDnsName;
	}
	
	public AzAttributeValueDouble createAzAttributeValue(
		AzDataTypeIdDouble u, Double v){
		AzAttributeValueDouble azAttrValDouble = 
			new AzAttributeValueDoubleImpl(v);
		return azAttrValDouble;
	}
	
	public AzAttributeValueHexBinary createAzAttributeValue(
		AzDataTypeIdHexBinary u, AzDataByteArray v){
		AzAttributeValueHexBinary azAttrValHexBinary = 
			new AzAttributeValueHexBinaryImpl(v);
		return azAttrValHexBinary;
	}
	
	public AzAttributeValueInteger createAzAttributeValue(
		AzDataTypeIdInteger u, Long v){
		AzAttributeValueInteger azAttrValInteger = 
			new AzAttributeValueIntegerImpl(v);
		return azAttrValInteger;
	}
	
	public AzAttributeValueIpAddress createAzAttributeValue(
		AzDataTypeIdIpAddress u, String v){
		AzAttributeValueIpAddress azAttrValIpAddress = 
			new AzAttributeValueIpAddressImpl(v);
		return azAttrValIpAddress;
	}
	
	public AzAttributeValueRfc822Name createAzAttributeValue(
		AzDataTypeIdRfc822Name u, String v){
		AzAttributeValueRfc822Name azAttrValRfc822Name = 
			new AzAttributeValueRfc822NameImpl(v);
		return azAttrValRfc822Name;
	}
	
	public AzAttributeValueString createAzAttributeValue(
			AzDataTypeIdString u, String v){
		AzAttributeValueString azAttrValString =
			new AzAttributeValueStringImpl(v);
		return azAttrValString;
	}
	
	public AzAttributeValueTime createAzAttributeValue(
		AzDataTypeIdTime u, AzDataDateTime v){
		AzAttributeValueTime azAttrValTime = 
			new AzAttributeValueTimeImpl(v);
		return azAttrValTime;
	}
	
	public AzAttributeValueX500Name createAzAttributeValue(
		AzDataTypeIdX500Name u, X500Principal v){
		AzAttributeValueX500Name azAttrValX500Name = 
			new AzAttributeValueX500NameImpl(v);
		return azAttrValX500Name;
	}
	
	public AzAttributeValueYearMonthDuration createAzAttributeValue(
		AzDataTypeIdYearMonthDuration u, AzDataYearMonthDuration v){
		AzAttributeValueYearMonthDuration azAttrValYearMonthDuration = 
			new AzAttributeValueYearMonthDurationImpl(v);
		return azAttrValYearMonthDuration;
	}

	public <U extends Enum<U> & AzDataTypeId, V>
			AzAttributeValue<U,V> createAzAttributeValue(U u, V v){
		AzAttributeValue<U,V> azAttributeValue = null;
		if (u.equals(
				AzDataTypeIdYearMonthDuration.AZ_DATATYPE_ID_YEARMONTHDURATION) 
			&& (v instanceof AzDataYearMonthDuration) ) {
				AzAttributeValueYearMonthDuration azAttrValYearMonthDuration = 
					new AzAttributeValueYearMonthDurationImpl(
						(AzDataYearMonthDuration) v);
			
		} else if (u.equals(AzDataTypeIdString.AZ_DATATYPE_ID_STRING)
			&& (v instanceof String) ) {
			
		} else if (u.equals(AzDataTypeIdString.AZ_DATATYPE_ID_STRING)
			&& (v instanceof URI) ) {
		
		}
		return azAttributeValue;
	}

	// Helper class create methods
	public AzDataDateTime createAzDataDateTime(
			Date date,
			int actualTimeZone,
			int intendedTimeZone,
			int nanoSeconds){
		AzDataDateTimeImpl testAzDataDateTime = 
			new AzDataDateTimeImpl(
					date, actualTimeZone, intendedTimeZone, nanoSeconds);
		return testAzDataDateTime;
	}
	
	public AzDataByteArray createAzDataByteArray(
			byte[] byteArray){
		AzDataByteArrayImpl azDataByteArray = 
			new AzDataByteArrayImpl(byteArray);
		return azDataByteArray;
	}

*/	

bool AzEntity::addAzAttribute(AzAttribute* azAttribute)
{
	bool result = false;
	
	AzCategoryId categoryId = azAttribute->azCategoryId;
            
    if (categoryId == AzCategoryIdObligation || categoryId == AzCategoryIdStatusDetail)
	{
		AzAttribute* pNewElement = new AzAttribute;
		pNewElement->azCategoryId = categoryId;
		mixedAttributes[getAzCategoryIdString(categoryId)] = *pNewElement;  // assign new hash
		
		result = true;
    }
            
	return result;
}

bool AzEntity::addAzAttribute(AzCategoryId w,
							  string issuer,
							  string attributeId, 
							  AzAttributeValue* attributeValue)
{
	bool result = false;
	if (w == AzCategoryIdObligation || w == AzCategoryIdStatusDetail) 
	{
		createAzAttribute(issuer, attributeId, *attributeValue);
		result = true;
	}
	return result;
}	

AzAttribute* AzEntity::getAttributeByAttribId(string attributeId)
{
	attrSetType* attrSet = getAzAttributeSetByAttribId(attributeId);
	AzAttribute* azAttr = NULL; 
	for(attrSetType::iterator mp_iter = attrSet->begin(); 
		mp_iter != attrSet->end(); 
		mp_iter++)       
	{
		if (mp_iter->first == attributeId)
		{
			azAttr = new AzAttribute(mp_iter->second);
			break;
		}
	}
	return azAttr;
}

attrSetType* AzEntity::getAzAttributeSetByAttribId(string attributeId)
{
	return getAttributeByAttribId(attributeId, &attributes);
}

attrSetType* AzEntity::getAttributeByAttribId(string attributeId, attrSetType* attributeSet)
{
	attrSetType* setMatchedAttrs = new attrSetType();
	AzAttribute* attr = NULL;
	AzAttribute* attrMatch = NULL;

	attrSetType::iterator it = attributeSet->begin(); 
	while (it != attributeSet->end())
	{
		AzAttribute currentAttr = it->second;
		if (currentAttr.getAttributeId() == attributeId) 
		{
			attr = new AzAttribute(currentAttr);
			(*setMatchedAttrs)[attributeId] = *attr;  // insert into hash
		}
		it++;
	}
	return setMatchedAttrs;
}
