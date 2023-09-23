#include "ShiftXor.hh"

ShiftXor::ShiftXor(int n, int k, int w, int opt, vector<string> param) {
    _n = n;
    _k = k;
    _w = w;
    _opt = opt;

    _m = _n - _k;
    //memset(_encode_matrix, 0, (_k+_m) * _k * sizeof(int));
    //if(ShiftXor_DEBUG_ENABLE) cout << "ShiftXor::constructor ends" << endl;
}

ECDAG* ShiftXor::Encode() {
    ECDAG* ecdag = new ECDAG();
//    vector<int> data;
//    vector<int> code;
//    for (int i=0; i<_k; i++) data.push_back(i);
//    for (int i=_k; i<_n; i++) code.push_back(i);
//    if (ShiftXor_DEBUG_ENABLE) {
//        cout << "ShiftXor::Encode.data:";
//        for (int i=0; i<data.size(); i++) cout << " " << data[i];
//        cout << endl;
//        cout << "ShiftXor::Encode.code:";
//        for (int i=0; i<code.size(); i++) cout << " " << code[i];
//        cout << endl;
//    }
//
//    generate_matrix(_encode_matrix, _n, _k, 8);
//    for (int i=0; i<_m; i++) {
//        vector<int> coef;
//        for (int j=0; j<_k; j++) {
//            coef.push_back(_encode_matrix[(i+_k)*_k+j]);
//        }
//        ecdag->Join(code[i], data, coef);
//    }
    return ecdag;
}

ECDAG* ShiftXor::Decode(vector<int> from, vector<int> to) {
    
    ECDAG* ecdag = new ECDAG();
    return ecdag;
}

void ShiftXor::Place(vector<vector<int>>& group) {
}

