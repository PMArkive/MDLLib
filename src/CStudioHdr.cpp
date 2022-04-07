//Handles all interactions with the data

#include <cstdio>
#include <map>
#include <cstring>
#include "CStudioHdr.h"

bool CStudioHdr::LoadModel(const char *mdl) {
    //Read MDL data into buffer
    FILE *fp = fopen(mdl, "rb");
    if(!fp) {
        fprintf(stderr, "Failed to open model file %s\n", mdl);
        return false;
    }

    auto *model = new studiohdr_t;
    if (fread(model, sizeof(studiohdr_t), 1, fp) != 1){
        fprintf(stderr, "MDL file %s is invalid, truncated header\n", mdl);
        return false;
    }

    //Ensure correct file type
    if(model->id != IDSTUDIOHEADER) {
        fprintf(stderr, "MDL file %s is invalid, wrong header id\n", mdl);
        return false;
    }

    auto *modelFULL = malloc(model->length);
    fseek(fp, 0, SEEK_SET);
    fread(modelFULL, model->length, 1, fp);

    auto *hdr2 = new studiohdr2_t;
    fseek(fp, model->studiohdr2index, SEEK_SET);
    fread(hdr2, sizeof(studiohdr2_t), 1, fp);

    //Easy data from header
    mEyes = model->eyeposition;
    mIllumPos = model->illumposition;
    mHull.min = model->hull_min; mHull.max = model->hull_max;
    mViewBB.min = model->view_bbmin; mViewBB.max = model->view_bbmax;
    mFlags = model->flags;

    //Data arrays
    Deserialize(&mBoneVec, model->bone_index, model->bone_count, &fp);
    Deserialize(&mBoneControlVec, model->bone_controller_index, model->bone_controller_count, &fp);
    Deserialize(&mHitboxSetVec, model->hitboxset_index, model->hitboxset_count, &fp);
    Deserialize(&mAnimVec, model->anim_index, model->anim_count, &fp);
    Deserialize(&mSeqVec, model->sequence_index, model->sequence_count, &fp);
    Deserialize(&mMatVec, model->material_index, model->material_count, &fp);
    Deserialize(&mMatDirVec, model->material_dir_index, model->material_dir_count, &fp);
    Deserialize(&mBodyVec, model->bodypart_index, model->bodypart_count, &fp);


    labelindexes.resize(mSeqVec.size());
    activityindexes.resize(mSeqVec.size());

    for(auto i = 0; i < mSeqVec.size(); i++) {
        labelindexes[i].sznameindex = mSeqVec[i].szlabelindex;
        activityindexes[i].sznameindex = mSeqVec[i].szactivitynameindex;
    }

    ReadStringTable(model, model->studiohdr2index + hdr2->sznameindex, &fp);

    if(model->studiohdr2index && hdr2->sznameindex)
        mName = StringTable[0].c_str();
    else
        mName = strdup(model->name);

    auto test = sizeof(mstudioseqdesc_t);

    delete hdr2;
    delete model;
    fclose(fp);
    return true;
}

template<typename T>
void CStudioHdr::Deserialize(std::vector<T> *vector, size_t index, size_t count, FILE **fp) {
    vector->resize(count);
    fseek(*fp, index, SEEK_SET);
    fread(vector->data(), sizeof(T), count, *fp);
}

template<typename T>
void CStudioHdr::UpdateIndex(std::vector<T> *vector, size_t startItem, size_t firstOffset, size_t stringDiff, size_t index) {
    static auto doneCount = 0;
    if (doneCount < vector->size()) {
        for (auto item = 0; item < vector->size(); item++) {
            size_t itemDiff = startItem + (sizeof(T) * item) + (vector->data() + item)->sznameindex - firstOffset;
            if ( itemDiff == stringDiff) {
                (vector->data() + item)->sznameindex = index;
                doneCount++;
            }
        }
    }
}

void CStudioHdr::UpdateIndexSeq(std::vector<mstudiotexturedir_t> *vector, size_t startItem, size_t firstOffset, size_t stringDiff, size_t index) {
        for (auto item = 0; item < vector->size(); item++) {
            size_t itemDiff = startItem + (sizeof(mstudioseqdesc_t) * item) + (vector->data() + item)->sznameindex - firstOffset;
            if ( itemDiff == stringDiff) {
                (vector->data() + item)->sznameindex = index;
            }
        }
}

void CStudioHdr::ReadStringTable(studiohdr_t *model, size_t lowest, FILE **fp) {
    //TODO: make sure bone 0 is always the first name in the model
    lowest = lowest - 1; //There is an empty string (null byte) before the string table for things without names
    fseek(*fp, lowest, SEEK_SET);              //Seek to first string
    size_t stringsSize = model->length - lowest; //Get total length of string table
    char *firstString = new char[stringsSize];
    char *strings = firstString;               //Split off working pointer
    char *lastString = strings + stringsSize;  //Stop loop here

    //Read all the strings into the buffer
    fread(strings, 1, stringsSize, *fp);

    auto index = 0;
    do {
        //Reassign name index on all parts to the index of the string table
        auto stringDiff = strings - firstString;
        UpdateIndex(&mBoneVec, model->bone_index, lowest, stringDiff, index);
        UpdateIndex(&mHitboxSetVec, model->hitboxset_index, lowest, stringDiff, index);
        UpdateIndex(&mAnimVec, model->anim_index, lowest, stringDiff, index);
        UpdateIndexSeq(&labelindexes, model->sequence_index, lowest, stringDiff, index);
        UpdateIndexSeq(&activityindexes, model->sequence_index, lowest, stringDiff, index);
        UpdateIndex(&mMatVec, model->material_index, lowest, stringDiff, index);
        UpdateIndex(&mMatDirVec, 0, lowest, stringDiff, index); //Special case, these offsets are relative to the start
        UpdateIndex(&mBodyVec, model->bodypart_index, lowest, stringDiff, index);
        index++;


        //Split off string from table into vector
        StringTable.emplace_back(std::string(strings));
        strings += StringTable.back().size()+1;
    } while (strings != lastString);
    delete[] firstString;
}