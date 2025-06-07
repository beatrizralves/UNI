
import java.util.Arrays;

import org.eclipse.mosaic.fed.application.ambassador.simulation.communication.AdHocModuleConfiguration;
import org.eclipse.mosaic.fed.application.ambassador.simulation.communication.CamBuilder;
import org.eclipse.mosaic.fed.application.ambassador.simulation.communication.CellModule;
import org.eclipse.mosaic.fed.application.ambassador.simulation.communication.CellModuleConfiguration;
import org.eclipse.mosaic.fed.application.ambassador.simulation.communication.ReceivedAcknowledgement;
import org.eclipse.mosaic.fed.application.ambassador.simulation.communication.ReceivedV2xMessage;
import org.eclipse.mosaic.fed.application.app.AbstractApplication;
import org.eclipse.mosaic.fed.application.app.api.CommunicationApplication;
import org.eclipse.mosaic.fed.application.app.api.os.RoadSideUnitOperatingSystem;
import org.eclipse.mosaic.fed.application.app.api.os.modules.CellCommunicative;
import org.eclipse.mosaic.interactions.communication.V2xMessageTransmission;
import org.eclipse.mosaic.lib.enums.AdHocChannel;
import org.eclipse.mosaic.lib.geo.GeoCircle;
import org.eclipse.mosaic.lib.geo.GeoPoint;
import org.eclipse.mosaic.lib.objects.addressing.CellMessageRoutingBuilder;
import org.eclipse.mosaic.fed.application.app.api.communication.CommunicationModule;
import org.eclipse.mosaic.lib.objects.v2x.GenericV2xMessage;
import org.eclipse.mosaic.lib.objects.v2x.MessageRouting;
import org.eclipse.mosaic.lib.objects.v2x.V2xMessage;
import org.eclipse.mosaic.lib.util.scheduling.Event;
import org.eclipse.mosaic.rti.DATA;
import org.eclipse.mosaic.rti.TIME;

public class RoadSideUnitApp extends AbstractApplication<RoadSideUnitOperatingSystem>
        implements CommunicationApplication, CellCommunicative {
    /**
     * Interval at which messages are sent (every 2 seconds).
     */
    private final static long TIME_INTERVAL = 2 * TIME.SECOND;

    public void sample() {

    }

    @Override
    public void onStartup() {
        // adhoc
        getLog().infoSimTime(this, "Initialize application");
        getOs().getAdHocModule().enable(new AdHocModuleConfiguration()
                .addRadio()
                .channel(AdHocChannel.CCH)
                .power(50)
                .create());
        // cell
        getOs().getCellModule().enable(new CellModuleConfiguration()
                .maxDownlinkBitrate(50 * DATA.MEGABIT)
                .maxUplinkBitrate(50 * DATA.MEGABIT));

        getLog().infoSimTime(this, "Activated WLAN Module");
        getOs().getEventManager().addEvent(getOs().getSimulationTime() + TIME_INTERVAL, this);
    }

    @Override
    public void onShutdown() {
        getLog().infoSimTime(this, "Shutdown application");

    }

    @Override
    public void processEvent(Event event) throws Exception {

        // próximo evento
        getOs().getEventManager().addEvent(getOs().getSimulationTime() + TIME_INTERVAL, this);
    }

    @Override
    public void onMessageReceived(ReceivedV2xMessage receivedV2xMessage) {

        V2xMessage message = receivedV2xMessage.getMessage();

        if (message instanceof GenericV2xMessage) {
            String payload = ((GenericV2xMessage) message).getMessageType();
            String[] split = payload.split("\\|");
            int msgID = Integer.parseInt(split[0]);

            if (msgID == 1) {
                // se mensagem for recebida via celular(fog)
                getLog().infoSimTime(this, "RSU: Mensage Cellular Received: " + payload);
                // cria um routing para o ad-hoc
                MessageRouting routing_adhoc = getOs().getAdHocModule()
                        .createMessageRouting()
                        .viaChannel(AdHocChannel.CCH)
                        .topoBroadCast(5);

                message = new GenericV2xMessage(routing_adhoc, payload, (long) payload.length());
                // envia msg via ad-hoc para o veiculos
                getOs().getAdHocModule().sendV2xMessage(message);
                // getLog().infoSimTime(this, "Cell --> Adhoc |" + payload);

            } else if (msgID == 0) {
                // se mensagem for recebida via ad-hoc(carro)
                getLog().infoSimTime(this, "RSU: Mensage AdHoc Received: " + payload);

                // cria um routing para o fog
                MessageRouting routing_celular = getOs().getCellModule()
                        .createMessageRouting()
                        .topoCast(new byte[] { 10, 2, 0, 1 }); // mensagem enviado para o ip fog

                message = new GenericV2xMessage(routing_celular, payload, (long) payload.length());

                // envia msg via celular para o fog
                // getLog().infoSimTime(this, "Adhoc --> Cell |" + payload);
                getOs().getCellModule().sendV2xMessage(message);

            } else if (msgID == 2) {
                // se mensagem for recebida via celular(fog)
                getLog().infoSimTime(this, "RSU: Mensage Cellular Received: " + payload);
                // cria um routing para o ad-hoc
                MessageRouting routing_adhoc = getOs().getAdHocModule()
                        .createMessageRouting()
                        .viaChannel(AdHocChannel.CCH)
                        .topoCast(split[1], 5);

                message = new GenericV2xMessage(routing_adhoc, payload, (long) payload.length());
                // envia msg via ad-hoc para o veiculo correspondente
                getOs().getAdHocModule().sendV2xMessage(message);

            } else if (msgID == 3) {
                // se mensagem for recebida via celular(fog)
                getLog().infoSimTime(this, "RSU: Mensage Cellular Received: " + payload);
                // cria um routing para o ad-hoc
                MessageRouting routing_adhoc_v1 = getOs().getAdHocModule()
                        .createMessageRouting()
                        .viaChannel(AdHocChannel.CCH)
                        .topoCast(split[1], 5);
                MessageRouting routing_adhoc_v2 = getOs().getAdHocModule()
                        .createMessageRouting()
                        .viaChannel(AdHocChannel.CCH)
                        .topoCast(split[3], 5);

                // cria um routing para o semaforo
                MessageRouting routing_celular = getOs().getCellModule()
                        .createMessageRouting()
                        .topoCast("tl_1");

                // mensagem enviado para o  semáforo
                message = new GenericV2xMessage(routing_celular, payload, (long) payload.length());
                getOs().getCellModule().sendV2xMessage(message);

                message = new GenericV2xMessage(routing_adhoc_v1, payload, (long) payload.length());
                // envia msg via ad-hoc para o veiculo 1
                getOs().getAdHocModule().sendV2xMessage(message);
                // envia msg via ad-hoc para o veiculo 2
                message = new GenericV2xMessage(routing_adhoc_v2, payload, (long) payload.length());
                getOs().getAdHocModule().sendV2xMessage(message);

            }

        }
    }

    @Override
    public void onAcknowledgementReceived(ReceivedAcknowledgement arg0) {

    }

    @Override
    public void onCamBuilding(CamBuilder arg0) {

    }

    @Override
    public void onMessageTransmitted(V2xMessageTransmission arg0) {

    }

    @Override
    public CellModule getCellModule() {
        // TODO Auto-generated method stub
        throw new UnsupportedOperationException("Unimplemented method 'getCellModule'");
    }
}
