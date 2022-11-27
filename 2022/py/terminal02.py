
import time
import sys


if __name__ == "__main__":
  for i in range(100):
    time.sleep(0.05)
    sys.stdout.write(u"\u001b[1000D" + str(i + 1) + "%")
    sys.stdout.flush()
  print()