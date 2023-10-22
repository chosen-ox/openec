#include "Computation.hh"
#include <immintrin.h>
#include <emmintrin.h>


mutex Computation::_cLock;

int Computation::singleMulti(int a, int b, int w) {
    Computation::_cLock.lock();
    int res = galois_single_multiply(a, b, w);
    Computation::_cLock.unlock();
    return res;
}

#define CHUNK_SIZE 131072
#define SLICE 32
#define TOTAL_SLICE_NUM (CHUNK_SIZE / SLICE)

void Computation::Multi(char **dst, char **src, int *mat, int rowCnt, int colCnt, int len, string lib) {
	cout << 0 << endl;
    if (lib == "Jerasure") {
	    cout << -1 << endl;
        Computation::_cLock.lock();
        jerasure_matrix_encode(colCnt, rowCnt, GF_W, mat, src, dst, len);
        Computation::_cLock.unlock();
    } else if (lib == "Shift") {
        // HID matrix:
        // |  1    &  z^1  &  z^2  &  z^3  &  z^4  &  z^5  &  z^6  &  z^7  |
        // |  1    &  1    &  1    &  1    &  1    &  1    &  1    &  1    |
        // |  z^7  &  z^6  &  z^5  &  z^4  &  z^3  &  z^2  &  z^1  &  1    |

        int d = rowCnt / 2;
        uint64_t slice_offset_d;    
        uint64_t chunk_slice_id;
        uint64_t slice_id;
        __m256i mxx1, mxx2;
        
        const uint64_t chunk_size = 128;//窗口宽度为128个cell
        const uint64_t chunk_num = TOTAL_SLICE_NUM / chunk_size;//一个数据包的窗口数量
        for (chunk_slice_id = 0; chunk_slice_id < chunk_num; chunk_slice_id++) {
		cout << "id" << chunk_slice_id << endl;
		cout << 1<< endl;
            const uint64_t slice_id_start = chunk_slice_id * chunk_size;//当前窗口的起始cell序号
            const uint64_t slice_id_end = chunk_slice_id * chunk_size + chunk_size;//下一个窗口的开始cell序号
            //按x1的计算位置先把x1的当前窗口部分放到3个parity_packet中
            for (slice_id = slice_id_start; slice_id < slice_id_end; slice_id++) {

                slice_offset_d = slice_id * SLICE; // point to the start point of memory block (packet $slice_id)


		cout << 2<< endl;
                mxx1 = _mm256_loadu_si256((__m256i * ) & src[0][slice_offset_d]);//取出x1的一个cell，大小为32字节

                for (unsigned int i = 0; i < d; i++) {
                    _mm256_storeu_si256((__m256i * ) & dst[i][slice_offset_d + (d - i) * (colCnt - 1) * SLICE],
                                        mxx1);
                    _mm256_storeu_si256((__m256i * ) & dst[i + d][slice_offset_d], mxx1);
                }
		cout << 3 << endl;
                if ((rowCnt % 2) == 1)
		cout << 4 << endl;
                    _mm256_storeu_si256((__m256i * ) & dst[d][slice_offset_d], mxx1);

            }
		cout << 5<< endl;

            //按x2的计算位置把x2的当前窗口部分与3个parity_packet进行xor并更新parity_packet
            for (unsigned int i = 1; i < colCnt; i++) {
                for (slice_id = slice_id_start; slice_id < slice_id_end; slice_id++) {
                    slice_offset_d =
                            slice_id * SLICE; // point to the start point of memory block (packet $slice_id)

                    // load memory block of file into register
                    mxx1 = _mm256_loadu_si256((__m256i * ) & src[i][slice_offset_d]);//取出x2的一个cell，大小为32字节

                    for (unsigned int j = 0; i < d; j++) {
                        // xor and store in packet 1
                        int offset1 = slice_offset_d + (d - j) * (colCnt - 1 - i) * SLICE;
                        mxx2 = _mm256_xor_si256(mxx1, *((__m256i * ) & dst[i][offset1]));
                        _mm256_storeu_si256((__m256i * ) & dst[i][offset1], mxx2);


                        // xor and store in packet 3
                        int offset2 = slice_offset_d + (d - j) * i * SLICE;
                        mxx2 = _mm256_xor_si256(mxx1, *((__m256i * ) & dst[i + d][offset2]));
                        _mm256_storeu_si256((__m256i * ) & dst[i + d][offset2], mxx2);
                    }


                    if ((rowCnt % 2) == 1)

                        mxx2 = _mm256_xor_si256(mxx1, *((__m256i * ) & dst[d][slice_offset_d]));
                        _mm256_storeu_si256((__m256i * ) & dst[d][slice_offset_d], mxx2);
                }
            }
        }


    } else {
        // first transfer the mat into char*
        char *imatrix;
        imatrix = (char *) calloc(rowCnt * colCnt, sizeof(char));
        for (int i = 0; i < rowCnt * colCnt; i++) {
            char tmpc = mat[i];
            imatrix[i] = tmpc;
        }
        // call isa-l library
        unsigned char itable[32 * rowCnt * colCnt];
        ec_init_tables(colCnt, rowCnt, (unsigned char *) imatrix, itable);
        ec_encode_data(len, colCnt, rowCnt, itable, (unsigned char **) src, (unsigned char **) dst);
    }
}




