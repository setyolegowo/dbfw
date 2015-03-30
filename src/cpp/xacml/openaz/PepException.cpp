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

#include "PepException.h"

PepException::PepException() : exception("PEP Exception") 
{
	printf("PEP Exception\r\n");
	throw std::exception("PEP Exception");
};

PepException::PepException(std::string msg) : exception(msg.c_str())
{	
	printf("PEP Exception cause : %s\r\n", msg.c_str());
	throw std::exception(msg.c_str());
};

PepException::PepException(PepException& exc) : exception(exc.what())
{
	printf("PEP Exception cause : %s\r\n", exc.what());
	throw std::exception(exc.what());
};

PepException::PepException(PepException& exc, std::string msg) : exception(exc.what())
{
	printf("PEP Exception message : %s\r\n", msg);
	throw PepException(exc.what());
};
