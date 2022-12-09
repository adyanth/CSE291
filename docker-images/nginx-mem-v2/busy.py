print("on")
import time
from random import randint
a = [0]*10000000

while True:
  rnd = randint(0, 10000000-1000000)
  for i in range(rnd, rnd+1000000):
    a[i] += 1
  time.sleep(1)

print("off")
