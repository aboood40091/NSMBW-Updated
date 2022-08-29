# MIT License

# Copyright (c) 2022 RoadrunnerWMC

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.


from pathlib import Path
import sys


KAMEK_ROOT_DIR = Path('Kamek')

CODEWARRIOR_EXE = KAMEK_ROOT_DIR / 'cw' / 'mwcceppc.exe'
KAMEK_EXE = KAMEK_ROOT_DIR / 'Kamek' / 'bin' / 'Debug' / 'net6.0' / 'Kamek'
KSTDLIB_DIR = KAMEK_ROOT_DIR / 'k_stdlib'

ADDRESS_MAP_TXT = Path('address-map.txt')

SRC_DIR = Path('src')
INCLUDE_DIR = Path('include')

BUILD_DIR = Path('build')

OUT_REGIONS = ['P1', 'E1', 'J1', 'P2', 'E2', 'J2']

NINJA_FILE = Path('build.ninja')


def make_ninjafile():
    src_files = sorted(SRC_DIR.glob('**/*.cpp'))
    lines = []
    lines.append(f'# NOTE: This file is generated by {Path(__file__).name}.')
    lines.append('')

    lines.append(f'mwcceppc = {CODEWARRIOR_EXE}')
    if sys.platform == 'win32':
        lines.append('cc = $mwcceppc')
    else:
        lines.append('cc = wine $mwcceppc')
    lines.append(f'kamek = {KAMEK_EXE}')
    lines.append(f'kstdlib = {KSTDLIB_DIR}')
    lines.append('')

    lines.append(f'addrmap = {ADDRESS_MAP_TXT}')
    lines.append('')

    lines.append(f'srcdir = {SRC_DIR}')
    lines.append(f'includedir = {INCLUDE_DIR}')
    lines.append(f'builddir = {BUILD_DIR}')  # NOTE: "builddir" has special
    lines.append('')                         # significance to Ninja

    lines.append('cflags ='
        ' -I-'
        ' -i $kstdlib'
        ' -i $includedir'
        ' -Cpp_exceptions off'
        ' -enum int'
        ' -O4,s'
        ' -use_lmw_stmw on'
        ' -fp hard'
        ' -rostr'
        ' -sdata 0'
        ' -sdata2 0'
        ' -RTTI off')
    lines.append('')

    lines.append('rule cw')
    lines.append('  command = $cc $cflags -c -o $out $in')
    lines.append('  description = $cc $in')
    lines.append('')

    lines.append('rule km')
    lines.append('  command ='
        ' $kamek'
        ' $in'
        ' -dynamic'
        ' -versions=$addrmap'
        " '-output-kamek=$builddir/$$KV$$.bin'")
    lines.append('  description = Linking all outputs with $kamek ...')
    lines.append('')

    # Add "cw" edges for all .cpp -> .o files.
    for fp in src_files:
        lines.append('build'
            f' $srcdir/{fp.relative_to(SRC_DIR).with_suffix(".o")}:'
            f' cw $srcdir/{fp.relative_to(SRC_DIR)}')
    lines.append('')

    # Add a "km" edge for the final linking step.
    # This line is a bit of a cheat. Ninja needs to know that the
    # linking outputs are P1.bin, E1.bin, etc, but Kamek derives those
    # filenames from the address map rather than the command line. So we
    # write this build edge without using "$out" anywhere at all.
    linking_line = ['build']
    for region in OUT_REGIONS:
        linking_line.append(f' $builddir/{region}.bin')
    linking_line.append(': km')
    for fp in src_files:
        linking_line.append(f' $srcdir/{fp.relative_to(SRC_DIR).with_suffix(".o")}')
    lines.append(''.join(linking_line))
    lines.append('')
    
    return '\n'.join(lines)


def main():
    NINJA_FILE.write_text(make_ninjafile(), encoding='utf-8')
    print(f'Ninja file written to {NINJA_FILE}')


if __name__ == '__main__':
    main()