#include "stdafx.h"
#include "utilities.h"
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <ctime>

#include <vector>

#include "windows.h"

#include <iterator>
#include <cstdlib> 

#include <experimental\filesystem>

#include "MeshClass.h"
#include "CMDDEFS.h"
#include "FILENAMEDEFS.h"

namespace fs = std::experimental::filesystem;

using namespace std;

float pivot = 0.5;
fs::path exported_texture_file_names_folder_relative((std::string)"Extracted Texture Names Data");
fs::path exported_meshes_folder_relative("Extracted Data");
//fs::path meshes_folder_relative("BD_Meshes");
fs::path exported_texture_file_names_folder_absolute(fs::current_path().string() + (string)"/" + exported_texture_file_names_folder_relative.string());
fs::path exported_meshes_folder_absolute(fs::current_path().string() + (string)"/" + exported_meshes_folder_relative.string());
fs::path meshes_folder_absolute(fs::current_path().string()/* + (string)"/" + meshes_folder_relative.string()*/);

int main(int argc, char** argv)
{
	fs::create_directory(exported_texture_file_names_folder_absolute);
	fs::create_directory(exported_meshes_folder_absolute);

	vector<string> filenames_to_convert;
	vector<mesh> meshes;
	
	clock_t start = 0, end = 0;
	double duration = 0;

	/*meshes.push_back((mesh)"E:/Programmi/C++/TPInspector/Debug/NavySloopShip_Layer2.mdb");
	meshes[0].findOffsets();
	meshes[0].ingest();
	meshes[0].DEV_saveOtherData();*/
	cout << "Treasure Planet: Battle at Procyon mesh opener" <<endl << endl << "by InkySka" << endl << endl << endl;

	if (argc > 1)
	{
		if (strcmp(argv[1], "/all") == 0)
		{
			filenames_to_convert = fileFinderByExtension(meshes_folder_absolute.string(), mesh_data_extension);
		}
		else
		{
			/*for (int i = 1; i < argc; ++i)
			{
				fs::path tmp((string)argv[i]);
				if(fs::is_regular_file(tmp))filenames_to_convert.push_back(tmp.string());
				
			}*/
			//cout << "Found " << filenames_to_convert.size() << " files" << endl;
			cout << "NOT IMPLEMENTED YET" << endl;
			return 0;
		}
	}
	else
	{
		string temp;
		cout << "STANDALONE MODE " << endl;
		cout << "Input command: ";
		while (true)
		{
			if (cin >> temp)
			{
				if (temp == "/all")
				{
					filenames_to_convert = fileFinderByExtension(meshes_folder_absolute.string(), mesh_data_extension);
					break;
				}
				else cout << "Wrong command..." << endl << endl;
			}
			else
			{
				cout << "UNHANDLED ERROR!" << endl;
				return 1;
			}
		}
		
	}
	cout << "Found " << filenames_to_convert.size() << " files" << endl << endl;
	for (unsigned int i = 0; i < filenames_to_convert.size(); ++i)
	{
		mesh temp(filenames_to_convert[i]);
		meshes.push_back(temp);

		cout << "MESH " << i + 1 << " - " << meshes[i].getFilePath() << endl;
		cout << "Ingesting and tidying... (can take a while)" << endl;
		start = clock();
		meshes[i].findOffsets();
		meshes[i].ingest();
		end = clock();
		duration = (end-start) / (double)CLOCKS_PER_SEC;
		cout << "Done in " << to_string(duration) << " seconds" << endl << endl;

		cout << "Saving..." << endl;
		start = clock();
		meshes[i].saveToFile();
		end = clock();
		duration = (end - start) / (double)CLOCKS_PER_SEC;
		cout << "Done in " << to_string(duration) << " seconds" << endl << endl;
		cout << endl;
	}
	cout << "-----------------------" << endl;
	cout << "Done!" << endl;
	cout << "Exported 3D data folder: " << exported_meshes_folder_absolute << endl;
	cout << "Exported texture names folder: " << exported_texture_file_names_folder_absolute << endl << endl;

	system("PAUSE");
	return 0;
}

