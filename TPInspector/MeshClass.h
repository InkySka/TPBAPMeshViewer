#pragma once
#include <fstream>
#include <vector>
#include <experimental\filesystem>
#include "FILENAMEDEFS.h"
#include "Offsets.h"

namespace fs = std::experimental::filesystem;

struct vertex
{
	float x, y, z;
	float u, v;
	float transparency;
	float other_data_a, other_data_b; //currently unknown. placeholder

	unsigned int dim;
};

struct face
{
	unsigned short vertex1, vertex2, vertex3, material_n;

	bool hasMoreData = false;//for testing purposes
	std::vector<unsigned int> other_data; //even if there should be nothing

	unsigned int dim;
};

struct material
{
	std::string texture_name;
	std::vector<unsigned int> other_data;

	unsigned int dim;

	inline bool operator< (const material& other) const
	{
		return this->texture_name < other.texture_name;
	}
	inline bool operator== (const material& other) const
	{
		return this->texture_name == other.texture_name;
	}
};

class mesh
{
public:
	mesh(const fs::path& i_file_path) :
		file_path(i_file_path) 
	{}

	void ingest();//read from file
	void findOffsets();
	void DEV_saveOtherData(const fs::path& i_folder_path = exported_meshes_folder_absolute) const;

	void getVertexData();

	void saveToFile() const;
	fs::path getFilePath() const;
	
private:
	std::vector<vertex> findVertices(const std::streamoff& i_offset);
	std::vector<face> findFaces(const std::streamoff& i_offset);
	std::vector<material> findMaterials(const std::streamoff& i_offset);
	void saveOBJToFile(const fs::path& i_folder_path = exported_meshes_folder_absolute) const;
	void saveMTLToFile(const fs::path& i_folder_path = exported_meshes_folder_absolute) const;
	void saveTextureNamesToFile(const fs::path& i_folder_path = exported_texture_file_names_folder_absolute) const;
	void tidyUp();//populate actually used materials. there are some that appear in the mesh's file but are not used in the faces

	unsigned int type;

	fs::path file_path;

	std::vector<std::streamoff> vertex_data_offset;

	std::vector<std::streamoff> face_data_offset;

	std::streamoff material_data_offset;

	//std::streamoff collision_data_offset;

	std::vector<std::vector<vertex>> vertices;

	std::vector<std::vector<face>> faces;
	
	std::vector<material> materials;
	std::vector<std::vector<material>> actually_used_materials;

	bool error_flag = false;
	std::vector<std::string> errors;
};

