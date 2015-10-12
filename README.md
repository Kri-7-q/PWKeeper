# PWKeeper
Programm to manage my credentials with a GUI.

Stores:
- Provider (the name of a provider, for instance a internet shop)
- Username (Login name)
- Password
- Security question
- Answer to the question
- Password length (needed to generate a random password)
- A list of characters (needed to generate a random password)
- Last modified date and time

## Generate Password
The application can generate a random password. Therefore it needs information about the length of password
and which characters are allowed.  
It takes a string to define the allowed characters.  
Format to define character:  
Type | Example | Description
---|---|---
Range | *[A-Z] | define a range of character from 'A' to 'Z' in the systems character set.
| | The asterisk means an arbitary amount of this characters.
---|---|---
Range | 3[0-9] | Three number from 0 to 9.
---|---|---
Set | 2{#+?@<>} | Defines a set of character. In this case the character: #, +, ?, @, <, >
| | The generator should use two of them.
