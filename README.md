# OpenGL Font

Made by C/OpenGL

## Introduction:

Use PNG and .fnt file to save font

This soft is the example for load Font from Image, load font, show text (like dialog box in games)

In .fnt files, you can read them by Notepad/Notepad++, in these files, you can see the numbers
- First number is Count of Characters
- Second number is Height of Characters (All characters have same height. So, I save it only once)
- All the next lines are info of characters
- id, x, y, w, h, xo, yo, wf
- id: ID of character in ASCII table, e.g. A is 65
- x: x crop position in Image
- y: y crop position in Image
- w: Width of character crop
- h: Height of character crop
- xo: x Offset to draw into new Image (First, you crop a small image, and then you make a larger image from it, make all character same height)
- yo: y Offset to draw into new Image
- wf: Width Full: Finally Width of Character

6 fonts are available in Folder Fonts

## Image:

![Image](https://i.imgur.com/aKUtzRy.png)

## Update:

Add 2 New fonts, Dialog

--- End ---
