# COMPILATION
Just run:
        make compile
    or
        make src/scanner.c
        make compile

Then the executable will be inside ./bin folder

If you want to recompile it, please run: 
        make clean
        make compile
    or
        make compile
        make compile

# RUN
Run the executable providing the files that you want via redirections

 ./bin/scanner.exe < input_file > output_file

 If you run it without providing an input file, the program will not terminate.