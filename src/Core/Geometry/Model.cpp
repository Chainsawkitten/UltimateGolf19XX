#include "Model.hpp"
//#include "util.hpp"
using namespace Geometry;
Model::Model(const char* filename) {
	std::ifstream modelFile;
	modelFile.open(filename, std::ios_base::binary);
	if (!modelFile.is_open()) {
		fprintf(stderr, "Couldn't open model file %s for reading.\n", filename);
		fflush(stderr);
	}

	modelFile.read(reinterpret_cast<char*>(&vertexNr), sizeof(vertexNr));
	vertexData = new Vertex[vertexNr];
	modelFile.read(reinterpret_cast<char*>(&vertexData[0]), vertexNr * sizeof(Vertex));

	modelFile.close();

	indexNr = vertexNr;
	indexData = new unsigned int[indexNr];
	for (unsigned int i = 0; i < indexNr; i++) {
		indexData[i] = i;
	}

	GenerateBuffers();
	GenerateVertexArray();
}

Model::~Model() {
	delete[] vertexData;
	delete[] indexData;
}

Geometry::Geometry3D:: Vertex* Model::Vertices() const {
	return vertexData;
}

unsigned int Model::VertexCount() const {
	return vertexNr;
}

unsigned int* Model::Indices() const {
	return indexData;
}

unsigned int Model::IndexCount() const {
	return indexNr;
}

void Model::Save(const char* filename) const {
	std::ofstream modelFile;
	modelFile.open(filename, std::ios_base::binary);

	modelFile.write(reinterpret_cast<const char*>(&vertexNr), sizeof(vertexNr));
	modelFile.write(reinterpret_cast<const char*>(&vertexData[0]), vertexNr * sizeof(Vertex));

	modelFile.close();
}