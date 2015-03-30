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

#ifndef DECISIONHANDLER_H
#define DECISIONHANDLER_H

#include "PepResponse.h"

//class PepRequest;
class PepRequest;
INTERFACE DecisionHandler
{   // a virtual base class ("interface") which needs to be defined by child class implemention
public:
	DecisionHandler(){}; 
	virtual ~DecisionHandler(){}; 
	virtual PepResponse* decide(PepRequest* request) = 0;
};

#endif  // DECISIONHANDLER_H