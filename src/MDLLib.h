#ifndef MDLLIB_H
#define MDLLIB_H
#include <vector>
#include <map>
#include "CStudioHdr.h"

class MdlCache {
    std::map<const char *, CStudioHdr> cache;
public:
    CStudioHdr *LoadModel(const char *);
};

#endif //MDLLIB_H
