#ifndef _ECNODE_HH_
#define _ECNODE_HH_

#include "ECTask.hh"
#include "../inc/include.hh"
#include "../protocol/AGCommand.hh"

#define ECNODE_DEBUG false
using namespace std;

class ECNode {
  private: 
    int _nodeId;

    // _childNodes record all children of current node
    vector<ECNode*> _childNodes;
    // For each <key, value> pair in _coefMap:
    // key: target _nodeId that we want to compute
    // value: corresponding coefs to compute key
    unordered_map<int, vector<int>> _coefMap;

    // _refNo records the number of parents of current node
    unordered_map<int, int> _refNumFor;

    bool _hasConstraint;
    int _consId; 

    // oectasks type0:Load; type1:Fetch; type2: Compute; type3: Cache (enabled by default); type4: Persist
    unsigned int _ip;
    unordered_map<int, ECTask*> _oecTasks;

  public:
    ECNode(int id);
    ~ECNode();

    int getNodeId();

    void cleanChilds();
    void setChilds(vector<ECNode*> childs);
    int getChildNum();
    vector<ECNode*> getChildren();

    void addCoefs(int calfor, vector<int> coefs);
    unordered_map<int, vector<int>> getCoefmap();

    void incRefNumFor(int id); // increase refnum for id
    int getRefNumFor(int id);
    unordered_map<int, int> getRefMap();

    // parseForClient compute tasks
    void parseForClient(vector<ECTask*>& tasks);

    // parseForOEC tasks
    void parseForOEC(unsigned int ip);
    vector<unsigned int> candidateIps(unordered_map<int, unsigned int> sid2ip,
                           unordered_map<int, unsigned int> cid2ip,
                           vector<unsigned int> allIps,
                           int n, int k, int w, bool locality);
    unordered_map<int, ECTask*> getTasks();
    void clearTasks();
    unsigned int getIp();  
    AGCommand* parseAGCommand(string stripename,
                              int n, int k, int w,
                              int num,
                              unordered_map<int, pair<string, unsigned int>> stripeobjs,
                              unordered_map<int, unsigned int> cid2ip);

    // for debug
    void dump(int parent);
    void dumpRawTask();
};

#endif
