#pragma once
#include <types.h>
#include <dol/bases/d_a_player_base.hpp>
#include <dol/mLib/m3d/m3d.hpp>

class dPyMdlBase_c {
public:
    dPyMdlBase_c(u8);
    virtual ~dPyMdlBase_c();

    virtual int getFaceJointIdx() const;
    virtual void createModel();
    virtual void initialize();
    virtual void play();
    virtual void _calc();
    virtual void calc2();
    virtual void draw();
    virtual void* getBodyMdl();
    virtual const void* getAnmResFile() const;
    virtual void setPlayerMode(int);
    virtual void setColorType(u8);
    virtual void setDark(int);
    virtual void vf3C();
    virtual void onStarAnm();
    virtual void offStarAnm();
    virtual void onStarEffect();
    virtual void offStarEffect();
    virtual void getJointMtx(mMtx_c*, int);
    virtual bool getHeadPropelJointMtx(mMtx_c*);
    virtual bool vf58(int, char*, int);
    virtual void setAnm(int anmID, float rate, float blendDuration, float frame);
    // ...

public:
    bool isAnmStop() const
    {
        return mAnmChr.isStop();
    }

    float getAnmFrameMax() const
    {
        return mAnmChr.mNumFrames;
    }

    float checkAnmFrame(float frame) const
    {
        return mAnmChr.checkFrame(frame);
    }

public:
    u32 _4[(0x28 - 4) / sizeof(u32)];
    m3d::anmChr_c mAnmChr;
    u32 _60[(0x20C - 0x60) / sizeof(u32)];
};
static_assert(sizeof(dPyMdlBase_c) == 0x20c);

class dPyAnm_HIO_c {
public:
    u8 mID;
    float mRate;
    float mBlendDuration;
};
static_assert(sizeof(dPyAnm_HIO_c) == 0xC);

class dPyAnmMain_HIO_c {
public:
    dPyAnm_HIO_c mPyAnm_HIO[177];
};
static_assert(sizeof(dPyAnmMain_HIO_c) == 0x84C);

class dPyMdlBase_HIO_c {
public:
    u32 _0[0x28 / sizeof(u32)];
    dPyAnmMain_HIO_c mPyAnmMain_HIO;
    u32 _874[(0x950 - 0x874) / sizeof(u32)];
};
static_assert(sizeof(dPyMdlBase_HIO_c) == 0x950);

class dPyMdlMng_c {
public:
    static float getAnmRate(int anmID) {
        return m_hio.mPyAnmMain_HIO.mPyAnm_HIO[anmID].mRate;
    }
    
    static float getAnmBlendDuration(int anmID) {
        return m_hio.mPyAnmMain_HIO.mPyAnm_HIO[anmID].mBlendDuration;
    }

public:
    enum ModelType_e {
    };

public:
    dPyMdlMng_c(ModelType_e);
    virtual ~dPyMdlMng_c();

public:
    void setAnm(int anmID, float rate, float blendDuration, float frame) {
        mpPyMdlBase->setAnm(
            anmID,
            rate,
            blendDuration,
            frame
        );
    }
    
    void setAnm(int anmID, float blendDuration, float frame) {
        mpPyMdlBase->setAnm(
            anmID,
            getAnmRate(anmID),
            blendDuration,
            frame
        );
    }
    
    void setAnm(int anmID, float frame) {
        mpPyMdlBase->setAnm(
            anmID,
            getAnmRate(anmID),
            getAnmBlendDuration(anmID),
            frame
        );
    }

public:
    dPyMdlBase_c* mpPyMdlBase;
    u32 _8;

public:
    static dPyMdlBase_HIO_c m_hio;
};
static_assert(sizeof(dPyMdlMng_c) == 0xC);

class dAcPy_c : public daPlBase_c {
public:
    void setWaterWalkFlag();
    void jumpExecAir();

    // Actually a virtual function of dPlBase_c, but we can guarantee that
    // instances of dAcPy_c will definitely call this one specifically
    int isStar() const;

public:
    u32 _14d4[(0x1564 - 0x14D4) / sizeof(u32)];
    int jumpSoundRelated;
    u32 _1568[(0x27CC - 0x1568) / sizeof(u32)];
    u8 _27cc; // Some sort of direction
    u32 _27d0[(0x2A60 - 0x27D0) / sizeof(u32)];
    dPyMdlMng_c mPyMdlMng;
    u32 _2a6c[(0x2BA8 - 0x2A6C) / sizeof(u32)];
};
static_assert(sizeof(dAcPy_c) == 0x2BA8);
