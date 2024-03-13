#pragma once
#include <lib/egg/heap/eggFrmHeap.hpp>
#include <lib/nw4r/g3d/anmobj.h>
#include <dol/mLib/m_allocator.h>
/// @file

/// @ingroup mlib
namespace m3d {

    void resetMaterial(); ///< Turns off all indirect texture processing.

    struct AnmType {
        enum Value {
            AnmChr,
            AnmVis,
            AnmMatClr,
            AnmTexPat,
            AnmTexSrt,
            AnmScn,
        };
    };

    class banm_c {
    public:
        virtual ~banm_c();
        virtual AnmType::Value getType() const = 0;
        virtual void remove();
        virtual void play();

    public:
        nw4r::g3d::AnmObj* mpAnmObj;
        EGG::FrmHeap* mpHeap;
        mAllocator_c mAllocator;
    };

    // Values unknown, these are just guesses
    enum playMode_e {
        Play = 0,
        Paused = 1,
        PlayReverse = 2,
        Unk4 = 4,
    };

    class fanm_c : public banm_c {
    public:
        virtual ~fanm_c();
        virtual AnmType::Value getType() const = 0;
        virtual void play();

        bool checkFrame(float) const;
        bool isStop() const;

    public:
        float mNumFrames;
        float mStartFrame;
        float mCurrentFrame;
        u8 mPlayMode;
        // 3 bytes padding
    };

    class anmChr_c : public fanm_c {
    public:
        virtual ~anmChr_c();
        virtual AnmType::Value getType() const;
    };

}
