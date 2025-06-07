/*
 * Copyright (c) 2020 Fraunhofer FOKUS and others. All rights reserved.
 *
 * See the NOTICE file(s) distributed with this work for additional
 * information regarding copyright ownership.
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contact: mosaic@fokus.fraunhofer.de
 */

import org.eclipse.mosaic.fed.application.ambassador.simulation.communication.CamBuilder;
import org.eclipse.mosaic.fed.application.ambassador.simulation.communication.CellModuleConfiguration;
import org.eclipse.mosaic.fed.application.ambassador.simulation.communication.ReceivedAcknowledgement;
import org.eclipse.mosaic.fed.application.ambassador.simulation.communication.ReceivedV2xMessage;
import org.eclipse.mosaic.fed.application.app.AbstractApplication;
import org.eclipse.mosaic.fed.application.app.api.CommunicationApplication;
import org.eclipse.mosaic.fed.application.app.api.os.TrafficLightOperatingSystem;
import org.eclipse.mosaic.interactions.communication.V2xMessageTransmission;
import org.eclipse.mosaic.lib.objects.trafficlight.TrafficLight;
import org.eclipse.mosaic.lib.objects.trafficlight.TrafficLightProgram;
import org.eclipse.mosaic.lib.objects.trafficlight.TrafficLightProgramPhase;
import org.eclipse.mosaic.lib.objects.trafficlight.TrafficLightState;
import org.eclipse.mosaic.lib.objects.v2x.GenericV2xMessage;
import org.eclipse.mosaic.lib.objects.v2x.V2xMessage;
import org.eclipse.mosaic.lib.util.scheduling.Event;
import org.eclipse.mosaic.rti.DATA;
import org.eclipse.mosaic.rti.TIME;
import org.eclipse.mosaic.fed.application.app.api.os.TrafficLightOperatingSystem;

import java.lang.reflect.Array;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.time.Duration;

/**
 * Simple Traffic Light example Application.
 *
 * - Prints out the lanes controlled by the traffic light group
 * - Prints out the traffic lights this group consists of
 * - Sets the remaining duration for the current traffic light phase
 *
 * See http://sumo.dlr.de/wiki/Simulation/Traffic_Lights and
 * {@link org.eclipse.mosaic.app.tutorial.TrafficLightApp} for more details.
 */
@SuppressWarnings("JavadocReference")
public class TrafficLightControlApp extends AbstractApplication<TrafficLightOperatingSystem>
        implements CommunicationApplication {

    private boolean lengthened = false;
    public final static TrafficLightState RED = new TrafficLightState(true, false, false);
    HashMap<Integer, ArrayList<String>> J1_index = new HashMap<Integer, ArrayList<String>>();

    @Override
    public void onStartup() {
        getLog().infoSimTime(this, "Startup");

        getLog().infoSimTime(this, "Getting controlled Lanes");

        for (String lane : getOs().getControlledLanes()) {
            getLog().infoSimTime(this, "Controlled lane: {}", lane);
        }
        getOs().getCellModule().enable(new CellModuleConfiguration()
                .maxDownlinkBitrate(50 * DATA.MEGABIT)
                .maxUplinkBitrate(50 * DATA.MEGABIT));

        getOs().getCellModule().enable(new CellModuleConfiguration());
        getLog().infoSimTime(this, "Activated CellModule");

        // in the mapping_config we refer not to one separated traffic light but to
        // traffic light group that is responsible for e.g. one crossroad
        // an example for it will be 5 traffic signals that we get in the next code line
        // for the first traffic light group with group id 252864801
        // each traffic signal is responsible for each direction, in which a vehicle can
        // drive on this crossroad
        final List<TrafficLight> trafficLights = getOs().getTrafficLightGroup().getTrafficLights();

        for (TrafficLight trafficLight : trafficLights) {
            getLog().infoSimTime(this, "Traffic light: {}", trafficLight.toString());
        }
        // index--> [from,to]
        J1_index.put(0, new ArrayList<String>(Arrays.asList("-E2", "-E0")));// J1_0_0
        J1_index.put(1, new ArrayList<String>(Arrays.asList("-E2", "E3"))); // J1_1_0
        J1_index.put(2, new ArrayList<String>(Arrays.asList("-E2", "E1"))); // J1_12_0.
        J1_index.put(3, new ArrayList<String>(Arrays.asList("-E1", "E2"))); // J1_3_0
        J1_index.put(4, new ArrayList<String>(Arrays.asList("-E1", "-E0"))); // J1_4_0
        J1_index.put(5, new ArrayList<String>(Arrays.asList("-E1", "E3"))); // J1_13_0
        J1_index.put(6, new ArrayList<String>(Arrays.asList("-E3", "E1"))); // J1_6_0
        J1_index.put(7, new ArrayList<String>(Arrays.asList("-E3", "E2"))); // J1_7_0
        J1_index.put(8, new ArrayList<String>(Arrays.asList("-E3", "-E0"))); // J1_14_0
        J1_index.put(9, new ArrayList<String>(Arrays.asList("E0", "E3"))); // J1_9_0
        J1_index.put(10, new ArrayList<String>(Arrays.asList("E0", "E1"))); // J1_10_0
        J1_index.put(11, new ArrayList<String>(Arrays.asList("E0", "E2"))); // J1_15_0

        getOs().getEventManager().addEvent(getOs().getSimulationTime() + TIME.SECOND, this);
    }

    @Override
    public void onShutdown() {
        getLog().infoSimTime(this, "Shutdown");
    }

    @Override
    public void processEvent(Event event) throws Exception {
        getLog().infoSimTime(this,
                "Current phase for the traffic light group {} is {} and the remaining duration is {}",
                getOs().getTrafficLightGroup().getGroupId(), getOs().getCurrentPhase(),
                getOs().getCurrentPhase().getRemainingDuration());

        // remaining duration is measured in milliseconds
        if (getOs().getCurrentProgram().getCurrentPhase().getRemainingDuration() == 1000 && !lengthened) {
            getOs().setRemainingDurationOfCurrentPhase(5_000);
            lengthened = true;
        }

        getOs().getEventManager()
                .addEvent(new Event(getOs().getSimulationTime() + 1 * TIME.SECOND, this::checkTrafficLightPhase));
    }

    public void checkTrafficLightPhase(Event event) {
        TrafficLightProgramPhase currentPhase = getOs().getCurrentPhase();
        getLog().infoSimTime(this,
                "Current phase for the traffic light group {} is {} and the remaining duration is {}",
                getOs().getTrafficLightGroup().getGroupId(), getOs().getCurrentPhase(),
                getOs().getCurrentPhase().getRemainingDuration());
    }

    public void onMessageReceived(ReceivedV2xMessage receivedV2xMessage) {
        V2xMessage message = receivedV2xMessage.getMessage();

        if (message instanceof GenericV2xMessage) {

            String payload = ((GenericV2xMessage) message).getMessageType();

            getLog().infoSimTime(this, "Traffic Light: Mensage V2X Received: " + payload);
            String[] split = payload.split("\\|");
            int msgID = Integer.parseInt(split[0]);

            if (msgID == 3 && split[5].equals("J1")) {

                for (Integer index : J1_index.keySet()) {
                    // split[2] é o from e split[4] é o to
                    if (J1_index.get(index).get(0).equals(split[2]) || J1_index.get(index).get(0).equals(split[4])) {

                        // getLog().infoSimTime(this, "Traffic Light:{} Previous State: {}",
                        // getOs().getTrafficLightGroup().getTrafficLights().get(index),
                        // getOs().getTrafficLightGroup().getTrafficLights().get(index).getCurrentState());
                        getLog().infoSimTime(this, "index: {}, J1_index: {}",
                                index, J1_index.get(index));
                        int vermelho = index + 4;
                        getOperatingSystem().switchToPhaseIndex(vermelho);// vermelho
                        getOs().getTrafficLightGroup().getTrafficLights().get(index)
                                .setCurrentState(TrafficLightState.RED);
                        getLog().infoSimTime(this, "Traffic Light CRASH:{} Current State: {}",
                                getOs().getTrafficLightGroup().getTrafficLights().get(index),
                                getOs().getTrafficLightGroup().getTrafficLights().get(index).getCurrentState());

                        // getLog().infoSimTime(this, "Traffic Light:{} Current State: {}",
                        // getOs().getTrafficLightGroup().getTrafficLights().get(index),
                        // getOs().getTrafficLightGroup().getTrafficLights().get(index).getCurrentState());

                    }

                }
            }

        }
    }

    @Override
    public void onAcknowledgementReceived(ReceivedAcknowledgement acknowledgement) {
        // TODO Auto-generated method stub
        throw new UnsupportedOperationException("Unimplemented method 'onAcknowledgementReceived'");
    }

    @Override
    public void onCamBuilding(CamBuilder camBuilder) {
        // TODO Auto-generated method stub
        throw new UnsupportedOperationException("Unimplemented method 'onCamBuilding'");
    }

    @Override
    public void onMessageTransmitted(V2xMessageTransmission v2xMessageTransmission) {
        // TODO Auto-generated method stub
        throw new UnsupportedOperationException("Unimplemented method 'onMessageTransmitted'");
    }
}
