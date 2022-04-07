#include "MDLLib.h"

CStudioHdr *MdlCache::LoadModel(const char *mdl) {
    if (!cache.count(mdl))
        cache[mdl].LoadModel(mdl);

    return &cache[mdl];
}