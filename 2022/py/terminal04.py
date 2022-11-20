
import time
import sys


if __name__ == "__main__":
  for i in range(100):
    time.sleep(0.05)
    print("\u001b[3D", end="")
    sys.stdout.flush()
    time.sleep(0.1)
    print(str(i + 1) + "%", end="")
    time.sleep(0.1)
    sys.stdout.flush()
  print()