Running Configuration:
    Generator:
        gcc -o Generator Generator.c
        ./Generator
    Main Process:
        gcc -o Prodcons Prodcons.c -lpthread
        ./Prodcons
