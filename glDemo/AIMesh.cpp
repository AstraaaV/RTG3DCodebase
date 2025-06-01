
#include "AIMesh.h"
#include "TextureLoader.h"

using namespace std;
using namespace glm;


AIMesh::AIMesh(std::string _filename, GLuint _meshIndex)
{
	const struct aiScene* scene = aiImportFile(_filename.c_str(),
		aiProcess_GenSmoothNormals |
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);

	if (!scene)
	{
		cout << "AIMesh failed to load : " << _filename << endl;
		return;
	}

	if (_meshIndex >= scene->mNumMeshes)
	{
		cout << "Invalid Mesh Index: " << _meshIndex << " for " << _filename << endl;
		aiReleaseImport(scene);
		return;
	}

	aiMesh* mesh = scene->mMeshes[_meshIndex];

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	// Setup VBO for vertex position data
	glGenBuffers(1, &m_meshVertexPosBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_meshVertexPosBuffer);
	glBufferData(GL_ARRAY_BUFFER, mesh->mNumVertices * sizeof(aiVector3D), mesh->mVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Setup VBO for vertex normal data
	glGenBuffers(1, &m_meshNormalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_meshNormalBuffer);
	glBufferData(GL_ARRAY_BUFFER, mesh->mNumVertices * sizeof(aiVector3D), mesh->mNormals, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(1);

	// *** normal mapping *** Setup VBO for tangent and bi-tangent data
	glGenBuffers(1, &m_meshTangentBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_meshTangentBuffer);
	glBufferData(GL_ARRAY_BUFFER, mesh->mNumVertices * sizeof(aiVector3D), mesh->mTangents, GL_STATIC_DRAW);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(4);

	glGenBuffers(1, &m_meshBiTangentBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_meshBiTangentBuffer);
	glBufferData(GL_ARRAY_BUFFER, mesh->mNumVertices * sizeof(aiVector3D), mesh->mBitangents, GL_STATIC_DRAW);
	glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(5);

	if (mesh->mTextureCoords && mesh->mTextureCoords[0])
	{
		// Setup VBO for texture coordinate data (for now use uvw channel 0 only when accessing mesh->mTextureCoords)
		glGenBuffers(1, &m_meshTexCoordBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_meshTexCoordBuffer);
		glBufferData(GL_ARRAY_BUFFER, mesh->mNumVertices * sizeof(aiVector3D), mesh->mTextureCoords[0], GL_STATIC_DRAW);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
		glEnableVertexAttribArray(2);
	}

	// Setup VBO for mesh index buffer (face index array)

	m_numFaces = mesh->mNumFaces;

	// Setup contiguous array
	const GLuint numBytes = m_numFaces * 3 * sizeof(GLuint);
	GLuint* faceIndexArray = (GLuint*)malloc(numBytes);

	if (!faceIndexArray)
	{
		cout << "[AIMESH] Failed to allocate memory for faceIndexArray: " << _filename << endl;
		aiReleaseImport(scene);
		return;
	}

	for (unsigned int f = 0; f < m_numFaces; ++f)
	{
		aiFace& face = mesh->mFaces[f];

		faceIndexArray[f * 3 + 0] = face.mIndices[0];
		faceIndexArray[f * 3 + 1] = face.mIndices[1];
		faceIndexArray[f * 3 + 2] = face.mIndices[2];
	}

	glGenBuffers(1, &m_meshFaceIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_meshFaceIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numBytes, faceIndexArray, GL_STATIC_DRAW);

	free(faceIndexArray);

	glBindVertexArray(0);

	// Once done, release all resources associated with this import
	aiReleaseImport(scene);
}

AIMesh::~AIMesh()
{
	if (m_meshVertexPosBuffer) glDeleteBuffers(1, &m_meshVertexPosBuffer);
	if (m_meshNormalBuffer) glDeleteBuffers(1, &m_meshNormalBuffer);
	if (m_meshTangentBuffer) glDeleteBuffers(1, &m_meshTangentBuffer);
	if (m_meshBiTangentBuffer) glDeleteBuffers(1, &m_meshBiTangentBuffer);
	if (m_meshTexCoordBuffer) glDeleteBuffers(1, &m_meshTexCoordBuffer);
	if (m_meshFaceIndexBuffer) glDeleteBuffers(1, &m_meshFaceIndexBuffer);
	if (m_vao) glDeleteVertexArrays(1, &m_vao);
}

// Texture setup methods

void AIMesh::addTexture(GLuint _textureID)
{
	this->m_textureID = _textureID;
}

void AIMesh::addTexture(std::string _filename, FREE_IMAGE_FORMAT _format)
{
	m_textureID = loadTexture(_filename, _format);
}

// ***normal mapping*** - helper functions to add normal map image to the object
void AIMesh::addNormalMap(GLuint _normalMapID)
{
	this->m_normalMapID = _normalMapID;
}

void AIMesh::addNormalMap(std::string _filename, FREE_IMAGE_FORMAT _format)
{
	m_normalMapID = loadTexture(_filename, _format);
}


// Rendering functions

void AIMesh::setupTextures()
{
	if (m_meshTexCoordBuffer != 0) {

		if (m_textureID != 0) {

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_textureID);

			//  *** normal mapping ***  check if normal map added - if so bind to texture unit 1
			if (m_normalMapID != 0) {

				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, m_normalMapID);

				// Restore default
				glActiveTexture(GL_TEXTURE0);
			}
		}
	}
}


void AIMesh::render()
{

	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, m_numFaces * 3, GL_UNSIGNED_INT, (const GLvoid*)0);
}

