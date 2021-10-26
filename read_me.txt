/*******************************************************************
 *   		Assignment 2:  DESIGN OF A MINI SHELL
 *	    		File:  miniShell.c
 *           	Student Name:  Kartik Sharma
 * Programming Language Used:  C
 *    Operating System of PC: Windows 10(program was compiled and run on Ubuntu 20.04 LTS(app in microsoft store))
 *       	    Roll no.:  200101054
 *******************************************************************/

When a command is typed into mini shell, it first looks for the command in the list of built-ins that it maintains. 
 * If present, it will call the corresponding function (the mapping from built-in command name to the command function is implemented using **function pointers** ). 
 * If not, it will fork a new process, load the command's image into the child process and wait for the child process to finish execution before displaying the prompt again.
 * If the command is not found (the corresponding `.c` file is not found), an error message indicating that the command was not found will be displayed.
  
  When errors occur, appropriate error messages will be displayed(mostly).
  

# Commands Implemented:

  -Internal : coded in main file itself

	    1)help : displays all implemented commands
	    2)cd [dir] 
	    3)exit
	    4)echo [string to be echoed]
	    5)pwd
	    6)(i)printenv : displays the predefined environment variables[HOME,USER,TERM,SHELL]
	      (ii)printenv [VAR] : displays the environment variable and its value
	    7)setenv [VAR=xyz] : change or create new 
	    8)history : displays all command line input taken upto history command
 
  -External : coded in different files in cmds folder

	    1)cat [file1 file2 ... ] : contents of one or more files displayed
	    2)touch [file] : one file is created
	    3)rm [file1 file2 ... ] : one or more files can be deleted
	    4)cp [file_from dir/file_to] 
	    5)(i) mv [file1 dir/] : move a file from pwd to a directory
	      (ii) mv [file1 file2] : rename a file
	    6)mkdir [dir1 dir2 ... ] : one or more folders can be made 
	    7)rmdir [dir1 dir2 ... ] : one or more directories can be deleted
	    8)(i) ls : display contents of pwd
	      (ii)ls [dir1 dir2 ... ] : display contents of one or more directories
	    9)clear
	    10)date

# Features Implemented:

1) Input,Output & Error Redirection : Operators = [<, >, >>, 2>, 2>>]  
    example commands: ls >> outputfile 2> errfile ***(implementation requires spaces around redirection operators)***  
		      cat < file1 > file2
2) Access,Modify & Create Environment Variables


# Compiling Process :

  [Step 1 can be skipped]
1) Compile all the files in cmds folder in this way:
   gcc -o cat cat.c
   gcc -o touch touch.c
   gcc -o rm rm.c
   gcc -o cp cp.c
   gcc -o mv mv.c
   gcc -o mkdir mkdir.c
   gcc -o rmdir rmdir.c
   gcc -o ls ls.c
   gcc -o clear clear.c
   gcc -o date date.c

   [Step 2 can be skipped]
2) Move out of the cmds folder & compile the main file:
   cd ..
   gcc miniShell.c

3) Run the main file:
   ./a.out
   
   
   
credits : https://github.com/ashly13/Mini-Shell
