#include "stdafx.h"
#include "utilities.h"
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>

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

	cout << "Exported 3D data folder: " << exported_meshes_folder_absolute << endl;
	cout << "Exported texture names folder: " << exported_texture_file_names_folder_absolute << endl << endl;
	
	if (argc > 1)
	{
		if (strcmp(argv[1], "-all") == 0)
		{
			filenames_to_convert = fileFinderByExtension(meshes_folder_absolute.string(), mesh_data_extension);
			
			cout << "Found " << filenames_to_convert.size() << " files" << endl << endl;
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
	for (unsigned int i = 0; i < filenames_to_convert.size(); ++i)
	{
		mesh temp(filenames_to_convert[i]);
		meshes.push_back(temp);

		cout << "MESH " << i + 1 << " - " << meshes[i].getFilePath() << endl;
		cout << "Ingesting and tidying... (can take a while)" << endl;
		meshes[i].findOffsets();
		meshes[i].ingest();
	
		cout << "Saving..." << endl << endl;
		meshes[i].saveToFile();
	}
	cout << "Done!";
	return 0;
}

