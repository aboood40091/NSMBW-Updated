#pragma once
#include <types.h>
#include <dol/bases/d_actor.hpp>

class dAcPyKey_c {
public:
    bool buttonWalk(int* p_dir) const;
    bool buttonDush() const;
    bool buttonJump();

public:
    u32 _0[0x164 / sizeof(u32)];
};
static_assert(sizeof(dAcPyKey_c) == 0x164);

struct PlayerSpeedDataInner {
    float _0;
    float _4;
    float _8;
    float _c;
    float _10;
    float _14;
    float _18;
    float _1c;
    float _20;
};
static_assert(sizeof(PlayerSpeedDataInner) == 0x24);

struct PlayerSpeedData {
    float _0;
    float _4;
    float _8;
    PlayerSpeedDataInner _c;
    PlayerSpeedDataInner _30;
    PlayerSpeedDataInner _54;
};
static_assert(sizeof(PlayerSpeedData) == 0x78);

class daPlBase_c : public dActor_c {
public:
    bool isSaka();
    float getSakaMoveAccele(u8 dir);
    bool isStatus(int status_bit);
    void icePowerChange(int);
    const PlayerSpeedData* getSpeedData();
    void getSpeedDataInner(PlayerSpeedDataInner*);
    float getSakaStopAccele(u8 dir);
    int getPowerChangeType(bool);
    s16 getMukiAngle(u8 dir);
    void offStatus(int status_bit);
    void startSound(int soundId, int);

public:
    u32 _394[(0xEA4 - 0x394) / sizeof(u32)];
    dAcPyKey_c mAcPyKey;
    u32 _1008[(0x1060 - 0x1008) / sizeof(u32)];
    PlayerSpeedData* mpSpeedDataNormal;
    PlayerSpeedData* mpSpeedDataStar;
    u32 _1068[(0x10D4 - 0x1068) / sizeof(u32)];
    u32 _10d4;
    u32 _10d8[(0x14A4 - 0x10D8) / sizeof(u32)];
    u32 mAction;
    u32 _14a8[(0x14D4 - 0x14A8) / sizeof(u32)];

public:
    static const float sc_DirSpeed[2];
};
static_assert(sizeof(daPlBase_c) == 0x14D4);
