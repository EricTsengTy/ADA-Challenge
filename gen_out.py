#!/usr/bin/env python3
import os, sys

if len(sys.argv) < 3:
    exit()

indir = sys.argv[1]
outdir = sys.argv[2] + "/team15"

os.makedirs(outdir)

for testcase in os.listdir(indir):
    prefix = testcase[:-3]
    outfile = prefix + ".out"
    os.popen(f'./final-version/magic < {indir}/{testcase} > {outdir}/{outfile}')
