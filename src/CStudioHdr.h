//
// Created by blake on 3/28/22.
//

#ifndef MDLLIB_CSTUDIOHDR_H
#define MDLLIB_CSTUDIOHDR_H
#include "source/studio.h"
#include <string>
#include <vector>
#include <unordered_map>

struct Bounding {
    Vector min;
    Vector max;
};

class CStudioHdr {
public:
    ~CStudioHdr(){
        if(mName != StringTable[0].c_str())
            delete[] mName;
    }

    bool LoadModel(const char *mdl);

    const char *mName;
    Vector      mEyes;
    Vector      mIllumPos;
    Bounding    mHull;
    Bounding    mViewBB;
    uint32_t    mFlags;


    std::vector<mstudiobone_t>          mBoneVec;
    std::vector<mstudiobonecontroller_t>mBoneControlVec;
    std::vector<mstudiohitboxset_t>     mHitboxSetVec;
    std::vector<mstudioanimdesc_t>      mAnimVec;
    std::vector<mstudioseqdesc_t>       mSeqVec;
    std::vector<mstudiotexture_t>       mMatVec;
    std::vector<mstudiotexturedir_t>    mMatDirVec;
    std::vector<mstudiobodyparts_t>     mBodyVec;

    std::vector<mstudiotexturedir_t> labelindexes;
    std::vector<mstudiotexturedir_t> activityindexes;


    /* TODO: Fully explore this. Check crowbar source code for it?
     * StringTable layout: NOTE: this is deduplicated, so it is not necessarily in this order
     * - Bone Names
     * - Attachment Names
     * -
     * - hitboxset names
     * - animation names
     * - sequence labels
     * -
     * - bodygroups
     *
     * - mMatDirVec
     * - ???
     * - ???
     * - ???
     */
    std::vector<std::string> StringTable;
private:

    template<typename T>
    void Deserialize(std::vector<T> *vector, size_t index, size_t count, FILE **fp);

    template<typename T>
    void UpdateIndex(std::vector<T> *vector, size_t startItem, size_t firstOffset, size_t stringDiff, size_t index);
    void UpdateIndexSeq(std::vector<mstudiotexturedir_t> *vector, size_t startItem, size_t firstOffset, size_t stringDiff, size_t index);

    void ReadStringTable(studiohdr_t *model, size_t lowest, FILE **fp);
};


#endif //MDLLIB_CSTUDIOHDR_H
