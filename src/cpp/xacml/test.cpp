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

#include <stdio.h>
#include <string.h>
#include "OpenAzService.hpp"
#include "openaz/AzXacmlStrings.h"

#define MSGSIZE 5000

int main(int argc, char* argv[])
{
	////Debugging/Test messages:
	char strMessage[MSGSIZE];

	memset(strMessage, 0, MSGSIZE);
	strcat(strMessage, "Results from OpenAzTester:\r\n");
	////
	OpenAzService* pNewService = new OpenAzService();

	////Debugging/Test messages:
	if (pNewService == NULL) {
		strcat(strMessage, "-> Service could not be created.\r\n");
		exit(0);
	} else
		strcat(strMessage, "-> Service created.\r\n");
	////
	
	PepRequest* pNewReq = new PepRequest((AbstractService*) pNewService);
	pNewReq->setAzRequestContext(pNewService->createAzRequestContext());

	////Debugging/Test messages:
	if (pNewReq == NULL) {
		strcat(strMessage, "-> Error Creating PEP Request.\r\n");
		exit(0);
	} else
		strcat(strMessage, "-> PEP Request and Context created.\r\n");
	////

	string resourceName = "file\\\\toplevel";
	string actions[] = {"read","write","delete","read","write","delete"};
	
	for (unsigned int i = 0; i < actions->length(); i++) {
		//tm date = ;
		hash_map<string, string> subject;

		typedef pair <string, string> String_Pair;
		subject.insert(String_Pair(X_ATTR_SUBJECT_ID, "sydney"));
		subject.insert(String_Pair(X_ATTR_SUBJECT_AUTHN_LOC_AUTHENTICATION_METHOD, "sydney"));

		hash_map<string, string> environment;
		environment.insert(String_Pair(X_ATTR_ENV_CURRENT_DATE, "12.12.2012"));
		PepRequest* req = pNewReq->newPepRequest(subject,actions[i],resourceName, environment);

		////Debugging/Test messages:
		if (req == NULL)
			strcat(strMessage, "-> Error Creating Qualified PEP Request.\r\n");
		else {
			strcat(strMessage, "-> Qualified PEP Request created :\r\n");
			if (i == 0) {
				strcat(strMessage, " * Subject = ");
				strcat(strMessage, subject.begin()->first.c_str());
				strcat(strMessage, " = ");
				strcat(strMessage, subject.begin()->second.c_str());
				strcat(strMessage, "\r\n");
				strcat(strMessage, " * Subject = ");
				strcat(strMessage, (++subject.begin())->first.c_str());
				strcat(strMessage, " = ");
				strcat(strMessage, (++subject.begin())->second.c_str());
				strcat(strMessage, "\r\n");
			}
			strcat(strMessage, " * Action = ");
			strcat(strMessage, actions[i].c_str());
			strcat(strMessage, "\r\n");
			if (i == 0) {
				strcat(strMessage, " * Resource = ");
				strcat(strMessage, resourceName.c_str());
				strcat(strMessage, "\r\n");
				strcat(strMessage, " * Environment = ");
				strcat(strMessage, environment.begin()->first.c_str());
				strcat(strMessage, " = ");
				strcat(strMessage, environment.begin()->second.c_str());
				strcat(strMessage, "\r\n");
			} else {
				strcat(strMessage, "* ...\r\n");
			}
		}
		////
		// CM - redundant?
		req->setAzRequestContext(pNewReq->getAzRequestContext());
		req->setService((AbstractService*) pNewService);

		PepResponse* resp = req->decide();
		bool allowed = resp->allowed();

		hash_map<string,Obligation>* Obligations = resp->getObligations();

		////Debugging/Test messages:
		if (resp == NULL)
			strcat(strMessage, "-> Error Calling Decide.\r\n");
		else {
			strcat(strMessage, "-> Decide() method called : ");
			strcat(strMessage, " RESULT = ");
			if(allowed)
				strcat(strMessage, "Allowed\r\n");
			else
				strcat(strMessage, "Denied\r\n");
			strcat(strMessage, "-> Obligations : \r\n");
			hash_map<string,Obligation>::iterator it = Obligations->begin();
			do {
				strcat(strMessage, "* ");
				strcat(strMessage, it->first.c_str());  // print out Obligation descr
				strcat(strMessage, "\r\n");
			} while (++it != Obligations->end());

		}
		////

		// clean up
		delete resp;
		delete req;
	}

	//Debugging/Test messages:
	// MessageBoxA(NULL,strMessage,"Test Results",NULL);
	sprintf(stderr, strMessage);

	// clean up
	delete pNewReq;
	delete pNewService;

	return 0;
}
