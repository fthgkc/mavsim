clc; clear all;
dir=pwd();
fptr=file('open',dir+'/other_team_1.txt','old')
data = read(fptr,1,2);
file('close',fptr);
