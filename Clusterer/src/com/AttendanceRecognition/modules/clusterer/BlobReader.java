package com.AttendanceRecognition.modules.clusterer;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;

import com.AttendanceRecognition.modules.clusterer.exceptions.NoQRHeaderException;

public class BlobReader {

    private String QR_header;
    private FileReader file_reader_obj;
    private BufferedReader buffered_filereader;
    private ArrayList<Integer> rowCounter;

    public String getQRHeader() {
        return QR_header;
    }

    public ArrayList<Integer> getBlobData() {

        return rowCounter;
    }

    private void setHeader() throws NoQRHeaderException {

        try {
            QR_header = buffered_filereader.readLine();
            if (QR_header.equals("")) {
                throw new NoQRHeaderException("No QR present in text file (QR must be blank)");
            }
        } catch (IOException e) {
            System.out.println("could not read file");
            e.printStackTrace();
        } catch (NoQRHeaderException nqhe) {
            
            throw nqhe;
        }

    }

    private void readFileTextLines() {

        try {

            String sCurrentLine;

            while ((sCurrentLine = buffered_filereader.readLine()) != null) {

                //System.out.println(sCurrentLine);
                //count of 1 per line.
                int count = processTextLine(sCurrentLine);
                rowCounter.add(count);

            }
        } catch (Exception e) {

        }
    }

    private int processTextLine(String text_line) {

        char[] textlineCharArray = text_line.toCharArray();
        int counter = 0;

        for (char c : textlineCharArray) {
            if (c == ',') {
                continue;
            }

            if (c == '1') {
                counter++;
            }
        }

        return counter;
    }

    public BlobReader() {

        System.out.println("please use parametrized constructor");
    }

    public BlobReader(String file_path) throws NoQRHeaderException {

        rowCounter = new ArrayList<Integer>();

        try {

            file_reader_obj = new FileReader(file_path);
            buffered_filereader = new BufferedReader(file_reader_obj);

            //first get the header
            setHeader();

            //read the text file's lines (after header)
            readFileTextLines();
        } catch (FileNotFoundException e) {

            System.out.println("could not open the text file.");
            e.printStackTrace();
        }
    }

}
