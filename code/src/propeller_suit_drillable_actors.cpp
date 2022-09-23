// MIT License

// Copyright (c) 2022 RoadrunnerWMC

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <kamek.h>


// A few actors can be inappropriately killed by Propeller Mario
// performing the spin-drill move. This can be fixed by disabling the
// "SpinFall" bit ((bitfield >> 13) & 1) of the "attack bitfield" in the
// initialization struct for the actor's dCc_c collision controller.

#ifdef C00800
// Falling Icicle, daEnIcicle_c (sprite 265, actor 339, EN_ICICLE)
kmWrite8(0x80ad0eba, 0xdf);  // for 1x1 size
kmWrite8(0x80ad0ede, 0xdf);  // for 1x2 size
#endif  // C00800

#ifdef C00801
// Boo Circle Boo, daEnRotarionGhost_c [sic] (EN_ROTATION_GHOST)
kmWrite8(0x80ad415a, 0x88);
#endif  // C00801