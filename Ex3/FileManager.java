package com.example;

import java.nio.file.*;
import java.security.MessageDigest;
import java.io.IOException;

public class FileManager {
    private final Path dir = Paths.get("/brainstorm");

    public String readFile(String filename) throws IOException {
        return Files.readString(dir.resolve(filename));
    }

    public void writeFile(String filename, String content) throws IOException {
        Files.writeString(dir.resolve(filename), content);
    }

    public byte[] hashFile(String filename) throws Exception {
        MessageDigest digest = MessageDigest.getInstance("SHA-256");
        byte[] content = Files.readAllBytes(dir.resolve(filename));
        return digest.digest(content);
    }
}
