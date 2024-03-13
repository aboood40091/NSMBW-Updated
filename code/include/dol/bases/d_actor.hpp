#pragma once
#include <dol/bases/d_base_actor.hpp>
#include <dol/collision/d_cc.hpp>
#include <dol/collision/d_bc.hpp>
#include <dol/collision/d_rc.hpp>

class dPropelParts_c;

class dActor_c : public dBaseActor_c {
public:
    dActor_c();

    virtual int preCreate();
    virtual void postCreate(fBase_c::MAIN_STATE_e status);

    virtual int preDelete();
    virtual void postDelete(fBase_c::MAIN_STATE_e status);

    virtual int preExecute();
    virtual void postExecute(fBase_c::MAIN_STATE_e status);

    virtual int preDraw();
    virtual void postDraw(fBase_c::MAIN_STATE_e status);

    virtual ~dActor_c();

    virtual char* getKindString() const;

    virtual bool ActorDrawCullCheck();

    virtual void kill(); // unofficial name
    virtual void vf68(); // unknown

    virtual s8* getPlrNo();
    virtual mVec2_c getLookatPos() const;
    virtual bool isSpinLiftUpEnable();

    virtual void setSpinLiftUpActor(dActor_c* carryingActor);
    virtual void setCarryFall(dActor_c* carryingActor, int unk);

    virtual void setEatTongue(dActor_c* eaterActor);
    virtual void setEatTongueOff(dActor_c* eaterActor);
    virtual void setEatMouth(dActor_c* eatenActor);
    virtual void setEatSpitOut(dActor_c* eatenActor);
    virtual bool setEatScore(dActor_c* eaterActor); // unofficial name
    virtual void eatMove(dActor_c* eaterActor);

    virtual void removeCc();
    virtual void reviveCc();
    virtual void restoreScale(); // unofficial name

    virtual void calcSpitOutPos(dActor_c* eaterActor);
    virtual float calcEatScaleRate(dActor_c* eaterActor);
    virtual void calcEatInScale(dActor_c* eaterActor);

    virtual void endOfLevelClear(); // unofficial name

    virtual void vfB4(); // unknown
    virtual void vfB8(); // unknown
    virtual void vfBC(); // unknown
    virtual void vfC0(); // unknown
    virtual void vfC4(); // unknown

    virtual void waterSplashEffect(const mVec3_c& pos, float scale);
    virtual void yoganSplashEffect(const mVec3_c& pos, float scale);
    virtual void poisonSplashEffect(const mVec3_c& pos, float scale);

public:
    bool checkZoneBoundaries(u32 flags); // unofficial name, &2 = does not delete offscreen actors, &4 = only checks zone boundaries &8 = checks ride output instead of bounds

    u64 getEventMask() const {
        return ((u64)(mEventMasks[0]) << 32) | mEventMasks[1];
    }
public:
    int mCarryPlayerNo; // -1 if not being carried
    u32 mCarryFlags; // &1 = thrown, &2 = being carried
    u8 mCarryDirection;
    // 3 bytes padding

    u32 mComboCount; // used by Yoshi Fire and Shells to keep track of combos, value 0-7

    u32 _138; // unused?
    u32 _13C; // set to 0 in constructor, never used again?
    float _140; // set to 1.0 in constructor, never used again?
    u32 _144; // unused?

    dCc_c mCc;
    dBc_c mBc;
    dRc_c mRc;

    mVec2_c _310; // related to zone borders maybe?
    mVec2_c mVisibleAreaSize; // doubled
    mVec2_c mVisibleAreaOffset;
    mVec2_c _328;
    mVec2_c mMaxBoundDistance; // from zone edge
    float mDestroyBoundDistanceTop;
    float mDestroyBoundDistanceBottom;
    float mDestroyBoundDistanceLeft;
    float mDestroyBoundDistanceRight;

    u8 mDirection;
    u8 mAreaNo;
    u8 _34A; // a bitfield related to collision
    // 1 byte padding

    u8* mpByteStorage; // links to dActorCreateMng_c
    u16* mpShortStorage; // links to dActorCreateMng_c
    u8 mEventNums[2];
    // 2 bytes padding
    u32 mEventMasks[2];

    u32 _360; // used creatively by various actors
    u16 mSpawnInfoFlags;
    bool mKillIfDisabled; // something with execStop
    // 1 byte padding

    u32 mEaterActorID; // unique id of actor who ate this one
    u8 mEatState; // 0 = normal, 2 = eaten, 4 = spat out
    u8 mEatSpitType; // 4 = fireball, 5 = iceball
    // 2 bytes padding
    mVec3_c mScaleBeforeBeingEaten;

    u32 mScoreType; // 0 = 200 points, 1 = 1000 points, 2 = no points
    u32 mLookAtMode;
    u32 _384; // a bitfield, only used by koopalings apparently

    dPropelParts_c* mpPropelParts;
    u8 mKind;
    s8 mPlayerNo;
    u8 mDisableMask;
    u8 mLayerID;
    bool mDeleteForever;
    bool mAppearsOnBackfence;
    u8 _392[2];

public:
    static bool mExecStopReq;
    static bool mDrawStopReq;
    static bool mExecStop;
    static bool mDrawStop;
};
static_assert(sizeof(dActor_c) == 0x394);
