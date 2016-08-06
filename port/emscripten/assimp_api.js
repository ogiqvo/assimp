var assimp = {};
assimp.getVKeysize = (function(){
  var c_getVKeysize = Module.cwrap('ex_getVKeysize','void',[]);
  return function(){
    return c_getVKeysize();
  };
})();
assimp.getQKeysize = (function(){
  var c_getQKeysize = Module.cwrap('ex_getQKeysize','void',[]);
  return function(){
    return c_getQKeysize();
  };
})();
assimp.getV3Dsize = (function(){
  var c_getV3Dsize = Module.cwrap('ex_getV3Dsize','void',[]);
  return function(){
    return c_getV3Dsize();
  };
})();
assimp.getfloatsize = (function(){
  var c_getfloatsize = Module.cwrap('ex_getfloatsize','void',[]);
  return function(){
    return c_getfloatsize();
  };
})();
assimp.getintsize = (function(){
  var c_getintsize = Module.cwrap('ex_getintsize','void',[]);
  return function(){
    return c_getintsize();
  };
})();
assimp.getuintsize = (function(){
  var c_getuintsize = Module.cwrap('ex_getuintsize','void',[]);
  return function(){
    return c_getuintsize();
  };
})();
assimp.getdoublesize = (function(){
  var c_getdoublesize = Module.cwrap('ex_getdoublesize','void',[]);
  return function(){
    return c_getdoublesize();
  };
})();
assimp.getlongsize = (function(){
  var c_getlongsize = Module.cwrap('ex_getlongsize','void',[]);
  return function(){
    return c_getlongsize();
  };
})();
assimp.getErrorString = (function(){
  var c_getErrorString = Module.cwrap('ex_getErrorString','string',[]);
  return function(){
    return c_getErrorString();
  };
})();
assimp.aiImportFile = (function(){
  var c_aiImportFile = Module.cwrap('ex_aiImportFile','string',['string']);
  return function(filepath,bytes){
    try{
      Module.FS.createDataFile('/',filepath,bytes,true,true);
      console.log('assimp_api.js: Load',filepath,bytes.length,'byte(s)');
    }catch(e){
      console.error(e);
    }
    return c_aiImportFile(filepath);
  };
})();
