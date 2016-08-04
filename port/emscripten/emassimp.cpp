#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <iostream>
#include <fstream>

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

const aiScene* ex_aiImportFile(const char *cFilename,int postProcess){
  std::cout << "read file" << cFilename << std::endl;

  std::ifstream infile(cFilename);
  if(!infile.good()){
    std::cout << "file" << cFilename << "isnt good" << std::endl;
    return NULL;
  }
  
  const aiScene *cScene = aiImportFile(cFilename, (unsigned int) postProcess);
  return cScene;
}
}
