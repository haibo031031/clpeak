#include <clpeak.h>


int clPeak::runComputeInteger(cl::CommandQueue &queue, cl::Program &prog, device_info_t &devInfo)
{
    float timed, gflops;
    cl_uint workPerWI;
    cl::NDRange globalSize, localSize;
    cl_int A = 4;
    int iters = devInfo.computeIters;

    if(!isComputeInt)
        return 0;

    try
    {
        cl::Context ctx = queue.getInfo<CL_QUEUE_CONTEXT>();

        uint globalWIs = (devInfo.numCUs) * (devInfo.computeWgsPerCU) * (devInfo.maxWGSize);
        uint t = MIN((globalWIs * sizeof(cl_int)), devInfo.maxAllocSize);
        t = roundToPowOf2(t);
        globalWIs = t / sizeof(cl_int);

        cl::Buffer outputBuf = cl::Buffer(ctx, CL_MEM_WRITE_ONLY, (globalWIs * sizeof(cl_int)));

        globalSize = globalWIs;
        localSize = devInfo.maxWGSize;

        cl::Kernel kernel_v1(prog, "compute_integer_v1");
        kernel_v1.setArg(0, outputBuf), kernel_v1.setArg(1, A);

        cl::Kernel kernel_v2(prog, "compute_integer_v2");
        kernel_v2.setArg(0, outputBuf), kernel_v2.setArg(1, A);

        cl::Kernel kernel_v4(prog, "compute_integer_v4");
        kernel_v4.setArg(0, outputBuf), kernel_v4.setArg(1, A);

        cl::Kernel kernel_v8(prog, "compute_integer_v8");
        kernel_v8.setArg(0, outputBuf), kernel_v8.setArg(1, A);

        cl::Kernel kernel_v16(prog, "compute_integer_v16");
        kernel_v16.setArg(0, outputBuf), kernel_v16.setArg(1, A);

        cout << NEWLINE TAB TAB "Integer compute (GIOPS)" << endl;
        cout << setprecision(2) << fixed;

        ///////////////////////////////////////////////////////////////////////////
        // Vector width 1
        cout << TAB TAB TAB "int   : ";   cout.flush();

        workPerWI = 4096;      // Indicates integer operations executed per work-item

        timed = run_kernel(queue, kernel_v1, globalSize, localSize, iters);

        gflops = ((float)globalWIs * workPerWI) / timed / 1e3f;
        cout << gflops << endl;
        ///////////////////////////////////////////////////////////////////////////

        // Vector width 2
        cout << TAB TAB TAB "int2  : ";   cout.flush();

        workPerWI = 4096;

        timed = run_kernel(queue, kernel_v2, globalSize, localSize, iters);

        gflops = ((float)globalWIs * workPerWI) / timed / 1e3f;
        cout << gflops << endl;
        ///////////////////////////////////////////////////////////////////////////

        // Vector width 4
        cout << TAB TAB TAB "int4  : ";   cout.flush();

        workPerWI = 4096;

        timed = run_kernel(queue, kernel_v4, globalSize, localSize, iters);

        gflops = ((float)globalWIs * workPerWI) / timed / 1e3f;
        cout << gflops << endl;
        ///////////////////////////////////////////////////////////////////////////

        // Vector width 8
        cout << TAB TAB TAB "int8  : ";   cout.flush();

        workPerWI = 4096;

        timed = run_kernel(queue, kernel_v8, globalSize, localSize, iters);

        gflops = ((float)globalWIs * workPerWI) / timed / 1e3f;
        cout << gflops << endl;
        ///////////////////////////////////////////////////////////////////////////

        // Vector width 16
        cout << TAB TAB TAB "int16 : ";   cout.flush();

        workPerWI = 4096;

        timed = run_kernel(queue, kernel_v16, globalSize, localSize, iters);

        gflops = ((float)globalWIs * workPerWI) / timed / 1e3f;
        cout << gflops << endl;
        ///////////////////////////////////////////////////////////////////////////
    }
    catch(cl::Error error)
    {
        cerr << error.what() << "(" << error.err() << ")" << endl;
        cerr << TAB TAB TAB "Tests skipped" << endl;
        return -1;
    }

    return 0;
}

