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

vector<mesh> meshes;



int main(int argc, char** argv)
{
	meshes.resize(argc-1);
	int temp_result = findVertices(argc, argv);
	switch (temp_result)
	{
	case 0: break;
	default: cout << "Error in mesh " << temp_result - 1 << endl << endl;
	}

	cout << "Done. " << argc-1 << " meshes processed.\n";

	for (int i = 0; i < argc - 1; ++i)
	{
		cout << "--- MESH " << i + 1 << " DATA ---" << endl
			<< "Found " << meshes[i].vertices.size() << " vertices" << endl << endl;

		/*for (int j = 0; j < meshes[i].vertices.size(); ++j)
		{

		}*/
	}
	
    return 0;
}

