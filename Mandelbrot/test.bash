#!/bin/bash

# Compile the code with optimizations
g++ -O3 -pthread mandelbrot.cpp -o mandel

OUTPUT_FILE="stored_time.txt"
echo "threads,run,real_seconds" > "$OUTPUT_FILE"

# Loop from 1 up to 24 threads
for t in $(seq 1 24); do
    echo "Running tests for $t threads..."
    
    for ((run=1; run<=10; run++)); do
        # Use Bash built-in time with %R (elapsed time in seconds)
        elapsed=$(TIMEFORMAT='%R'; { time ./mandel $t > /dev/null; } 2>&1)
        
        echo "$t,$run,$elapsed" >> "$OUTPUT_FILE"
    done
done

echo "All runs completed. Data saved to $OUTPUT_FILE"