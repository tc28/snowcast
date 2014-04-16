i) structure of my program:
this program's structure is quite simple:
i just have a main in my program
in the main()
i first set up tcp (code copied from the stencil code on cs033 website)
then set up udp (code copied from cs033 lecutre notes)
then included the select loop (code copied from lab08 memtop)

ii) any unresolved bug
I think there is at least one bug:
I am NOT able to print the welcome message from the server
(I don't know why),
but I am able to print the change-station message from the server
This bug will lead to a significant problem:
since I don't know the number of stations from the message,
I am not able to check for valid/incorrect station number from user input

iii) to compile
just run the original make: I did not make any changes to it