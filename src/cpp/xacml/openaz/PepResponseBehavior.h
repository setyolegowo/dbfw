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

#ifndef PEPRESPONSEBEHAVIOR_H
#define PEPRESPONSEBEHAVIOR_H

enum PepResponseBehavior 
{	
	/** The behavior is to allow (Permit) access by returning true when the condition for which this behavior is assigned occurs  */
    RETURN_YES,
    
	/** The behavior is to disallow (Deny) access by returning false when the condition for which this behavior is assigned occurs  */
    RETURN_NO,
    
	/** The behavior is to disallow (Deny) access by throwing a PepException when the condition for which this behavior is assigned occurs  */
    THROW_EXCEPTION
};

#endif // PEPRESPONSEBEHAVIOR_H
