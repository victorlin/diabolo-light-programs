#!/usr/bin/env python3

"""
Usage: read_fcp_xml.py <path/to/Info.fcpxml>
"""

import re
import sys
from textwrap import dedent
import xml.etree.ElementTree as ET


def main():
    if len(sys.argv) != 2:
        print(__doc__)
        return 1
    generate_scenes(sys.argv[1])


def generate_scenes(fcp_xml):
    with open(fcp_xml) as f:
        root = ET.fromstring(f.read())

    marker_pattern = re.compile(r'LED_MARKER ([A-Z]+_\d), ([A-Z]+_\d)( \/\/ .*)?')

    scene_number = 1
    scene_template = 'case {number}: return Scene({color1}, {color2}, {duration}); {comment}'
    scene_lines = []

    for video in root.iter('video'):
        if video.attrib['name'].startswith('LED_MARKER'):
            match = marker_pattern.search(video.attrib['name'])
            if match:
                color1 = match.group(1)
                color2 = match.group(2)
                comment = match.group(3) or ''
                scene_lines.append(
                    scene_template.format(
                        number=scene_number,
                        color1=color1,
                        color2=color2,
                        duration=duration_to_ms(video.attrib['duration']),
                        comment=comment,
                    )
                )
            else:
                scene_lines.append(
                    scene_template.format(
                        number=scene_number,
                        color1='',
                        color2='',
                        duration=duration_to_ms(video.attrib['duration']),
                        comment='',
                    )
                )
            scene_number += 1

    print(dedent("""\
        Scene get_current_scene(uint8_t scene_num) {{
            switch (scene_num) {{
                {case_statements}
            }}
        }}

        const static uint8_t TOTAL_SCENES = {size};
    """).format(case_statements='\n        '.join(scene_lines),
                size=len(scene_lines)))


def duration_to_ms(duration_str):
    numerator, denominator = duration_str.replace("s","").split("/")
    return int(int(numerator) / int(denominator) * 1000)


main()
