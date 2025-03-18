package com.example;

import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.security.MessageDigest;

public class Transaction {
    private final String snapshotName;
    private final SnapshotManager snapshotManager;
    private final FileManager fileManager;
    private byte[] initialHash;

    public Transaction(String snapshotName, SnapshotManager snapshotManager, FileManager fileManager) {
        this.snapshotName = snapshotName;
        this.snapshotManager = snapshotManager;
        this.fileManager = fileManager;
    }

    public void start(String filename) throws Exception {
        Path path = Paths.get("/brainstorm", filename);

        // Datei erzeugen, falls sie nicht existiert
        if (Files.notExists(path)) {
            Files.createFile(path);
            Files.writeString(path, ""); // optional initialer Inhalt
        }

        snapshotManager.createSnapshot(snapshotName);
        initialHash = fileManager.hashFile(filename);
    }

    public boolean commit(String filename) throws Exception {
        if (checkConflicts(filename)) {
            rollback();
            return false; // Konflikt erkannt, Rollback ausgeführt
        }
        snapshotManager.deleteSnapshot(snapshotName);
        return true; // Kein Konflikt, Commit erfolgreich
    }

    public void rollback() throws Exception {
        snapshotManager.rollbackSnapshot(snapshotName);
        snapshotManager.deleteSnapshot(snapshotName);
    }

    private boolean checkConflicts(String filename) throws Exception {
        byte[] currentHash = fileManager.hashFile(filename);
        return !MessageDigest.isEqual(initialHash, currentHash);
    }
}
