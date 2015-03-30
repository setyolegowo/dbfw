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

#ifndef OPENAZXACMLOBJECT_H
#define OPENAZXACMLOBJECT_H
 
class OpenAzXacmlObject {

	public:
		OpenAzXacmlObject(string inLineType, string inObject)
		{
			this.object = inObject;
			this.lineType = inLineType;
			readAhead = false;
		};

		OpenAzXacmlObject(string inLineType, 
								 string inObject, 
								 bool inReadAhead,
								 string inReadAheadObject)
		{
			object = inObject;
			lineType = inLineType;
			readAhead = inReadAhead;
			readAheadObject = inReadAheadObject;
		};

		string getLineType() 
		{
			return lineType;
		};

		string getObject() 
		{
			return object;
		};

		bool getReadAhead() 
		{
			return readAhead;
		};

		string getReadAheadObject() 
		{
			return readAheadObject;
		};

		void setReadAhead(bool inReadAhead)
		{
			readAhead = inReadAhead;
		};

		void setReadAheadObject(string inReadAheadObject) 
		{
			readAheadObject = inReadAheadObject;
		};

	private:
		string object;
		string lineType;
		bool   readAhead; 
		string readAheadObject;
};

#endif  // OPENAZXACMLOBJECT_H
