// OBJ_Loader.h - A Single Header OBJ Model Loader

#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "vec2.h"
#include "vec3.h"
#include "Mesh.h"


namespace {	

	namespace algorithm
	{

		// Check to see if a vec3 Point is within a 3 vec3 Triangle
		bool inTriangle(vec3 point, vec3 tri1, vec3 tri2, vec3 tri3)
		{
			// Starting vars
			vec3 u = tri2 - tri1;
			vec3 v = tri3 - tri1;
			vec3 w = point - tri1;
			vec3 n = crossProduct(u, v);

			float y = (dotProduct(crossProduct(u, w), n) / dotProduct(n, n));
			float b = (dotProduct(crossProduct(u, w), n) / dotProduct(n, n));
			float a = 1 - y - b;

			// Projected point
			vec3  p = (a * tri1) + (b * tri2) + (y * tri3);

			if (a >= 0 && a <= 1
				&& b >= 0 && b <= 1
				&& y >= 0 && y <= 1)
			{
				return true;
			}
			else
				return false;
		}

		// Split a String into a string array at a given token
		inline void split(const std::string &in,
			std::vector<std::string> &out,
			std::string token)
		{
			out.clear();

			std::string temp;

			for (int i = 0; i < int(in.size()); i++)
			{
				std::string test = in.substr(i, token.size());

				if (test == token)
				{
					if (!temp.empty())
					{
						out.push_back(temp);
						temp.clear();
						i += (int)token.size() - 1;
					}
					else
					{
						out.push_back("");
					}
				}
				else if (i + token.size() >= in.size())
				{
					temp += in.substr(i, token.size());
					out.push_back(temp);
					break;
				}
				else
				{
					temp += in[i];
				}
			}
		}

		// Get tail of string after first token and possibly following spaces
		inline std::string tail(const std::string &in)
		{
			size_t token_start = in.find_first_not_of(" \t");
			size_t space_start = in.find_first_of(" \t", token_start);
			size_t tail_start = in.find_first_not_of(" \t", space_start);
			size_t tail_end = in.find_last_not_of(" \t");
			if (tail_start != std::string::npos && tail_end != std::string::npos)
			{
				return in.substr(tail_start, tail_end - tail_start + 1);
			}
			else if (tail_start != std::string::npos)
			{
				return in.substr(tail_start);
			}
			return "";
		}

		// Get first token of string
		inline std::string firstToken(const std::string &in)
		{
			if (!in.empty())
			{
				size_t token_start = in.find_first_not_of(" \t");
				size_t token_end = in.find_first_of(" \t", token_start);
				if (token_start != std::string::npos && token_end != std::string::npos)
				{
					return in.substr(token_start, token_end - token_start);
				}
				else if (token_start != std::string::npos)
				{
					return in.substr(token_start);
				}
			}
			return "";
		}

		// Get element at given index position
		template <class T>
		inline const T & getElement(const std::vector<T> &elements, std::string &index)
		{
			int idx = std::stoi(index);
			if (idx < 0)
				idx = int(elements.size()) + idx;
			else
				idx--;
			return elements[idx];
		}
	}

	class ObjLoader
	{
	public:

		// Loaded Mesh Objects
		ObjModel* LoadedModel;
		// Loaded Material Objects
		std::vector<Material> LoadedMaterials;

		ObjLoader() {
			LoadedModel = new ObjModel();
		}
		~ObjLoader() {
			LoadedModel->meshes.clear();
		}

		bool LoadFile(std::string Path)
		{
			// If the file is not an .obj file return false
			if (Path.substr(Path.size() - 4, 4) != ".obj")
				return false;


			std::ifstream file(Path);

			if (!file.is_open())
				return false;

			LoadedModel = new ObjModel();

			std::vector<vec3> Positions;
			std::vector<vec2> TCoords;
			std::vector<vec3> Normals;

			std::vector <Vertex> MeshPositions;
			std::vector <Texcoord> MeshTexcoords;
			std::vector <Normal> MeshNormals;

			std::vector<unsigned int> Indices;

			std::vector<std::string> MeshMatNames;

			bool listening = false;
			std::string meshname;

			Mesh* tempMesh;

			std::string curline;
			while (std::getline(file, curline))
			{

				// Generate a Mesh Object or Prepare for an object to be created
				if (algorithm::firstToken(curline) == "o" || algorithm::firstToken(curline) == "g" || curline[0] == 'g')
				{
					if (!listening)
					{
						listening = true;

						if (algorithm::firstToken(curline) == "o" || algorithm::firstToken(curline) == "g")
						{
							meshname = algorithm::tail(curline);
						}
						else
						{
							meshname = "unnamed";
						}
					}
					else
					{
						// Generate the mesh to put into the array
						if (!MeshPositions.empty())
						{
							// Create Mesh
							tempMesh = new Mesh(MeshPositions, MeshNormals, MeshTexcoords, Indices);
							tempMesh->MeshName = meshname;

							// Insert Mesh
							LoadedModel->meshes.push_back(tempMesh);

							// Cleanup
							MeshPositions.clear();
							MeshNormals.clear();
							MeshTexcoords.clear();

							Indices.clear();
							meshname.clear();

							meshname = algorithm::tail(curline);
						}
						else
						{
							if (algorithm::firstToken(curline) == "o" || algorithm::firstToken(curline) == "g")
							{
								meshname = algorithm::tail(curline);
							}
							else
							{
								meshname = "unnamed";
							}
						}
					}

				}
				// Generate a Position
				if (algorithm::firstToken(curline) == "v")
				{
					std::vector<std::string> spos;
					vec3 vpos;
					algorithm::split(algorithm::tail(curline), spos, " ");

					vpos.x = std::stof(spos[0]);
					vpos.y = std::stof(spos[1]);
					vpos.z = std::stof(spos[2]);

					Positions.push_back(vpos);
				}
				// Generate Texture Coordinate
				if (algorithm::firstToken(curline) == "vt")
				{
					std::vector<std::string> stex;
					vec2 vtex;
					algorithm::split(algorithm::tail(curline), stex, " ");

					vtex.x = std::stof(stex[0]);
					vtex.y = std::stof(stex[1]);

					TCoords.push_back(vtex);
				}
				// Generate a Normal;
				if (algorithm::firstToken(curline) == "vn")
				{
					std::vector<std::string> snor;
					vec3 vnor;
					algorithm::split(algorithm::tail(curline), snor, " ");

					vnor.x = std::stof(snor[0]);
					vnor.y = std::stof(snor[1]);
					vnor.z = std::stof(snor[2]);

					Normals.push_back(vnor);
				}
				// Generate a Face (vertices & indices)
				if (algorithm::firstToken(curline) == "f")
				{
					// Generate the vertices (position, texture coords, normal)
					std::vector<Vertex> vVerts;
					std::vector<Texcoord> vTexcoords;
					std::vector<Normal> vNormals;
					GenVerticesFromRawOBJ(vVerts, vTexcoords, vNormals,
						Positions, TCoords, Normals, curline);

					// Add Vertices
					for (int i = 0; i < int(vVerts.size()); i++)
					{
						MeshPositions.push_back(vVerts[i]);
						MeshTexcoords.push_back(vTexcoords[i]);
						MeshNormals.push_back(vNormals[i]);
					}

					std::vector<unsigned int> iIndices;
					VertexTriangluation(iIndices, vVerts);
					// Add Indices
					for (int i = 0; i < int(iIndices.size()); i++)
					{
						unsigned int indnum = (unsigned int)((MeshPositions.size()) - vVerts.size()) + iIndices[i];
						Indices.push_back(indnum);

					}

				}
				// Get Mesh Material Name
				if (algorithm::firstToken(curline) == "usemtl")
				{
					MeshMatNames.push_back(algorithm::tail(curline));

					// Create new Mesh, if Material changes within a group
					if (!MeshPositions.empty())
					{
						// Create Mesh
						tempMesh = new Mesh(MeshPositions, MeshNormals, MeshTexcoords, Indices);
						tempMesh->MeshName = meshname;
						int i = 2;
						while (1) {
							tempMesh->MeshName = meshname + "_" + std::to_string(i);

							for (auto &m : LoadedModel->meshes)
								if (m->MeshName == tempMesh->MeshName)
									continue;
							break;
						}

						// Insert Mesh
						LoadedModel->meshes.push_back(tempMesh);

						// Cleanup
						MeshPositions.clear();
						MeshNormals.clear();
						MeshTexcoords.clear();
						Indices.clear();
					}

				}
				// Load Materials
				if (algorithm::firstToken(curline) == "mtllib")
				{
					// Generate LoadedMaterial

					// Generate a path to the material file
					std::vector<std::string> temp;
					algorithm::split(Path, temp, "/");

					std::string pathtomat = "";

					if (temp.size() != 1)
					{
						for (int i = 0; i < temp.size() - 1; i++)
						{
							pathtomat += temp[i] + "/";
						}
					}


					pathtomat += algorithm::tail(curline);

					// Load Materials
					LoadMaterials(pathtomat);
				}
			}


			// Deal with last mesh
			if (!MeshPositions.empty())
			{
				// Create Mesh
				tempMesh = new Mesh(MeshPositions, MeshNormals, MeshTexcoords, Indices);
				tempMesh->MeshName = meshname;

				// Insert Mesh
				LoadedModel->meshes.push_back(tempMesh);
			}

			file.close();

			// Set Materials for each Mesh
			for (int i = 0; i < MeshMatNames.size(); i++)
			{
				std::string matname = MeshMatNames[i];

				// Find corresponding material name in loaded materials
				// when found copy material variables into mesh material
				for (int j = 0; j < LoadedMaterials.size(); j++)
				{
					if (LoadedMaterials[j].name == matname)
					{
						LoadedModel->meshes[i]->MeshMaterial = LoadedMaterials[j];
						break;
					}
				}
			}

			if (LoadedModel->meshes.empty())
			{
				return false;
			}
			else
			{
				Positions.clear();
				TCoords.clear();
				Normals.clear();

				MeshPositions.clear();
				MeshTexcoords.clear();
				MeshNormals.clear();
				Indices.clear();
				LoadedMaterials.clear();
				return true;
			}
		}



	private:
		// Generate vertices from a list of positions, 
		//	tcoords, normals and a face line
		void GenVerticesFromRawOBJ(std::vector<Vertex>& oVerts,
			std::vector<Texcoord>& oTexcoords,
			std::vector<Normal>& oNormals,
			const std::vector<vec3>& iPositions,
			const std::vector<vec2>& iTCoords,
			const std::vector<vec3>& iNormals,
			std::string icurline)
		{
			std::vector<std::string> sface, svert;
			Vertex vVert;
			Texcoord vTexcoord;
			Normal vNormal;
			algorithm::split(algorithm::tail(icurline), sface, " ");

			bool noNormal = false;

			// For every given vertex do this
			for (int i = 0; i < int(sface.size()); i++)
			{
				// See What type the vertex is.
				int vtype;

				algorithm::split(sface[i], svert, "/");

				// Check for just position - v1
				if (svert.size() == 1)
				{
					// Only position
					vtype = 1;
				}

				// Check for position & texture - v1/vt1
				if (svert.size() == 2)
				{
					// Position & Texture
					vtype = 2;
				}

				// Check for Position, Texture and Normal - v1/vt1/vn1
				// or if Position and Normal - v1//vn1
				if (svert.size() == 3)
				{
					if (svert[1] != "")
					{
						// Position, Texture, and Normal
						vtype = 4;
					}
					else
					{
						// Position & Normal
						vtype = 3;
					}
				}

				// Calculate and store the VertexOld
				switch (vtype)
				{
				case 1: // P
				{
					vVert = algorithm::getElement(iPositions, svert[0]);
					vTexcoord = vec2(0, 0);
					noNormal = true;
					oVerts.push_back(vVert);
					oTexcoords.push_back(vTexcoord);
					break;
				}
				case 2: // P/T
				{
					vVert = algorithm::getElement(iPositions, svert[0]);
					vTexcoord = algorithm::getElement(iTCoords, svert[1]);
					noNormal = true;
					oVerts.push_back(vVert);
					oTexcoords.push_back(vTexcoord);
					break;
				}
				case 3: // P//N
				{
					vVert = algorithm::getElement(iPositions, svert[0]);
					vTexcoord = vec2(0, 0);
					vNormal = algorithm::getElement(iNormals, svert[2]);
					oVerts.push_back(vVert);
					oTexcoords.push_back(vTexcoord);
					oNormals.push_back(vNormal);
					break;
				}
				case 4: // P/T/N
				{
					vVert = algorithm::getElement(iPositions, svert[0]);
					vTexcoord = algorithm::getElement(iTCoords, svert[1]);
					vNormal = algorithm::getElement(iNormals, svert[2]);
					oVerts.push_back(vVert);
					oTexcoords.push_back(vTexcoord);
					oNormals.push_back(vNormal);
					break;
				}
				default:
				{
					break;
				}
				}
			}

			// take care of missing normals
			// these may not be truly acurate but it is the 
			// best they get for not compiling a mesh with normals	
			if (noNormal)
			{
				vec3 A = vec3(oVerts[0].x - oVerts[1].x,
					oVerts[0].y - oVerts[1].y,
					oVerts[0].z - oVerts[1].z);
				vec3 B = vec3(oVerts[2].x - oVerts[1].x,
					oVerts[2].y - oVerts[1].y,
					oVerts[2].z - oVerts[1].z);

				vec3 normal = crossProduct(A, B);

				for (int i = 0; i < int(oVerts.size()); i++)
				{
					oNormals.push_back(normal);
				}
			}
		}


		void VertexTriangluation(std::vector<unsigned int>& oIndices,
			const std::vector<Vertex>& iVerts)
		{
			// If there are 2 or less verts,
			// no triangle can be created,
			// so exit
			if (iVerts.size() < 3)
			{
				return;
			}
			// If it is a triangle no need to calculate it
			if (iVerts.size() == 3)
			{
				oIndices.push_back(0);
				oIndices.push_back(1);
				oIndices.push_back(2);
				return;
			}

			// Create a list of vertices
			std::vector<Vertex> tVerts = iVerts;

			while (true)
			{
				// For every vertex
				for (int i = 0; i < int(tVerts.size()); i++)
				{
					// pPrev = the previous vertex in the list
					Vertex pPrev;
					if (i == 0)
					{
						pPrev = tVerts[tVerts.size() - 1];
					}
					else
					{
						pPrev = tVerts[i - 1];
					}

					// pCur = the current vertex;
					Vertex pCur = tVerts[i];

					// pNext = the next vertex in the list
					Vertex pNext;
					if (i == tVerts.size() - 1)
					{
						pNext = tVerts[0];
					}
					else
					{
						pNext = tVerts[i + 1];
					}

					// Check to see if there are only 3 verts left
					// if so this is the last triangle
					if (tVerts.size() == 3)
					{
						// Create a triangle from pCur, pPrev, pNext
						for (int j = 0; j < int(tVerts.size()); j++)
						{
							if (iVerts[j] == pCur)
								oIndices.push_back(j);
							if (iVerts[j] == pPrev)
								oIndices.push_back(j);
							if (iVerts[j] == pNext)
								oIndices.push_back(j);
						}

						tVerts.clear();
						break;
					}
					if (tVerts.size() == 4)
					{
						// Create a triangle from pCur, pPrev, pNext
						for (int j = 0; j < int(iVerts.size()); j++)
						{
							if (iVerts[j] == pCur)
								oIndices.push_back(j);
							if (iVerts[j] == pPrev)
								oIndices.push_back(j);
							if (iVerts[j] == pNext)
								oIndices.push_back(j);
						}

						Vertex tempVec;
						for (int j = 0; j < int(tVerts.size()); j++)
						{
							if (tVerts[j] != pCur
								&& tVerts[j] != pPrev
								&& tVerts[j] != pNext)
							{
								tempVec = tVerts[j];
								break;
							}
						}

						// Create a triangle from pCur, pPrev, pNext
						for (int j = 0; j < int(iVerts.size()); j++)
						{
							if (iVerts[j] == pPrev)
								oIndices.push_back(j);
							if (iVerts[j] == pNext)
								oIndices.push_back(j);
							if (iVerts[j] == tempVec)
								oIndices.push_back(j);
						}

						tVerts.clear();
						break;
					}

					// If Vertex is not an interior vertex
					float angle = angleBetween(vec3(pPrev.x, pPrev.y, pPrev.z) - vec3(pCur.x, pCur.y, pCur.z), 
						vec3(pNext.x, pNext.y, pNext.z) - vec3(pCur.x, pCur.y, pCur.z)) * (180 / 3.14159265359);

					if (angle <= 0 && angle >= 180)
						continue;

					// If any vertices are within this triangle
					bool inTri = false;
					for (int j = 0; j < int(iVerts.size()); j++)
					{
						if (algorithm::inTriangle(vec3(iVerts[j].x, iVerts[j].y, iVerts[j].z), 
							vec3(pPrev.x, pPrev.y, pPrev.z),
							vec3(pCur.x, pCur.y, pCur.z),
							vec3(pNext.x, pNext.y, pNext.z))
							&& iVerts[j] != pPrev
							&& iVerts[j] != pCur
							&& iVerts[j] != pNext)
						{
							inTri = true;
							break;
						}
					}
					if (inTri)
						continue;

					// Create a triangle from pCur, pPrev, pNext
					for (int j = 0; j < int(iVerts.size()); j++)
					{
						if (iVerts[j] == pCur)
							oIndices.push_back(j);
						if (iVerts[j] == pPrev)
							oIndices.push_back(j);
						if (iVerts[j] == pNext)
							oIndices.push_back(j);
					}

					// Delete pCur from the list
					for (int j = 0; j < int(tVerts.size()); j++)
					{
						if (tVerts[j] == pCur)
						{
							tVerts.erase(tVerts.begin() + j);
							break;
						}
					}

					// reset i to the start
					// -1 since loop will add 1 to it
					i = -1;
				}

				// if no triangles were created
				if (oIndices.size() == 0)
					break;

				// if no more vertices
				if (tVerts.size() == 0)
					break;
			}
		}

		// Load Materials from .mtl file
		bool LoadMaterials(std::string path)
		{
			// If the file is not a material file return false
			if (path.substr(path.size() - 4, path.size()) != ".mtl")
				return false;

			std::ifstream file(path);

			// If the file is not found return false
			if (!file.is_open())
				return false;

			Material tempMaterial;

			bool listening = false;

			// Go through each line looking for material variables
			std::string curline;
			while (std::getline(file, curline))
			{
				// new material and material name
				if (algorithm::firstToken(curline) == "newmtl")
				{
					if (!listening)
					{
						listening = true;

						if (curline.size() > 7)
						{
							tempMaterial.name = algorithm::tail(curline);
						}
						else
						{
							tempMaterial.name = "none";
						}
					}
					else
					{
						// Generate the material

						// Push Back loaded Material
						LoadedMaterials.push_back(tempMaterial);

						// Clear Loaded Material
						tempMaterial = Material();

						if (curline.size() > 7)
						{
							tempMaterial.name = algorithm::tail(curline);
						}
						else
						{
							tempMaterial.name = "none";
						}
					}
				}
				// Ambient Color
				if (algorithm::firstToken(curline) == "Ka")
				{
					std::vector<std::string> temp;
					algorithm::split(algorithm::tail(curline), temp, " ");

					if (temp.size() != 3)
						continue;

					tempMaterial.Ka.x = std::stof(temp[0]);
					tempMaterial.Ka.y = std::stof(temp[1]);
					tempMaterial.Ka.z = std::stof(temp[2]);
				}
				// Diffuse Color
				if (algorithm::firstToken(curline) == "Kd")
				{
					std::vector<std::string> temp;
					algorithm::split(algorithm::tail(curline), temp, " ");

					if (temp.size() != 3)
						continue;

					tempMaterial.Kd.x = std::stof(temp[0]);
					tempMaterial.Kd.y = std::stof(temp[1]);
					tempMaterial.Kd.z = std::stof(temp[2]);
				}
				// Specular Color
				if (algorithm::firstToken(curline) == "Ks")
				{
					std::vector<std::string> temp;
					algorithm::split(algorithm::tail(curline), temp, " ");

					if (temp.size() != 3)
						continue;

					tempMaterial.Ks.x = std::stof(temp[0]);
					tempMaterial.Ks.y = std::stof(temp[1]);
					tempMaterial.Ks.z = std::stof(temp[2]);
				}
				// Specular Exponent
				if (algorithm::firstToken(curline) == "Ns")
				{
					tempMaterial.Ns = std::stof(algorithm::tail(curline));
				}
				// Optical Density
				if (algorithm::firstToken(curline) == "Ni")
				{
					tempMaterial.Ni = std::stof(algorithm::tail(curline));
				}
				// Dissolve
				if (algorithm::firstToken(curline) == "d")
				{
					tempMaterial.d = std::stof(algorithm::tail(curline));
				}
				// Illumination
				if (algorithm::firstToken(curline) == "illum")
				{
					tempMaterial.illum = std::stoi(algorithm::tail(curline));
				}
				// Ambient Texture Map
				if (algorithm::firstToken(curline) == "map_Ka")
				{
					tempMaterial.map_Ka = algorithm::tail(curline);
				}
				// Diffuse Texture Map
				if (algorithm::firstToken(curline) == "map_Kd")
				{
					tempMaterial.map_Kd = algorithm::tail(curline);
				}
				// Specular Texture Map
				if (algorithm::firstToken(curline) == "map_Ks")
				{
					tempMaterial.map_Ks = algorithm::tail(curline);
				}
				// Specular Hightlight Map
				if (algorithm::firstToken(curline) == "map_Ns")
				{
					tempMaterial.map_Ns = algorithm::tail(curline);
				}
				// Alpha Texture Map
				if (algorithm::firstToken(curline) == "map_d")
				{
					tempMaterial.map_d = algorithm::tail(curline);
				}
				// Bump Map
				if (algorithm::firstToken(curline) == "map_Bump" || algorithm::firstToken(curline) == "map_bump")
				{
					tempMaterial.map_bump = algorithm::tail(curline);
				}
			}

			// Deal with last material

			// Push Back loaded Material
			LoadedMaterials.push_back(tempMaterial);

			// Test to see if anything was loaded
			// If not return false
			if (LoadedMaterials.empty())
				return false;
			// If so return true
			else
				return true;
		}
	};
}