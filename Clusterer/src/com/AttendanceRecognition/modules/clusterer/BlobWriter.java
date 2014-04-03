package com.AttendanceRecognition.modules.clusterer;

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.Map;

public class BlobWriter {

    Map<String, ArrayList<Integer>> map;

    BlobWriter(Map<String, ArrayList<Integer>> map) {
        this.map = map;
    }

    public void writeBlob(String output_dir) {

        output_dir = output_dir.replace("/", "//");
        File out_dir = new File(output_dir);

        if (!out_dir.isDirectory()) {
            System.out.println("Output directory location invalid");
        } else {
            Iterator<String> iterator = map.keySet().iterator();

            while (iterator.hasNext()) {
                String header = (String) iterator.next();
                ArrayList<Integer> rowBlob = map.get(header);
                File f = new File(output_dir + header + ".txt");
                System.out.println("currently writing file with header " + header);

                PrintWriter pw;
                try {
                    f.createNewFile();
                    FileWriter fw = new FileWriter(f);
                    pw = new PrintWriter(fw);
                    pw.println(header);

                    for (int i : rowBlob) {

                        pw.println(i);

                    }
                    pw.close();

                } catch (IOException e) {

                    System.out.println("error encountered while writing to file.");
                    e.printStackTrace();
                }

            }

            System.out.println("Done writing clustered files");
        }
    }
}
