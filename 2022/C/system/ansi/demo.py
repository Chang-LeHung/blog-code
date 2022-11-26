import sys, tty
def syntax_highlight(input):
    stripped = input.rstrip()
    return stripped + u"\u001b[41m" + " " *  (len(input) - len(stripped)) + u"\u001b[0m"

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
                print("get ctrl-c")
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
            elif char == 127:
                input = input[:index-1] + input[index:]
                index -= 1
            # Print current input-string
            sys.stdout.write(u"\u001b[1000D")
            sys.stdout.write(u"\u001b[0K")
            sys.stdout.write(syntax_highlight(input))
            sys.stdout.write(u"\u001b[1000D")
            if index > 0:
                sys.stdout.write(u"\u001b[" + str(index) + "C")
            sys.stdout.flush()
            
            
if __name__ == '__main__':
  command_line()
