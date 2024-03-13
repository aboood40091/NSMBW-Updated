#include <dol/bases/d_a_player.hpp>
#include <lib/MSL_C/math.h>

#include <base/hooks.h>

extern "C" {

const float luigi__x_speed_inc = 0.07f;
const float luigi__swim_y_speed_add = 1.8f;

extern const float FLOAT_ARRAY_802eedc0[2];

void daPlBase_c__slipPowerSet_1(daPlBase_c *_this)
{
    if (_this->isSaka())
    {
        _this->mAccelF = _this->getSakaMoveAccele((_this->mSpeedF < 0.0) ? 1 : 0);
        int walk_dir;
        if (_this->mAcPyKey.buttonWalk(&walk_dir))
            _this->mAccelF *= FLOAT_ARRAY_802eedc0[_this->mBc.getSakaUpDown(walk_dir)];
    }
    else
    {
        if (_this->isStatus(0x31))
            _this->mAccelF = 0.05;
        else
            _this->mAccelF = 0.09;
    }

    _this->icePowerChange(1);
}

void daPlBase_c__slipPowerSet_0(daPlBase_c *_this)
{
    if (fabsf(_this->mSpeedF) > _this->getSpeedData()->_8)
    {
        _this->mAccelF = 0.75;
        return;
    }

    {
        // HACK: Patching addresses 0x802ef118 & 0x802ef208 using Kamek is not enough
        // as Kamek is not able to perform patches before the ctors of the original game run
        _this->mpSpeedDataNormal->_c._c = 0.04f;
    }

    PlayerSpeedDataInner local_4c;
    _this->getSpeedDataInner(&local_4c);

    int walk_dir;
    if (_this->mAcPyKey.buttonWalk(&walk_dir))
    {
        if (_this->mSpeedF * daPlBase_c::sc_DirSpeed[_this->mDirection] < 0.0)
        {
            _this->mAccelF = local_4c._c;
            if (_this->isStatus(0x89))
                _this->mAccelF *= 3;

            return;
        }

        if (_this->isSaka())
        {
            _this->mAccelF = _this->getSakaMoveAccele((_this->mSpeedF < 0.0) ? 1 : 0) * FLOAT_ARRAY_802eedc0[_this->mBc.getSakaUpDown(walk_dir)];
            _this->icePowerChange(0);
            return;
        }

        const float x_speed_1_mag = fabsf(_this->mSpeedF);
        const float x_speed_2_mag = fabsf(_this->mMaxSpeedF);

        if (x_speed_1_mag < 0.5)
        {
            _this->mAccelF = local_4c._10;
        }
        else if (x_speed_1_mag < _this->getSpeedData()->_0)
        {
            if (_this->mAcPyKey.buttonDush())
                _this->mAccelF = local_4c._18;
            else
                _this->mAccelF = local_4c._14;
        }
        else if (x_speed_1_mag < _this->getSpeedData()->_4)
        {
            if (x_speed_2_mag >= _this->getSpeedData()->_4)
                _this->mAccelF = local_4c._1c;
            else
                _this->mAccelF = local_4c._0;
        }
        else
        {
            if (x_speed_2_mag >= _this->getSpeedData()->_4)
                _this->mAccelF = local_4c._20;
            else
                _this->mAccelF = local_4c._0;
        }
    }
    else
    {
        if (_this->isSaka())
            _this->mAccelF = _this->getSakaStopAccele((_this->mSpeedF < 0.0) ? 1 : 0);
        else if (_this->mSpeedF * daPlBase_c::sc_DirSpeed[_this->mDirection] < 0.0)
            _this->mAccelF = local_4c._8;
        else if (fabsf(_this->mSpeedF) < _this->getSpeedData()->_0)
            _this->mAccelF = local_4c._4;
        else
            _this->mAccelF = local_4c._0;

        if (_this->isStatus(0x89))
            _this->mAccelF *= 3;

        _this->icePowerChange(0);
    }
}

}

kmBranchDefAsm(0x8004c394, 0x8004c398)
{
    // No stack saving needed
    nofralloc

    lis       r3, luigi__x_speed_inc@ha
    lfs       f0, luigi__x_speed_inc@l(r3)
    blr
}

kmBranchDefAsm(0x801307fc, 0x80130800) // TODO: Patch check at 8013080c
{
    // No stack saving needed
    nofralloc

    lis       r3, luigi__swim_y_speed_add@ha
    lfs       f1, luigi__swim_y_speed_add@l(r3)
    blr
}

kmCall(0x80135290, daPlBase_c__slipPowerSet_0); // Originally daPlBase_c::normalPowerSet()

kmCall(0x8004ba98, daPlBase_c__slipPowerSet_1); // Originally daPlBase_c::slipPowerSet(1)
kmCall(0x8004baa8, daPlBase_c__slipPowerSet_0); // Originally daPlBase_c::slipPowerSet(0)

kmWrite32(0x802ef118, 0x3D23D70A); // 0.04f
kmWrite32(0x802ef208, 0x3D23D70A); // ^^^

kmWrite32(0x802f591c, 0x3FCCCCCD); // 1.6f
kmWrite32(0x802f5920, 0x3F800000); // 1.0f
kmWrite32(0x802f5924, 0x3E800000); // 0.25f
kmWrite32(0x802f5928, 0xBE3851EC); // -0.18f
kmWrite32(0x802f5930, 0xBE3851EC); // ^^^
kmWrite32(0x802f5934, 0xBE800000); // -0.25f
kmWrite32(0x802f5950, 0xBE99999A); // -0.3f
kmWrite32(0x802f5954, 0xBCF5C28F); // -0.03f
kmWrite32(0x802f5958, 0xBE800000); // -0.25f
kmWrite32(0x802f5964, 0x40000000); // 2.0f
kmWrite32(0x802f5968, 0x3F99999A); // 1.2f
kmWrite32(0x802f5990, 0xBD0F5C29); // -0.035f
kmWrite32(0x802f5994, 0xBD4CCCCD); // -0.05f
kmWrite32(0x802f5998, 0xBD99999A); // -0.075f
// I don't think the following are relevant
kmWrite32(0x802f59c0, 0xBE3851EC); // -0.18f
kmWrite32(0x802f59c4, 0xBE800000); // -0.25f
kmWrite32(0x802f59dc, 0xBDCCCCCD); // -0.1f
kmWrite32(0x802f59e0, 0xBDCCCCCD); // ^^^
kmWrite32(0x802f59e4, 0xBDA3D70A); // -0.08f
kmWrite32(0x802f59ec, 0xBEAE147B); // -0.34f

kmWrite32(0x802f58b8, 0x3D4CCCCD); // 0.05f
kmWrite32(0x802f58bc, 0x3D4CCCCD); // ^^^
kmWrite32(0x802f58c0, 0x3D23D70A); // 0.04f

kmWrite32(0x8042db68, 0xBD4CCCCD); // -0.05f
kmWrite32(0x8042db70, 0xBD0F5C29); // -0.035f
kmWrite32(0x8042db6c, 0xBDA7EF9E); // -0.082f
kmWrite32(0x8042bb3c, 0x40000000); // 2.0f

// ------------------------------------------------------------------------------------------------
static int s_jumpAnmID[4] = {-1, -1, -1, -1};
static float s_jumpAnmRate[4] = {0.0f, 0.0f, 0.0f, 0.0f};

class sLib
{
public:
    static bool chase(float *value, float target, float step);
};

kmBranchDefCpp(0x80128120, 0x8012812c, void, dAcPy_c *_this)
{
    _this->mAction = 0;
    s_jumpAnmID[_this->mPlayerNo] = -1;
    s_jumpAnmRate[_this->mPlayerNo] = 0.5;
    _this->mAngle.x = 0;
    _this->mAngle.y = _this->getMukiAngle(_this->mDirection);
    _this->setWaterWalkFlag();
}

kmBranchDefCpp(0x80127cb8, 0x80127ce8, void, dAcPy_c *_this, int status_bit)
{
    _this->offStatus(status_bit);
    _this->mPyMdlMng.setAnm(6, 10.0, 0.0);
    s_jumpAnmID[_this->mPlayerNo] = 6; // New addition
}

extern "C" {

extern int DWORD_ARRAY_802f5108[3];

int dAcPy_c__getJump2AnimID(dAcPy_c *_this)
{
    if (_this->isStatus(0xd))
        return 68;

    if (_this->isStatus(0xf))
        return 119;

    if (_this->jumpSoundRelated == 2)
        return -1;

    return DWORD_ARRAY_802f5108[_this->jumpSoundRelated];
}

int dAcPy_c__getJump3AnimID(dAcPy_c *_this)
{
    if (_this->isStatus(0xd))
        return 84;

    if (_this->isStatus(0xf))
        return 119;

    if (_this->jumpSoundRelated == 2)
        return -1;

    return 158;
}

int dAcPy_c__getPhysicsAnimType(dAcPy_c *_this)
{
    if ((_this->_10d4 & 1) == 0
      //&& !_this->forceNeutralJumpFall() // TODO: This occurs in states such as triple jump, figure out equivalent in NSMBW (May be unnecessary)
        && _this->isStar() == 0)
    {
        if (_this->mAcPyKey.buttonJump())
            return 0;
        else
            return 1;
    }
    return 2;
}

void dAcPy_c__jumpStartWithPhysics(dAcPy_c *_this)
{
    if (_this->isStatus(0xc))
        _this->mAction = 1;

    else if (_this->isStatus(0x10))
    {
        if (_this->mPyMdlMng.mpPyMdlBase->isAnmStop())
        {
            _this->mPyMdlMng.setAnm(6, 3.0, 0.0);
            s_jumpAnmID[_this->mPlayerNo] = 6;
            _this->mAction = 1;
        }
    }
    else
    {
        if (_this->mSpeed.y < 0.0)
        {
            const int anmID = dAcPy_c__getJump2AnimID(_this);
            if (anmID != -1)
            {
                if (anmID == 68)
                    _this->mPyMdlMng.setAnm(68, 10.0, 0.0);
                else
                    _this->mPyMdlMng.setAnm(anmID, 0.0);

                s_jumpAnmID[_this->mPlayerNo] = anmID;
                _this->mAction = 1;
            }
        }
        if (dAcPy_c__getPhysicsAnimType(_this) == 0 &&
            (_this->mPyMdlMng.mpPyMdlBase->isAnmStop() || _this->mSpeed.y < 2.5))
        {
            const int anmID = dAcPy_c__getJump3AnimID(_this);
            if (anmID != -1)
            {
                switch (anmID)
                {
                case 84:
                    _this->mPyMdlMng.setAnm(84, 3.0, 0.0); // Not 100% accurate, but perhaps good enough
                    break;
                default:
                    _this->mPyMdlMng.setAnm(anmID, 0.0); // Not 100% accurate, but perhaps good enough
                    break;
                case 158:
                    _this->mPyMdlMng.setAnm(158, 10.0, 0.0);
                }
                s_jumpAnmID[_this->mPlayerNo] = dAcPy_c__getJump2AnimID(_this);
                _this->mAction = 1;
            }
        }
    }
    _this->jumpExecAir();
}

void dAcPy_c__jumpExecAirWithPhysics(dAcPy_c *_this)
{
    if (_this->jumpSoundRelated == 2)
    {
        u8 dir = _this->mDirection;
        if (dir != _this->_27cc || (_this->_10d4 >> 1 & 1) != 0)
        {
            _this->_27cc = dir;
            _this->mPyMdlMng.setAnm(
                11,
                0.0,
                _this->mPyMdlMng.mpPyMdlBase->getAnmFrameMax() - 1.0);
            s_jumpAnmID[_this->mPlayerNo] = 11;
            //_this->setForceNeutralJumpFall(false); // TODO: Figure out equivalent in NSMBW (May be unnecessary)
        }
        if (_this->mPyMdlMng.mpPyMdlBase->isAnmStop())
        {
            //_this->setForceNeutralJumpFall(false); // TODO: Figure out equivalent in NSMBW (May be unnecessary)
        }
    }
    switch (dAcPy_c__getPhysicsAnimType(_this))
    {
    case 0:
    {
        const int anmID = dAcPy_c__getJump3AnimID(_this);
        if (anmID != -1)
        {
            const float rate = dPyMdlMng_c::getAnmRate(anmID);
            float blendDuration;
            switch (anmID)
            {
            case 84:
                blendDuration = 3.0; // Not 100% accurate, but perhaps good enough
                break;
            default:
                blendDuration = dPyMdlMng_c::getAnmBlendDuration(anmID);
                break;
            case 158:
                blendDuration = 10.0;
            }
            sLib::chase(&(s_jumpAnmRate[_this->mPlayerNo]), 1.5, 0.02);
            _this->mPyMdlMng.setAnm(
                anmID,
                s_jumpAnmRate[_this->mPlayerNo] * rate,
                blendDuration,
                0.0);
        }

        if (_this->mPyMdlMng.mpPyMdlBase->checkAnmFrame(8.0) ||
            _this->mPyMdlMng.mpPyMdlBase->checkAnmFrame(19.0))
        {
            _this->startSound(284, 0); // 284 -> SE_PLY_PRPL_FLY
        }
    }
        break;
    case 1:
        if (s_jumpAnmID[_this->mPlayerNo] != -1)
        {
            _this->mPyMdlMng.setAnm(s_jumpAnmID[_this->mPlayerNo], 5.0, 0.0);
        } // Fall-through
    default:
        s_jumpAnmRate[_this->mPlayerNo] = 0.5;
        break;
    }

    _this->jumpExecAir();
}

}

kmWritePointer(0x80324e88, &dAcPy_c__jumpStartWithPhysics);
kmWritePointer(0x80324e94, &dAcPy_c__jumpExecAirWithPhysics);
