# AssembHell

> An assembler created as a final project at a university course

#### The Assembler was created to handle specific processor requirements given in the course
#### Further info and the processor requirements are described later in more detail
--------------------------------------------------------------------------------------------

## Table of contents
[Baby Steps](#baby-steps)  
[Usage](#usage)  
[Information](#info)  
[License](#license)  

</br></br>

<a name="baby-steps"/>

## Baby Steps
### Requirements
The assembler was tested on Linux (Arch) and on MacOS Mojave
and it will only require gcc.</br>
### Installation
Simply open the terminal and clone
```bash
$ git clone https://github.com/BlueBlur23691/AssembHell.git 
$ cd AssembHell
 ```
 Then compile it typing 
 ```bash
$ make
 ```
 And run the executable
</br></br>

<a name="usage"/>

## Usage
### Input
The assembler only accepts as extensions</br>
* To parse a file simply run it as an argument of the executable
```bash
  ./as file.as
```
* Further more the assembler will accept multiple files as arguments
```bash
  ./as file1.as file2.as file3.as
```
### Output

 for a file  </br>
 `file.as` </br>
 the assembler will create files  </br>
 `file.ob file.ext file.ent`
 
 #### ob extension </br>
 This is the file that holds the code for all the opcodes, operands and data in Base64 </br>
 At the top of it there are 2 numbers which are the number of opcode and operand codes and data codes respectively </br>
 Further info is described later</br>
 
  #### ext extension </br>
  This is the file that holds the name of the extern labels and their place in memory(as operands) which</br> are defined</br> in other file. </br>
  The linker is the one who uses this file to search for the labels in other files
  Extern labels are defined as </br>`.extern LABEL_NAME`
  
  ####  ent extension </br>
  This is the file that holds the name of the global (entry) labels and their place in memory which are defined in this file and global to use in other files. </br>
  The linker is the one who uses this file to search for the labels in other files
  Entry labels are defined as </br>`.Entry LABEL_NAME`
 
  In case of an error the assembler creates an error file name `errors.txt` which describes the errors




