import org.eclipse.mosaic.fed.application.ambassador.simulation.communication.AdHocModuleConfiguration;
import org.eclipse.mosaic.fed.application.ambassador.simulation.communication.CamBuilder;
import org.eclipse.mosaic.fed.application.ambassador.simulation.communication.ReceivedAcknowledgement;
import org.eclipse.mosaic.fed.application.ambassador.simulation.communication.ReceivedV2xMessage;
import org.eclipse.mosaic.fed.application.ambassador.simulation.perception.index.objects.TrafficLightObject;
import org.eclipse.mosaic.fed.application.app.AbstractApplication;
import org.eclipse.mosaic.fed.application.app.api.CommunicationApplication;
import org.eclipse.mosaic.fed.application.app.api.VehicleApplication;
import org.eclipse.mosaic.fed.application.app.api.os.VehicleOperatingSystem;
import org.eclipse.mosaic.interactions.communication.V2xMessageTransmission;
import org.eclipse.mosaic.lib.enums.AdHocChannel;
import org.eclipse.mosaic.lib.objects.v2x.GenericV2xMessage;
import org.eclipse.mosaic.lib.objects.v2x.MessageRouting;
import org.eclipse.mosaic.lib.objects.v2x.V2xMessage;
import org.eclipse.mosaic.lib.objects.vehicle.VehicleData;
import org.eclipse.mosaic.lib.util.scheduling.Event;
import org.eclipse.mosaic.rti.TIME;

import javax.annotation.Nonnull;
import javax.annotation.Nullable;

public class V2XmsgApp extends AbstractApplication<VehicleOperatingSystem>
        implements VehicleApplication, CommunicationApplication {
    // adhoc
    public void onStartup() {
        getOs().getAdHocModule().enable(new AdHocModuleConfiguration()
                .camMinimalPayloadLength(200L)
                .addRadio()
                .channel(AdHocChannel.CCH)
                .power(50)
                .create());
        getLog().infoSimTime(this, "Set up");

        // 100ms
        getOs().getEventManager().addEvent(getOs().getSimulationTime() + 100 * TIME.MILLI_SECOND, this);
    }

    /**
     * Sending CAM and scheduling next events every second.
     */
    @Override
    public void processEvent(Event event) {
        sendV2X();

        getOs().getEventManager().addEvent(getOs().getSimulationTime() + 100 * TIME.MILLI_SECOND, this);
    }

    private void sendV2X() {
        GenericV2xMessage message = this.constructMessage();

        getOs().getAdHocModule().sendV2xMessage(message);
        getLog().infoSimTime(this, "Car: Mensage V2X Sent: " + message.getMessageType());
    }

    // msg payload : String msgID = "0", vehicleID, edgeID, speed, aceleration,
    // direction, positionlat, positionlong, signals;
    private GenericV2xMessage constructMessage() {
        MessageRouting routing = getOs().getAdHocModule()
                .createMessageRouting()
                .topoBroadCast(5);

        // V2I,velocidade, aceleração, direção, posição GPS e indicação dos piscas para
        // mudança de direção
        String msgID = "0", vehicleID, edgeID, speed, aceleration, direction, positionlat, positionlong, signals;
        if (getOs().getVehicleData() != null && getOs().getVehicleParameters() != null) {
            vehicleID = getOs().getVehicleData().getName();
            edgeID = getOs().getVehicleData().getRoadPosition().getConnectionId();
            speed = String.valueOf(getOs().getVehicleData().getSpeed());
            aceleration = String.valueOf(getOs().getVehicleData().getLongitudinalAcceleration());
            direction = String.valueOf(getOs().getVehicleData().getHeading());
            positionlat = String.valueOf(getOs().getVehicleData().getPosition().getLatitude());
            positionlong = String.valueOf(getOs().getVehicleData().getPosition().getLongitude());
            if (getOs().getVehicleData().getVehicleSignals().isBlinkerRight()) {
                signals = "right";
            } else if (getOs().getVehicleData().getVehicleSignals().isBlinkerLeft()) {
                signals = "left";
            } else {
                signals = "front";
            }

        } else {
            vehicleID = "unknown vehicle";
            edgeID = "unknown edge";
            speed = "unknown speed";
            aceleration = "unknown aceleration";
            direction = "unknown direction";
            positionlat = "unknown positionlat";
            positionlong = "unknown positionlong";
            signals = "unknown signals";
        }
        // String message = msgID + "|" + vehicleID + "|" + edgeID;
        String message = msgID + "|" + vehicleID + "|" + edgeID + "|" + speed + "|"
                + aceleration + "|" + direction + "|" + positionlat + "|" + positionlong +
                "|" + signals;
        // String message = msgID + "|coche para RSU";
        return new GenericV2xMessage(routing, message, (long) message.length());
    }

    @Override
    public void onMessageReceived(ReceivedV2xMessage receivedV2xMessage) {
        V2xMessage message = receivedV2xMessage.getMessage();

        if (message instanceof GenericV2xMessage) {

            String payload = ((GenericV2xMessage) message).getMessageType();

            getLog().infoSimTime(this, "Car: Mensage V2X Received: " + payload);
        }
    }

    @Override
    public void onAcknowledgementReceived(ReceivedAcknowledgement acknowledgedMessage) {
    }

    @Override
    public void onMessageTransmitted(V2xMessageTransmission v2xMessageTransmission) {
    }

    @Override
    public void onShutdown() {
        getLog().infoSimTime(this, "Tear down");
    }

    @Override
    public void onVehicleUpdated(@Nullable VehicleData previousVehicleData, @Nonnull VehicleData updatedVehicleData) {

    }

    @Override
    public void onCamBuilding(CamBuilder arg0) {

    }

}