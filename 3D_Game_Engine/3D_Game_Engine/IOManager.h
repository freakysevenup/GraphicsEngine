#pragma once

#pragma region INCLUDES
/// External Dependencies
#include <vector>
#include <string>

/// Internal Dependencies

#pragma endregion

class IOManager
{
public:

	//static bool readFileToBuffer(std::string filePath, std::vector<char> buffer);
	/*Passing a vector into the above buffer will only create a copy of the vector passed
	what we want to do is use the empty vector that is passed and fill that vector with the
	data that we need.
	*/
	static bool readFileToBuffer(std::string filePath, std::vector<unsigned char> & buffer);
	static bool readFileToBuffer(std::string filePath, std::string & buffer);
};
