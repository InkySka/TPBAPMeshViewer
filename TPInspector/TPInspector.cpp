#include "stdafx.h"
#include <fstream>
#include <string>
#include <iostream>
#include "utilities.h"
#include <sstream>

#include <vector>

#include "windows.h"

#include <iterator>

using namespace std;

bool isLittleEndian = true;

int main(int argc, char** argv)
{
	ifstream file;
	istringstream processing;

	for (int i = 1; i < argc; ++i)
	{
		file.open(argv[1], ios::binary);
		
		char* temp = new char[4]; //used for 32bit readings 
		unsigned int vertNum(0);

		//get number of vertices

		file.seekg(24); //at the number of vertices

		file.read(temp, 4);
		vertNum = getFromCharArray(temp);

		//cout << vertNum;

		
		/*---------------------------
		create an array of vertices
		---------------------------*/
		vertex* vertices = new vertex[vertNum];
		ostringstream buffer;

		for (int j = 0; j < vertNum; ++j)
		{
			file.read(temp, 4); //number of bytes for the vertex data
			unsigned int numBytesVertex(getFromCharArray(temp));
			//cout << numBytesVertex << endl;
			//char* vertexDataExtr = new char[numBytesVertex];

			streampos currPos = file.tellg(); //saving curent position in the stream

			/*
				FAILSAFE STUFF
			*/
			//cout << endl << (numBytesVertex - 4);

			file.seekg((numBytesVertex - 4), ios::cur);
			file.read(temp, 4);
			cout << (short)temp[0] << endl << (short)temp[1] << endl << (short)temp[2] << endl << (short)temp[3] << endl;
			cout << getFromCharArray(temp);
			//cout << endl << std::hex<<(int)ciao <<endl <<getFromCharArray(temp)<<endl;

			/*for (int k = 0; k < numBytesVertex; ++k)
			{
				file.read(vertexDataExtr, numBytesVertex);
			}*/

			/*char forFailsafe[4]{
				vertexDataExtr[numBytesVertex - 4],
				vertexDataExtr[numBytesVertex - 3],
				vertexDataExtr[numBytesVertex - 2],
				vertexDataExtr[numBytesVertex - 1],
			}; // failsafe. last four bytes must be 0xFF*/

			if (getFromCharArray(temp) != (unsigned int)0xFFFFFFFF) //failsafe
			{
				cout << endl << "ERROR - expected end of vertex (0xFFFFFFFF)" << endl;
				return 1;
			}
			/*
				END OF FAILSAFE
				assuming we have a valid sequence. going back to the start of the vertex's data
			*/
			file.seekg(currPos);
			cout << "AAAAAAAAAAAAAAAAAAAAAA";
		}

		//at this point the char sequence must be right



		/*char temp[1];
		fileParser.read(temp, 1);
		extracted.push_back(*temp);
		fileParser.read(temp, 1);
		extracted.push_back(*temp);*/
		/*bool foundEnd = false;

		for (int j = 0; !foundEnd && j!=63; ++j)
		{
			char tempf[1];
			fileParser.read(tempf, 1);
			unsigned char temp = *tempf;
			extracted.push_back(temp);
			cout << std::hex <<(unsigned int)temp << " " <<j<<endl;
			if (j > 4)
			{
				if ((extracted[j - 3] == 0xFF) &&
					(extracted[j - 2] == 0xFF) &&
					(extracted[j - 1] == 0xFF) &&
					(extracted[j] == 0xFF))
				{
					foundEnd = true;
				}
			}
		}
		*/
	}
    return 0;
}

