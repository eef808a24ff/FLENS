#include <flens/lapack/interface/include/config.h>

namespace flens { namespace lapack {

extern "C" {

//-- zungqr --------------------------------------------------------------------
void
LAPACK_DECL(zungqr)(const INTEGER            *M,
                    const INTEGER            *N,
                    const INTEGER            *K,
                    DOUBLE_COMPLEX           *A,
                    const INTEGER            *LDA,
                    const DOUBLE_COMPLEX     *TAU,
                    DOUBLE_COMPLEX           *WORK,
                    const INTEGER            *LWORK,
                    INTEGER                  *INFO)
{
    using std::max;
    using std::min;
//
//  Test the input parameters so that we pass LAPACK error checks
//
    const bool lQuery = (*LWORK==-1);

    *INFO = 0;
    if (*M<0) {
        *INFO = -1;
    } else if (*N<0 || *N>*M) {
        *INFO = -2;
    } else if (*K<0 || *K>*N) {
        *INFO = -3;
    } else if (*LDA<max(INTEGER(1), *M)) {
        *INFO = -5;
    } else if (*LWORK<max(INTEGER(1), *N) && !lQuery) {
        *INFO = -8;
    }
    if (*INFO!=0) {
        *INFO = -(*INFO);
        LAPACK_ERROR("ZUNGQR", INFO);
        *INFO = -(*INFO);
        return;
    }
//
//  Handle worksize query
//
    if (lQuery) {
        // TODO: implement wsq
        ASSERT(0);
    }
//
//  Call FLENS implementation
//
    auto zA         = reinterpret_cast<CXX_DOUBLE_COMPLEX *>(A);
    const auto zTAU = reinterpret_cast<const CXX_DOUBLE_COMPLEX *>(TAU);
    auto zWORK      = reinterpret_cast<CXX_DOUBLE_COMPLEX *>(WORK);

    ZGeMatrixView          _A      = ZFSView(*M, *N, zA, *LDA);
    ZConstDenseVectorView  _TAU    = ZConstArrayView(*K, zTAU, 1);
    ZDenseVectorView       _WORK   = ZArrayView(*LWORK, zWORK, 1);

    ungqr(_A, _TAU, _WORK);
}

} // extern "C"

} } // namespace lapack, flens
