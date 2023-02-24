#!/bin/bash

####################
# Lab 1 Exercise 7
# Name:
# Student No:
# Lab Group:
####################

# declare var
prefix=""
numFiles=0
number1=0
number2=0
newPrefix=""

# Enter prefix (only alphabets):
while true
do
  echo "Enter prefix (only alphabets):"
  read prefix #usage of var must come with '$' but not this line?
  if [[ $prefix =~ ^[a-zA-Z]+$ ]]; 
  then
    break
  else
    echo "INVALID"
    echo "Please enter a valid prefix [a-z A-Z]:"
  fi
done
#Please enter a valid number [0-9]:
#Enter 2 numbers:
#Files Created
#Enter NEW prefix (only alphabets):
#Files Renamed
# Number of files to create:
echo "Number of files to create:"
while true
do
  read numFiles
  if [[ $numFiles =~ ^[1-9][1-9]*$ ]]; 
  then
    break
  else
    echo "Please enter a valid number [0-9]:"
  fi
done

# Enter $N numbers:
echo "Enter 2 numbers:"
while true
do
  read number1
  if [[ $number1 =~ ^[1-9][1-9]*$ ]]; 
  then
    break
  else
    echo "Please enter a valid number [0-9]:"
  fi
done

while true
do
  read number2
  if [[ $number2 =~ ^[1-9][1-9]*$ ]]; 
  then
    break
  else
    echo "Please enter a valid number [0-9]:"
  fi
done


touch "$prefix_$number1.txt" "$prefix_$number2.txt"
echo ""
echo "Files Created"
ls *.txt
echo ""


# Enter NEW prefix (only alphabets):
while true
do
  echo "Enter NEW prefix (only alphabets):"
  read newPrefix 
  if [[ $newPrefix =~ ^[a-zA-Z]+$ ]]; 
  then
    break
  else
    echo "INVALID"
    echo "Please enter a valid prefix [a-z A-Z]:"
  fi
done

mv "$prefix_$number1.txt" "$newPrefix_$number1.txt"
mv "$prefix_$number2.txt" "$newPrefix_$number2.txt"
echo ""
echo "Files Renamed"
ls *.txt
echo ""


# Please enter a valid number [0-9]:
# Please enter a valid prefix [a-z A-Z]:

####################
# Strings that you may need for prompt (not in order)
####################

#################### Steps #################### 

# Fill in the code to request user for the prefix

# Check the validity of the prefix #

# Enter numbers and create files #

# Fill in the code to request user for the new prefix

# Renaming to new prefix #





