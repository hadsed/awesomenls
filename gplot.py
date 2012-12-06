"""

File: gplot.py
Author: Hadayat Seddiqi
Description: Reads in all outputted frames of a 
             certain name format, creates gnuplot 
             script to output them as GIFs.

"""
import sys
import os
import glob
import string

i = 0
path = ''
fnames = []
pnames = []

# Search for all frames in the directory
for infile in glob.glob(os.path.join(path, 'frame????')):
    fnames.append(infile)
    fnames[i] = infile
    pnames.append(i)
    i += 1

# Open the gnuplot script and write some preliminary
# settings for plotting
file = open('plot.gnu', 'w')
file.write('clear\n')
file.write('set key off\n')
file.write('set xrange [-10:10]\n')
file.write('set yrange [0.0:1.0]\n')
file.write('set term gif\n')

# Loop through all frames, write in gnuplot piece
# to create GIF for each frame
for k in range(0,i):
    pnames[k] = fnames[k] + '.gif'
    file.write('set output \"' + pnames[k] + '\"\n')
    file.write('set title \"Frame ' + fnames[k][-4:] + '\"\n')
    file.write('plot \"' + fnames[k] + '\"\n')

file.closed
