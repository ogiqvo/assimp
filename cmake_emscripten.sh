#!/usr/bin/env sh
# Copy me to PROJECT_ROOT_DIR/build

# ls -1 | grep -v 'cmake_em' | xargs rm -fr
# rm -fr ../CMakeCache.txt
# rm -fr ../CMakeFiles
# emcmake cmake -DCMAKE_TOOLCHAIN_FILE= /usr/share/emscripten/cmake/Modules/Platform/Emscripten.cmake \
# 	-DCMAKE_CXX_FLAGS="-DASSIMP_BUILD_NO_3DS_IMPORTER  -DASSIMP_BUILD_NO_X_IMPORTER  -DASSIMP_BUILD_NO_MD3_IMPORTER  -DASSIMP_BUILD_NO_MDL_IMPORTER  -DASSIMP_BUILD_NO_MDC_IMPORTER  -DASSIMP_BUILD_NO_MD2_IMPORTER  -DASSIMP_BUILD_NO_MD5_IMPORTER  -DASSIMP_BUILD_NO_PLY_IMPORTER  -DASSIMP_BUILD_NO_C4D_IMPORTER  -DASSIMP_BUILD_NO_ASE_IMPORTER  -DASSIMP_BUILD_NO_HMP_IMPORTER  -DASSIMP_BUILD_NO_SMD_IMPORTER  -DASSIMP_BUILD_NO_STL_IMPORTER  -DASSIMP_BUILD_NO_LWO_IMPORTER  -DASSIMP_BUILD_NO_DXF_IMPORTER  -DASSIMP_BUILD_NO_NFF_IMPORTER  -DASSIMP_BUILD_NO_RAW_IMPORTER  -DASSIMP_BUILD_NO_SIB_IMPORTER  -DASSIMP_BUILD_NO_OFF_IMPORTER  -DASSIMP_BUILD_NO_AC_IMPORTER  -DASSIMP_BUILD_NO_BVH_IMPORTER  -DASSIMP_BUILD_NO_IRRMESH_IMPORTER  -DASSIMP_BUILD_NO_IRR_IMPORTER  -DASSIMP_BUILD_NO_Q3D_IMPORTER  -DASSIMP_BUILD_NO_B3D_IMPORTER  -DASSIMP_BUILD_NO_TERRAGEN_IMPORTER  -DASSIMP_BUILD_NO_CSM_IMPORTER  -DASSIMP_BUILD_NO_3D_IMPORTER  -DASSIMP_BUILD_NO_LWS_IMPORTER  -DASSIMP_BUILD_NO_OGRE_IMPORTER  -DASSIMP_BUILD_NO_OPENGEX_IMPORTER  -DASSIMP_BUILD_NO_MS3D_IMPORTER  -DASSIMP_BUILD_NO_COB_IMPORTER  -DASSIMP_BUILD_NO_BLEND_IMPORTER  -DASSIMP_BUILD_NO_Q3BSP_IMPORTER  -DASSIMP_BUILD_NO_NDO_IMPORTER  -DASSIMP_BUILD_NO_IFC_IMPORTER  -DASSIMP_BUILD_NO_XGL_IMPORTER  -DASSIMP_BUILD_NO_ASSBIN_IMPORTER  -DASSIMP_BUILD_NO_GLTF_IMPORTER  -DASSIMP_BUILD_NO_3MF_IMPORTER  -DASSIMP_BUILD_NO_COLLADA_EXPORTER  -DASSIMP_BUILD_NO_XFILE_EXPORTER  -DASSIMP_BUILD_NO_STEP_EXPORTER  -DASSIMP_BUILD_NO_OBJ_EXPORTER  -DASSIMP_BUILD_NO_STL_EXPORTER  -DASSIMP_BUILD_NO_PLY_EXPORTER  -DASSIMP_BUILD_NO_GLTF_EXPORTER  -DASSIMP_BUILD_NO_ASSBIN_EXPORTER  -DASSIMP_BUILD_NO_ASSXML_EXPORTER  -DASSIMP_BUILD_NO_3DS_EXPORTER  -std=c++11" \
# 	-DCMAKE_BUILD_TYPE=Debug \
# 	-DASSIMP_BUILD_TESTS=OFF \
# 	..
# emmake make
# rm -fr *.js *.html *.mem
em++ -o _assimp.js \
     -O2 \
     --memory-init-file 0 \
     -I../include \
     -s EXPORTED_FUNCTIONS="['_ex_getVKeysize','_ex_getQKeysize','_ex_getV3Dsize','_ex_getfloatsize','_ex_getintsize','_ex_getuintsize','_ex_getdoublesize','_ex_getlongsize','_ex_getErrorString','_ex_aiImportFile','FS']" \
     code/CMakeFiles/assimp.dir/*.o \
     code/CMakeFiles/assimp.dir/__/contrib/ConvertUTF/*.o \
     code/CMakeFiles/assimp.dir/__/contrib/clipper/*.o \
     code/CMakeFiles/assimp.dir/__/contrib/irrXML/*.o \
     code/CMakeFiles/assimp.dir/__/contrib/unzip/*.o \
     tools/assimp_cmd/CMakeFiles/assimp_cmd.dir/CompareDump.cpp.o \
     tools/assimp_cmd/CMakeFiles/assimp_cmd.dir/Export.cpp.o \
     tools/assimp_cmd/CMakeFiles/assimp_cmd.dir/ImageExtractor.cpp.o \
     tools/assimp_cmd/CMakeFiles/assimp_cmd.dir/Info.cpp.o \
     tools/assimp_cmd/CMakeFiles/assimp_cmd.dir/WriteDumb.cpp.o \
     ../port/emscripten/emassimp.cpp

cat ../port/emscripten/pre.js \
    _assimp.js \
    ../port/emscripten/assimp_api.js \
    ../port/emscripten/post.js \
    > assimp.js

cp -f assimp.js ../port/emscripten/test/assimp.js
