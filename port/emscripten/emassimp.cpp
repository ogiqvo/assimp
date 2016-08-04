#include <assimp/cimport.h>
#include <assimp/scene.h>

extern "C"{

typedef struct AiScene{
} AiScene;

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

AiScene* ex_aiImportFile(char *filename,long postProcessing){
  return NULL;
}

}
