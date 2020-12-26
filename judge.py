#!/usr/bin/env python3
'''
For Linux or Mac
'''
import os

s = 0
for i in range(11):
    print(f'==== public-test-cases/{i:02d}.in ====')
    cur = os.popen(f'./validator/checker --public public-test-cases/{i:02d}.in best-answer/{i:02d}.out').read()
    print(cur,end="")
    s += float(cur)
print('==== Total Score ====')
print(s)
