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

  // aiImportFile
  
std::string escape_json(const std::string &s) {
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
    void appendResultJson(std::string& target){
      target.append("{'type':'AiBone'}");
    };    
  };

  /**
   *
   */
  enum AiPrimitiveType{
    AI_PRIMITIVE_TYPE_POINT=0x1,
    AI_PRIMITIVE_TYPE_LINE=0x2,
    AI_PRIMITIVE_TYPE_TRIANGLE=0x4,
    AI_PRIMITIVE_TYPE_POLYGON=0x8
  };

  /**
   *
   */
  class AiMesh{
  public:
    AiMesh(){}
    int getNumVertices(){return m_numVertices;}
    int getNumFaces(){return m_numFaces;}
    int getMaterialIndex(){return m_materialIndex;}
    std::string getName(){return m_name;}
    int getNumPositionBuffer(){return m_vertices.size();}
    float getPositionBufferAt(int i){return m_vertices.at(i);}
    int getNumFaceBuffer(){return m_faces.size();}
    int getFaceBufferAt(int i){return m_faces.at(i);}
    int getNumFaceOffsetsBuffer(){return m_faceOffsets.size();}
    int getFaceOffsetsBufferAt(int i){return m_faceOffsets.at(i);}
    int getNumNormalBuffer(){return m_normals.size();}
    int getNormalBufferAt(int i){return m_normals.at(i);}
    int getNumTangentBuffer(){return m_tangents.size();}
    int getTangentBufferAt(int i){return m_tangents.at(i);}
    int getNumBitangentBuffer(){return m_bitangents.size();}
    int getBitangentBufferAt(int i){return m_bitangents.at(i);}
    int getNumBones(){return m_bones.size();}
    AiBone* getBoneAt(int i){return m_bones.at(i);}
    void appendResultJson(std::string& target){
      target.append("{'type':'AiMesh','m_primitiveTypes':[");
      bool isFirst = true;
      for(AiPrimitiveType t : m_primitiveTypes){
	if(isFirst){isFirst=false;}else{target.append(",");}
	target.append(std::to_string(t));
      }
      target.append("],'m_numVertices':");
      target.append(std::to_string(m_numVertices));
      target.append(",'m_numFaces':");
      target.append(std::to_string(m_numFaces));
      target.append(",'m_materialIndex':");
      target.append(std::to_string(m_materialIndex));
      target.append(",'m_name':'");
      target.append(escape_json(m_name));
      target.append("','m_vertices':[");
      isFirst = true;
      for(float f : m_vertices){
	if(isFirst){isFirst=false;}else{target.append(",");}
	target.append(std::to_string(f));
      }
      target.append("],'m_faces':[");
      isFirst = true;
      for(int f : m_faces){
	if(isFirst){isFirst=false;}else{target.append(",");}
	target.append(std::to_string(f));
      }
      target.append("],'m_faceOffsets':[");
      isFirst = true;
      for(int f : m_faceOffsets){
	if(isFirst){isFirst=false;}else{target.append(",");}
	target.append(std::to_string(f));
      }
      target.append("],'m_normals':[");
      isFirst = true;
      for(float f : m_normals){
	if(isFirst){isFirst=false;}else{target.append(",");}
	target.append(std::to_string(f));
      }
      target.append("],'m_tangents':[");
      isFirst = true;
      for(float f : m_tangents){
	if(isFirst){isFirst=false;}else{target.append(",");}
	target.append(std::to_string(f));
      }
      target.append("],'m_bitangents':[");
      isFirst = true;
      for(float f : m_bitangents){
	if(isFirst){isFirst=false;}else{target.append(",");}
	target.append(std::to_string(f));
      }
      target.append("],'m_bones':[");
      isFirst = true;
      for(AiBone *b : m_bones){
	if(isFirst){isFirst=false;}else{target.append(",");}
	b->appendResultJson(target);
      }
      target.append("]}");
    }
    
  private:
    std::set<AiPrimitiveType> m_primitiveTypes;
    int m_numVertices;
    int m_numFaces;
    int m_materialIndex;
    std::string m_name;
    std::vector<float> m_vertices;
    std::vector<int> m_faces;
    std::vector<int> m_faceOffsets;
    std::vector<float> m_normals;
    std::vector<float> m_tangents;
    std::vector<float> m_bitangents;
    std::vector<AiBone*> m_bones;
  };

  /**
   *
   */
  class AiScene{
  public:
    AiScene(){}
    int getNumMeshes(){
      return m_meshes.size();
    }
    AiMesh* getMeshAt(int i){return m_meshes.at(i);}
    void addMesh(AiMesh* p){m_meshes.push_back(p);}
    void appendResultJson(std::string& target){
      target.append("{'type':'AiScene','meshes':[");
      for(AiMesh* pMesh : m_meshes){
	pMesh->appendResultJson(target);
      }
      target.append("]}");
    }
  private:
    std::vector<AiMesh*> m_meshes;
  };

  /**
   *
   */
  static bool loadMeshes(const aiScene* cScene,AiScene* jScene){
    for (unsigned int meshNr = 0; meshNr < cScene->mNumMeshes; meshNr++){
      const aiMesh *cMesh = cScene->mMeshes[meshNr];
      std::cout << "converting mesh [" << cMesh->mName.C_Str() << "]" << std::endl;
      
      AiMesh *pMesh = new AiMesh();
      jScene->addMesh(pMesh);
    }
    return true;
  }
  
  const char* ex_aiImportFile(const char *cFilename){
    std::cout << "read file " << cFilename << std::endl;

    std::ifstream infile(cFilename);
    if(!infile.good()){
      std::cout << "file" << cFilename << "isnt good" << std::endl;
      return NULL;
    }

    AiScene jScene;
    const aiScene *cScene = aiImportFile(cFilename, (unsigned int) 0);
    if (!loadMeshes(cScene,&jScene)){goto error;}
    goto end;
  error:
    std::cout << "problem detected" << std::endl;
  end:
    aiReleaseImport(cScene);
    
    std::string resultJson;
    jScene.appendResultJson(resultJson);
    return resultJson.c_str();
  }
}
