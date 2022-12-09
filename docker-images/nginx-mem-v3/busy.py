print("on")
import time
from random import randint
a = [0]*10000000

c = 0
while True:
  if randint(0, c) > 100: # do not run till 20 sec
    time.sleep(1)
  c += 1
  rnd = randint(0, 10000000-100000)
  for i in range(rnd, rnd+10000):
    a[i] += 1
  time.sleep(0.2)

print("off")
