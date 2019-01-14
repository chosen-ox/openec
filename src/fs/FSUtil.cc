#include "FSUtil.hh"

UnderFS* FSUtil::createFS(string type, vector<string> param, Config* conf) { 
  UnderFS* toret;
  if (type == "hadoop3") {
    cout << "hadoop3" << endl;
    #ifdef HDFS3
    toret = new Hadoop3(param, conf);
    #endif
  } else if (type == "hadoop20") {
    cout << "hadoop20" << endl;
    #ifdef HDFSRAID
    toret = new Hadoop20(param, conf);
    #endif
  } else if (type == "qfs") {
    cout << "qfs" << endl;
    #ifdef QFS
    toret = new QFS(param, conf);
    #endif
  } else {
    cout << "unrecognized FS type!" << endl;
    toret = NULL;
  }

  return toret;
}

void FSUtil::deleteFS(string type, UnderFS* fshandler) {
  if (type == "hadoop3") {
    cout << "hadoop3" << endl;
    #ifdef HDFS3
    delete (Hadoop3*)fshandler;
    #endif
  } else if (type == "hadoop20") {
    cout << "hadoop20" << endl;
    #ifdef HDFSRAID
    delete (Hadoop20*)fshandler;
    #endif
  } else if (type == "qfs") {
    cout << "qfs" << endl;
    #ifdef QFS
    delete (QFS*)fshandler;
    #endif
  }
}
