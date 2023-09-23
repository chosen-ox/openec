//
// Created by Vincent on 2023/9/10.
//

#ifndef _SHIFT_XOR_HH_
#define _SHIFT_XOR_HH_

#include "../inc/include.hh"
#include "Computation.hh"

#include "ECBase.hh"


class ShiftXor : public ECBase {
private:
    //int _encode_matrix[RSBINDX_N_MAX * RSBINDX_N_MAX];
    int _m;


public:
    ShiftXor(int n, int k, int w, int opt, vector <string> param);

    ECDAG *Encode();

    ECDAG *Decode(vector<int> from, vector<int> to);

    void Place(vector <vector<int>> &group);

};


#endif //_SHIFT_XOR_HH_

