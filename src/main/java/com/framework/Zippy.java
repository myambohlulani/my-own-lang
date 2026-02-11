package com.framework;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.List;

import java.nio.file.Files;
import java.nio.file.Paths;
import java.nio.charset.Charset;

/*
 * Could not decide what to name the compiler hence, ended up with Zippy :)
 */
public class Zippy {
    private static boolean hadError = false;

    public static void main(String[] args) throws IOException {
        // many files
        if (args.length > 1) {
            System.out.println("Usage: Zippy [script]");
            System.exit(64); // command used incorrectly in unix terms
        } else if (args.length == 1) {
            // execute this file
            runFile(args[0]);
        } else {
            // REPL
            runPrompt();
        }
    }

    private static void runFile(String file_path) throws IOException {
        byte[] bytes = Files.readAllBytes(Paths.get(file_path));
        run(new String(bytes, Charset.defaultCharset()));

        if (Zippy.hadError) {
            System.exit(65); // indication of error
        }
    }

    private static void runPrompt() throws IOException {
        InputStreamReader input = new InputStreamReader(System.in);
        BufferedReader reader = new BufferedReader(input);

        for (;;) {
            System.out.println("$ ");
            String line = reader.readLine();
            if (line == null)
                break;

            run(line);
            Zippy.hadError = false;
        }
    }

    private static void run(String source) {
        final Scanner scanner = new Scanner(source);
        List<Token> tokens = scanner.scanTokens();

        for (Token token : tokens) {
            System.out.println(token);
        }
    }

    private static void error(int line, String message) {
        report(line, "", message);
    }

    private static void report(int line, String source, String message) {
        System.err.println("[line " + line + "] Error" + source + ": " + message);
        Zippy.hadError = true;
    }

}
