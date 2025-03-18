package com.example;

public class BrainstormApp {
    public static void main(String[] args) throws Exception {
        if (args.length < 2) {
            System.out.println("Usage: start|commit|rollback <idea>");
            return;
        }

        String action = args[0];
        String idea = args[1];

        SnapshotManager snapshotMgr = new SnapshotManager("brainstormPool/brainstorm");
        FileManager fileMgr = new FileManager();
        Transaction tx = new Transaction("tx-" + idea, snapshotMgr, fileMgr);

        switch (action) {
            case "start":
                tx.start(idea + ".txt");
                new ProcessBuilder("nano", "/brainstorm/" + idea + ".txt")
                        .inheritIO().start().waitFor();
                break;
            case "commit":
                if (tx.commit(idea + ".txt")) {
                    System.out.println("Transaktion erfolgreich.");
                } else {
                    System.out.println("Konflikt erkannt. Rollback ausgeführt.");
                }
                break;
            case "rollback":
                tx.rollback();
                System.out.println("Rollback durchgeführt.");
                break;
            default:
                System.out.println("Ungültige Aktion.");
        }
    }
}
