#include <types.h>
#include <dol/mLib/m_vec.hpp>

// Forward declaration
class dActor_c;
class dBc_c;
class dRide_ctr_c;

class dRc_c {
public:
    void* vtable;

    dActor_c* mpOwner;
    mVec3_c* mpOwnerPos;

    dRc_c* mpNext;

    dRide_ctr_c* mpRide_ctr[2];
    dBc_c* mpBc;

    float _1C;
    float _20;
    float _24;
    float _28;

    s16 mRotation;
    u8 mChainlinkMode;
    u8 mRideType; // 0 = dRideRoll_c, 2 = dRide2Point_c, 4 = dRideCircle_c
    u8 mFlags; // &1 = ??, &2 = ??, &4 = set on player when riding Lakitu Clouds/Clown Cars
    u8 _31; // bool maybe?
    u8 mLayer;
    // 1 byte padding
};
