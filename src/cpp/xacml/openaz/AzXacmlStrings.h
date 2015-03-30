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

#ifndef AZXACMLSTRINGS_H
#define AZXACMLSTRINGS_H

/** XACML Namespace: Policy schema: <b>urn:oasis:names:tc:xacml:2.0:policy:schema:os</b> */
	static const std::string X_SCHEMA_POLICY = 
		"urn:oasis:names:tc:xacml:2.0:policy:schema:os";
	/** XACML Namespace: Request/Response context schema: <b>urn:oasis:names:tc:xacml:2.0:context:schema:os</b> */
	static const std::string X_SCHEMA_CONTEXT = 
		"urn:oasis:names:tc:xacml:2.0:context:schema:os";
	
	//XACML Action Attributes

	/** XACML Action Attribute: <b>urn:oasis:names:tc:xacml:1.0:action:action-id</b>. 
	 * <br>This attribute identifies the action for which access is requested. */
	static const std::string X_ATTR_ACTION_ID = 
		"urn:oasis:names:tc:xacml:1.0:action:action-id";

	/** XACML action-id Attribute <b>Value</b>: Where the action is implicit, 
	 * the <b>value</b> of the <b>action-id attribute</b> SHALL be: 
	 * <b>urn:oasis:names:tc:xacml:1.0:action:implied-action</b> */
	static const std::string X_ATTR_ACTION_IMPLIED_ACTION = 
		"urn:oasis:names:tc:xacml:1.0:action:implied-action";
	
	/** XACML Action Attribute: <b>urn:oasis:names:tc:xacml:1.0:action:action-namespace</b>.
	 * <br>This attribute identifies the namespace in which the action-id attribute is defined. */
	static const std::string X_ATTR_ACTION_NAMESPACE = 
		"urn:oasis:names:tc:xacml:1.0:action:action-namespace";
	
	//XACML Resource Attributes
	/** XACML Resource Attribute: <b>urn:oasis:names:tc:xacml:1.0:resource:resource-id</b> */
	static const std::string X_ATTR_RESOURCE_ID = 
		"urn:oasis:names:tc:xacml:1.0:resource:resource-id";
	/** XACML Resource Attribute: <b>urn:oasis:names:tc:xacml:2.0:resource:target-namespace</b> */
	static const std::string X_ATTR_RESOURCE_TARGET_NAMESPACE = 
		"urn:oasis:names:tc:xacml:2.0:resource:target-namespace";
	/** XACML Resource Attribute: <b>urn:oasis:names:tc:xacml:1.0:resource:xpath</b> */
	static const std::string X_ATTR_RESOURCE_XPATH = 
		"urn:oasis:names:tc:xacml:1.0:resource:xpath";
	
	//XACML Environment Attributes:
	/** XACML Environment Attribute: <b>urn:oasis:names:tc:xacml:2.0:context:schema:os</b> */
	static const std::string X_ATTR_ENV_CURRENT_TIME = 
		"urn:oasis:names:tc:xacml:1.0:environment:current-time";
	/** XACML Environment Attribute: <b>urn:oasis:names:tc:xacml:2.0:context:schema:os</b> */
	static const std::string X_ATTR_ENV_CURRENT_DATE = 
		"urn:oasis:names:tc:xacml:1.0:environment:current-date";
	/** XACML Environment Attribute: <b>urn:oasis:names:tc:xacml:2.0:context:schema:os</b> */
	static const std::string X_ATTR_ENV_CURRENT_DATE_TIME = 
		"urn:oasis:names:tc:xacml:1.0:environment:current-dateTime";
	
	//XACML Subject Attributes:
	/** XACML Subject Attribute: <b>urn:oasis:names:tc:xacml:1.0:subject:subject-id</b> */
	static const std::string X_ATTR_SUBJECT_ID = 
		"urn:oasis:names:tc:xacml:1.0:subject:subject-id";
	/** XACML Subject Attribute: <b>urn:oasis:names:tc:xacml:1.0:subject-category</b> */
	static const std::string X_ATTR_SUBJECT_CATEGORY = 
		"urn:oasis:names:tc:xacml:1.0:subject-category";
	/** XACML Subject Attribute: <b>urn:oasis:names:tc:xacml:1.0:subject:subject-id-qualifier</b> */
	static const std::string X_ATTR_SUBJECT_ID_QUALIFIER = 
		"urn:oasis:names:tc:xacml:1.0:subject:subject-id-qualifier";
	/** XACML Subject Attribute: <b>urn:oasis:names:tc:xacml:1.0:subject:key-info</b> */
	static const std::string X_ATTR_SUBJECT_KEY_INFO = 
		"urn:oasis:names:tc:xacml:1.0:subject:key-info";
	/** XACML Subject Attribute: <b>urn:oasis:names:tc:xacml:1.0:subject:authentication-time</b> */
	static const std::string X_ATTR_SUBJECT_AUTHENTICATION_TIME = 
		"urn:oasis:names:tc:xacml:1.0:subject:authentication-time";
	/** XACML Subject Attribute: <b>urn:oasis:names:tc:xacml:1.0:subject:authn-locality:authentication-method</b> */
	static const std::string X_ATTR_SUBJECT_AUTHN_LOC_AUTHENTICATION_METHOD = 
	   "urn:oasis:names:tc:xacml:1.0:subject:authn-locality:authentication-method";
	/** XACML Subject Attribute: <b>urn:oasis:names:tc:xacml:1.0:subject:request-time</b> */
	static const std::string X_ATTR_SUBJECT_REQUEST_TIME = 
		"urn:oasis:names:tc:xacml:1.0:subject:request-time";
	/** XACML Subject Attribute: <b>urn:oasis:names:tc:xacml:1.0:subject:session-start-time</b> */
	static const std::string X_ATTR_SUBJECT_SESSION_START_TIME = 
		"urn:oasis:names:tc:xacml:1.0:subject:session-start-time";
	/** XACML Subject Attribute: <b>urn:oasis:names:tc:xacml:1.0:subject:authn-locality:ip-address</b> */
	static const std::string X_ATTR_SUBJECT_AUTHN_LOC_IP_ADDRESS = 
		"urn:oasis:names:tc:xacml:1.0:subject:authn-locality:ip-address";
	/** XACML Subject Attribute: <b>urn:oasis:names:tc:xacml:1.0:subject:authn-locality:dns-name</b> */
	static const std::string X_ATTR_SUBJECT_AUTHN_LOC_DNS_NAME = 
		"urn:oasis:names:tc:xacml:1.0:subject:authn-locality:dns-name";
	/** XACML Subject Attribute: <b>http://www.ietf.org/rfc/rfc2256.txt#userPassword</b> */
	static const std::string X_ATTR_SUBJECT_USER_PASSWORD = 
		"http://www.ietf.org/rfc/rfc2256.txt#userPassword";
	
#endif // AZXACMLSTRINGS_H
