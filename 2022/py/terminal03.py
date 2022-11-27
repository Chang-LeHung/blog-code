
import time
import sys


if __name__ == "__main__":
  for i in range(100):
    time.sleep(0.05)
    print("\u001b[3D" + str(i + 1) + "%", end="")
    sys.stdout.flush()
  print()