#!/usr/bin/env python3

# MIT License
#
# Copyright (c) 2022 RoadrunnerWMC
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

import argparse
import dataclasses
import json
from pathlib import Path
import sys
from typing import Any, Dict, Optional, Set


PROJECT_SAFE_NAME = 'nsmbw_updated'
PROJECT_DISPLAY_NAME = 'NSMBW Updated'
REGIONS = ['P', 'E', 'J', 'K', 'W', 'C']
VERSIONS = ['P1', 'E1', 'J1', 'P2', 'E2', 'J2', 'P3', 'J3', 'K', 'W', 'C']
DEFAULT_VERSION = 'P1'
LANG_FOLDER_NAMES = {
    'P1': 'EU',
    'E1': 'US',
    'J1': 'JP',
    'P2': 'EU',
    'E2': 'US',
    'J2': 'JP',
    'P3': 'EU',
    'J3': 'JP',
    'K': 'KR',
    'W': 'TW',
    'C': 'CN',
}


BUILD_DIR = Path('build')

RIIVO_DISC_ROOT = BUILD_DIR / PROJECT_SAFE_NAME
RIIVO_CONFIG_DIR = BUILD_DIR / 'riivolution'

RIIVO_DISC_CODE = RIIVO_DISC_ROOT / 'Code'
RIIVO_DISC_OBJECT = RIIVO_DISC_ROOT / 'Object'
RIIVO_DISC_STAGE = RIIVO_DISC_ROOT / 'Stage'
RIIVO_DISC_FOLDER_NAMES = ['Code', 'Object', 'Stage', *sorted(set(LANG_FOLDER_NAMES.values()))]

RIIVO_XML = RIIVO_CONFIG_DIR / f'{PROJECT_SAFE_NAME}.xml'


########################################################################
########################### Utility functions ##########################
########################################################################


def detect_game_version(root: Path) -> Optional[str]:
    """
    Detect the game version at the specified path.
    """
    # TODO: P3 and J3 can't be detected from just the disc root...

    if (root / 'COPYDATE_CODE_2009-10-03_232911').is_file():
        return 'P1'
    elif (root / 'COPYDATE_CODE_2009-10-03_232303').is_file():
        return 'E1'
    elif (root / 'COPYDATE_CODE_2009-10-03_231655').is_file():
        return 'J1'
    elif (root / 'COPYDATE_CODE_2010-01-05_152101').is_file():
        return 'P2'
    elif (root / 'COPYDATE_CODE_2010-01-05_143554').is_file():
        return 'E2'
    elif (root / 'COPYDATE_CODE_2010-01-05_160530').is_file():
        return 'J2'
    elif (root / 'COPYDATE_CODE_2010-03-12_153510').is_file():
        return 'K'
    elif (root / 'COPYDATE_CODE_2010-03-15_160349').is_file():
        return 'W'
    elif (root / 'COPYDATE_CODE_2016-05-03_111248').is_file():
        return 'C'


def ninja_escape_spaces(thing: Any) -> str:
    """
    Call str() on `thing` (probably a str or Path), and apply
    Ninja-style space escaping
    """
    return str(thing).replace(' ', '$ ')


########################################################################
############################# Config class #############################
########################################################################


@dataclasses.dataclass
class Config:
    """
    Class that represents configuration options provided by the user
    """
    game_roots: Dict[str, Path] = dataclasses.field(default_factory=dict)
    include_enhancements: bool = True

    def set_up_arg_parser(self, parser: argparse.ArgumentParser) -> None:
        """
        Add arguments to the ArgumentParser
        """
        parser.add_argument('--game-root', type=Path, action='append',
            help='the path to an extracted game root (i.e. with the Effect, Env, HomeButton2, etc. subdirectories).'
            ' You can specify this multiple times for different game versions, and the resulting Riivolution patch will include assets for all of them.')
        parser.add_argument('--enhancements', choices=('on', 'off'), default='on',
            help='whether to include enhancements in addition to bugfixes')

        # TODO: let the user configure
        # - path to CodeWarrior
        # - path to Kamek

    def read_args(self, args: argparse.Namespace) -> None:
        """
        Figure out the configuration data from the CLI arguments
        """
        if args.game_root:
            for game_root in args.game_root:
                version = detect_game_version(game_root)
                if version is None:
                    raise ValueError(f"Couldn't identify the game version at {game_root}. Are you sure that's a path to NSMBW?")
                else:
                    print(f'Found {version} NSMBW at {game_root}')
                    self.game_roots[version] = game_root
        else:
            print('WARNING: No game roots specified -- patched assets will not be built!')

        self.include_enhancements = (args.enhancements == 'on')


########################################################################
################################# Code #################################
########################################################################


CODE_BUILD_VERSIONS = ['P1', 'E1', 'J1', 'P2', 'E2', 'J2', 'K', 'W']

CODE_ROOT_DIR = Path('code')
KAMEK_ROOT_DIR = Path('Kamek')

CODEWARRIOR_EXE = KAMEK_ROOT_DIR / 'cw' / 'mwcceppc.exe'
KAMEK_EXE = KAMEK_ROOT_DIR / 'Kamek' / 'bin' / 'Debug' / 'net6.0' / 'Kamek'
KSTDLIB_DIR = KAMEK_ROOT_DIR / 'k_stdlib'

ADDRESS_MAP_TXT = CODE_ROOT_DIR / 'address-map.txt'

CODE_SRC_DIR = CODE_ROOT_DIR / 'src'
CODE_INCLUDE_DIR = CODE_ROOT_DIR / 'include'

PREPROC_FLAGS = {
    #      Version       Region      v1            v2            vk           vw           vc
    'P1': {'VERSION_P1', 'REGION_P', 'IS_V1',      'IS_PRE_V2',  'IS_PRE_K',  'IS_PRE_W',  'IS_PRE_C'},
    'E1': {'VERSION_E1', 'REGION_E', 'IS_V1',      'IS_PRE_V2',  'IS_PRE_K',  'IS_PRE_W',  'IS_PRE_C'},
    'J1': {'VERSION_J1', 'REGION_J', 'IS_V1',      'IS_PRE_V2',  'IS_PRE_K',  'IS_PRE_W',  'IS_PRE_C'},
    'P2': {'VERSION_P2', 'REGION_P', 'IS_POST_V1', 'IS_V2',      'IS_PRE_K',  'IS_PRE_W',  'IS_PRE_C'},
    'E2': {'VERSION_E2', 'REGION_E', 'IS_POST_V1', 'IS_V2',      'IS_PRE_K',  'IS_PRE_W',  'IS_PRE_C'},
    'J2': {'VERSION_J2', 'REGION_J', 'IS_POST_V1', 'IS_V2',      'IS_PRE_K',  'IS_PRE_W',  'IS_PRE_C'},
    'K':  {'VERSION_K',  'REGION_K', 'IS_POST_V1', 'IS_POST_V2',              'IS_PRE_W',  'IS_PRE_C'},
    'W':  {'VERSION_W',  'REGION_W', 'IS_POST_V1', 'IS_POST_V2', 'IS_POST_K',              'IS_PRE_C'},
    # 'C':  {'VERSION_C',  'REGION_C', 'IS_POST_V1', 'IS_POST_V2', 'IS_POST_K', 'IS_POST_W',           },
}
assert set(PREPROC_FLAGS) == set(CODE_BUILD_VERSIONS)


def get_preproc_flags(config: Config, version: str) -> Set[str]:
    """
    Get the set of preprocessor flags to use for this game version.
    """
    flags = set(PREPROC_FLAGS[version])
    if config.include_enhancements:
        flags.add('ENHANCEMENTS')
    return flags


@dataclasses.dataclass
class TranslationUnit:
    """
    Represents one translation unit (.cpp file)
    """
    cpp_file: Path

    # Sometimes we need different builds for different game versions
    # (such as when actor IDs are referenced in the code), but often we
    # can just build for P1 and reuse it everywhere. This dict defines
    # that relationship.
    builds: Dict[str, str] = dataclasses.field(
        default_factory=lambda: {v: DEFAULT_VERSION for v in CODE_BUILD_VERSIONS})

    def read_config(self, path: Path) -> None:
        """
        Read additional config data from an optional .json file
        """
        if not path.is_file():
            return
        with path.open(encoding='utf-8') as f:
            j = json.load(f)

        if builds_list := j.get('builds'):
            # Invert it
            self.builds = {}
            for build, users in builds_list.items():
                for user in users:
                    self.builds[user] = build

            if set(self.builds) != set(CODE_BUILD_VERSIONS):
                raise ValueError(f"{path.name} doesn't specify the right set of build versions:"
                    f' {sorted(self.builds)} vs {sorted(CODE_BUILD_VERSIONS)}')

    def iter_builds(self):
        """
        Iterator over the .o files that need to be built for this TU.
        Yields (version string, .o file path) pairs.
        """
        for version in set(self.builds.values()):
            yield version, self.o_file_for_version(version)

    def o_file_for_version(self, version: str) -> Path:
        """
        Return the .o file that should be linked for the specified game
        version.
        """
        if len(set(self.builds.values())) == 1:
            # No need to mangle the version name into the .o filename
            return self.cpp_file.with_suffix('.o')
        else:
            return self.cpp_file.parent / (self.cpp_file.stem + f'_{self.builds[version]}.o')


def make_code_rules(config: Config) -> str:
    """
    Create Ninja rules to build the .bin files in the Code directory
    """
    # Find all TUs, and read any configs
    tus = []
    for fp in sorted(CODE_SRC_DIR.glob('**/*.cpp')):
        tu = TranslationUnit(fp)
        tu.read_config(fp.with_suffix('.json'))
        tus.append(tu)

    use_wine = (sys.platform == 'win32')

    lines = [f"""
mwcceppc = {CODEWARRIOR_EXE}
cc = {'' if use_wine else 'wine '}$mwcceppc
kamek = {KAMEK_EXE}
kstdlib = {KSTDLIB_DIR}
addrmap = {ADDRESS_MAP_TXT}
includedir = {CODE_INCLUDE_DIR}

cflags = $
  -I- $
  -i $kstdlib $
  -i $includedir $
  -Cpp_exceptions off $
  -enum int $
  -O4,s $
  -use_lmw_stmw on $
  -fp hard $
  -rostr $
  -sdata 0 $
  -sdata2 0 $
  -RTTI off

rule cw
  command = $cc $cflags -c -o $out $in
  description = $cc $in
"""]

    # Add "cw" edges for all .cpp -> .o files.
    for tu in tus:
        for version, o_file in tu.iter_builds():
            lines.append('build'
                f' {ninja_escape_spaces(o_file)}:'
                f' cw {ninja_escape_spaces(tu.cpp_file)}\n'
                f' cflags = $cflags{"".join(f" -D{v}" for v in get_preproc_flags(config, version))}')
    lines.append('')

    lines.append(f"""
rule km
  command = $kamek $in -dynamic -versions=$addrmap -output-kamek=$out -select-version=$selectversion
  description = $kamek -> $out
""")

    for version in CODE_BUILD_VERSIONS:
        lines.append('build')
        out_fp = RIIVO_DISC_CODE / f'{version}.bin'
        lines[-1] += f' $builddir/{ninja_escape_spaces(out_fp.relative_to(BUILD_DIR))}'

        lines[-1] += ': km'
        for tu in tus:
            o_file = tu.o_file_for_version(version)
            lines[-1] += f' {ninja_escape_spaces(o_file)}'
        lines[-1] += f'\n selectversion = {version}'

        lines.append('')
    
    return '\n'.join(lines)


########################################################################
################################ Credits ###############################
########################################################################


CREDITS_PY = Path('credits/credits.py')

# No bugs in the credits in these versions, so no need to add build
# rules for their credits files
CREDITS_VERSIONS_BLACKLIST = {'K', 'W'}


def make_credits_rules(config: Config) -> str:
    """
    Create Ninja rules to update the credits
    """
    if not config.game_roots:
        return ''

    lines = [f"""
rule credits
  command = $py {CREDITS_PY} $in $out
  description = Editing credits...
"""]

    already_covered_staffrolls = set()

    for version, root in config.game_roots.items():
        if version in CREDITS_VERSIONS_BLACKLIST:
            continue

        lang_folder = root / LANG_FOLDER_NAMES[version]

        # Find all staffroll.bin's
        staffrolls = []
        for possible_parent in [lang_folder, *lang_folder.iterdir()]:
            staffroll_fp = possible_parent / 'staffroll' / 'staffroll.bin'
            if staffroll_fp.is_file():
                staffrolls.append(staffroll_fp)

        if not staffrolls:
            raise ValueError(f"Couldn't find a single staffroll.bin in {lang_folder}...")

        # Add build edges
        for staffroll in staffrolls:
            staffroll_relative = staffroll.relative_to(root)

            if staffroll_relative in already_covered_staffrolls:
                continue

            target_dir = f'$builddir/{(RIIVO_DISC_ROOT / staffroll_relative).relative_to(BUILD_DIR)}'
            lines.append(f'build {ninja_escape_spaces(target_dir)}: credits {ninja_escape_spaces(staffroll)}')

            already_covered_staffrolls.add(staffroll_relative)

    return '\n'.join(lines)


########################################################################
############################ Riivolution XML ###########################
########################################################################


CREATE_RIIVOLUTION_XML_PY = Path('create_riivolution_xml.py')

def make_riivolution_xml_rules(config: Config) -> str:
    """
    Create Ninja rules to build the Riivolution XML
    """
    lines = [f"""
rule riixml
  command = $py {CREATE_RIIVOLUTION_XML_PY} $out /{PROJECT_SAFE_NAME} {','.join(REGIONS)} '{PROJECT_DISPLAY_NAME}'"""]

    for folder in RIIVO_DISC_FOLDER_NAMES:
        lines[-1] += f' --folder={folder},/{folder}'

    lines.append('  description = Generating Riivolution XML...')
    lines.append('')

    lines.append(f'build $builddir/{RIIVO_XML.relative_to(BUILD_DIR)}: riixml')
    
    return '\n'.join(lines)


########################################################################
################################ General ###############################
########################################################################

NINJA_FILE = Path('build.ninja')


def make_ninja_file(config: Config) -> str:
    """
    Make the overall Ninja file
    """
    txt = f"""# NOTE: This file is generated by {Path(__file__).name}.

py = {sys.executable}

# NOTE: "builddir" has special significance to Ninja (see the manual)
builddir = {BUILD_DIR}

{make_code_rules(config)}
{make_credits_rules(config)}
{make_riivolution_xml_rules(config)}
"""

    while '\n\n\n' in txt:
        txt = txt.replace('\n\n\n', '\n\n')

    return txt


def main(argv=None) -> None:
    """
    Main function
    """

    config = Config()

    parser = argparse.ArgumentParser(
        description=f'Configure {PROJECT_DISPLAY_NAME} to prepare for building.')
    config.set_up_arg_parser(parser)

    args = parser.parse_args(argv)
    config.read_args(args)

    NINJA_FILE.write_text(make_ninja_file(config), encoding='utf-8')
    print(f'Wrote {NINJA_FILE}')


if __name__ == '__main__':
    main()
