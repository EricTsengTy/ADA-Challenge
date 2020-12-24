#!/usr/bin/env python3
'''
For Linux or Mac
'''
import os

for i in range(11):
    print(f'==== public-test-cases/{i:02d}.in ====')
    os.system(f'./validator/checker --public public-test-cases/{i:02d}.in best-answer/{i:02d}.out')