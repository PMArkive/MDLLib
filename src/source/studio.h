#ifndef MDLLIB_STUDIO_H
#define MDLLIB_STUDIO_H

#include <cstdint>
#include "datatypes.h"

#define STUDIO_VERSION		49

/*
 * Layout:
 * Header
 * Header2
 * mBoneVec
 * Animations
 * Sequences
 * Textures
 * QC
 * KV
 * BoneTransforms
 * BoneFlexDrivers
 * BodyGroupPresets
 * StringTable
 */

#define IDSTUDIOHEADER			(('T'<<24)+('S'<<16)+('D'<<8)+'I')
struct studiohdr_t
{
	uint32_t			id;
	uint32_t			version;
	uint32_t		    checksum;		// this has to be the same in the phy and vtx files to load!
	
	char				name[64];

	uint32_t			length;

	Vector				eyeposition;	// ideal eye position

	Vector				illumposition;	// illumination center
	
	Vector				hull_min;		// ideal movement hull size
	Vector				hull_max;			

	Vector				view_bbmin;		// clipping bounding box
	Vector				view_bbmax;		

	uint32_t			flags;

	uint32_t			bone_count;			// bones
	uint32_t			bone_index;

	uint32_t			bone_controller_count;		// bone controllers
	uint32_t			bone_controller_index;

	uint32_t			hitboxset_count;
	uint32_t			hitboxset_index;

	uint32_t			anim_count;			// animations/poses
	uint32_t			anim_index;		// animation descriptions

	uint32_t			sequence_count;				// sequences
	uint32_t			sequence_index;

	uint32_t	        activitylistversion;	// initialization flag - have the sequences been indexed?
	uint32_t        	eventsindexed;

	uint32_t			material_count;
	uint32_t			material_index;

	// raw textures search paths
	uint32_t			material_dir_count;
	uint32_t			material_dir_index;

	// replaceable textures tables
	uint32_t			numskinref;
	uint32_t			numskinfamilies;
	uint32_t			skinindex;

	uint32_t			bodypart_count;
	uint32_t			bodypart_index;

	// queryable attachable points

	uint32_t			numlocalattachments;
	uint32_t			localattachmentindex;

	// animation node to animation node transition graph
	uint32_t			numlocalnodes;
	uint32_t			localnodeindex;
	uint32_t			localnodenameindex;

	uint32_t			numflexdesc;
	uint32_t			flexdescindex;

	uint32_t			numflexcontrollers;
	uint32_t			flexcontrollerindex;

	uint32_t			numflexrules;
	uint32_t			flexruleindex;

	uint32_t			numikchains;
	uint32_t			ikchainindex;

	uint32_t			nummouths;
	uint32_t			mouthindex;


	uint32_t			numlocalposeparameters;
	uint32_t			localposeparamindex;

	uint32_t			surfacepropindex;

	uint32_t			keyvalueindex;
	uint32_t			keyvaluesize;

	uint32_t			numlocalikautoplaylocks;
	uint32_t			localikautoplaylockindex;

	float				mass;
	uint32_t			contents;

	// external animations, models, etc.
	uint32_t			numincludemodels;
	uint32_t			includemodelindex;

    // Relocated to studiohdr2_t
	uint32_t			unused_virtualModel;

	uint32_t			szanimblocknameindex;
	uint32_t			numanimblocks;
	uint32_t			animblockindex;

    // Relocated to studiohdr2_t
	uint32_t			unused_animblockModel;

	uint32_t			bonetablebynameindex;

	// Relocated to studiohdr2_t
	int                 unused_pVertexBase;
	int                 unused_pIndexBase;

	uint8_t				constdirectionallightdot;

    uint8_t				rootLOD;

    uint8_t				numAllowedRootLODs;

    uint8_t				unused[1];

	uint32_t			unused4; // zero out if version < 47

	uint32_t			numflexcontrollerui;
	uint32_t			flexcontrolleruiindex;

	float				flVertAnimFixedPointScale;

	uint32_t	        surfacepropLookup;	// this index must be cached by the loader, not saved in the file

	uint32_t			studiohdr2index;

	uint32_t			unused2[1];
};

struct studiohdr2_t
{
    uint32_t srcbone_transform_count;
    uint32_t srcbone_transform_index;

    uint32_t illumpos_attach_index;

    float    flMaxEyeDeflection;

    uint32_t linearboneindex;

    uint32_t sznameindex;

    uint32_t m_nBoneFlexDriverCount;
    uint32_t m_nBoneFlexDriverIndex;

    uint32_t m_pFeModel; // this is functionally the same as having an index and a function, but more readable.

    uint32_t m_nBodyGroupPresetCount;
    uint32_t m_nBodyGroupPresetIndex;

    uint32_t padding_unused; // This hasn't been used before, use it if you need it.

    void *virtualModel;
    void *animblockModel;

    void *pVertexBase;
    void *pIndexBase;

    uint32_t reserved[44];
};

struct mstudiobone_t
{
    uint32_t			sznameindex;
    int32_t	    	 	parent;		// parent bone
    int32_t			    bonecontroller[6];	// bone controller index, -1 == none

    // default values
    Vector				pos;
    Quaternion			quat;
    Vector			    rot;
    // compression scale
    Vector				posscale;
    Vector				rotscale;

    matrix3x4_t			poseToBone;
    Quaternion			qAlignment;
    uint32_t			flags;
    uint32_t			proctype;
    uint32_t			procindex;		// procedural rule
    mutable int			physicsbone;	// index into physically simulated bone
    int					surfacepropidx;	// index into string table for property name

    int					contents;		// See BSPFlags.h for the contents flags
    int					surfacepropLookup;	// this index must be cached by the loader, not saved in the file
    int					unused[7];		// remove as appropriate
};

struct mstudiobonecontroller_t
{
    int					bone;	// -1 == 0
    int					type;	// X, Y, Z, XR, YR, ZR, M
    float				start;
    float				end;
    int					rest;	// byte index value at rest
    int					inputfield;	// 0-3 user set controller, 4 mouth
    int					unused[8];
};

struct mstudiohitboxset_t
{
    int					sznameindex;
    int					numhitboxes;
    int					hitboxindex;
};

struct mstudioanimdesc_t
{
    int					baseptr;

    int					sznameindex;
    inline char * const pszName( void ) const { return ((char *)this) + sznameindex; }

    float				fps;		// frames per second
    int					flags;		// looping/non-looping flags

    int					numframes;

    // piecewise movement
    int					nummovements;
    int					movementindex;

    int					ikrulezeroframeindex;

    int					unused1[5];			// remove as appropriate (and zero if loading older versions)

    int					animblock;
    int					animindex;	 // non-zero when anim data isn't in sections

    int					numikrules;
    int					ikruleindex;	// non-zero when IK rule is stored in the mdl
    int					animblockikruleindex; // non-zero when IK data is stored in animblock file

    int					numlocalhierarchy;
    int					localhierarchyindex;

    int					sectionindex;
    int					sectionframes; // number of frames used in each fast lookup section, zero if not used

    short				zeroframespan;	// frames per span
    short				zeroframecount; // number of spans
    int					zeroframeindex;
    mutable float		zeroframestalltime;		// saved during read stalls
};

struct mstudioseqdesc_t
{
    int					baseptr;

    int					szlabelindex;

    int					szactivitynameindex;

    int					flags;		// looping/non-looping flags

    int					activity;	// initialized at loadtime to game DLL values
    int					actweight;

    int					numevents;
    int					eventindex;

    Vector				bbmin;		// per sequence bounding box
    Vector				bbmax;

    int					numblends;

    // Index into array of shorts which is groupsize[0] x groupsize[1] in length
    int					animindexindex;

    int					movementindex;	// [blend] float array for blended movement
    int					groupsize[2];
    int					paramindex[2];	// X, Y, Z, XR, YR, ZR
    float				paramstart[2];	// local (0..1) starting value
    float				paramend[2];	// local (0..1) ending value
    int					paramparent;

    float				fadeintime;		// ideal cross fate in time (0.2 default)
    float				fadeouttime;	// ideal cross fade out time (0.2 default)

    int					localentrynode;		// transition node at entry
    int					localexitnode;		// transition node at exit
    int					nodeflags;		// transition rules

    float				entryphase;		// used to match entry gait
    float				exitphase;		// used to match exit gait

    float				lastframe;		// frame that should generation EndOfSequence

    int					nextseq;		// auto advancing sequences
    int					pose;			// index of delta animation between end and nextseq

    int					numikrules;

    int					numautolayers;	//
    int					autolayerindex;

    int					weightlistindex;

    // FIXME: make this 2D instead of 2x1D arrays
    int					posekeyindex;

    int					numiklocks;
    int					iklockindex;

    // Key values
    int					keyvalueindex;
    int					keyvaluesize;

    int					cycleposeindex;		// index of pose parameter to use as cycle index

    int					activitymodifierindex;
    int					numactivitymodifiers;

    int					animtagindex;
    int					numanimtags;

    int					rootDriverIndex;

    int					unused[2];		// remove/add as appropriate (grow back to 8 ints on version change!)
};

struct mstudiotexturedir_t {
    int sznameindex;
};

struct mstudiotexture_t
{
    int						sznameindex;
    int						flags;
    int						used;
    //padding + unused studiomdl garbage pointers
    int						unused[13];

};

struct mstudiobodyparts_t
{
    int					sznameindex;
    int					nummodels;
    int					base;
    int					modelindex; // index into models array
};

struct mstudiomodel_t
{
    char				name[64];

    int					type;

    float				boundingradius;

    int					nummeshes;
    int					meshindex;

    int					numvertices;		// number of unique vertices/normals/texcoords
    int					vertexindex;		// vertex Vector
    int					tangentsindex;		// tangents Vector

    int					numattachments;
    int					attachmentindex;

    int					numeyeballs;
    int					eyeballindex;

    int					unused[10];
};

#endif // MDLLIB_STUDIO_H
