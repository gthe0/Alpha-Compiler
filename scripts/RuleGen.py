def gen_output(tokens):
    """
    A simple function that generates the wanted output
    by taking an array of 3 tokens
    Params:
        -- tokens: An array of tokens/strings
    """
    content, identifier, category = tokens[0], tokens[1], tokens[2]
    if identifier == "_":
        print(f'"{content}"\t\t\t\t{{return ({content});}}')
    elif category == "KEYWORD":
        print(f'"{content}"\t\t\t\t{{return ({identifier});}}')
    elif category == "PUNCT":
        print(f'"{content}"\t\t\t\t{{return (\'{content}\');}}')
    else :
        print(f'"{content}"\t\t\t\t{{return ({identifier});}}')


def tokenize(line):
    """
    A simple function that takes a string with three words
    and divides it into separate substrings/tokens.

        Params:
        -- line: A string
    """
    tokens = line.split()
    if len(tokens) >= 3:
        return tokens
    else:
        print(f'Invalid input: {line}')


def main():
    
    filename = input('Please enter relative path to the list:\n')

    try:
        with open(filename, "r") as file:
            for line in file:
                tokens = tokenize(line)
                gen_output(tokens)
                
    except FileNotFoundError:
        print(f"File '{filename}' not found.")
        
if __name__ == "__main__":
    main()