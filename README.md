# AssembHell

> An assembler created as a final project at a university course
--------------------------------------------------------------------------------------------

#### The Assembler was created to handle specific processor requirements given in the course
#### Further info and the processor requirements are described later in more detail
--------------------------------------------------------------------------------------------

## Table of contents
[Baby Steps](#baby-steps)  
[Usage](#usage)  
[Information](#info)  
[License](#license)  

</br>

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
 `file.ob file.ext file.ent`</br>
  > See [examples](/examples)
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
 


<a name="info"/>

## Information

### Processor
The size of a binary code is 12 bits. The processor only works with integers and uses 2’s complement.</br>
The processor has 8 registers (r1,r2,r3,r4,r5,r6,r7) and each one of them holds 12 bits. </br>
Registers are called by character @ </br>
The memory size consists of 1024 binary code ( 1024 * 12 bits ) .</br>
Chars are represented in ascii.

### Assembly

| Code          | Opcodes       | 
| ------------- |:-------------:| 
| 0             | mov           | 
| 1             | cmp           |  
| 2             | add           |
| 3             | sub           | 
| 4             | not           |  
| 5             | clr           |
| 6             | lea           | 
| 7             | inc           |  
| 8             | dec           |
| 9             | jmp           | 
| 10             | bne           |  
| 11             | red           |
| 12             | prn           | 
| 13             | jsr           |  
| 14             | rts           |
| 15             | stop          |

Opcodes are represented by </br>

|   11  -  9    |    8 - 5      |      4 - 2     | 1  0 |
| ------------- |:-------------:| --------------:|-----:|
| First operand | opcode code   | Second operand | Type |

</br>
A legal label name must start with an alphabetic char and must be shorter than 31 chars
Initialize a label using : after the label's name  </br>

 
Special cases of label uses are </br>
`LABEL_NAME: code ...` - A certain code</br>
`LABEL_NAME: .data ...` - array of integers</br>
`LABEL_NAME: .string ...` - A string</br>
`.extern LABEL_NAME` - A label which already exist in other file</br>
`.entry LABEL_NAME` - A global label which can be used in other files

</br>
 The binary code of a register is the number of register in binary</br>
 The binary code of an integer or a char is a binary representation of them ( char in ascii)</br>
 The binary code of a label is the place in memory it was initialized ( 1 for extern labels)
 
### Algorithm in a nutshell
The assembler works in 2 different cycles
####  First Cycle 
In the first cycle the assembler parses every opcode and operand and save their binary code in memory.</br>
Whenever the assembler encouters a label as an operand it ignores the label and saves it to the second cycle.</br>
Every label who is being initialized is saved in the assembler memory in order to be used in the second cycle.
####  Between Cycles
The assembler orderes the memory in a way that every data label will come</br>
after the memory section dedicated to the opcodes and their operands</br>
If there are no errors the assembler will continue.
####  Second Cycle
The assembler uses the labels stored in the first cycle in other to parse every label operand into his binary code.</br>
The assembler parses every `.entry` call in this cycle and tags every entry label using the labels stored in the first cycle.</br>
####  End
If there are no errors the assembler will continue.</br>
It translates the binary code to Base64 and creates the file `program_name.ob`</br>
Using the data stored is creates the files `program_name.ext program_name.ent`

<a name="license"/>

## License
This project is licensed under the GNU General Public License v3.0 . See [License](LICENSE) for more info
