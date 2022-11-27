
import time


if __name__ == "__main__":
  for i in range(100):
    time.sleep(0.1)
    print(f"{i}%", end="")
    print("\r", end="")