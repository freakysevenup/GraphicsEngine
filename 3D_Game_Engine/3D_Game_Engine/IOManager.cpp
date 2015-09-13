
#pragma region INCLUDES
/// External Dependencies
#include <fstream>

/// Internal Dependencies
#include "IOManager.h"

#pragma endregion



bool IOManager::readFileToBuffer(std::string filePath, std::vector<unsigned char>& buffer)
{
	std::ifstream file(filePath, std::ios::binary);
	if(file.fail())
	{
		perror(filePath.c_str());
		return false;
	}

	//To figure out the size of a file we want to use a method to find the 
	//end of the file. this will tell us how many bytes the file is

	//Seek to the end of the file
	file.seekg(0, std::ios::end);

	//Get the file size
	int fileSize = file.tellg();
	file.seekg(0, std::ios::beg);

	//at the top of every file is a file header it is information about a file, like
	//when it was last edited for example. You will not normally see this information
	//so if we don't want to read those bytes we can stop that from happening
	//This is not necessary however it is better safe than sorry.

	fileSize -= file.tellg();

	//resize the buffer to the size of the file we are reading
	buffer.resize(fileSize);

	//&(buffer[0]) is like saying what is the address of the first element of the buffer array, 
	//that is where we want to start reading from.
	file.read((char *)&(buffer[0]),fileSize);
	file.close();

	//The file was successfully opened and stored in the buffer
	return true;

}

bool IOManager::readFileToBuffer(std::string filePath, std::string & buffer)
{
	std::ifstream file(filePath, std::ios::binary);
	if(file.fail())
	{
		perror(filePath.c_str());
		return false;
	}

	//To figure out the size of a file we want to use a method to find the 
	//end of the file. this will tell us how many bytes the file is

	//Seek to the end of the file
	file.seekg(0, std::ios::end);

	//Get the file size
	int fileSize = file.tellg();
	file.seekg(0, std::ios::beg);

	//at the top of every file is a file header it is information about a file, like
	//when it was last edited for example. You will not normally see this information
	//so if we don't want to read those bytes we can stop that from happening
	//This is not necessary however it is better safe than sorry.

	fileSize -= file.tellg();

	//resize the buffer to the size of the file we are reading
	buffer.resize(fileSize);

	//&(buffer[0]) is like saying what is the address of the first element of the buffer array, 
	//that is where we want to start reading from.
	file.read((char *)&(buffer[0]),fileSize);
	file.close();

	//The file was successfully opened and stored in the buffer
	return true;
}
