import sys, tty
 
def command_line():
    tty.setraw(sys.stdin)
    while True: # loop for each line
    # Define data-model for an input-string with a cursor
        input = ""
        while True: # loop for each character
            char = ord(sys.stdin.read(1)) # read one char and get char code
            
            # Manage internal data-model
            if char == 3: # CTRL-C
                return
            elif 32 <= char <= 126:
                input = input + chr(char)
            elif char in {10, 13}:
                sys.stdout.write(u"\u001b[1000D")
                print ("\nechoing... ", input)
                input = ""

            # Print current input-string
            sys.stdout.write(u"\u001b[1000D")  # Move all the way left
            sys.stdout.write(input)
            sys.stdout.flush()
            
            
if __name__ == '__main__':
    command_line()
    