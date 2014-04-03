package com.AttendanceRecognition.modules.clusterer;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;

import com.AttendanceRecognition.modules.clusterer.exceptions.BlobUnequalSizeException;


public class BlobClusterer {

    //this will contain all data of attendance sheets (qr-data + blob of the sheet)
    Map<String, ArrayList<Integer>> map;

    public Map<String, ArrayList<Integer>> getBlob() {
        return map;
    }

    public void mapData(String headerString, ArrayList<Integer> blob) throws BlobUnequalSizeException {

        try {
            if (map.containsKey(headerString)) {

                //System.out.println("there is a sheet with a qr_header - so add data to the same ");
                ArrayList<Integer> existingBlob = map.get(headerString);
                if (existingBlob.size() != blob.size()) {
                    throw new BlobUnequalSizeException("Current blob in Map does not match size of new blob");
                }

                for (int i = 0; i < existingBlob.size(); i++) {
                    int currentRowValueExistingBlob = existingBlob.get(i);
                    int currentRowValueNewBlob = blob.get(i);
                    int sum = (currentRowValueExistingBlob + currentRowValueNewBlob);
                    existingBlob.set(i, sum);
                }

                map.put(headerString, existingBlob);
            } else {

                //System.out.println("there no other sheet with same ");
                map.put(headerString, blob);
            }
        } catch (BlobUnequalSizeException buse) {
            
            throw buse;
        } 
    }

    public BlobClusterer() {
        map = new HashMap<String, ArrayList<Integer>>();
    }

    /**
     * For debugging purposes
     *
     */
    public void iterateBlob() {
        Iterator<String> iterator = map.keySet().iterator();
        while (iterator.hasNext()) {
            Object key = iterator.next();
            Object value = map.get(key);
            System.out.println(key);
            System.out.println(value);
        }

    }
}
