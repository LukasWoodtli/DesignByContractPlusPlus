#!/usr/bin/env python

import os
import re
from subprocess import call
import filecmp
import fileinput

THIS_SCRIPT_DIR = os.path.dirname(os.path.realpath(__file__))
TEST_EXEC = os.path.join(THIS_SCRIPT_DIR, "../build/test/traceTest")

EXPECTED_LOG_FILE = os.path.join(THIS_SCRIPT_DIR, "expected_log_contracts.txt")
ACTUAL_LOG_FILE = os.path.join(THIS_SCRIPT_DIR, "../build/test/log_contracts.txt")

PATH_REGEX = r"(/[^/]+)+/"
def replace_pathes(log_file):
    for line in fileinput.input(log_file, inplace=True):
        print re.sub(PATH_REGEX, "{{%%PATH%%}}:", line),


print "Running test (python)"

retVal = call([TEST_EXEC])

if retVal != 0:
    print "Test failed"
    exit(retVal)

replace_pathes(ACTUAL_LOG_FILE)

if filecmp.cmp(EXPECTED_LOG_FILE, ACTUAL_LOG_FILE):
    print "Files compared successfully!"
    exit(0)
else:
    print "Files: %s and %s differ" % (EXPECTED_LOG_FILE, ACTUAL_LOG_FILE)
    exit(1)
