import sys, tty
 
def command_line():
    tty.setraw(sys.stdin)
    while True: # loop for each line
    # Define data-model for an input-string with a cursor
        input = ""
        index = 0
        while True: # loop for each character
            char = ord(sys.stdin.read(1)) # read one char and get char code
            
            # Manage internal data-model
            if char == 3: # CTRL-C
                return
            elif 32 <= char <= 126:
                input = input[:index] + chr(char) + input[index:]
                index += 1
            elif char in {10, 13}:
                sys.stdout.write(u"\u001b[1000D")
                print ("\nechoing... ", input)
                input = ""
                index = 0
            elif char == 27:
                next1, next2 = ord(sys.stdin.read(1)), ord(sys.stdin.read(1))
                if next1 == 91:
                    if next2 == 68: # Left
                        index = max(0, index - 1)
                    elif next2 == 67: # Right
                        index = min(len(input), index + 1)
                
            # Print current input-string
            sys.stdout.write(u"\u001b[1000D") # Move all the way left
            sys.stdout.write(input)
            sys.stdout.write(u"\u001b[1000D") # Move all the way left again
            if index > 0:
                sys.stdout.write(u"\u001b[" + str(index) + "C") # Move cursor too index
            sys.stdout.flush()
            
if __name__ == '__main__':
    command_line()
    