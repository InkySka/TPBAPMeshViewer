#include "stdafx.h"
#include <fstream>
#include <string>
#include <iostream>
#include "utilities.h"
#include <sstream>

#include <vector>

#include "windows.h"

#include <iterator>
#include <cstdlib> 

using namespace std;

bool isLittleEndian = true;

int main(int argc, char** argv)
{
	ifstream file;
	istringstream processing;

	for (int i = 1; i < argc; ++i)
	{
		file.open(argv[1], ios::binary);

		data_4bytes temp;
		wchar_t temp4(0);
		unsigned int vertNum(0);

		//get number of vertices

		file.seekg(24); //at the number of vertices
		file.read(temp.c, 4);
		vertNum = temp.i;

		//cout<<vertNum<<endl<<endl;
		
		/*---------------------------
		create an array of vertices
		---------------------------*/
		vertex* vertices = new vertex[vertNum];
		
		for (int j = 0; j < vertNum; ++j)
		{
			file.read(temp.c, 4); //number of bytes for the vertex data
			unsigned int numBytesVertex(temp.ui);

			cout << std::hex << numBytesVertex << endl << endl;

			/*
				FAILSAFE STUFF
			*/
			streampos currPos = file.tellg(); //saving curent position in the stream

			file.seekg((numBytesVertex - 4), ios::cur);
			file.read(temp.c, 4);

			if (temp.ui != (unsigned int)0xFFFFFFFF) //failsafe
			{
				cout << endl << "ERROR - expected end of vertex (0xFFFFFFFF)" << endl;
				return 1;
			}

			/*
				END OF FAILSAFE
				assuming we have a valid sequence. going back to the start of the vertex's data
			*/
			cout <<std::hex<< currPos << endl;
			file.seekg(currPos);
			//at this point the char sequence must be right
			file.read(temp.c, 4);
			
			vertices[j].x = temp.f;
			file.read(temp.c, 4);
			vertices[j].y = temp.f;
			file.read(temp.c, 4);
			vertices[j].z = temp.f;
		}


		
		
	}
    return 0;
}

