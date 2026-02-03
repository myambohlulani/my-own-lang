CC = gcc
OUT = ./App/app
SRC = ./src/main.c

build:
	@$(CC) $(SRC) -o $(OUT)

run:
	@$(OUT)

clean:
	@rm -rf $(OUT)

.PHONY: build run clean