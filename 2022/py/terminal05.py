

import time
import sys


def loading():
  print("...loading...".center(100))
  for i in range(1, 101):
    sys.stdout.write("\u001b[1000D")
    sys.stdout.flush()
    str = '[' + '#' * i + (100 - i) * ' ' + ']'
    sys.stdout.write(str)
    sys.stdout.flush()
    time.sleep(0.1)
  print()

if __name__ == '__main__':
  loading()
