#!/bin/bash


# This shell script measures running time and approximation ratio, not for outputting the vertex cover computed by 5 algorithms!!

# For each vertex, we use graphGen to generate 10 graphs, and write them to the correspongding input file,
# (i.e. when V = 5, we write graphs into 5input.txt, for V = 6, we write graphs into 6input.txt, and so forth)
# then, we pipe the input file with the program "a5-measure" to measure the running time and approximation ratio.

# The program "a5-measure" will run 10 times for each graph, so for each vertex, we let a5-measure run 100 times.

# For each vertex, we calculate the average and std deviation of running time and approximation ratio and output the result into a file called data.txt.

# For your convenience, After all the running steps finished, the script will remove input files and just leave the final data file data.txt.

# !!! Note !!!: The running time for V = 15 could be very long sometimes. So for your test convenience, you can just type: 
# "./graphGen 15 | ./a5-measure" in command line to see how it works.(running the program for 10 times is not that long compared with 100 times) 

i=0

while [ "$i" -lt "10" ] # i < 10, for each run, generate 10 graphs
do
	./graphGen 5 >> 5input.txt # V = 5
		
        i=$(($i+1))
done

echo v = 5 >> data.txt
cat 5input.txt | ./a5-measure >> data.txt




i=0

while [ "$i" -lt "10" ] # i < 10, for each run, generate 10 graphs
do
	./graphGen 6 >> 6input.txt # V = 6
	
        i=$(($i+1))
done

echo v = 6 >> data.txt
cat 6input.txt | ./a5-measure >> data.txt




i=0

while [ "$i" -lt "10" ] # i < 10, for each run, generate 10 graphs
do
	./graphGen 7 >> 7input.txt # V = 7

        i=$(($i+1))
done

echo v = 7 >> data.txt
cat 7input.txt | ./a5-measure >> data.txt




i=0

while [ "$i" -lt "10" ] # i < 10, for each run, generate 10 graphs
do
	./graphGen 8 >> 8input.txt # V = 8
	
        i=$(($i+1))
done

echo v = 8 >> data.txt
cat 8input.txt | ./a5-measure >> data.txt




i=0

while [ "$i" -lt "10" ] # i < 10, for each run, generate 10 graphs
do
	./graphGen 9 >> 9input.txt # V = 9

        i=$(($i+1))
done

echo v = 9 >> data.txt
cat 9input.txt | ./a5-measure >> data.txt




i=0

while [ "$i" -lt "10" ] # i < 10, for each run, generate 10 graphs
do
	./graphGen 10 >> 10input.txt # V = 10

        i=$(($i+1))
done

echo v = 10 >> data.txt
cat 10input.txt | ./a5-measure >> data.txt




i=0

while [ "$i" -lt "10" ] # i < 10, for each run, generate 10 graphs
do
	./graphGen 11 >> 11input.txt # V = 11

        i=$(($i+1))
done

echo v = 11 >> data.txt
cat 11input.txt | ./a5-measure >> data.txt




i=0

while [ "$i" -lt "10" ] # i < 10, for each run, generate 10 graphs
do
	./graphGen 12 >> 12input.txt # V = 12

        i=$(($i+1))
done

echo v = 12 >> data.txt
cat 12input.txt | ./a5-measure >> data.txt




i=0

while [ "$i" -lt "10" ] # i < 10, for each run, generate 10 graphs
do
	./graphGen 13 >> 13input.txt # V = 13

        i=$(($i+1))
done

echo v = 13 >> data.txt
cat 13input.txt | ./a5-measure >> data.txt




i=0

while [ "$i" -lt "10" ] # i < 10, for each run, generate 10 graphs
do
	./graphGen 14 >> 14input.txt # V = 14

        i=$(($i+1))
done

echo v = 14 >> data.txt
cat 14input.txt | ./a5-measure >> data.txt




i=0

while [ "$i" -lt "10" ] # i < 10, for each run, generate 10 graphs
do
	./graphGen 15 >> 15input.txt # V = 15

        i=$(($i+1))
done

echo v = 15 >> data.txt
cat 15input.txt | ./a5-measure >> data.txt

rm 5input.txt 6input.txt 7input.txt 8input.txt 9input.txt 10input.txt 11input.txt 12input.txt 13input.txt 14input.txt 15input.txt
