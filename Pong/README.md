# PONG

## Kontroller för Chipkit
Skriv in ett tecken för kommando (och updatera):

btn4 -> Vänstra spelaren upp / Meny upp

btn3 -> Vänstra spelaren ner / Meny Ner

btn2 -> Vänstra spelaren upp

btn1 -> Vänstra spelaren ner / Bekräfta menyval

## Kontroller för ConsVer
Skriv in ett tecken för kommando (och updatera):

w -> Vänstra spelaren upp / Meny upp

s -> Vänstra spelaren ner / Meny Ner

u -> Vänstra spelaren upp

j -> Vänstra spelaren ner

d -> Bekräfta menyval

annat/tomt -> bara uppdatera

## Byggt på 
This is an empty project template.
Just add your own code to the existing C or assembly files.
Any added C or assembly files will automatically be built,
no need to edit the Makefile.

The template contains the following source files:

main.c:
	Contains the entry point main()

vectors.S:
	Contains the interrupt handler and the exception handler

stubs.c:
	Contains stubs for functions run during micro controller
	initialization
