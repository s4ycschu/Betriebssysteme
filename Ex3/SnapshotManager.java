package com.example;

import java.io.IOException;

public class SnapshotManager {
    private final String filesystem;

    public SnapshotManager(String filesystem) {
        this.filesystem = filesystem;
    }

    public boolean createSnapshot(String snapshotName) throws IOException, InterruptedException {
        return execute("sudo", "zfs", "snapshot", filesystem + "@" + snapshotName);
    }

    public boolean rollbackSnapshot(String snapshotName) throws IOException, InterruptedException {
        return execute("sudo", "zfs", "rollback", filesystem + "@" + snapshotName);
    }

    public boolean deleteSnapshot(String snapshotName) throws IOException, InterruptedException {
        return execute("sudo", "zfs", "destroy", filesystem + "@" + snapshotName);
    }

    private boolean execute(String... cmd) throws IOException, InterruptedException {
        Process p = new ProcessBuilder(cmd).start();
        return p.waitFor() == 0;
    }
}
