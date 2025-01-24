#!/usr/bin/env python3

"""
Usage: read_fcp_xml.py <path/to/Info.fcpxml>
"""

import re
import sys
import xml.etree.ElementTree as ET


def main():
    if len(sys.argv) != 2:
        print(__doc__)
        return 1
    generate_scenes(sys.argv[1])


def generate_scenes(fcp_xml):
    with open(fcp_xml) as f:
        root = ET.fromstring(f.read())

        marker_pattern = re.compile(r'LED_MARKER ([A-Z_0-9]+), ([A-Z_0-9]+)( \/\/ .*)?')

        for video in root.iter('video'):
            if video.attrib['name'].startswith('LED_MARKER'):
                match = marker_pattern.search(video.attrib['name'])
                if match:
                    color1 = match.group(1)
                    color2 = match.group(2)
                    comment = match.group(3) or ''
                    print(f"Scene({color1}, {color2}, {duration_to_ms(video.attrib['duration'])}), // {comment}")
                else:
                    print(f"Scene(, , {duration_to_ms(video.attrib['duration'])}),")


def duration_to_ms(duration_str):
    if "/" in duration_str:
        numerator, denominator = duration_str.replace("s","").split("/")
        return int(int(numerator) / int(denominator) * 1000)
    else:
        return int(duration_str.replace("s","")) * 1000


main()
