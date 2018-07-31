/*
	Treasure Planet: Battle at Procyon Inspector - mesh viewer and editor.

	*** by InkySka ***

	e-mail: underscore.marczero@gmail.com
	Twitter: @InkySka

	Official repository: https://github.com/InkySka/TPBAPMeshViewer
	.mdb files info: https://treasure-planet.wikia.com/wiki/.mdb_mesh_files
	Note: reverse-engineering is still undergoing and any help is appreciated.

	Treasure Planet Wikia: https://treasure-planet.wikia.com/

	(C) InkySka 2018 - published under the MIT Licence.

	Main file.
*/

#include "stdafx.h"

#include "MeshClass.h"
#include "CMDDEFS.h"
#include "FILENAMEDEFS.h"
#include "utilities.h"
#include "rendering.h"

#include <iostream>
#include <sstream>
#include <ctime>

#include <experimental\filesystem>

namespace fs = std::experimental::filesystem;

using namespace std;

/*
	Initialization of global variables (most of which are declared in utilities.h or FILENAMEDEFS.h)
*/
float pivot = 0.5;
fs::path exported_texture_file_names_folder_relative((std::string)"Extracted Texture Names Data");
fs::path exported_meshes_folder_relative("Extracted Data");
//fs::path meshes_folder_relative("BD_Meshes");
fs::path exported_texture_file_names_folder_absolute(fs::current_path().string() + (string)"/" + exported_texture_file_names_folder_relative.string());
fs::path exported_meshes_folder_absolute(fs::current_path().string() + (string)"/" + exported_meshes_folder_relative.string());
fs::path meshes_folder_absolute(fs::current_path().string()/* + (string)"/" + meshes_folder_relative.string()*/);
bool running = true;
std::vector<fs::path> filenames;
std::vector<int> file_ids_to_convert;
std::vector<int> file_ids_to_render;
std::vector<int> file_ids_to_save;
std::vector<std::string> commands;
std::vector<mesh> meshes;
GLFWwindow* window;
fs::path vertex_shader_path((string) fs::current_path().parent_path().string() + (string)"/Shaders/SimpleVertexShader.vertexshader");
fs::path fragment_shader_path((string)fs::current_path().parent_path().string() + (string)"/Shaders/SimpleFragmentShader.fragmentshader");


/*
	Main: command interpretation
*/
int main()
{
	/*
		Copyright stuff and credits
	*/
	cout << "Treasure Planet : Battle at Procyon Inspector - mesh viewer and editor." << endl
		<< "*** by InkySka ***" << endl << endl
		<< "e-mail : underscore.marczero@gmail.com" << endl
		<< "Twitter : @InkySka" << endl << endl
		<< "Official repository : https://github.com/InkySka/TPBAPMeshViewer" << endl
		<< ".mdb file format info : https://treasure-planet.wikia.com/wiki/.mdb_mesh_files" << endl
		<< "Note: reverse - engineering is still undergoing and any help is appreciated." << endl << endl
		<< "Treasure Planet Wikia : https://treasure-planet.wikia.com/" << endl << endl
		<< "(C)InkySka 2018 - published under the MIT Licence." << endl;
	do //while (BOOL running)
	{
		/*
			Temporary string that does a lot of different things.
		*/
		string temp; 

		/*
			Getting commands into a string - they have to be space-separated.
		*/
		cout << "TPI> ";
		getline(cin, temp);

		/*
			Converting the command string into several strings via a stringstream,
			then adding the command to the command vector.
		*/
		istringstream parser(temp);
		while (parser)
		{
			temp.erase();
			parser >> temp;
			if (temp != "\n") commands.push_back(temp);
		}
		
		/*
			----------------------------
			Command interpreting section
			----------------------------
		*/

		/*
			/find: finds all of the valid files in the mesh folder (the current folder by default)
					and puts them in the filenames vector.

			Parameters: none.
		*/
		if (commands[0] == "/find")
		{
			TPI_findValidFiles();
		}

		/*
			/list: lists all of the contents of a file or fileID vector.
			Requires: see below.

			Parameters: one.

			The user can choose one from the list:
				-folder: lists all of the files in the current folder vector (filenames)
					Requires: /find
				-toingest: lists all of the files in the file IDs to ingest ("convert") vector (file_ids_to_convert)
					Requires: /ingest
				-loaded: lists all of the files in the meshes vector (meshes)
					Requires: /ingest
				-torender: lists all of the files in the file IDs to render vector (file_ids_to_render)
					Requires: /render
				-tosave: lists all of the files in the file IDs to save vector (file_ids_to_save)
					Requires: /save
			
			Note: while -toingest and -loaded seem to display the same list, they really do not.
				The first in fact outputs the files that are marked for ingestion. It accesses the file_ids_to_convert
				folder, which is erased after every ingestion. It is a "last files ingested" list, really.
				The second directly accesses the meshes vector using their getFilePath() function and shows those
				meshes that have been loaded into RAM.
		*/
		else if (commands[0] == "/list")
		{
			if (commands[1] == "-folder")
			{
				if (filenames.size() > 0) TPI_list(0);
				else cout << "No files in the list! Use /find first." << endl;
			}
			else if (commands[1] == "-toingest")
			{
				if (file_ids_to_convert.size() > 0) TPI_list(1);
				else cout << "No files marked for ingestion! Use /ingest first." << endl;
			}
			else if (commands[1] == "-torender")
			{
				if (file_ids_to_render.size() > 0) TPI_list(2);
				else cout << "No files marked for rendering! Use /render first." << endl;
			}
			else if (commands[1] == "-tosave")
			{
				if (file_ids_to_save.size() > 0) TPI_list(3);
				else cout << "No files marked for rendering! Use /render first." << endl;
			}
			else if (commands[1] == "-loaded")
			{
				if (meshes.size() > 0) TPI_list(4);
				else cout << "No files ingested! Use /ingest first." << endl;
			}
		}

		/*
			/ingest: ingest the marked (see below) .mdb files and load them into RAM,
				specifically into the meshes vector (see the mesh class for information
				on initialization).
				This operation can also be regarded to as "conversion" because it
				converts the .mdb files into the human-readable mesh object.
			Requires: /find

			Parameters: arbitrary.

			The user can choose one from the list:
				-all: loads all of the files in the filenames vector.
				-list: loads hand-picked files from the filenames vector, 
					whose IDs are specified in the list.
						Followed by the ID(s) of the file(s), taken from the filenames vector.
						The IDs can be retrieved with /list -folder.
						Comma (",") separates individual numbers, dash ("-") gives the files 
						from [left argument] to [right argument].
							(e.g. 2, 5, 7, 10-15, 23, 25, 30-40)
				<file names>: loads hand-picked files from the filenames vector.
					Here the user can specify the actual file name(s) (with either absolute
					or relative path). 
					The files still have to be in the filenames folder.

			Note: input is processed by the TPI_I_normalizeInput() function.
		*/
		else if (commands[0] == "/ingest")
		{

			//filenames_to_convert.erase(filenames_to_convert.begin(), filenames_to_convert.end());
			file_ids_to_convert = TPI_I_normalizeInput(filenames.size(), 1);
			removeDuplicates(file_ids_to_convert);
			
			if (file_ids_to_convert.size() > 0) TPI_ingestMeshes();
			else cout << "No files to convert! Use /ingest first." << endl;
			//file_ids_to_convert.erase(file_ids_to_convert.begin(), file_ids_to_convert.end());
		}

		/*
			/render: render the marked (see below) meshes using OpenGL.
			Requires: /ingest

			Parameters (listed in order): arbitrary.

			The user can choose one from the list:
			-all: loads all of the files in the meshes vector.
			-list: loads hand-picked files from the meshes vector,
				whose IDs are specified in the list.
					Followed by the ID(s) of the file(s), taken from the meshes vector.
					The IDs can be retrieved with /list -loaded.
					Comma (",") separates individual numbers, dash ("-") gives the files
					from [left argument] to [right argument].
						(e.g. 2, 5, 7, 10-15, 23, 25, 30-40)
			<file names>: loads hand-picked files from the meshes vector.
				Here the user can specify the actual file name(s) (with either absolute
				or relative path).
				The files have to be in the meshes folder.

			Note: input is processed by the TPI_I_normalizeInput() function.
		*/
		else if (commands[0] == "/render")
		{
			vector<int> temp_ids = TPI_I_normalizeInput(meshes.size(), 1);
			file_ids_to_render.insert(file_ids_to_render.end(), temp_ids.begin(), temp_ids.end());
			removeDuplicates(file_ids_to_render);

			if (file_ids_to_render.size() > 0)
			{
				/*
					OpenGL libraries initialisation.
				*/
				initLibs();
				
				for (int i = 0; i < file_ids_to_render.size(); ++i)
				{
					/*
						Rendering.
					*/
					meshes[file_ids_to_render[i]].render();
					/*for (int i = 2; i < temp_3D_data.size(); i+=3)
					{
						cout << temp_3D_data[i - 2] << " " << temp_3D_data[i - 1] << " " << temp_3D_data[i] << endl;
					}*/
				}
			}
			else cout << "No files to convert! Use /ingest first." << endl;
		}

		/*
			/save: save the marked (see below) meshes to OBJ or back to MDB.
			Requires: /ingest

			Parameters (listed in order): arbitrary.

			1. The user can choose one from the list:
				-obj: save the meshes to WaveFront OBJ files (with MTL material files) - a.k.a. exporting.
				-mdb: save the meshes to .mdb files - a.k.a. importing

			2. The user can choose one from the list:
				-all: saves all of the files present in the meshes vector.
				-list: saves hand-picked files present in the meshes vector,
					whose IDs are specified in the list.
						Followed by the ID(s) of the file(s), taken from the meshes vector.
						The IDs can be retrieved with /list -loaded.
						Comma (",") separates individual numbers, dash ("-") gives the files
						from [left argument] to [right argument].
							(e.g. 2, 5, 7, 10-15, 23, 25, 30-40)
				<file names>: loads hand-picked files from the meshes vector.
					Here the user can specify the actual file name(s) (with either absolute
					or relative path).
					The files still have to be in the meshes folder.

			Note: input is processed by the TPI_I_normalizeInput() function.
		*/
		else if (commands[0] == "/save")
		{
			if (commands[1] == "-obj")
			{

			}
			else if (commands[1] == "-mdb")
			{
				vector<int> temp_ids = TPI_I_normalizeInput(meshes.size(), 2);
				removeDuplicates(temp_ids);
				for (int i = 0; i < temp_ids.size(); ++i)
				{
					meshes[i].convertToMDB();
				}
			}
		}

		/*
			/scale: scale the meshes dimensions.
			Requires: /ingest

			Parameters (listed in order): arbitrary.

			1. Mandatory:
			-factor: specifies the factor of the scale
				Followed by <INT factor>.
				The scaling process is: axis *= factor.
			

			2. The user can choose one from the list:
				-all: scales all of the files present in the meshes vector.
				-list: scales hand-picked files present in the meshes vector,
					whose IDs are specified in the list.
						Followed by the ID(s) of the file(s), taken from the meshes vector.
						The IDs can be retrieved with /list -loaded.
						Comma (",") separates individual numbers, dash ("-") gives the files
						from [left argument] to [right argument].
							(e.g. 2, 5, 7, 10-15, 23, 25, 30-40)
				<file names>: loads hand-picked files from the meshes vector.
					Here the user can specify the actual file name(s) (with either absolute
					or relative path).
					The files still have to be in the meshes folder.

		Note: input is processed by the TPI_I_normalizeInput() function.
		*/
		else if (commands[0] == "/scale")
		{
			if (commands[1] == "-factor")
			{
				string factor_temp;
				vector<int> temp_ids = TPI_I_normalizeInput(meshes.size(), 3);
				removeDuplicates(temp_ids);
				for (int i = 0; i < temp_ids.size(); ++i)
				{
					meshes[i].scale(stoi(commands[2]));
					cout << "Scaled " << meshes[temp_ids[i]].getFilePath() << endl;
				}
			}
			else cout << "Bad syntax." << endl;
		}
		
		/*
			/exit: sets BOOL running to false, thus making the program return.
			
			Parameters: none.
		*/
		else if (commands[0] == "/exit")
		{
			running = false;
		}
		else cout << "Wrong command!" << endl;

		/*
			Erasing the commands vector after every iteration.
		*/
		commands.erase(commands.begin(), commands.end());
	} while (running);

	return 0;
}

