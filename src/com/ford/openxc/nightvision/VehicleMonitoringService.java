
package com.ford.openxc.nightvision;

import android.app.Service;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.Handler;
import android.os.IBinder;
import android.util.Log;

import com.openxc.VehicleManager;
import com.openxc.measurements.Measurement;
import com.openxc.measurements.HeadlampStatus;
import com.openxc.measurements.UnrecognizedMeasurementTypeException;
import com.openxc.remote.VehicleServiceException;

public class VehicleMonitoringService extends Service {
    private final static String TAG = "VehicleMonitoringService";
    public static final String ACTION_VEHICLE_HEADLAMPS_OFF =
        "com.ford.openxc.HEADLAMPS_OFF";

    private final Handler mHandler = new Handler();
    private VehicleManager mVehicleManager;

    HeadlampStatus.Listener mHeadlampListener =
            new HeadlampStatus.Listener() {
        public void receive(Measurement measurement) {
            final HeadlampStatus status = (HeadlampStatus) measurement;
            mHandler.post(new Runnable() {
                public void run() {
                    if(status.getValue().booleanValue()) {
                        if(!NightVisionActivity.isRunning()) {
                            Intent intent = new Intent(
                                    VehicleMonitoringService.this,
                                    NightVisionActivity.class);
                            intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
                            VehicleMonitoringService.this.startActivity(intent);
                        }
                    } else {
                        sendBroadcast(new Intent(ACTION_VEHICLE_HEADLAMPS_OFF));
                    }
                }
            });
        }
    };

    ServiceConnection mConnection = new ServiceConnection() {
        public void onServiceConnected(ComponentName className,
                IBinder service) {
            Log.i(TAG, "Bound to VehicleManager");