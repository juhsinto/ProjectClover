package com.AttendanceRecognition.modules.clusterer;

import com.AttendanceRecognition.modules.clusterer.exceptions.BlobUnequalSizeException;
import com.AttendanceRecognition.modules.clusterer.exceptions.NoQRHeaderException;

import java.io.File;
import java.util.ArrayList;
import java.util.Map;

public class ClustererDriver {

    // String inputDir, String outputDir
    public ClustererDriver(String input_file_dir, String output_file_dir) {

        try {
            input_file_dir = input_file_dir.concat("/");
            output_file_dir = output_file_dir.concat("/");

            BlobReader br;
            BlobClusterer bc;
            bc = new BlobClusterer();
            Map<String, ArrayList<Integer>> map = null;

            input_file_dir = input_file_dir.replace("/", "//");
            File f = new File(input_file_dir);

            File[] files = f.listFiles();
            for (File current_file : files) {

                String file_abs_path = current_file.getAbsolutePath();
                String file_ext = file_abs_path.substring(file_abs_path.length() - 4, file_abs_path.length());

                if (current_file.isDirectory()) {
                    System.out.println(current_file.getAbsolutePath() + "=> this is not a valid file");
                    continue;
                }

                if (file_ext.compareTo(".txt") != 0) {
                    System.out.println(current_file.getAbsolutePath() + "=> this is not a valid file");
                    continue;
                }

                System.out.println("Currently processing " + current_file);

                br = new BlobReader(current_file.getAbsolutePath());
                bc.mapData(br.getQRHeader(), br.getBlobData());
                map = bc.getBlob();
            }

            BlobWriter bw = new BlobWriter(map);
            bw.writeBlob(output_file_dir);
        } catch (NoQRHeaderException nqhe) {
            System.out.println("Exception occurred " + nqhe.getMessage());
        } catch (BlobUnequalSizeException buse) {
            System.out.println("Exception occurred " + buse.getMessage());
        } catch (Exception e) {
            System.out.println("Unknown Exception occurered while clustering blobs in Map");
            e.printStackTrace();
        }
    }
}
