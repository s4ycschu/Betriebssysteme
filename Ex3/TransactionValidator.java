package com.example;

import java.util.Random;

public class TransactionValidator {
    public static void main(String[] args) {
        SnapshotManager snapshotMgr = new SnapshotManager("brainstormPool/brainstorm");
        FileManager fileMgr = new FileManager();

        int threads = 5; // Anzahl paralleler Transaktionen
        String filename = "parallelIdea.txt";

        Runnable transactionTask = () -> {
            String txName = "tx-thread-" + Thread.currentThread().getId();
            Transaction tx = new Transaction(txName, snapshotMgr, fileMgr);
            try {
                tx.start(filename);
                String content = fileMgr.readFile(filename);
                content += "\nNeue Zeile von Thread " + Thread.currentThread().getId();
                fileMgr.writeFile(filename, content);
                if (tx.commit(filename)) {
                    System.out.println(txName + ": Erfolgreich committed.");
                } else {
                    System.out.println(txName + ": Konflikt erkannt, Rollback ausgeführt.");
                }
            } catch (Exception e) {
                System.out.println(txName + ": Fehler: " + e.getMessage());
                try { tx.rollback(); } catch(Exception ex) {}
            }
        };

        Thread[] threadPool = new Thread[threads];

        for (int i = 0; i < threads; i++) {
            threadPool[i] = new Thread(transactionTask);
            threadPool[i].start();
            try { Thread.sleep(new Random().nextInt(500)); } catch (InterruptedException ignored) {}
        }

        for (Thread thread : threadPool) {
            try { thread.join(); } catch (InterruptedException ignored) {}
        }

        System.out.println("Validierung abgeschlossen.");
    }
}
