#include "MDLLib.h"
#include <chrono>
#include <iostream>

int main() {
    MdlCache *g_MdlCache = new MdlCache;
    auto start = std::chrono::high_resolution_clock::now();
    CStudioHdr *testMdl = g_MdlCache->LoadModel("/home/blake/Code/SourceVK/2b_v1.mdl");
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end-start);
    std::cout << ((testMdl != NULL)? "Loaded model: " : "Failed to load model: ") << testMdl->mName
        << " in " << duration.count() << " microseconds" << std::endl;
    delete g_MdlCache;
}

