#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <emscripten/bind.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>


const std::string escape_json(const std::string &s) {
  std::ostringstream o;
  for (auto c = s.cbegin(); c != s.cend(); c++) {
    if (*c == '"' || *c == '\\' || ('\x00' <= *c && *c <= '\x1f')) {
      o << "\\u"
	<< std::hex << std::setw(4) << std::setfill('0') << (int)*c;
    } else {
      o << *c;
    }
  }
  return o.str();
}
  
/**
 *
 */
class AiBone{
public:
  void appendResultJson(std::stringstream& ss){
    ss << "{\"type\":\"AiBone\"}";
  };    
};

/**
 *
 */
class AiMesh{
public:
  AiMesh(){}
  void setPrimitiveTypes(int types){m_primitiveTypesRawValue=types;}
  void setNumVertices(int i){m_numVertices = i;}
  void setNumFaces(int i){m_numFaces=i;}
  void setMaterialIndex(int idx){m_materialIndex=idx;}
  void setIsPureTriangle(bool b){m_isPureTriangle=b;}
  void setFaceBufferSize(int i){m_faceBufferSize=i;}
  void setName(std::string str){m_name = str;}
  void setVerticesB64(std::string str){m_verticesB64=str;}
  void setFacesB64(std::string str){m_facesB64=str;}
  void setFaceOffsetsB64(std::string str){m_faceOffsetsB64=str;}
  void setNormalsB64(std::string str){m_normalsB64=str;}
  void setTangentsB64(std::string str){m_tangentsB64=str;}
  void setBitangentsB64(std::string str){m_bitangentsB64=str;}
    
  void appendResultJson(std::stringstream& ss){
    ss << "{\"type\":\"AiMesh\",\"m_primitiveTypes\":" << m_primitiveTypesRawValue <<
      ",\"m_numVertices\":" << m_numVertices <<
      ",\"m_numFaces\":" << m_numFaces <<
      ",\"m_materialIndex\":" << m_materialIndex <<
      ",\"m_isPureTriangle\":" << m_isPureTriangle <<
      ",\"m_faceBufferSize\":" << m_faceBufferSize <<
      ",\"m_name\":\"" << escape_json(m_name) <<
      "\",\"m_verticesB64\":\"" << m_verticesB64 <<
      "\",\"m_facesB64\":\"" << m_facesB64 <<
      "\",\"m_faceOffsetsB64\":\"" << m_faceOffsetsB64 <<
      "\",\"m_normalsB64\":\"" << m_normalsB64 <<
      "\",\"m_tangentsB64\":\"" << m_tangentsB64 <<
      "\",\"m_bitangentsB64\":\"" << m_bitangentsB64 <<
      "\",\"m_bones\":[";
    bool isFirst = true;
    for(AiBone* bone : m_bones){
      if(isFirst){isFirst=false;}else{ss << ",";}
      bone->appendResultJson(ss);
    }
    ss << "]}";
  }
   
private:
  int m_primitiveTypesRawValue;
  int m_numVertices;
  int m_numFaces;
  int m_materialIndex;
  bool m_isPureTriangle;
  int m_faceBufferSize;
  std::string m_name;
  std::string m_verticesB64;
  std::string m_facesB64;
  std::string m_faceOffsetsB64;
  std::string m_normalsB64;
  std::string m_tangentsB64;
  std::string m_bitangentsB64;
  std::vector<AiBone*> m_bones;
};

/**
 *
 */
class AiScene{
public:
  AiScene(){}
  void addMesh(AiMesh* p){m_meshes.push_back(p);}
  void appendResultJson(std::stringstream& ss){
    ss << "{\"type\":\"AiScene\",\"meshes\":[";
    for(AiMesh* pMesh : m_meshes){
      pMesh->appendResultJson(ss);
    }
    ss << "]}";
  }
private:
  std::vector<AiMesh*> m_meshes;
};

// http://yano.hatenadiary.jp/entry/20100908/1283945820
bool encode_base64(const std::vector<unsigned char>& src, std::string& dst){
  const std::string table("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/");
  std::string cdst;

  for (std::size_t i = 0; i < src.size(); ++i) {
    switch (i % 3) {
    case 0:
      cdst.push_back(table[(src[i] & 0xFC) >> 2]);
      if (i + 1 == src.size()) {
	cdst.push_back(table[(src[i] & 0x03) << 4]);
	cdst.push_back('=');
	cdst.push_back('=');
      }

      break;
    case 1:
      cdst.push_back(table[((src[i - 1] & 0x03) << 4) | ((src[i + 0] & 0xF0) >> 4)]);
      if (i + 1 == src.size()) {
	cdst.push_back(table[(src[i] & 0x0F) << 2]);
	cdst.push_back('=');
      }

      break;
    case 2:
      cdst.push_back(table[((src[i - 1] & 0x0F) << 2) | ((src[i + 0] & 0xC0) >> 6)]);
      cdst.push_back(table[src[i] & 0x3F]);

      break;
    }
  }

  dst.swap(cdst);

  return true;
}

static std::string generateBufferB64str(void* cData, size_t size){
  std::vector<unsigned char> buffer;
  unsigned char* cDataC = reinterpret_cast<unsigned char*>(cData);
  for(int i = 0;i < size;i++){
    unsigned char d = reinterpret_cast<unsigned char>(*(cDataC+i));
    buffer.push_back(d);
  }
  std::string b64str;
  encode_base64(buffer,b64str);
  return b64str;
}

/**
 *
 */
static bool loadMeshes(const aiScene* cScene,AiScene* jScene){
  for (unsigned int meshNr = 0; meshNr < cScene->mNumMeshes; meshNr++){
    const aiMesh *cMesh = cScene->mMeshes[meshNr];
    std::cout << "converting mesh [" << cMesh->mName.C_Str() << "]" << std::endl;
      
    AiMesh *pMesh = new AiMesh();
    jScene->addMesh(pMesh);

    int primTypes = cMesh->mPrimitiveTypes;
    pMesh->setPrimitiveTypes(primTypes);
    pMesh->setMaterialIndex(cMesh->mMaterialIndex);
    pMesh->setName(cMesh->mName.C_Str());
    bool isPureTriangle = cMesh->mPrimitiveTypes == aiPrimitiveType_TRIANGLE;
    size_t faceBufferSize;
    if(isPureTriangle){
      faceBufferSize = cMesh->mNumFaces*3*sizeof(unsigned int);
    }else{
      int numVertexReferences = 0;
      for (unsigned int face = 0; face < cMesh->mNumFaces; face++){
	numVertexReferences += cMesh->mFaces[face].mNumIndices;
      }
      faceBufferSize = numVertexReferences * sizeof(unsigned int);
    }

    pMesh->setNumVertices(cMesh->mNumVertices);
    pMesh->setNumFaces(cMesh->mNumFaces);
    pMesh->setIsPureTriangle(isPureTriangle);
    pMesh->setFaceBufferSize(faceBufferSize);
    
    if(cMesh->mNumVertices > 0){
      pMesh->setVerticesB64(generateBufferB64str(cMesh->mVertices, cMesh->mNumVertices * sizeof(aiVector3D)));
    }

    if(cMesh->mNumFaces > 0){
      if (isPureTriangle) {
	char* faceBuffer = (char*) malloc(faceBufferSize);

	size_t faceDataSize = 3 * sizeof(unsigned int);
	for (unsigned int face = 0; face < cMesh->mNumFaces; face++){
	  memcpy(faceBuffer + face * faceDataSize, cMesh->mFaces[face].mIndices, faceDataSize);
	}

	pMesh->setFacesB64(generateBufferB64str(faceBuffer, faceBufferSize));
	free(faceBuffer);
      }else{
	char* faceBuffer = (char*) malloc(faceBufferSize);
	char* offsetBuffer = (char*) malloc(cMesh->mNumFaces * sizeof(unsigned int));
	  
	size_t faceBufferPos = 0;
	for (unsigned int face = 0; face < cMesh->mNumFaces; face++){
	  size_t faceBufferOffset = faceBufferPos / sizeof(unsigned int);
	  memcpy(offsetBuffer + face * sizeof(unsigned int), &faceBufferOffset, sizeof(unsigned int));
	    
	  size_t faceDataSize = cMesh->mFaces[face].mNumIndices * sizeof(unsigned int);
	  memcpy(faceBuffer + faceBufferPos, cMesh->mFaces[face].mIndices, faceDataSize);
	  faceBufferPos += faceDataSize;
	}
	  
	pMesh->setFacesB64(generateBufferB64str(faceBuffer, faceBufferSize));
	pMesh->setFaceOffsetsB64(generateBufferB64str(offsetBuffer, cMesh->mNumFaces * sizeof(unsigned int)));
	  
	free(faceBuffer);
	free(offsetBuffer);
      }
    }

    if(cMesh->HasNormals()){
      pMesh->setNormalsB64(generateBufferB64str(cMesh->mNormals, cMesh->mNumVertices * 3 * sizeof(float)));
    }
      
    if(cMesh->mTangents != NULL){
      pMesh->setTangentsB64(generateBufferB64str(cMesh->mTangents, cMesh->mNumVertices * 3 * sizeof(float)));
    }
	
    if(cMesh->mBitangents != NULL){
      pMesh->setBitangentsB64(generateBufferB64str(cMesh->mBitangents, cMesh->mNumVertices * 3 * sizeof(float)));
    }

    for(int c = 0;c < AI_MAX_NUMBER_OF_COLOR_SETS;c++){
      // TODO
    }

    for(int c = 0;c < AI_MAX_NUMBER_OF_TEXTURECOORDS;c++){
      // TODO
    }

    for(unsigned int b = 0;b < cMesh->mNumBones;b++){
      aiBone *cBone = cMesh->mBones[b];
    }
  }
  return true;
}
  
extern "C"{
  int ex_getVKeysize(){
    const int res = sizeof(aiVectorKey);
    return res;
  }

  int ex_getQKeysize(){
    const int res = sizeof(aiQuatKey);
    return res;
  }

  int ex_getV3Dsize(){
    const int res = sizeof(aiVector3D);
    return res;
  }

  int ex_getfloatsize(){
    const int res = sizeof(float);
    return res;
  }

  int ex_getintsize(){
    const int res = sizeof(int);
    return res;
  }

  int ex_getuintsize(){
    const int res = sizeof(unsigned int);
    return res;
  }

  int ex_getdoublesize(){
    const int res = sizeof(double);
    return res;
  }

  int ex_getlongsize(){
    const int res = sizeof(long);
    return res;
  }

  const char* ex_getErrorString(){
    const char *err = aiGetErrorString();
    return err;
  }

  const char* ex_aiImportFile(const char *cFilename,long postProcess){
    std::cout << "read file " << cFilename << std::endl;

    std::ifstream infile(cFilename);
    if(!infile.good()){
      std::cout << "file" << cFilename << "isnt good" << std::endl;
      return NULL;
    }

    AiScene jScene;
    const aiScene *cScene = aiImportFile(cFilename, (unsigned int) postProcess);
    if (!loadMeshes(cScene,&jScene)){goto error;}
    goto end;
  error:
    std::cout << "problem detected" << std::endl;
  end:
    aiReleaseImport(cScene);
    
    std::stringstream ss;
    jScene.appendResultJson(ss);
    ss.clear(std::stringstream::goodbit);
    std::string str = ss.str();
    const char* cstr = str.c_str();
    int cstrlen = strlen(cstr);
    char* cstr_cpy = (char*)malloc(cstrlen);
    strcpy(cstr_cpy,cstr);
    std::cout << "result " << cstr_cpy << std::endl;
    return cstr_cpy;
  }
}
