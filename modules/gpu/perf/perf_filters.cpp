#include "perf_precomp.hpp"

using namespace std;
using namespace testing;

namespace {

//////////////////////////////////////////////////////////////////////
// Blur

DEF_PARAM_TEST(Sz_Type_KernelSz, cv::Size, MatType, int);

PERF_TEST_P(Sz_Type_KernelSz, Filters_Blur, Combine(GPU_TYPICAL_MAT_SIZES, Values(CV_8UC1, CV_8UC4), Values(3, 5, 7)))
{
    cv::Size size = GET_PARAM(0);
    int type = GET_PARAM(1);
    int ksize = GET_PARAM(2);

    cv::Mat src(size, type);
    fillRandom(src);

    cv::gpu::GpuMat d_src(src);
    cv::gpu::GpuMat d_dst;

    cv::gpu::blur(d_src, d_dst, cv::Size(ksize, ksize));

    TEST_CYCLE()
    {
        cv::gpu::blur(d_src, d_dst, cv::Size(ksize, ksize));
    }
}

//////////////////////////////////////////////////////////////////////
// Sobel

PERF_TEST_P(Sz_Type_KernelSz, Filters_Sobel, Combine(GPU_TYPICAL_MAT_SIZES, Values(CV_8UC1, CV_8UC4, CV_32FC1), Values(3, 5, 7, 9, 11, 13, 15)))
{
    cv::Size size = GET_PARAM(0);
    int type = GET_PARAM(1);
    int ksize = GET_PARAM(2);

    cv::Mat src(size, type);
    fillRandom(src);

    cv::gpu::GpuMat d_src(src);
    cv::gpu::GpuMat d_dst;
    cv::gpu::GpuMat d_buf;

    cv::gpu::Sobel(d_src, d_dst, -1, 1, 1, d_buf, ksize);

    TEST_CYCLE()
    {
        cv::gpu::Sobel(d_src, d_dst, -1, 1, 1, d_buf, ksize);
    }
}

//////////////////////////////////////////////////////////////////////
// Scharr

PERF_TEST_P(Sz_Type, Filters_Scharr, Combine(GPU_TYPICAL_MAT_SIZES, Values(CV_8UC1, CV_8UC4, CV_32FC1)))
{
    cv::Size size = GET_PARAM(0);
    int type = GET_PARAM(1);

    cv::Mat src(size, type);
    fillRandom(src);

    cv::gpu::GpuMat d_src(src);
    cv::gpu::GpuMat d_dst;
    cv::gpu::GpuMat d_buf;

    cv::gpu::Scharr(d_src, d_dst, -1, 1, 0, d_buf);

    TEST_CYCLE()
    {
        cv::gpu::Scharr(d_src, d_dst, -1, 1, 0, d_buf);
    }
}

//////////////////////////////////////////////////////////////////////
// GaussianBlur

PERF_TEST_P(Sz_Type_KernelSz, Filters_GaussianBlur, Combine(GPU_TYPICAL_MAT_SIZES, Values(CV_8UC1, CV_8UC4, CV_32FC1), Values(3, 5, 7, 9, 11, 13, 15)))
{
    cv::Size size = GET_PARAM(0);
    int type = GET_PARAM(1);
    int ksize = GET_PARAM(2);

    cv::Mat src(size, type);
    fillRandom(src);

    cv::gpu::GpuMat d_src(src);
    cv::gpu::GpuMat d_dst;
    cv::gpu::GpuMat d_buf;

    cv::gpu::GaussianBlur(d_src, d_dst, cv::Size(ksize, ksize), d_buf, 0.5);

    TEST_CYCLE()
    {
        cv::gpu::GaussianBlur(d_src, d_dst, cv::Size(ksize, ksize), d_buf, 0.5);
    }
}

//////////////////////////////////////////////////////////////////////
// Laplacian

PERF_TEST_P(Sz_Type_KernelSz, Filters_Laplacian, Combine(GPU_TYPICAL_MAT_SIZES, Values(CV_8UC1, CV_8UC4, CV_32FC1, CV_32FC4), Values(1, 3)))
{
    cv::Size size = GET_PARAM(0);
    int type = GET_PARAM(1);
    int ksize = GET_PARAM(2);

    cv::Mat src(size, type);
    fillRandom(src);

    cv::gpu::GpuMat d_src(src);
    cv::gpu::GpuMat d_dst;

    cv::gpu::Laplacian(d_src, d_dst, -1, ksize);

    TEST_CYCLE()
    {
        cv::gpu::Laplacian(d_src, d_dst, -1, ksize);
    }
}

//////////////////////////////////////////////////////////////////////
// Erode

PERF_TEST_P(Sz_Type, Filters_Erode, Combine(GPU_TYPICAL_MAT_SIZES, Values(CV_8UC1, CV_8UC4)))
{
    cv::Size size = GET_PARAM(0);
    int type = GET_PARAM(1);

    cv::Mat src(size, type);
    fillRandom(src);

    cv::Mat ker = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));

    cv::gpu::GpuMat d_src(src);
    cv::gpu::GpuMat d_dst;
    cv::gpu::GpuMat d_buf;

    cv::gpu::erode(d_src, d_dst, ker, d_buf);

    TEST_CYCLE()
    {
        cv::gpu::erode(d_src, d_dst, ker, d_buf);
    }
}

//////////////////////////////////////////////////////////////////////
// Dilate

PERF_TEST_P(Sz_Type, Filters_Dilate, Combine(GPU_TYPICAL_MAT_SIZES, Values(CV_8UC1, CV_8UC4)))
{
    cv::Size size = GET_PARAM(0);
    int type = GET_PARAM(1);

    cv::Mat src(size, type);
    fillRandom(src);

    cv::Mat ker = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));

    cv::gpu::GpuMat d_src(src);
    cv::gpu::GpuMat d_dst;
    cv::gpu::GpuMat d_buf;

    cv::gpu::dilate(d_src, d_dst, ker, d_buf);

    TEST_CYCLE()
    {
        cv::gpu::dilate(d_src, d_dst, ker, d_buf);
    }
}

//////////////////////////////////////////////////////////////////////
// MorphologyEx

CV_ENUM(MorphOp, cv::MORPH_OPEN, cv::MORPH_CLOSE, cv::MORPH_GRADIENT, cv::MORPH_TOPHAT, cv::MORPH_BLACKHAT)
#define ALL_MORPH_OPS ValuesIn(MorphOp::all())

DEF_PARAM_TEST(Sz_Type_Op, cv::Size, MatType, MorphOp);

PERF_TEST_P(Sz_Type_Op, Filters_MorphologyEx, Combine(GPU_TYPICAL_MAT_SIZES, Values(CV_8UC1, CV_8UC4), ALL_MORPH_OPS))
{
    cv::Size size = GET_PARAM(0);
    int type = GET_PARAM(1);
    int morphOp = GET_PARAM(2);

    cv::Mat src(size, type);
    fillRandom(src);

    cv::Mat ker = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));

    cv::gpu::GpuMat d_src(src);
    cv::gpu::GpuMat d_dst;
    cv::gpu::GpuMat d_buf1;
    cv::gpu::GpuMat d_buf2;

    cv::gpu::morphologyEx(d_src, d_dst, morphOp, ker, d_buf1, d_buf2);

    TEST_CYCLE()
    {
        cv::gpu::morphologyEx(d_src, d_dst, morphOp, ker, d_buf1, d_buf2);
    }
}

//////////////////////////////////////////////////////////////////////
// Filter2D

PERF_TEST_P(Sz_Type_KernelSz, Filters_Filter2D, Combine(GPU_TYPICAL_MAT_SIZES, Values(CV_8UC1, CV_8UC4, CV_32FC1, CV_32FC4), Values(3, 5, 7, 9, 11, 13, 15)))
{
    cv::Size size = GET_PARAM(0);
    int type = GET_PARAM(1);
    int ksize = GET_PARAM(2);

    cv::Mat src(size, type);
    fillRandom(src);

    cv::Mat kernel(ksize, ksize, CV_32FC1);
    fillRandom(kernel, 0.0, 1.0);

    cv::gpu::GpuMat d_src(src);
    cv::gpu::GpuMat d_dst;

    cv::gpu::filter2D(d_src, d_dst, -1, kernel);

    TEST_CYCLE()
    {
        cv::gpu::filter2D(d_src, d_dst, -1, kernel);
    }
}

} // namespace
