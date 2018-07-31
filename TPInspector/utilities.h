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

	Utilities file: miscellaneous functions.
*/

#pragma once

#include <vector>
#include <map>

#include <experimental\filesystem>

#include "MeshClass.h"

namespace fs = std::experimental::filesystem;

extern bool running;

extern bool isLittleEndian;

extern float pivot; //defined in main

std::vector<fs::path> fileFinderByExtension(const fs::path& I_folder, const std::string& I_ext, const bool& I_includeSubDirs = false);
std::vector<fs::path> fileFinderByExtension(const std::vector<fs::path>& I_files, const std::string& I_extension);

extern inline float pivoter(const float& I_val);

union data_4bytes
{
	int i;
	unsigned int ui;
	float f;
	char c[4];
	unsigned char uc[4];
	short s[2];
};

void TPI_ingestMeshes();
void TPI_saveToFile();
void TPI_findValidFiles(const std::string& I_extension = mesh_data_extension);
void TPI_list(const int& mode);
void TPI_changeFolder();
void TPI_changeOutputFolder();

std::vector<int> TPI_I_normalizeInput(const std::vector<int>& input);
std::vector<int> TPI_I_normalizeInput(const int& input, const int& index_to_start_from);

/*///<summary>
///Shows the input to screen
///</summary>
///<remarks>
///Overloaded for a vector(int (file ID) or string) or a single value (int (file ID) or string).
///</remarks>
void TPI_show(const std::vector<int> input);*/

/*std::vector<fs::path> generateFilenameList(); //taken from filenames.
std::vector<int> generateFileIDList(const int& mode);*/

//std::vector<int> TPI_I_normalizeInput();

extern std::vector<fs::path> filenames;
extern std::vector<int> file_ids_to_convert;
extern std::vector<int> file_ids_to_render;
extern std::vector<int> file_ids_to_save;

void removeDuplicates(std::vector<int> I_vec);

extern std::vector<std::string> commands;
extern std::vector<mesh> meshes;